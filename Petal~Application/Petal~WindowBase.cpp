#include "Petal~WindowBase.h"

#include "Petal~Main.h"
#include "Petal~VSDebugOutput.h"

#include <exception>
#include <utility>
#include <format>
#include <mutex>

namespace
{
	namespace PetalUnnamed
	{
		thread_local ::Petal::ptr<::Petal::Abstract::Window> global_window{ nullptr };
		static_assert(
			sizeof(Petal::win32lptr) >= sizeof(Petal::ptr<Petal::Abstract::Window>),
			"[Petal] Size of user data (WIN32 Window) is too small"); // User data for at least a pointer of window object
		
		volatile Petal::tsize window_class_number{ 0 }; // Number the unnamed window class. Defualt window class name depend on it.
		::std::mutex window_class_number_mutex{};
	}
	namespace PetalUnnamed::IWin32
	{
		using namespace Petal;
		[[nodiscard]] win32atom RegisterPetalWindowClass(const Win32WindowClass& window_class) noexcept;
		[[nodiscard]] win32bool UnregisterPetalWindowClass(win32word class_atom) noexcept;
		[[nodiscard]] win32hwnd PetalCreateWindow(win32atom class_atom, Abstract::Window& window, const WindowCreatingArgs& args, boolean interpret_args_size_as_client_size) noexcept;
		[[nodiscard]] win32bool GetWinMessage(Win32Message& message, win32hwnd hwnd, win32msg filter_min, win32msg filter_max) noexcept;
		[[nodiscard]] win32bool PeekWinMessage(Win32Message& message, win32hwnd hwnd, win32msg filter_min, win32msg filter_max, win32msg remove) noexcept;
		[[nodiscard]] win32bool PetalDestroyWindow(win32hwnd target_window) noexcept;
		void MessageProcess(Win32Message& message) noexcept;
	}
}

namespace Petal
{
	win32lres CALLBACK CommonWindowProcess(win32hwnd window_handle, win32msg message, win32wprm w_param, win32lprm l_param)
	{
		// https://learn.microsoft.com/zh-cn/windows/win32/api/winuser/nf-winuser-getwindowlongptra
		// 如上文档所述，每个 Window 的 User data 会被初始化为 0，当使用本框架的 Window 类管理窗口时，
		// User data 由框架使用，用户不应该去修改 User data，这将可能引发错误。
		// 有关线程安全：
		//     PetalUnnamed::global_window 是 thread_local 的，
		//     而每次调用 Create 窗口函数，必须先完成一系列消息处理尤其是 Create 消息才会返回，
		//     在处理这些消息时，PetalUnnamed::global_window 已经被利用，因此是线程安全的。
		if (auto window_ptr = IWindow::WindowLongPtr(window_handle, GWLP_USERDATA))
		{
			return reinterpret_cast<ptr<Abstract::Window>>(window_ptr)->Process(message, w_param, l_param);
		}
		else if (PetalUnnamed::global_window)
		{
			PetalUnnamed::global_window->Bind(window_handle);
			return PetalUnnamed::global_window->Process(message, w_param, l_param);
		}
		return IWindow::SystemDefWndProc(window_handle, message, w_param, l_param);
	}
}

namespace Petal::Abstract
{
	void Window::Bind(win32hwnd new_window_handle)
	{
		if (Valid())
		{
			throw ::std::exception{ "[Petal] Cannot bind new handle to (Petal::Abstract::Window)object that already have a valid handle" };
		}
		if (!new_window_handle)
		{
			throw ::std::exception{ "[Petal] Cannot bind NULL to (Petal::Abstract::Window) object" };
		}
		window_handle = new_window_handle;
		IWindow::UpdateWindowLongPtr(new_window_handle, GWLP_USERDATA, reinterpret_cast<win32lptr>(this));
	}
	void Window::Unbind() noexcept
	{
		if (Valid())
		{
			IWindow::UpdateWindowLongPtr(window_handle, GWLP_USERDATA, reinterpret_cast<win32lptr>(nullptr));
		}
		window_handle = {};
	}
	auto Window::Create(win32atom class_atom, const WindowCreatingArgs& args, boolean interpret_args_size_as_client_size) -> win32handle
	{
		if (Valid() && !Destroy())
		{
			throw ::std::exception{ "[Petal] Failed to destroy window in Window::Create" };
		}

		PetalUnnamed::global_window = this;

		auto window_handle = PetalUnnamed::IWin32::PetalCreateWindow(class_atom, *this, args, interpret_args_size_as_client_size);

		PetalUnnamed::global_window = nullptr;

		if (window_handle == nullptr)
		{
			Petal_VSDbg(::std::format(Petal_DbgStr("[Petal] Failed in Window::Create! class_atom: \"{}\"\r\n"), class_atom).c_str());
		}
		else
		{
			Petal_VSDbg(::std::format(Petal_DbgStr("[Petal] Window(handle:{}) has been created\r\n"), static_cast<void*>(window_handle)).c_str());
		}
		return window_handle;
	}
	auto Window::Create(const WindowCreatingArgs& args, boolean interpret_args_size_as_client_size) -> win32handle
	{
		return this->Create(WindowClassRegister{}.Register(), args, interpret_args_size_as_client_size);
	}
	auto Window::Destroy() noexcept -> win32bool
	{
		const ptrc<void> temp_handle = static_cast<ptr<void>>(this->WindowHandle());

		auto result = PetalUnnamed::IWin32::PetalDestroyWindow(this->WindowHandle());

		if (result == win32_false)
		{
			Petal_VSDbg(::std::format(Petal_DbgStr("[Petal] Failed in Window::Destroy! Window_handle: {}\r\n"), temp_handle).c_str());
		}
		else
		{
			this->Unbind();
			Petal_VSDbg(::std::format(Petal_DbgStr("[Petal] Window(handle:{}) has been destroyed\r\n"), temp_handle).c_str());
		}

		return result;
	}
}

namespace Petal
{
	WindowClassRegister::WindowClassRegister(int) : Win32WindowClass
	{
		.cbSize = sizeof(Win32WindowClass),
		.style = CS_HREDRAW | CS_VREDRAW,
		.lpfnWndProc = &CommonWindowProcess,
		.cbClsExtra = 0,
		.cbWndExtra = 0,
		.hInstance = WinMain::HIns(),
		.hIcon = IWindow::LoadDefaultWinAppIcon(),
		.hCursor = IWindow::LoadDefaultWinAppCursor(),
		.hbrBackground = reinterpret_cast<win32hbrush>(COLOR_WINDOW),
		.lpszMenuName = nullptr,
		.lpszClassName = nullptr,
		.hIconSm = nullptr } {}
	WindowClassRegister::WindowClassRegister() : WindowClassRegister(0)
	{
		DefaultWindowClassName();
	}
	WindowClassRegister::WindowClassRegister(TStringView class_name) : WindowClassRegister(0)
	{
		ClassName(class_name);
	}
	void WindowClassRegister::DefaultWindowClassName()
	{
		TString number{};
		{
			const ::std::lock_guard<::std::mutex> lock(PetalUnnamed::window_class_number_mutex);
#if defined Petal_Enable_Unicode
			number = std::to_wstring(PetalUnnamed::window_class_number);
#else
			number = std::to_string(PetalUnnamed::window_class_number);
#endif
			PetalUnnamed::window_class_number += 1;
		}
		ClassName(Petal_TStr("Petal-window-class-") + number);
	}
	auto WindowClassRegister::Register() -> win32atom
	{
		win32atom atom{};

		atom = PetalUnnamed::IWin32::RegisterPetalWindowClass(Self());

		if (atom == 0)
		{
			Petal_VSDebugOutput(::std::format(Petal_TStr("[Petal] Failed in WindowClassRegister::Register! class_name: \"{}\"\r\n"), this->class_name).c_str());
		}
		else
		{
			Petal_VSDbg(::std::format(Petal_DbgStr("[Petal] Window class(atom:{}) has been registered\r\n"), atom).c_str());
		}
		
		return atom;
	}
}

namespace Petal
{
	[[nodiscard]] auto WindowClass::ClassInfo() const noexcept -> ::std::optional<Win32WindowClass>
	{
		::std::optional<Win32WindowClass> result = ::std::make_optional<Win32WindowClass>();
		if (IWindow::WindowClassInfo(WinMain::HIns(), IWindow::ToWinResource(ClassAtom()), *result) == win32_false)
		{
			return ::std::nullopt;
		}
		return result;
	}
	auto WindowClass::Unregister() noexcept -> boolean
	{
		auto result = PetalUnnamed::IWin32::UnregisterPetalWindowClass(this->ClassAtom());

		if (result)
		{
			Petal_VSDbg(::std::format(Petal_DbgStr("[Petal] Failed in WindowClass::Unregister! in class_atom: {}\r\n"), this->ClassAtom()).c_str());
		}
		else
		{
			Petal_VSDbg(::std::format(Petal_DbgStr("[Petal] Window class(atom:{}) has been unregistered\r\n"), this->ClassAtom()).c_str());
			this->Unbind();
		}

		return result;
	}
}

namespace Petal
{
	i32 MessageLoop(win32hwnd window_handle, win32msg message_filter_min, win32msg message_filter_max)
	{
		Win32Message message{};
		win32bool result{};
		win32error error{};
		for (; ; )
		{
			result = PetalUnnamed::IWin32::GetWinMessage(message, window_handle, message_filter_min, message_filter_max);
			switch (result)
			{
			case 0:
				goto out_of_loop;
				break;
			case -1:
				error = ::GetLastError();
				try
				{
					Petal_VSDbg(::std::format(Petal_DbgStr("[Petal] Exception in Petal::MessageLoop, error code: {}\r\n"), error).c_str());
				}
				catch (const ::std::exception&) {}
				goto out_of_loop;
				break;
			default:
				break;
			}
			PetalUnnamed::IWin32::MessageProcess(message);
		}
	out_of_loop:
		return static_cast<i32>(message.wParam);
	}

	i32 MessageLoop(Abstract::Process<>& user_process, boolean remove, boolean yield, win32hwnd window_handle, win32msg message_filter_min, win32msg message_filter_max)
	{
		Win32Message message{};
		win32bool result{};
		u32 flag{ PM_NOREMOVE };
		if (remove == true) flag = PM_REMOVE;
		if (yield == false) flag |= PM_NOYIELD;
		for (; ; )
		{
			result = PetalUnnamed::IWin32::PeekWinMessage(message, window_handle, message_filter_min, message_filter_max, flag);
			if (result == TRUE)
			{
				if (message.message == WM_QUIT)
				{
					break;
				}
				PetalUnnamed::IWin32::MessageProcess(message);
			}
			else
			{
				user_process.Execution();
			}
		}
		return static_cast<i32>(message.wParam);
	}
	
	void ExitMessageLoop(i32 exit_code) noexcept
	{
		::PostQuitMessage(exit_code);
	}
}

namespace
{
	namespace PetalUnnamed::IWin32
	{
		[[nodiscard]] win32bool PetalDestroyWindow(win32hwnd target_window) noexcept
		{
			return ::DestroyWindow(target_window);
		}
#ifdef Petal_Enable_Unicode
		[[nodiscard]] win32atom RegisterPetalWindowClass(const Win32WindowClass& window_class) noexcept
		{
			return ::RegisterClassExW(&window_class);
		}
		[[nodiscard]] win32bool UnregisterPetalWindowClass(win32word class_atom) noexcept
		{
			return ::UnregisterClassW(Petal::IWindow::ToWinResource(class_atom), WinMain::HIns());
		}
		[[nodiscard]] win32hwnd PetalCreateWindow(win32atom class_atom, Abstract::Window& window, const WindowCreatingArgs& args, boolean interpret_args_size_as_client_size) noexcept
		{
			Size2DI32 size{ args.WindowSize(interpret_args_size_as_client_size) };

			return ::CreateWindowExW
			(
				args.ex_style,
				Petal::IWindow::ToWinResource(class_atom),
				args.window_title.CStr(),
				args.style,
				args.position.x,
				args.position.y,
				size.width,
				size.height,
				nullptr,
				args.menu,
				WinMain::HIns(),
				args.user_data
			);
		}
		[[nodiscard]] win32bool GetWinMessage(Win32Message& message, win32hwnd hwnd, win32msg filter_min, win32msg filter_max) noexcept
		{
			return ::GetMessageW(&message, hwnd, filter_min, filter_max);
		}
		[[nodiscard]] win32bool PeekWinMessage(Win32Message& message, win32hwnd hwnd, win32msg filter_min, win32msg filter_max, win32msg remove) noexcept
		{
			return ::PeekMessageW(&message, hwnd, filter_min, filter_max, remove);
		}
		void MessageProcess(Win32Message& message) noexcept
		{
			::TranslateMessage(&message);
			::DispatchMessageW(&message);
		}
#else
		[[nodiscard]] win32atom RegisterPetalWindowClass(const Win32WindowClass& window_class) noexcept
		{
			return ::RegisterClassExA(&window_class);
		}
		[[nodiscard]] win32bool UnregisterPetalWindowClass(win32word class_atom) noexcept
		{
			return ::UnregisterClassA(Petal::IWindow::ToWinResource(class_atom), WinMain::HIns());
		}
		[[nodiscard]] win32hwnd PetalCreateWindow(win32atom class_atom, Abstract::Window& window, const WindowCreatingArgs& args, boolean interpret_args_size_as_client_size) noexcept
		{
			Size2DI32 size{ args.WindowSize(interpret_args_size_as_client_size) };

			return ::CreateWindowExA
			(
				args.ex_style,
				Petal::IWindow::ToWinResource(class_atom),
				args.window_title.CStr(),
				args.style,
				args.position.x,
				args.position.y,
				size.width,
				size.height,
				nullptr,
				args.menu,
				WinMain::HIns(),
				static_cast<::LPVOID>(&window)
			);
		}
		[[nodiscard]] win32bool GetWinMessage(Win32Message& message, win32hwnd hwnd, win32msg filter_min, win32msg filter_max) noexcept
		{
			return ::GetMessageA(&message, hwnd, filter_min, filter_max);
		}
		[[nodiscard]] win32bool PeekWinMessage(Win32Message& message, win32hwnd hwnd, win32msg filter_min, win32msg filter_max, win32msg remove) noexcept
		{
			return ::PeekMessageA(&message, hwnd, filter_min, filter_max, remove);
		}
		void MessageProcess(Win32Message& message) noexcept
		{
			::TranslateMessage(&message);
			::DispatchMessageA(&message);
		}
#endif
	}
}
