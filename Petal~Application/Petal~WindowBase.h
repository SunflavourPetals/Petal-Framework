#pragma once

#ifndef Petal_Header_WindowManger
#define Petal_Header_WindowManger

#include "Petal~WinTypes.h"
#include "Petal~Types.h"
#include "Petal~String.h"
#include "Petal~Process.h"

#include <utility>
#include <optional>
#include <type_traits>

namespace Petal
{
	class WindowClassArgs;
	class WindowCreatingArgs;
	class WindowClassHash;
	class WindowClass;
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
	[[nodiscard]] auto WindowClassInfo(win32hins hins, ptrc<TChar> window_class, Win32WindowClass& out) noexcept -> win32bool;
}

namespace Petal
{
	class WindowClassRegister : private Win32WindowClass
	{
	private:
		using SelfRef = WindowClassRegister&;
		SelfRef Self() noexcept { return *this; }
	public:
		win32atom Register();
		SelfRef Size(win32uint size) noexcept
		{
			cbSize = size;
			return Self();
		}
		SelfRef Style(win32uint style) noexcept
		{
			this->style = style;
			return Self();
		}
		SelfRef WindowProcess(win32wndproc window_process) noexcept
		{
			lpfnWndProc = window_process;
			return Self();
		}
		SelfRef ClassExtra(int class_extra) noexcept
		{
			cbClsExtra = class_extra;
			return Self();
		}
		SelfRef WindowExtra(int window_extra) noexcept
		{
			cbWndExtra = window_extra;
			return Self();
		}
		SelfRef Instance(win32hins instance) noexcept
		{
			hInstance = instance;
			return Self();
		}
		SelfRef Icon(win32hicon icon) noexcept
		{
			hIcon = icon;
			return Self();
		}
		SelfRef Cursor(win32hcursor cursor) noexcept
		{
			hCursor = cursor;
			return Self();
		}
		SelfRef Background(win32hbrush background_brush) noexcept
		{
			hbrBackground = background_brush;
			return Self();
		}
		SelfRef IconSm(win32hicon icon_sm) noexcept
		{
			hIconSm = icon_sm;
			return Self();
		}
		SelfRef ClassName(TStringView class_name) noexcept
		{
			this->class_name = StringToCStyleString(class_name);
			lpszClassName = this->class_name.c_str();
			return Self();
		}
		SelfRef ClassName(::std::nullptr_t) noexcept
		{
			lpszClassName = nullptr;
			return Self();
		}
		SelfRef MenuName(TStringView menu_name) noexcept
		{
			this->menu_name = StringToCStyleString(menu_name);
			lpszMenuName = this->menu_name.c_str();
			return Self();
		}
		SelfRef MenuName(::std::nullptr_t) noexcept
		{
			lpszMenuName = nullptr;
			return Self();
		}
		SelfRef MenuName(win32word menu_resource) noexcept
		{
			lpszMenuName = IWindow::ToWinResource(menu_resource);
			return Self();
		}
	public:
		WindowClassRegister();
		WindowClassRegister(TStringView class_name);
	private:
		WindowClassRegister(int);
		void DefaultWindowClassName();
	private:
		TString class_name{};
		TString menu_name{};
	};
}

namespace Petal
{
	class WindowCreatingArgs final
	{
	public:
		class TitleString
		{
		public:
			TitleString() = default;
			TitleString(TStringView window_title) { UpdateString(window_title); }
			TitleString& operator=(TStringView window_title) { UpdateString(window_title); return *this; }
			ptrc<TChar> CStr() const noexcept { return string.c_str(); }
		private:
			void UpdateString(TStringView str) { string = StringToCStyleString(str); }
		private:
			TString string;
		};
	public:
		static constexpr auto DefaultPosition() -> Position2DI32 { return { CW_USEDEFAULT, CW_USEDEFAULT }; }
		static constexpr auto DefaultSize() -> Size2DI32 { return { CW_USEDEFAULT, CW_USEDEFAULT }; }
		template <boolean interpret_size_as_client_size = true>
		[[nodiscard]] constexpr Size2DI32 WindowSize() const noexcept;
		[[nodiscard]] constexpr Size2DI32 WindowSize(boolean interpret_size_as_client_size) const noexcept
		{
			return interpret_size_as_client_size ? WindowSize<true>() : WindowSize<false>();
		}
	public:
		TitleString   window_title{ Petal_TStr("Petal~Window") };
		Position2DI32 position{ DefaultPosition() };
		Size2DI32     size{ DefaultSize() };
		win32dword    ex_style{ 0 };
		win32dword    style{ WS_OVERLAPPEDWINDOW };
		win32hmenu    menu{ nullptr };
		ptr<void>     user_data{ nullptr };
	};
}

namespace Petal
{
	class WindowClass
	{
	public:
		using Hash = WindowClassHash;
	public:
		[[nodiscard]] auto ClassInfo() const noexcept -> ::std::optional<Win32WindowClass>;
		[[nodiscard]] auto ClassAtom() const noexcept -> win32atom { return atom; }
		[[nodiscard]] auto Valid() const noexcept -> boolean { return ClassAtom() != 0; }
		auto Unregister() noexcept -> boolean;
		auto Reset() noexcept -> void { WindowClass temp{ ::std::move(*this) }; }
		auto Unbind() noexcept -> win32atom
		{
			return ::std::exchange(atom, win32atom{});
		}
	public:
		WindowClass() = default;
		WindowClass(win32atom atom) : atom{ atom } {}
		WindowClass(const WindowClass&) = delete;
		WindowClass(WindowClass&& o) noexcept
		{
			o.atom = ::std::exchange(this->atom, o.atom);
		}
		~WindowClass() noexcept { if (Valid()) Unregister(); }
		WindowClass& operator=(const WindowClass&) = delete;
		WindowClass& operator=(WindowClass&& o) noexcept
		{
			o.atom = ::std::exchange(this->atom, o.atom);
			return *this;
		}
		explicit operator bool() const noexcept { return Valid(); }
	private:
		win32atom atom{};
	};

	class WindowClassHash final
	{
	public:
		using KeyTy = WindowClass;
		using ResultTy = ::std::size_t;
		[[nodiscard]] ResultTy operator() (const KeyTy& o) const
			noexcept(noexcept(::std::declval<::std::hash<win32atom>>()({})))
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
		virtual win32lres Process(win32msg message, win32wprm w_param, win32lprm l_param) = 0;
	public:
		void Bind(win32hwnd window_handle);
		void Unbind() noexcept;
		[[nodiscard]] auto WindowHandle() const noexcept -> win32hwnd { return window_handle; }
		[[nodiscard]] auto Valid() const noexcept -> boolean { return WindowHandle() != nullptr; }
		auto Create(
			win32atom class_atom,
			const WindowCreatingArgs& args = {},
			boolean interpret_args_size_as_client_size = true) -> win32handle;
		auto Create(
			const WindowCreatingArgs& args = {},
			boolean interpret_args_size_as_client_size = true) -> win32handle;
		auto Destroy() noexcept -> win32bool;
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
	template <boolean interpret_size_as_client_size>
	[[nodiscard]] constexpr Size2DI32 WindowCreatingArgs::WindowSize() const noexcept
	{
		if constexpr (interpret_size_as_client_size)
		{
			if (this->size == WindowCreatingArgs::DefaultSize())
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
	[[nodiscard]] inline win32ctstr ToWinResource(win32word integer) noexcept
	{
		return reinterpret_cast<win32ctstr>(static_cast<win32ulptr>(integer));
	}
#ifdef Petal_Enable_Unicode
	inline win32lres CALLBACK SystemDefWndProc(win32hwnd window_handle, win32msg message, win32wprm w_param, win32lprm l_param) noexcept
	{
		return ::DefWindowProcW(window_handle, message, w_param, l_param);
	}
	[[nodiscard]] inline win32hicon LoadDefaultWinAppIcon() noexcept
	{
		return ::LoadIconW(nullptr, reinterpret_cast<win32tstr>(IDI_APPLICATION));
	}
	[[nodiscard]] inline  win32hcursor LoadDefaultWinAppCursor() noexcept
	{
		return ::LoadCursorW(nullptr, reinterpret_cast<win32tstr>(IDC_ARROW));
	}
	[[nodiscard]] inline win32lptr WindowLongPtr(win32hwnd hwnd, i32 index) noexcept
	{
		return ::GetWindowLongPtrW(hwnd, index);
	}
	[[nodiscard]] inline auto WindowClassInfo(win32hins hins, ptrc<TChar> window_class, Win32WindowClass& out) noexcept -> win32bool
	{
		return ::GetClassInfoExW(hins, window_class, &out);
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
	[[nodiscard]] inline win32hicon LoadDefaultWinAppIcon() noexcept
	{
		return ::LoadIconA(nullptr, reinterpret_cast<win32tstr>(IDI_APPLICATION));
	}
	[[nodiscard]] inline win32hcursor LoadDefaultWinAppCursor() noexcept
	{
		return ::LoadCursorA(nullptr, reinterpret_cast<win32tstr>(IDC_ARROW));
	}
	[[nodiscard]] inline win32lptr WindowLongPtr(win32hwnd hwnd, i32 index) noexcept
	{
		return ::GetWindowLongPtrA(hwnd, index);
	}
	[[nodiscard]] inline auto WindowClassInfo(win32hins hins, ptrc<TChar> window_class, Win32WindowClass& out) noexcept -> win32bool
	{
		return ::GetClassInfoExA(hins, window_class, &out);
	}
	inline win32lptr UpdateWindowLongPtr(win32hwnd hwnd, i32 index, win32lptr lptr) noexcept
	{
		return ::SetWindowLongPtrA(hwnd, index, lptr);
	}
#endif
}

#endif // !Petal_Header_WindowManger
