#include "Petal~WindowBase.h"

#include "Petal~Main.h"
#include "Petal~VSDebugOutput.h"

#include <exception>
#include <utility>
#include <format>

#ifdef CreateEvent
#undef CreateEvent
#endif

namespace
{
	namespace PetalUnnamed
	{
		thread_local ::Petal::ptr<::Petal::Abstract::Window> global_window{ nullptr };
		static_assert(
			sizeof(Petal::win32lptr) >= sizeof(Petal::ptr<Petal::Abstract::Window>),
			"[Petal] Size of user data (WIN32 Window) is too small"); // user data for at least a pointer of window object
	}
	namespace PetalUnnamed::IWin32
	{
		using namespace Petal;
		[[nodiscard]] win32atom RegisterPetalWindowClass(const Win32WindowClass& window_class) noexcept;
		[[nodiscard]] win32bool UnregisterPetalWindowClass(word class_atom) noexcept;
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
	Window::~Window()
	{
		if (this->Valid())
		{
			IWindow::UpdateWindowLongPtr(window_handle, GWLP_USERDATA, reinterpret_cast<win32lptr>(nullptr));
		}
	}
	void Window::Bind(win32hwnd window_handle)
	{
		if (this->Valid())
		{
			throw ::std::exception{ "[Petal] Cannot bind new handle to (Petal::Abstract::Window)object that already have a valid handle" };
		}
		if (!window_handle)
		{
			throw ::std::exception{ "[Petal] Cannot bind NULL to (Petal::Abstract::Window) object" };
		}
		this->window_handle = window_handle;
		IWindow::UpdateWindowLongPtr(window_handle, GWLP_USERDATA, reinterpret_cast<win32lptr>(this));
	}
	void Window::Unbind() noexcept
	{
		if (this->Valid())
		{
			IWindow::UpdateWindowLongPtr(window_handle, GWLP_USERDATA, reinterpret_cast<win32lptr>(nullptr));
		}
		this->window_handle = {};
	}
	[[nodiscard]] auto Window::WindowHandle() const noexcept -> win32hwnd
	{
		return this->window_handle;
	}
	[[nodiscard]] auto Window::Valid() const noexcept -> boolean
	{
		return this->window_handle != nullptr;
	}
	auto Window::Create(win32atom class_atom, const WindowCreatingArgs& args, boolean interpret_args_size_as_client_size) -> CreateResult
	{
		CreateResult result{};

		PetalUnnamed::global_window = this;

		result.window_handle = PetalUnnamed::IWin32::PetalCreateWindow(class_atom, *this, args, interpret_args_size_as_client_size);

		PetalUnnamed::global_window = nullptr;

		if (result.window_handle == nullptr)
		{
			result.win32_error = ::GetLastError();

			Petal_VSDbgT("[Petal] Failed in Window::Create!\r\n");
			Petal_VSDbg(::std::format(Petal_DbgStr("\t\tclass_atom: \"{}\"\r\n"), class_atom).c_str());
			Petal_VSDbg(::std::format(Petal_DbgStr("\t\terror code: {}\r\n"), result.win32_error).c_str());
			
			return result;
		}

		Petal_VSDbg(::std::format(Petal_DbgStr("[Petal] Window(handle:{}) has been created\r\n"), static_cast<void*>(result.window_handle)).c_str());

		return result;
	}
	auto Window::Create(const WindowCreatingArgs& args, boolean interpret_args_size_as_client_size) -> CreateResult
	{
		WindowClass window_class{};
		[[maybe_unused]] auto unused = window_class.Register();
		return this->Create(window_class.Unbind());
	}
	auto Window::Destroy() noexcept -> DestroyResult
	{
		DestroyResult result{};

		const ptrc<void> temp_handle = static_cast<ptr<void>>(this->WindowHandle());

		result.value = PetalUnnamed::IWin32::PetalDestroyWindow(this->WindowHandle());

		if (result.value == win32_false)
		{
			result.win32_error = ::GetLastError();

			Petal_VSDbgT("[Petal] Failed in Window::Destroy!\r\n");
			Petal_VSDbg(::std::format(Petal_DbgStr("\t\tWindow_handle: {}\r\n"), temp_handle).c_str());
			Petal_VSDbg(::std::format(Petal_DbgStr("\t\tWin32: error code {}\r\n"), result.win32_error).c_str());

			return result;
		}

		this->Unbind();

		Petal_VSDbg(::std::format(Petal_DbgStr("[Petal] Window(handle:{}) has been destroyed\r\n"), temp_handle).c_str());

		return result;
	}
	[[nodiscard]] auto Window::WindowLongPtr(int index) const noexcept -> win32lptr
	{
		return IWindow::WindowLongPtr(this->WindowHandle(), index);
	}
	[[nodiscard]] auto Window::UpdateWindowLongPtr(int index, win32lptr val) const noexcept -> win32lptr
	{
		return IWindow::UpdateWindowLongPtr(this->WindowHandle(), index, val);
	}
	[[nodiscard]] auto Window::GWLP_Id() const noexcept -> win32lptr
	{
		return this->WindowLongPtr(GWLP_ID);
	}
	[[nodiscard]] auto Window::GWLP_Style() const noexcept -> win32dword
	{
		return static_cast<win32dword>(this->WindowLongPtr(GWL_STYLE));
	}
	[[nodiscard]] auto Window::GWLP_ExStyle() const noexcept -> win32dword
	{
		return static_cast<win32dword>(this->WindowLongPtr(GWL_EXSTYLE));
	}
	[[nodiscard]] auto Window::GWLP_UserData() const noexcept -> win32lptr
	{
		return this->WindowLongPtr(GWLP_USERDATA);
	}
	[[nodiscard]] auto Window::GWLP_HInstance() const noexcept -> win32hins
	{
		return reinterpret_cast<win32hins>(this->WindowLongPtr(GWLP_HINSTANCE));
	}
	[[nodiscard]] auto Window::GWLP_WindowProcess() const noexcept -> win32wndproc
	{
		return reinterpret_cast<win32wndproc>(this->WindowLongPtr(GWLP_WNDPROC));
	}
	[[nodiscard]] auto Window::GWLP_ParentWindowHandle() const noexcept -> win32hwnd
	{
		return reinterpret_cast<win32hwnd>(this->WindowLongPtr(GWLP_HWNDPARENT));
	}
}

namespace Petal
{
	WindowClassHash::ResultTy WindowClassHash::operator() (const KeyTy& o) const
		noexcept(noexcept(WindowClassHash::HashValue({})))
	{
		return HashValue(o);
	}
	WindowClassHash::ResultTy WindowClassHash::HashValue(const KeyTy & o)
		noexcept(noexcept(::std::declval<::std::hash<win32atom>>()({})))
	{
		::std::hash<win32atom> hasher{};
		return hasher(o.ClassAtom());
	}
}

namespace Petal
{
	tsize WindowClassArgs::default_number = 0;
	WindowClassArgs::WindowClassArgs()
	{
#if defined Petal_Enable_Unicode
		TString number{ std::to_wstring(this->default_number) };
#else
		TString number{ std::to_string(this->default_number) };
#endif
		this->UpdateClassName(TString{ Petal_TStr("Petal-window-class=") } + number);
		this->default_number += 1;
	}
	WindowClassArgs::WindowClassArgs(TStringView class_name)
	{
		this->UpdateClassName(class_name);
	}
	[[nodiscard]] Win32WindowClass WindowClassArgs::BuildWindowClass() const noexcept
	{
		Win32WindowClass window_class{};
		window_class.cbSize = sizeof(Win32WindowClass);
		window_class.style = this->style;
		window_class.lpfnWndProc = this->window_process;
		window_class.cbClsExtra = this->class_extra;
		window_class.cbWndExtra = this->window_extra;
		window_class.hInstance = WinMain::HIns();
		window_class.hIcon = this->icon;
		window_class.hCursor = this->cursor;
		window_class.hbrBackground = this->background_brush;
		window_class.lpszMenuName = (this->using_int_menu_resource) ? IWindow::ToWinResource(this->menu_resource) :
			((this->menu_name.length() > 0) ? this->menu_name.c_str() : nullptr);
		window_class.lpszClassName = (this->class_name.length() > 0) ? this->class_name.c_str() : nullptr;
		window_class.hIconSm = this->icon_sm;
		return window_class;
	}
	[[nodiscard]] const TString& WindowClassArgs::ClassName() const noexcept
	{
		return this->class_name;
	}
	[[nodiscard]] const TString& WindowClassArgs::MenuName() const noexcept
	{
		return this->menu_name;
	}
	void WindowClassArgs::UpdateClassName(TStringView class_name) noexcept(noexcept(StringToCStyleString(class_name)))
	{
		this->class_name = StringToCStyleString(class_name);
	}
	void WindowClassArgs::UpdateMenuName(TStringView menu_name) noexcept(noexcept(StringToCStyleString(menu_name)))
	{
		this->menu_name = StringToCStyleString(menu_name);
	}
	void WindowClassArgs::UsingMenuName(TStringView menu_name) noexcept(noexcept(::std::declval<WindowClassArgs>().UpdateMenuName(menu_name)))
	{
		this->UpdateMenuName(menu_name);
		this->using_int_menu_resource = false;
	}
	void WindowClassArgs::UsingMenuResource(win32word menu_resource) noexcept
	{
		this->menu_resource = menu_resource;
		this->using_int_menu_resource = true;
	}
	void WindowClassArgs::EnableDoubleClickMessage(boolean enable) noexcept
	{
		if (enable)
		{
			this->style |= CS_DBLCLKS;
		}
		else
		{
			constexpr decltype(this->style) full{ ~(static_cast<decltype(this->style)>(0)) };
			this->style &= (full ^ CS_DBLCLKS);
		}
	}
}

namespace Petal
{
	WindowCreatingArgs::WindowCreatingArgs(
		TStringView title,
		Size2DI32 size,
		Position2DI32 position,
		win32dword style,
		win32dword ex_style,
		win32hmenu menu,
		ptr<void> user_data) :
		position{ position },
		size{ size },
		ex_style{ ex_style },
		style{ style },
		menu{ menu },
		user_data{ user_data }
	{
		this->UpdateTitle(title);
	}
	void WindowCreatingArgs::UpdateTitle(TStringView new_title)
	{
		this->window_title = StringToCStyleString(new_title);
	}
	[[nodiscard]] const TString& WindowCreatingArgs::Title() const noexcept
	{
		return this->window_title;
	}
	[[nodiscard]] constexpr Size2DI32 WindowCreatingArgs::WindowSize(boolean interpret_size_as_client_size) const noexcept
	{
		if (interpret_size_as_client_size)
		{
			return this->WindowSize<true>();
		}
		return this->WindowSize<false>();
	}
}

namespace Petal
{
	WindowClass::WindowClass(WindowClass&& o) noexcept
	{
		::std::swap(this->atom, o.atom);
		::std::swap(this->name, o.name);
	}
	WindowClass::~WindowClass() noexcept
	{
		if (this->Valid())
		{
			this->Unregister();
		}
	}
	WindowClass& WindowClass::operator=(WindowClass&& o) noexcept
	{
		::std::swap(*this, o);
		return *this;
	}
	[[nodiscard]] auto WindowClass::ClassAtom() const noexcept -> Atom
	{
		return this->atom;
	}
	[[nodiscard]] auto WindowClass::ClassName() const noexcept -> const Name&
	{
		return this->name;
	}
	[[nodiscard]] auto WindowClass::ClassNameView() const noexcept -> NameView
	{
		return { this->name };
	}
	[[nodiscard]] auto WindowClass::ClassNameCRef() const noexcept -> NameCRef
	{
		return { this->name };
	}
	[[nodiscard]] auto WindowClass::Valid() const noexcept -> boolean
	{
		return this->atom;
	}
	auto WindowClass::Register(const Win32WindowClass& window_class) -> RegisterResult
	{
		this->Reset();

		RegisterResult result{};

		result.class_atom = PetalUnnamed::IWin32::RegisterPetalWindowClass(window_class);

		if (result.class_atom == 0)
		{
			result.win32_error = ::GetLastError();

			Petal_VSDbgT("[Petal] Failed in WindowClass::Register!\r\n");
			Petal_VSDebugOutput(::std::format(Petal_TStr("\t\tclass_name: \"{}\"\r\n"), window_class.lpszClassName).c_str());
			Petal_VSDbg(::std::format(Petal_DbgStr("\t\terror code: {}\r\n"), result.win32_error).c_str());
			
			return result;
		}
		
		this->atom = result.class_atom;
		this->name = window_class.lpszClassName;

		Petal_VSDbg(::std::format(Petal_DbgStr("[Petal] Window class(atom:{}) has been registered\r\n"), result.class_atom).c_str());

		return result;
	}
	auto WindowClass::Register(const WindowClassArgs& window_class_args) -> RegisterResult
	{
		return this->Register(window_class_args.BuildWindowClass());
	}
	auto WindowClass::Unregister() noexcept -> UnregisterResult
	{
		UnregisterResult result{};

		result.value = PetalUnnamed::IWin32::UnregisterPetalWindowClass(this->ClassAtom());

		if (result.value == win32_false)
		{
			result.win32_error = ::GetLastError(); // noexcept

			Petal_VSDbgT("[Petal] Failed in WindowClass::Unregister!\r\n");
			Petal_VSDbg(::std::format(Petal_DbgStr("\t\tin class_atom: {}\r\n"), this->ClassAtom()).c_str());
			Petal_VSDbg(::std::format(Petal_DbgStr("\t\tWin32: error code {}\r\n"), result.win32_error).c_str());

			return result;
		}

		Petal_VSDbg(::std::format(Petal_DbgStr("[Petal] Window class(atom:{}) has been unregistered\r\n"), this->ClassAtom()).c_str());

		this->Unbind();

		return result;
	}
	auto WindowClass::Reset() noexcept -> void
	{
		WindowClass temp{ ::std::move(*this) };
	}
	auto WindowClass::Unbind() noexcept -> Atom
	{
		auto temp_atom = this->atom;
		this->atom = {};
		this->name = {};
		return temp_atom;
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

namespace Petal::IWindow
{
	[[nodiscard]] win32ctstr ToWinResource(word integer) noexcept
	{
		return reinterpret_cast<win32ctstr>(static_cast<win32ulptr>(integer));
	}
#ifdef Petal_Enable_Unicode
	win32lres CALLBACK SystemDefWndProc(win32hwnd window_handle, win32msg message, win32wprm w_param, win32lprm l_param) noexcept
	{
		return ::DefWindowProcW(window_handle, message, w_param, l_param);
	}
	[[nodiscard]] win32hicon LoadDefaultWinAppIcon() noexcept
	{
		return ::LoadIconW(nullptr, reinterpret_cast<win32tstr>(IDI_APPLICATION));
	}
	[[nodiscard]] win32hcursor LoadDefaultWinAppCursor() noexcept
	{
		return ::LoadCursorW(nullptr, reinterpret_cast<win32tstr>(IDC_ARROW));
	}
	[[nodiscard]] win32lptr WindowLongPtr(win32hwnd hwnd, i32 index) noexcept
	{
		return ::GetWindowLongPtrW(hwnd, index);
	}
	win32lptr UpdateWindowLongPtr(win32hwnd hwnd, i32 index, win32lptr lptr) noexcept
	{
		return ::SetWindowLongPtrW(hwnd, index, lptr);
	}
#else
	win32lres CALLBACK SystemDefWndProc(win32hwnd window_handle, win32msg message, win32wprm w_param, win32lprm l_param) noexcept
	{
		return ::DefWindowProcA(window_handle, message, w_param, l_param);
	}
	[[nodiscard]] win32hicon LoadDefaultWinAppIcon() noexcept
	{
		return ::LoadIconA(nullptr, reinterpret_cast<win32tstr>(IDI_APPLICATION));
	}
	[[nodiscard]] win32hcursor LoadDefaultWinAppCursor() noexcept
	{
		return ::LoadCursorA(nullptr, reinterpret_cast<win32tstr>(IDC_ARROW));
	}
	[[nodiscard]] win32lptr WindowLongPtr(win32hwnd hwnd, i32 index) noexcept
	{
		return ::GetWindowLongPtrA(hwnd, index);
	}
	win32lptr UpdateWindowLongPtr(win32hwnd hwnd, i32 index, win32lptr lptr) noexcept
	{
		return ::SetWindowLongPtrA(hwnd, index, lptr);
	}
#endif
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
		[[nodiscard]] win32bool UnregisterPetalWindowClass(word class_atom) noexcept
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
				args.Title().c_str(),
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
		[[nodiscard]] win32bool UnregisterPetalWindowClass(word class_atom) noexcept
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
				args.Title().c_str(),
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
