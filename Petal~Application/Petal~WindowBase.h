#pragma once

#ifndef Petal_Header_WindowManger
#define Petal_Header_WindowManger

#include "Petal~WinTypes.h"
#include "Petal~Types.h"
#include "Petal~String.h"
#include "Petal~Process.h"

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
	[[nodiscard]] auto ToWinResource(word integer) noexcept -> win32ctstr;
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
		[[nodiscard]] const TString& ClassName() const noexcept;
		[[nodiscard]] const TString& MenuName() const noexcept;
		void UpdateClassName(TStringView class_name) noexcept(noexcept(StringToCStyleString(class_name)));
		void UpdateMenuName(TStringView menu_name) noexcept(noexcept(StringToCStyleString(menu_name)));
		void UsingMenuName(TStringView menu_name) noexcept(noexcept(::std::declval<WindowClassArgs>().UpdateMenuName(menu_name)));
		void UsingMenuResource(win32word menu_resource) noexcept;
		void EnableDoubleClickMessage(boolean enable = true) noexcept;
	public:
		WindowClassArgs();
		WindowClassArgs(TStringView class_name);
		WindowClassArgs(const WindowClassArgs&) = default;
		WindowClassArgs(WindowClassArgs&&) noexcept = default;
		~WindowClassArgs() = default;
	public:
		static constexpr win32wndproc    default_window_process{ &CommonWindowProcess };
		static constexpr win32uint       default_style{ CS_HREDRAW | CS_VREDRAW };
		static constexpr int             default_class_extra{ 0 };
		static constexpr int             default_window_extra{ 0 };
		static constexpr win32hbrush     default_background_brush{ reinterpret_cast<win32hbrush>(COLOR_WINDOW) };
		static constexpr win32hicon      default_icon_sm{ nullptr };
		static inline const win32hicon   default_icon{ IWindow::LoadDefaultWinAppIcon() };
		static inline const win32hcursor default_cursor{ IWindow::LoadDefaultWinAppCursor() };
	private:
		// WIN32-RegisterClassEx requires that string Win32WindowClass::lpszClassName not be nullptr and not equal to null_tstr.
		// WIN32-RegisterClassEx requires class_name's length less than 256 after processing(StringToCStyleString).
		TString class_name;
		// When using_int_menu_res == false:
		// If menu_name != null_tstr, method BuildWindowClass will assign menu_name.c_str() to lpszMenuName. 
		// If menu_name == null_tstr, method BuildWindowClass will assign nullptr to lpszMenuName. 
		// Use StringToCStyleString to ignore NUL characters. 
		TString menu_name;
	public:
		win32wndproc window_process{ default_window_process };
		win32uint    style{ default_style };
		int          class_extra{ default_class_extra };
		int          window_extra{ default_window_extra };
		win32hicon   icon{ default_icon };
		win32hcursor cursor{ default_cursor };
		win32hbrush  background_brush{ default_background_brush };
		win32hicon   icon_sm{ default_icon_sm };
		win32word    menu_resource{ 0 };
		// Switch to fill WindowClass::lpszMenuName by menu_resource but not string menu_name when building Win32WindowClass.
		boolean      using_int_menu_resource{ false };
	private:
		static tsize default_number; // for unnamed instance
		friend class WindowClassSet;
	};
}

namespace Petal
{
	class WindowCreatingArgs final
	{
	public:
		void UpdateTitle(TStringView new_title);
		[[nodiscard]] const TString& Title() const noexcept;
		template <boolean interpret_size_as_client_size = true>
		[[nodiscard]] constexpr Size2DI32 WindowSize() const noexcept;
		[[nodiscard]] constexpr Size2DI32 WindowSize(boolean interpret_size_as_client_size) const noexcept;
	public:
		WindowCreatingArgs() = default;
		WindowCreatingArgs(
			TStringView   title,
			Size2DI32     size      = default_size,
			Position2DI32 position  = default_position,
			win32dword    style     = default_style,
			win32dword    ex_style  = default_ex_style,
			win32hmenu    menu      = default_menu,
			ptr<void>     user_data = nullptr);
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
		using NameView = TStringView;
		using NameCRef = TCStringRef;
		using RegisterResult = WindowClassRegisteringResult;
		using UnregisterResult = WindowClassUnregisteringResult;
		using Hash = WindowClassHash;
	public:
		[[nodiscard]] auto ClassAtom() const noexcept -> Atom;
		[[nodiscard]] auto ClassName() const noexcept -> const Name&;
		[[nodiscard]] auto ClassNameView() const noexcept -> NameView;
		[[nodiscard]] auto ClassNameCRef() const noexcept -> NameCRef;
		[[nodiscard]] auto Valid() const noexcept -> boolean;
		auto Register(const Win32WindowClass& window_class) -> RegisterResult;
		auto Register(const WindowClassArgs& window_class_args = {}) -> RegisterResult;
		auto Unregister() noexcept -> UnregisterResult;
		auto Reset() noexcept -> void;
		auto Unbind() noexcept -> Atom;
	public:
		WindowClass() = default;
		WindowClass(const WindowClass& o) = delete;
		WindowClass(WindowClass&& o) noexcept;
		~WindowClass() noexcept;
		WindowClass& operator=(const WindowClass& o) = delete;
		WindowClass& operator=(WindowClass&& o) noexcept;
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
			noexcept(noexcept(WindowClassHash::HashValue({})));
		[[nodiscard]] static ResultTy HashValue(const KeyTy& o)
			noexcept(noexcept(::std::declval<::std::hash<win32atom>>()({})));
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
		virtual win32lres Process(win32msg message, win32wprm w_param, win32lprm l_param) noexcept = 0;
	public:
		void Bind(win32hwnd window_handle);
		void Unbind() noexcept;
		[[nodiscard]] auto WindowHandle() const noexcept -> win32hwnd;
		[[nodiscard]] auto Valid() const noexcept -> boolean;
		auto Create(
			win32atom class_atom,
			const WindowCreatingArgs& args = {},
			boolean interpret_args_size_as_client_size = true) -> CreateResult;
		auto Create(
			const WindowCreatingArgs& args = {},
			boolean interpret_args_size_as_client_size = true) -> CreateResult;
		auto Destroy() noexcept -> DestroyResult;
	protected:
		[[nodiscard]] auto WindowLongPtr(int index) const noexcept -> win32lptr;
		[[nodiscard]] auto UpdateWindowLongPtr(int index, win32lptr val) const noexcept -> win32lptr;
		[[nodiscard]] auto GWLP_Id() const noexcept -> win32lptr;
		[[nodiscard]] auto GWLP_Style() const noexcept -> win32dword;
		[[nodiscard]] auto GWLP_ExStyle() const noexcept -> win32dword;
		[[nodiscard]] auto GWLP_UserData() const noexcept -> win32lptr;
		[[nodiscard]] auto GWLP_HInstance() const noexcept -> win32hins;
		[[nodiscard]] auto GWLP_WindowProcess() const noexcept -> win32wndproc;
		[[nodiscard]] auto GWLP_ParentWindowHandle() const noexcept -> win32hwnd;
	public:
		Window() = default;
		Window(const Window&) = delete;
		Window(Window&&) noexcept = delete;
		virtual ~Window();
		Window& operator= (const Window&) = delete;
		Window& operator= (Window&&) = delete;
	private:
		win32hwnd window_handle{};
	};
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

#endif // !Petal_Header_WindowManger
