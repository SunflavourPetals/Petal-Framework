#include "Petal~WindowManager.h"

#include "Petal~Main.h"
#include "Petal~Exception.h"
#include "Petal~VSDebugOutput.h"

#include <exception>
#include <utility>
#include <format>

#include <Windows.h>

namespace
{
	namespace PetalUnnamed::IWin32
	{
		using namespace Petal;
		ptr<Abstract::Window> temp_window_ptr{ nullptr };
		[[nodiscard]] win32atom RegisterPetalWindowClass(const WindowClass& window_class) noexcept;
		[[nodiscard]] win32bool UnregisterPetalWindowClass(word class_atom) noexcept;
		[[nodiscard]] win32hwnd CreatePetalWindow(win32atom class_atom, const WindowCreatingParameters& parameters) noexcept;
		[[nodiscard]] win32bool GetWinMessage(Win32Message& message, win32hwnd hwnd, win32msg filter_min, win32msg filter_max) noexcept;
		[[nodiscard]] win32bool PeekWinMessage(Win32Message& message, win32hwnd hwnd, win32msg filter_min, win32msg filter_max, win32msg remove) noexcept;
		void MessageProcess(Win32Message& message) noexcept;
	}
	namespace PetalUnnamed::WindowSetMutex
	{
		using namespace Petal;
		using Petal::boolean;

		class Mutex final
		{
		public:
			boolean Valid() const noexcept
			{
				return this->mutex_handle != nullptr;
			}
			dword Wait() noexcept
			{
				return ::WaitForSingleObject(this->mutex_handle, INFINITE);
			}
			win32bool Release() noexcept
			{
				return ::ReleaseMutex(this->mutex_handle);
			}
			Mutex() noexcept
			{
#if defined(Petal_Enable_Unicode)
				this->mutex_handle = ::CreateMutexW(nullptr, FALSE, nullptr);
#else
				this->mutex_handle = ::CreateMutexA(nullptr, FALSE, nullptr);
#endif
			}
			~Mutex() noexcept
			{
				::CloseHandle(this->mutex_handle);
			}
		private:
			win32handle mutex_handle{ nullptr };
		};
	}
}

namespace Petal::Abstract
{
	[[nodiscard]] win32hwnd Window::WindowHandle() const noexcept
	{
		return this->window_handle;
	}
	[[nodiscard]] boolean Window::Valid() const noexcept
	{
		return this->window_handle != nullptr;
	}
	[[nodiscard]] dword Window::WindowStyle() const noexcept
	{
		return static_cast<dword>(IWin32::WindowLongPtrGet(this->WindowHandle(), GWL_STYLE));
	}
	[[nodiscard]] dword Window::WindowExStyle() const noexcept
	{
		return static_cast<dword>(IWin32::WindowLongPtrGet(this->WindowHandle(), GWL_EXSTYLE));
	}
}

namespace Petal
{
	WrappedWindowClass::WrappedWindowClass(const TString& class_name, win32hicon icon, win32hcursor cursor, win32hbrush background_brush, win32hicon icon_sm, dword style, i32 class_extra, i32 window_extra) :
		style(style),
		class_extra(class_extra),
		window_extra(window_extra),
		icon(icon),
		cursor(cursor),
		background_brush(background_brush),
		icon_sm(icon_sm)
	{
		this->UpdateClassName(class_name);
	}
	WrappedWindowClass::WrappedWindowClass(const TString& class_name, const TString& menu_name, win32hicon icon, win32hcursor cursor, win32hbrush background_brush, win32hicon icon_sm, dword style, i32 class_extra, i32 window_extra) :
		WrappedWindowClass(class_name, icon, cursor, background_brush, icon_sm, style, class_extra, window_extra)
	{
		this->UsingMenuName(menu_name);
	}
	WrappedWindowClass::WrappedWindowClass(const TString& class_name, word menu_resource, win32hicon icon, win32hcursor cursor, win32hbrush background_brush, win32hicon icon_sm, dword style, i32 class_extra, i32 window_extra) :
		WrappedWindowClass(class_name, icon, cursor, background_brush, icon_sm, style, class_extra, window_extra)
	{
		this->UsingMenuName(menu_resource);
	}
	constexpr [[nodiscard]] auto WrappedWindowClass::DefaultWindowProcess() noexcept -> decltype(WindowClass::lpfnWndProc)
	{
		return &Ignore::PetalWindowProcess;
	}
	[[nodiscard]] WindowClass WrappedWindowClass::BuildWindowClass() const noexcept
	{
		WindowClass window_class{};
		window_class.cbSize = sizeof(WindowClass);
		window_class.style = this->style;
		window_class.lpfnWndProc = this->DefaultWindowProcess();
		window_class.cbClsExtra = this->class_extra;
		window_class.cbWndExtra = this->window_extra;
		window_class.hInstance = WinMain::hins;
		window_class.hIcon = this->icon;
		window_class.hCursor = this->cursor;
		window_class.hbrBackground = this->background_brush;
		window_class.lpszMenuName = (this->using_int_menu_res) ? IWin32::ToWinResource(this->menu_resource) :
			((this->menu_name.length() <= 0) ? nullptr : this->menu_name.c_str());
		window_class.lpszClassName = (this->class_name.length() <= 0) ? nullptr : this->class_name.c_str();
		window_class.hIconSm = this->icon_sm;
		return window_class;
	}
	[[nodiscard]] const TString& WrappedWindowClass::ClassName() const noexcept
	{
		return this->class_name;
	}
	[[nodiscard]] const TString& WrappedWindowClass::MenuName() const noexcept
	{
		return this->menu_name;
	}
	void WrappedWindowClass::UpdateClassName(const TString& class_name) noexcept(noexcept(StringToCStyleString(class_name)))
	{
		this->class_name = StringToCStyleString(class_name);
	}
	void WrappedWindowClass::UpdateMenuName(const TString& menu_name) noexcept(noexcept(StringToCStyleString(menu_name)))
	{
		this->menu_name = StringToCStyleString(menu_name);
	}
	void WrappedWindowClass::UsingMenuName(const TString& menu_name) noexcept(noexcept(::std::declval<WrappedWindowClass>().UpdateMenuName(menu_name)))
	{
		this->UpdateMenuName(menu_name);
		this->using_int_menu_res = false;
	}
	void WrappedWindowClass::UsingMenuName(word menu_resource) noexcept
	{
		this->menu_resource = menu_resource;
		this->using_int_menu_res = true;
	}
	void WrappedWindowClass::EnableDoubleClickMessage(boolean enable) noexcept
	{
		if (enable == true)
		{
			this->style |= CS_DBLCLKS;
		}
		else
		{
			constexpr decltype(this->style) full{ ~(static_cast<decltype(this->style)>(0)) };
			this->style &= (full ^ CS_DBLCLKS);
		}
	}
	[[nodiscard]] WindowClassRegisteringResult WrappedWindowClass::Register() const noexcept(false)
	{
		return IWindowClassSet().Register(*this);
	}
}

namespace Petal
{
	WindowCreatingParameters::WindowCreatingParameters(const TString& title, const Size2DI32& size, const Position2DI32& position, dword style, dword ex_style, win32hmenu menu, ptr<void> user_res_p) :
		position(position),
		size(size),
		ex_style(ex_style),
		style(style),
		menu(menu),
		user_res_p(user_res_p)
	{
		this->UpdateTitle(title);
	}
	boolean WindowCreatingParameters::UpdateTitle(const TString& new_title) noexcept
	{
		try
		{
			this->window_title = StringToCStyleString(new_title);
		}
#ifdef Petal_Enable_VSDebugOutput
		catch (::std::exception& e)
#else
		catch (::std::exception&)
#endif
		{
			Petal_VSDbgT("[Petal] std::exception: ");
			Petal_VSDbgA(e.what());
			Petal_VSDbgT("\r\n");
			return false;
		}
		return true;
	}
	[[nodiscard]] const TString& WindowCreatingParameters::Title() const noexcept
	{
		return this->window_title;
	}
}

namespace Petal
{
	WindowClassSet::~WindowClassSet()
	{
		this->UnregisterAll();
	}
	[[nodiscard]] WindowClassSet::RegisterResult WindowClassSet::Register(const WrappedWindowClass& wrapped_window_class) noexcept(false)
	{
		RegisterResult result{};
		WindowClass window_class{ wrapped_window_class.BuildWindowClass() };
		result.class_atom = PetalUnnamed::IWin32::RegisterPetalWindowClass(window_class);

		if (result.class_atom == 0)
		{
			result.error = ::GetLastError();

			Petal_VSDbgT("[Petal] Failed in WindowClassSet::Register method!\r\n");
			Petal_VSDbgT("\t\tclass_name: \"");
			Petal_VSDebugOutput(wrapped_window_class.ClassName().c_str());
			Petal_VSDbgT("\"\r\n");
			Petal_VSDbgT("\t\terror code: ");
			Petal_VSDbgA(::std::format("{}", result.error).c_str());
			Petal_VSDbgT("\r\n");

			return result;
		}

		this->set[result.class_atom] = wrapped_window_class.ClassName();

		return result;
	}
	[[nodiscard]] WindowClassSet::UnregisterResult WindowClassSet::Unregister(win32atom class_atom) noexcept(false)
	{
		UnregisterResult result{};

		auto debug_output
		{
			[](const win32atom& class_atom, const UnregisterResult& result)
			{
				Petal_VSDbgT("[Petal] Failed in WindowClassSet::Unregister series method!\r\n");
				Petal_VSDbgT("\t\tclass_atom: ");
				Petal_VSDbgA(::std::format("{}", class_atom).c_str());
				Petal_VSDbgT("\r\n");
				Petal_VSDbgT("\t\terror code: ");
				Petal_VSDbgA(::std::format("{}", result.error).c_str());
				Petal_VSDbgT("\r\n");
			}
		};

		auto name{ this->set.find(class_atom) };
		if (name == this->set.end())
		{
			result.error = ERROR_OBJECT_NOT_FOUND;
			result.value = FALSE;
			debug_output(class_atom, result);
			return result;
		}
		result.value = PetalUnnamed::IWin32::UnregisterPetalWindowClass(class_atom);

		if (result.value == FALSE)
		{
			result.error = ::GetLastError();
			debug_output(class_atom, result);
			return result;
		}

		auto erase_count{ this->set.erase(class_atom) };

		if (erase_count <= 0)
		{
			Petal_VSDbgT("[Petal] Exception in WindowClassSet::Unregister series method!\r\n");
			Petal_VSDbgT("\t\tFailed in erase class_atom: ");
			Petal_VSDbgA(::std::format("{}", class_atom).c_str());
			Petal_VSDbgT("\r\n");
		}

		Petal_VSDbgA(::std::format("[Petal] Window class(atom:{}) has been unregistered", class_atom).c_str());
		Petal_VSDbgT("\r\n");

		return result;
	}
	tsize WindowClassSet::UnregisterAll() noexcept(noexcept(::std::declval<WindowClassSet>().Unregister({})))
	{
		while (this->set.size() > 0)
		{
			auto result{ this->Unregister(this->set.begin()->first)};
		}
		return this->set.size();
	}
	[[nodiscard]] boolean WindowClassSet::Check(win32atom class_atom) const noexcept
	{
		try
		{
			auto name{ this->set.find(class_atom) };
			if (name == this->set.end())
			{
				return false;
			}
			return true;
		}
#ifdef Petal_Enable_VSDebugOutput
		catch (::std::exception& e)
#else
		catch (::std::exception&)
#endif
		{
			Petal_VSDbgT("[Petal] std::exception: ");
			Petal_VSDbgA(e.what());
			Petal_VSDbgT("\r\n");
			return false;
		}
	}
	[[nodiscard]] boolean WindowClassSet::Empty() const noexcept
	{
		return this->set.empty();
	}
	[[nodiscard]] const TString& WindowClassSet::operator[](win32atom class_atom) const noexcept
	{
		static TString null_tstr{};
		try
		{
			auto info_pair{ this->set.find(class_atom) };
			if (info_pair == this->set.end())
			{
				return null_tstr;
			}
			return info_pair->second;
		}
#ifdef Petal_Enable_VSDebugOutput
		catch (::std::exception& e)
#else
		catch (::std::exception&)
#endif
		{
			Petal_VSDbgT("[Petal] std::exception: ");
			Petal_VSDbgA(e.what());
			Petal_VSDbgT("\r\n");
			return null_tstr;
		}
	}
	[[nodiscard]] WindowClassSet& WindowClassSet::Instance() noexcept
	{
		static WindowClassSet window_class_set{};
		return window_class_set;
	}
}

namespace Petal
{
	WindowSet::~WindowSet()
	{
		this->DestroyAll();
	}
	[[nodiscard]] WindowSet::CreateResult WindowSet::Create(Abstract::Window& target_window, win32atom class_atom, const WindowCreatingParameters& parameters) noexcept(false)
	{
		static PetalUnnamed::WindowSetMutex::Mutex mutex;

		if (mutex.Valid() == false)
		{
			throw Exception{ Petal_MakeExcepArgs(ExceptionCode::InvalidMutex_In_C_WindowSet_M_Create, "Failed in create mutex when create window") };
		}

		mutex.Wait();

		CreateResult result{};
		if (IWindowClassSet().Check(class_atom) == false)
		{
			result.error = ERROR_CANNOT_FIND_WND_CLASS;
			try
			{
				Petal_VSDbgT("[Petal] Failed in WindowSet::Create method!\r\n");
				Petal_VSDbgT("\t\tCan not find class_atom: ");
				Petal_VSDbgA(::std::format("{}", class_atom).c_str());
				Petal_VSDbgT(" in window_class_set\r\n");
			}
			catch (::std::exception&) {}

			mutex.Release();

			return result;
		}

		PetalUnnamed::IWin32::temp_window_ptr = &target_window;
		result.window_handle = PetalUnnamed::IWin32::CreatePetalWindow(class_atom, parameters);
		PetalUnnamed::IWin32::temp_window_ptr = nullptr;

		if (result.window_handle == nullptr)
		{
			result.error = ::GetLastError();
			try
			{
				Petal_VSDbgT("[Petal] Failed in WindowSet::Create method!\r\n");
				Petal_VSDbgT("\t\tclass_atom: \"");
				Petal_VSDbgA(::std::format("{}", class_atom).c_str());
				Petal_VSDbgT("\"\r\n");
				Petal_VSDbgT("\t\terror code: ");
				Petal_VSDbgA(::std::format("{}", result.error).c_str());
				Petal_VSDbgT("\r\n");
			}
			catch (::std::exception&) {}

			mutex.Release();

			return result;
		}

		target_window.window_handle = result.window_handle;

		try
		{
			this->set[result.window_handle] = &target_window;
		}
		catch (::std::exception& e)
		{
			e;
			Petal_VSDbgA(e.what());
			Petal_VSDbgT("\r\n");
			mutex.Release();
			throw Exception{ Petal_MakeExcepArgs(ExceptionCode::FailedInRecordWindow_In_C_WindowSet_M_Create, "Failed in record window to set when create window") };
		}

		mutex.Release();

		return result;
	}
	[[nodiscard]] WindowSet::DestroyResult WindowSet::Destroy(Abstract::Window& window) noexcept(false)
	{
		static PetalUnnamed::WindowSetMutex::Mutex mutex;

		if (mutex.Valid() == false)
		{
			throw Exception{ Petal_MakeExcepArgs(ExceptionCode::InvalidMutex_In_C_WindowSet_M_Destroy, "Failed in create mutex when destroy window") };
		}

		mutex.Wait();

		DestroyResult result{};

		auto debug_output
		{
			[](const Abstract::Window& window, const DestroyResult& result) noexcept
			{
				try
				{
					Petal_VSDbgT("[Petal] Failed in WindowSet::Destroy series method!\r\n");
					Petal_VSDbgT("\t\tWindow_handle: ");
					Petal_VSDbgA(::std::format("{}", static_cast<ptr<void>>(window.WindowHandle())).c_str());
					Petal_VSDbgT("\r\n");
					Petal_VSDbgT("\t\terror code: ");
					Petal_VSDbgA(::std::format("{}", result.error).c_str());
					Petal_VSDbgT("\r\n");
				}
				catch (::std::exception&) {}
			}
		};

		decltype(this->set.find(window.WindowHandle())) window_pair;
		try
		{
			window_pair = this->set.find(window.WindowHandle());
		}
		catch (::std::exception&)
		{
			window_pair = this->set.end();
		}

		if (window_pair == this->set.end())
		{
			result.error = ERROR_INVALID_WINDOW_HANDLE;
			result.value = FALSE;
			debug_output(window, result);

			mutex.Release();

			return result;
		}
		result.value = ::DestroyWindow(window.WindowHandle());

		if (result.value == FALSE)
		{
			result.error = ::GetLastError();
			debug_output(window, result);

			mutex.Release();

			return result;
		}

		decltype(this->set.erase(window.WindowHandle())) erase_count;
		try
		{
			erase_count = this->set.erase(window.WindowHandle());
		}
		catch (::std::exception&)
		{
			erase_count = 0;
		}

		if (erase_count <= 0)
		{
			try
			{
				Petal_VSDbgT("[Petal] Exception in WindowSet::Destroy series method!\r\n");
				Petal_VSDbgT("\t\tFailed in erase window_handle: ");
				Petal_VSDbgA(::std::format("{}", static_cast<ptr<void>>(window.WindowHandle())).c_str());
				Petal_VSDbgT("\r\n");
			}
			catch (::std::exception&) {}

			mutex.Release();
			throw Exception{ Petal_MakeExcepArgs(ExceptionCode::FailedInEraseWindow_In_C_WindowSet_M_Destroy, "Failed in erase window from set when destroy window") };
		}

		try
		{
			Petal_VSDbgA(::std::format("[Petal] Window(handle:{}) has been destroyed", static_cast<void*>(window.window_handle)).c_str());
			Petal_VSDbgT("\r\n");
		}
		catch (::std::exception&) {}

		window.window_handle = nullptr;

		if (IWindowSet().Empty() == true)
		{
			ExitMessageLoop();
		}

		mutex.Release();

		return result;
	}
	tsize WindowSet::DestroyAll() noexcept(noexcept(::std::declval<WindowSet>().Destroy(*ptr<Abstract::Window>{})))
	{
		while (this->set.size() > 0)
		{
			auto result{ this->Destroy(*(this->set.begin()->second)) };
		}
		return this->set.size();
	}
	[[nodiscard]] boolean WindowSet::Check(const Abstract::Window& window) const noexcept
	{
		try
		{
			auto window_pair{ this->set.find(window.WindowHandle()) };
			if (window_pair == this->set.end())
			{
				return false;
			}
			return true;
		}
#ifdef Petal_Enable_VSDebugOutput
		catch (::std::exception& e)
#else
		catch (::std::exception&)
#endif
		{
			Petal_VSDbgT("[Petal] std::exception: ");
			Petal_VSDbgA(e.what());
			Petal_VSDbgT("\r\n");
			return false;
		}
	}
	[[nodiscard]] boolean WindowSet::Empty() const noexcept
	{
		return this->set.empty();
	}
	[[nodiscard]] ptr<Abstract::Window> WindowSet::operator[] (win32hwnd index) const noexcept
	{
		try
		{
			auto window_pair{ this->set.find(index) };
			if (window_pair == this->set.end())
			{
				return nullptr;
			}
			return window_pair->second;
		}
#ifdef Petal_Enable_VSDebugOutput
		catch (::std::exception& e)
#else
		catch (::std::exception&)
#endif
		{
			Petal_VSDbgT("[Petal] std::exception: ");
			Petal_VSDbgA(e.what());
			Petal_VSDbgT("\r\n");
			return nullptr;
		}
	}
	[[nodiscard]] WindowSet& WindowSet::Instance() noexcept
	{
		static WindowSet window_set{};
		return window_set;
	}
}

namespace Petal
{
	WindowClassSet& IWindowClassSet() noexcept { return WindowClassSet::Instance(); }
	WindowSet& IWindowSet() noexcept { return WindowSet::Instance(); }

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
				Petal_VSDbgT("[Petal] Exception in Petal::MessageLoop, error code: ");
				Petal_VSDbgA(::std::format("{}", error).c_str());
				Petal_VSDbgT("\r\n");
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

	i32 MessageLoop(Abstract::ProcessNR& user_process, boolean remove, boolean yield, win32hwnd window_handle, win32msg message_filter_min, win32msg message_filter_max)
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
					goto out_of_loop;
				}
				PetalUnnamed::IWin32::MessageProcess(message);
			}
			else
			{
				user_process.Execution();
			}
		}
	out_of_loop:
		return static_cast<i32>(message.wParam);
	}
	
	void ExitMessageLoop(i32 exit_code) noexcept
	{
		::PostQuitMessage(exit_code);
	}
}

namespace Petal::Ignore
{
	win32lres CALLBACK PetalWindowProcess(win32hwnd window_handle, win32msg message, win32wprm w, win32lprm l) noexcept
	{
		ptr<Abstract::Window> window{ IWindowSet()[window_handle] };
		if (window != nullptr)
		{
			return window->Process(message, w, l);
		}
		else if (PetalUnnamed::IWin32::temp_window_ptr != nullptr)
		{
			PetalUnnamed::IWin32::temp_window_ptr->window_handle = window_handle;
			return PetalUnnamed::IWin32::temp_window_ptr->Process(message, w, l);
		}
		return IWin32::DefaultWindowProcess(window_handle, message, w, l);
	}
}

namespace Petal::IWin32
{
	[[nodiscard]] win32ctstr ToWinResource(word integer) noexcept
	{
		return reinterpret_cast<win32ctstr>(static_cast<win32ulptr>(integer));
	}
#ifdef Petal_Enable_Unicode
	win32lres DefaultWindowProcess(const win32hwnd& hwnd, const win32msg& msg, const win32wprm& w, const win32lprm& l) noexcept
	{
		return ::DefWindowProcW(hwnd, msg, w, l);
	}
	[[nodiscard]] win32hicon LoadDefaultWinAppIcon() noexcept
	{
		return ::LoadIconW(nullptr, reinterpret_cast<win32tstr>(IDI_APPLICATION));
	}
	[[nodiscard]] win32hcursor LoadDefaultWinAppCursor() noexcept
	{
		return ::LoadCursorW(nullptr, reinterpret_cast<win32tstr>(IDC_ARROW));
	}
	[[nodiscard]] win32lptr WindowLongPtrGet(win32hwnd hwnd, i32 index) noexcept
	{
		return ::GetWindowLongPtrW(hwnd, index);
	}
#else
	win32lres DefaultWindowProcess(const win32hwnd& hwnd, const win32msg& msg, const win32wprm& w, const win32lprm& l) noexcept
	{
		return ::DefWindowProcA(hwnd, msg, w, l);
	}
	[[nodiscard]] win32hicon LoadDefaultWinAppIcon() noexcept
	{
		return ::LoadIconA(nullptr, reinterpret_cast<win32tstr>(IDI_APPLICATION));
	}
	[[nodiscard]] win32hcursor LoadDefaultWinAppCursor() noexcept
	{
		return ::LoadCursorA(nullptr, reinterpret_cast<win32tstr>(IDC_ARROW));
	}
	[[nodiscard]] win32lptr WindowLongPtrGet(win32hwnd hwnd, i32 index) noexcept
	{
		return ::GetWindowLongPtrA(hwnd, index);
	}
#endif
}

namespace
{
	namespace PetalUnnamed::IWin32
	{
#ifdef Petal_Enable_Unicode
		[[nodiscard]] win32atom RegisterPetalWindowClass(const WindowClass& window_class) noexcept
		{
			return ::RegisterClassExW(&window_class);
		}
		[[nodiscard]] win32bool UnregisterPetalWindowClass(word class_atom) noexcept
		{
			return ::UnregisterClassW(Petal::IWin32::ToWinResource(class_atom), WinMain::hins);
		}
		[[nodiscard]] win32hwnd CreatePetalWindow(win32atom class_atom, const WindowCreatingParameters& parameters) noexcept
		{
			Win32Rect rect{ 0, 0, parameters.size.width, parameters.size.height };
			::AdjustWindowRectEx(&rect, parameters.style, parameters.menu != nullptr, parameters.ex_style);
			i32 width{ rect.right - rect.left };
			i32 height{ rect.bottom - rect.top };
			return ::CreateWindowExW
			(
				parameters.ex_style,
				Petal::IWin32::ToWinResource(class_atom),
				parameters.Title().c_str(),
				parameters.style,
				parameters.position.x,
				parameters.position.y,
				width,
				height,
				nullptr,
				parameters.menu,
				WinMain::hins,
				static_cast<::LPVOID>(parameters.user_res_p)
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
		[[nodiscard]] win32atom RegisterPetalWindowClass(const WindowClass& window_class) noexcept
		{
			return ::RegisterClassExA(&window_class);
		}
		[[nodiscard]] win32bool UnregisterPetalWindowClass(word class_atom) noexcept
		{
			return ::UnregisterClassA(Petal::IWin32::ToWinResource(class_atom), WinMain::hins);
		}
		[[nodiscard]] win32hwnd CreatePetalWindow(win32atom class_atom, const WindowCreatingParameters& parameters) noexcept
		{
			Win32Rect rect{ 0, 0, parameters.size.width, parameters.size.height };
			::AdjustWindowRectEx(&rect, parameters.style, parameters.menu != nullptr, parameters.ex_style);
			i32 width{ rect.right - rect.left };
			i32 height{ rect.bottom - rect.top };
			return ::CreateWindowExA
			(
				parameters.ex_style,
				Petal::IWin32::ToWinResource(class_atom),
				parameters.Title().c_str(),
				parameters.style,
				parameters.position.x,
				parameters.position.y,
				width,
				height,
				nullptr,
				parameters.menu,
				WinMain::hins,
				static_cast<::LPVOID>(parameters.user_res_p)
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
