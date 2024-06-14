#pragma once

#ifndef Petal_Header_WindowManger
#define Petal_Header_WindowManger

#include "Petal~WinTypes.h"
#include "Petal~Types.h"
#include "Petal~String.h"
#include "Petal~Process.h"

#include <utility>

namespace Petal
{
	class WindowClassArgs;
	class WindowCreatingArgs;
	class WindowClassHash;
	class WindowClass;
	struct WindowClassRegisteringResult final
	{
		win32atom class_atom{ 0 };
		win32error win32_error{ win32_no_error };
		operator bool() const noexcept { return this->win32_error == win32_no_error; }
	};
	struct WindowClassUnregisteringResult final
	{
		win32bool value{ win32_true };
		win32error win32_error{ win32_no_error };
		operator bool() const noexcept { return this->win32_error == win32_no_error; }
	};
	struct WindowCreatingResult final
	{
		win32hwnd window_handle{ nullptr };
		win32error win32_error{ win32_no_error };
		operator bool() const noexcept { return this->win32_error == win32_no_error; }
	};
	struct WindowDestroyingResult final
	{
		win32bool value{ win32_false };
		win32error win32_error{ win32_no_error };
		operator bool() const noexcept { return this->win32_error == win32_no_error; }
	};
}

namespace Petal::Abstract
{
	class Window;
}

namespace Petal
{
	void ExitMessageLoop(i32 exit_code = 0) noexcept;
	win32lres CALLBACK CommonWindowProcess(
		win32hwnd window_handle,
		win32msg  message,
		win32wprm w_param,
		win32lprm l_param);
	i32 MessageLoop(
		win32hwnd window_handle = nullptr,
		win32msg  message_filter_min = 0,
		win32msg  message_filter_max = 0);
	i32 MessageLoop(
		Abstract::Process<>& user_process,
		boolean   remove = true,
		boolean   yield = true,
		win32hwnd window_handle = nullptr,
		win32msg  message_filter_min = 0,
		win32msg  message_filter_max = 0);
	template <typename UserProcess>
		requires requires(UserProcess p) { p(); }
	i32 MessageLoop(
		const UserProcess& user_process,
		boolean   remove = true,
		boolean   yield = true,
		win32hwnd window_handle = nullptr,
		win32msg  message_filter_min = 0,
		win32msg  message_filter_max = 0)
	{
		struct UP : Abstract::Process<>
		{
			const UserProcess& proc;
			UP(const UserProcess& user_proc) : proc{ user_proc } {}
			virtual void Execution() override { proc(); }
		};
		UP wrapped_proc{ user_process };

		return MessageLoop(wrapped_proc, remove, yield, window_handle, message_filter_min, message_filter_max);
	}
}

namespace Petal::IWindow
{
	auto CALLBACK SystemDefWndProc(win32hwnd window_handle, win32msg message, win32wprm w_param, win32lprm l_param) noexcept -> win32lres;
	auto UpdateWindowLongPtr(win32hwnd hwnd, i32 index, win32lptr lptr) noexcept -> win32lptr;
	[[nodiscard]] auto ToWinResource(win32word integer) noexcept -> win32ctstr;
	[[nodiscard]] auto LoadDefaultWinAppIcon() noexcept -> win32hicon;
	[[nodiscard]] auto LoadDefaultWinAppCursor() noexcept -> win32hcursor;
	[[nodiscard]] auto WindowLongPtr(win32hwnd hwnd, i32 index) noexcept -> win32lptr;
}

namespace Petal
{
	class WindowClassArgs final
	{
	public:
		using RegisterResult = WindowClassRegisteringResult;
	public:
		[[nodiscard]] Win32WindowClass BuildWindowClass() const noexcept;
		[[nodiscard]] const TString& ClassName() const noexcept { return class_name; }
		[[nodiscard]] const TString& MenuName() const noexcept { return menu_name; }
		void UpdateClassName(TStringView new_class_name) { class_name = StringToCStyleString(new_class_name); }
		void UpdateMenuName(TStringView new_menu_name) { menu_name = StringToCStyleString(new_menu_name); }
		void UsingMenuName(TStringView menu_name);
		void UsingMenuResource(win32word menu_resource) noexcept;
		void EnableDoubleClickMessage(boolean enable = true) noexcept;
	public:
		WindowClassArgs();
		WindowClassArgs(TStringView class_name) { UpdateClassName(class_name); }
		WindowClassArgs(const WindowClassArgs&) = default;
		WindowClassArgs(WindowClassArgs&&) noexcept = default;
		~WindowClassArgs() = default;
	private:
		// WIN32-RegisterClassEx requires that string Win32WindowClass::lpszClassName not be nullptr and not equal to "" or L"".
		// WIN32-RegisterClassEx requires class_name's length less than 256 after processing(StringToCStyleString).
		TString class_name;
		// When using_int_menu_res == false:
		// If menu_name != "" or L"", method BuildWindowClass will assign menu_name.c_str() to lpszMenuName. 
		// If menu_name == "" or L"", method BuildWindowClass will assign nullptr to lpszMenuName. 
		// Use StringToCStyleString to ignore NUL characters. 
		TString menu_name;
	public:
		win32wndproc window_process{ &CommonWindowProcess };
		win32uint    style{ CS_HREDRAW | CS_VREDRAW };
		int          class_extra{ 0 };
		int          window_extra{ 0 };
		win32hicon   icon{ IWindow::LoadDefaultWinAppIcon() };
		win32hcursor cursor{ IWindow::LoadDefaultWinAppCursor() };
		win32hbrush  background_brush{ reinterpret_cast<win32hbrush>(COLOR_WINDOW) };
		win32hicon   icon_sm{ nullptr };
		win32word    menu_resource{ 0 };
		// Switch to fill WindowClass::lpszMenuName by menu_resource but not string menu_name when building Win32WindowClass.
		boolean      using_int_menu_resource{ false };
	};
}

namespace Petal
{
	class WindowCreatingArgs final
	{
	public:
		void UpdateTitle(TStringView new_title) { window_title = StringToCStyleString(new_title); }
		[[nodiscard]] const TString& Title() const noexcept { return window_title; }
		template <boolean interpret_size_as_client_size = true>
		[[nodiscard]] constexpr Size2DI32 WindowSize() const noexcept;
		[[nodiscard]] constexpr Size2DI32 WindowSize(boolean interpret_size_as_client_size) const noexcept
		{
			return interpret_size_as_client_size ? WindowSize<true>() : WindowSize<false>();
		}
	public:
		WindowCreatingArgs() = default;
		WindowCreatingArgs(
			TStringView   title,
			Size2DI32     size      = default_size,
			Position2DI32 position  = default_position,
			win32dword    style     = default_style,
			win32dword    ex_style  = default_ex_style,
			win32hmenu    menu      = default_menu,
			ptr<void>     user_data = nullptr) :
			position{ position },
			size{ size },
			ex_style{ ex_style },
			style{ style },
			menu{ menu },
			user_data{ user_data }
		{
			UpdateTitle(title);
		}
		WindowCreatingArgs(const WindowCreatingArgs&) = default;
		WindowCreatingArgs(WindowCreatingArgs&&) noexcept = default;
		~WindowCreatingArgs() = default;
	public:
		static constexpr Position2DI32 default_position{ CW_USEDEFAULT, CW_USEDEFAULT };
		static constexpr Size2DI32     default_size{ CW_USEDEFAULT, CW_USEDEFAULT };
		static constexpr win32dword    default_ex_style{ 0L };
		static constexpr win32dword    default_style{ WS_OVERLAPPEDWINDOW };
		static constexpr win32hmenu    default_menu{ nullptr };
	private:
		TString window_title{ Petal_TStr("Petal~Window") };
	public:
		Position2DI32 position{ default_position };
		Size2DI32     size{ default_size };
		win32dword    ex_style{ default_ex_style };
		win32dword    style{ default_style };
		win32hmenu    menu{ default_menu };
		ptr<void>     user_data{ nullptr };
	};
}

namespace Petal
{
	class WindowClass
	{
	public:
		using Atom = win32atom;
		using Name = TString;
		using RegisterResult = WindowClassRegisteringResult;
		using UnregisterResult = WindowClassUnregisteringResult;
		using Hash = WindowClassHash;
	public:
		[[nodiscard]] auto ClassAtom() const noexcept -> Atom { return atom; }
		[[nodiscard]] auto ClassName() const noexcept -> const Name& { return name; }
		[[nodiscard]] auto Valid() const noexcept -> boolean { return ClassAtom(); }
		auto Register(const Win32WindowClass& window_class) -> RegisterResult;
		auto Register(const WindowClassArgs& window_class_args = {}) -> RegisterResult;
		auto Unregister() noexcept -> UnregisterResult;
		auto Reset() noexcept -> void { WindowClass temp{ ::std::move(*this) }; }
		auto Unbind() noexcept -> Atom
		{
			name = Name{};
			return ::std::exchange(atom, Atom{});
		}
	public:
		WindowClass() = default;
		WindowClass(const WindowClass& o) = delete;
		WindowClass(WindowClass&& o) noexcept
		{
			::std::swap(this->atom, o.atom);
			::std::swap(this->name, o.name);
		}
		~WindowClass() noexcept { if (Valid()) Unregister(); }
		WindowClass& operator=(const WindowClass& o) = delete;
		WindowClass& operator=(WindowClass&& o) noexcept
		{
			::std::swap(*this, o);
			return *this;
		}
	private:
		Name name{};
		Atom atom{};
	};

	class WindowClassHash final
	{
	public:
		using KeyTy = WindowClass;
		using ResultTy = ::std::size_t;
		[[nodiscard]] ResultTy operator() (const KeyTy& o) const
			noexcept(noexcept(WindowClassHash::HashValue({})))
		{
			return HashValue(o);
		}
		[[nodiscard]] static ResultTy HashValue(const KeyTy& o)
			noexcept(noexcept(::std::declval<::std::hash<win32atom>>()({})))
		{
			::std::hash<win32atom> hasher{};
			return hasher(o.ClassAtom());
		}
	};
}

namespace Petal::Abstract
{
	class Window
	{
	public:
		using CreateResult = WindowCreatingResult;
		using DestroyResult = WindowDestroyingResult;
	public:
		virtual win32lres Process(win32msg message, win32wprm w_param, win32lprm l_param) = 0;
	public:
		void Bind(win32hwnd window_handle);
		void Unbind() noexcept;
		[[nodiscard]] auto WindowHandle() const noexcept -> win32hwnd { return window_handle; }
		[[nodiscard]] auto Valid() const noexcept -> boolean { return WindowHandle() != nullptr; }
		auto Create(
			win32atom class_atom,
			const WindowCreatingArgs& args = {},
			boolean interpret_args_size_as_client_size = true) -> CreateResult;
		auto Create(
			const WindowCreatingArgs& args = {},
			boolean interpret_args_size_as_client_size = true) -> CreateResult;
		auto Destroy() noexcept -> DestroyResult;
	protected:
		[[nodiscard]] auto WindowLongPtr(int index) const noexcept -> win32lptr
		{
			return IWindow::WindowLongPtr(WindowHandle(), index);
		}
		[[nodiscard]] auto UpdateWindowLongPtr(int index, win32lptr val) const noexcept -> win32lptr
		{
			return IWindow::UpdateWindowLongPtr(WindowHandle(), index, val);
		}
		[[nodiscard]] auto GWLP_Id() const noexcept -> win32lptr
		{
			return WindowLongPtr(GWLP_ID);
		}
		[[nodiscard]] auto GWLP_Style() const noexcept -> win32dword
		{
			return static_cast<win32dword>(WindowLongPtr(GWL_STYLE));
		}
		[[nodiscard]] auto GWLP_ExStyle() const noexcept -> win32dword
		{
			return static_cast<win32dword>(WindowLongPtr(GWL_EXSTYLE));
		}
		[[nodiscard]] auto GWLP_UserData() const noexcept -> win32lptr
		{
			return WindowLongPtr(GWLP_USERDATA);
		}
		[[nodiscard]] auto GWLP_HInstance() const noexcept -> win32hins
		{
			return reinterpret_cast<win32hins>(WindowLongPtr(GWLP_HINSTANCE));
		}
		[[nodiscard]] auto GWLP_WindowProcess() const noexcept -> win32wndproc
		{
			return reinterpret_cast<win32wndproc>(WindowLongPtr(GWLP_WNDPROC));
		}
		[[nodiscard]] auto GWLP_ParentWindowHandle() const noexcept -> win32hwnd
		{
			return reinterpret_cast<win32hwnd>(WindowLongPtr(GWLP_HWNDPARENT));
		}
	public:
		Window() = default;
		Window(const Window&) = delete;
		Window(Window&&) noexcept = delete;
		virtual ~Window() { Unbind(); }
		Window& operator= (const Window&) = delete;
		Window& operator= (Window&&) = delete;
	private:
		win32hwnd window_handle{};
	};
}

// Implementation

namespace Petal
{
	inline void WindowClassArgs::UsingMenuName(TStringView menu_name)
	{
		this->UpdateMenuName(menu_name);
		this->using_int_menu_resource = false;
	}
	inline void WindowClassArgs::UsingMenuResource(win32word menu_resource) noexcept
	{
		this->menu_resource = menu_resource;
		this->using_int_menu_resource = true;
	}
	inline void WindowClassArgs::EnableDoubleClickMessage(boolean enable) noexcept
	{
		constexpr decltype(this->style) full{ ~(static_cast<decltype(this->style)>(0)) };
		enable ? style |= CS_DBLCLKS : style &= (full ^ CS_DBLCLKS);
	}
}

namespace Petal
{
	template <boolean interpret_size_as_client_size>
	[[nodiscard]] constexpr Size2DI32 WindowCreatingArgs::WindowSize() const noexcept
	{
		if constexpr (interpret_size_as_client_size)
		{
			if (this->size == WindowCreatingArgs::default_size)
			{
				return this->size;
			}
			else
			{
				Win32Rect rect{ 0, 0, this->size.width, this->size.height };
				::AdjustWindowRectEx(&rect, this->style, this->menu != nullptr, this->ex_style);
				return { .width = rect.right - rect.left, .height = rect.bottom - rect.top };
			}
		}
		return this->size;
	}
}

namespace Petal::IWindow
{
	inline [[nodiscard]] win32ctstr ToWinResource(win32word integer) noexcept
	{
		return reinterpret_cast<win32ctstr>(static_cast<win32ulptr>(integer));
	}
#ifdef Petal_Enable_Unicode
	inline win32lres CALLBACK SystemDefWndProc(win32hwnd window_handle, win32msg message, win32wprm w_param, win32lprm l_param) noexcept
	{
		return ::DefWindowProcW(window_handle, message, w_param, l_param);
	}
	inline [[nodiscard]] win32hicon LoadDefaultWinAppIcon() noexcept
	{
		return ::LoadIconW(nullptr, reinterpret_cast<win32tstr>(IDI_APPLICATION));
	}
	inline [[nodiscard]] win32hcursor LoadDefaultWinAppCursor() noexcept
	{
		return ::LoadCursorW(nullptr, reinterpret_cast<win32tstr>(IDC_ARROW));
	}
	inline [[nodiscard]] win32lptr WindowLongPtr(win32hwnd hwnd, i32 index) noexcept
	{
		return ::GetWindowLongPtrW(hwnd, index);
	}
	inline win32lptr UpdateWindowLongPtr(win32hwnd hwnd, i32 index, win32lptr lptr) noexcept
	{
		return ::SetWindowLongPtrW(hwnd, index, lptr);
	}
#else
	inline win32lres CALLBACK SystemDefWndProc(win32hwnd window_handle, win32msg message, win32wprm w_param, win32lprm l_param) noexcept
	{
		return ::DefWindowProcA(window_handle, message, w_param, l_param);
	}
	inline [[nodiscard]] win32hicon LoadDefaultWinAppIcon() noexcept
	{
		return ::LoadIconA(nullptr, reinterpret_cast<win32tstr>(IDI_APPLICATION));
	}
	inline [[nodiscard]] win32hcursor LoadDefaultWinAppCursor() noexcept
	{
		return ::LoadCursorA(nullptr, reinterpret_cast<win32tstr>(IDC_ARROW));
	}
	inline [[nodiscard]] win32lptr WindowLongPtr(win32hwnd hwnd, i32 index) noexcept
	{
		return ::GetWindowLongPtrA(hwnd, index);
	}
	inline win32lptr UpdateWindowLongPtr(win32hwnd hwnd, i32 index, win32lptr lptr) noexcept
	{
		return ::SetWindowLongPtrA(hwnd, index, lptr);
	}
#endif
}

#endif // !Petal_Header_WindowManger
