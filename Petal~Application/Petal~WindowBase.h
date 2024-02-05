#pragma once

#ifndef Petal_Header_WindowManger
#define Petal_Header_WindowManger

#include "Petal~WinTypes.h"
#include "Petal~Types.h"
#include "Petal~String.h"
#include "Petal~Process.h"

#include <unordered_map>
#include <unordered_set>
#include <list>
#include <memory>

#define Petal_Decl_WndProc(Fn_name) \
	::Petal::win32lres \
		CALLBACK \
		Fn_name( \
	::Petal::win32hwnd window_handle, \
	::Petal::win32msg  message, \
	::Petal::win32wprm w_param, \
	::Petal::win32lprm l_param)

namespace Petal
{
	class WindowClassArgs;
	class WindowCreatingArgs;
	class WindowClassSet;
	class WindowClassNameAtomHash;
	struct WindowClassNameAtomPair final
	{
		using Name = TString;
		using NameView = TStringView;
		using Atom = win32atom;
		Name name;
		Atom atom{ 0 };
	};
	struct WindowClassRegisteringResult final
	{
		win32atom class_atom{ 0 };
		win32error error{ win32_no_error };
	};
	struct WindowClassUnregisteringResult final
	{
		enum class Condition : i16
		{
			Unknown = 0,
			Success = 1,
			Win32,
			PetalFramework,
		};
		enum class Error : i16
		{
			Unknown = 0,
			CannotFindPair,
			FailedWhenEraseFromIWindowClassSet,
		};
		Condition condition{ Condition::Success };
		Error framework_error{ Error::Unknown };
		win32bool value{ win32_true };
		win32error win32_error{ win32_no_error };
	};
	struct WindowCreatingResult final
	{
		win32error win32_error{ win32_no_error };
		win32hwnd window_handle{ nullptr };
	};
	struct WindowDestroyingResult final
	{
		win32bool return_value{ win32_false };
		win32error win32_error{ win32_no_error };
	};
}

namespace Petal::Abstract
{
	class Window;
}

namespace Petal
{
	WindowClassSet& IWindowClassSet();
	i32 MessageLoop(win32hwnd window_handle = nullptr, win32msg message_filter_min = 0, win32msg message_filter_max = 0);
	i32 MessageLoop(Abstract::Process<>& user_process, boolean remove = true, boolean yield = true, win32hwnd window_handle = nullptr, win32msg message_filter_min = 0, win32msg message_filter_max = 0);
	void ExitMessageLoop(i32 exit_code = 0) noexcept;
}

namespace Petal::IWindow
{
	win32lres CALLBACK DefaultSysWndProc(win32hwnd window_handle, win32msg message, win32wprm w_param, win32lprm l_param) noexcept;
	[[nodiscard]] win32ctstr ToWinResource(word integer) noexcept;
	[[nodiscard]] win32hicon LoadDefaultWinAppIcon() noexcept;
	[[nodiscard]] win32hcursor LoadDefaultWinAppCursor() noexcept;
	[[nodiscard]] win32lptr WindowLongPtr(win32hwnd hwnd, i32 index) noexcept;
	[[nodiscard]] ptr<Abstract::Window> WindowFromCreateEvent(win32msg msg, win32lprm l);
}

namespace Petal
{
	class WindowClassNameAtomHash final
	{
	public:
		using KeyTy = ::std::unique_ptr<WindowClassNameAtomPair>;
		using ResultTy = ::std::size_t;
		[[nodiscard]] ResultTy operator() (const KeyTy& o) const
			noexcept(noexcept(WindowClassNameAtomHash::HashValue({})));
		[[nodiscard]] static ResultTy HashValue(const KeyTy& o)
			noexcept(noexcept(::std::declval<::std::hash<win32atom>>()({})));
	};
	class WindowClassArgs final
	{
	public:
		using RegisterResult = WindowClassRegisteringResult;
	public:
		[[nodiscard]] WindowClass BuildWindowClass() const noexcept;
		[[nodiscard]] const TString& ClassName() const noexcept;
		[[nodiscard]] const TString& MenuName() const noexcept;
		void UpdateClassName(TStringView class_name) noexcept(noexcept(StringToCStyleString(class_name)));
		void UpdateMenuName(TStringView menu_name) noexcept(noexcept(StringToCStyleString(menu_name)));
		void UsingMenuName(TStringView menu_name) noexcept(noexcept(::std::declval<WindowClassArgs>().UpdateMenuName(menu_name)));
		void UsingMenuResource(word menu_resource) noexcept;
		void EnableDoubleClickMessage(boolean enable = true) noexcept;
		RegisterResult Register() const noexcept(false);
	public:
		WindowClassArgs() = default;
		WindowClassArgs(TStringView class_name, win32wndproc window_process = nullptr);
		WindowClassArgs(const WindowClassArgs&) = default;
		WindowClassArgs(WindowClassArgs&&) noexcept = default;
		~WindowClassArgs() = default;
	public:
		static constexpr dword default_style{ CS_HREDRAW | CS_VREDRAW };
		static constexpr i32 default_class_extra{ 0 };
		static constexpr i32 default_window_extra{ 0 };
		static constexpr win32hbrush default_background_brush{ reinterpret_cast<win32hbrush>(COLOR_WINDOW) };
		static constexpr win32hicon default_icon_sm{ nullptr };
		static constexpr win32wndproc default_window_process{ &IWindow::DefaultSysWndProc };
		static inline const win32hicon default_icon{ IWindow::LoadDefaultWinAppIcon() };
		static inline const win32hcursor default_cursor{ IWindow::LoadDefaultWinAppCursor() };
	private:
		// WIN32-RegisterClassEx requires that string WindowClass::lpszClassName not be nullptr and not equal to null_tstr.
		// WIN32-RegisterClassEx requires class_name's length less than 256 after processing(StringToCStyleString).
		TString class_name;
		// When using_int_menu_res == false:
		// If menu_name != null_tstr, method BuildWindowClass will assign menu_name.c_str() to lpszMenuName. 
		// If menu_name == null_tstr, method BuildWindowClass will assign nullptr to lpszMenuName. 
		TString menu_name;
	public:
		win32wndproc window_process{ default_window_process };
		dword        style{ default_style };
		i32          class_extra{ default_class_extra };
		i32          window_extra{ default_window_extra };
		win32hicon   icon{ default_icon };
		win32hcursor cursor{ default_cursor };
		win32hbrush  background_brush{ default_background_brush };
		win32hicon   icon_sm{ default_icon_sm };
		word         menu_resource{ 0 };
		// Switch to fill WindowClass::lpszMenuName by menu_resource but not string menu_name when building WindowClass.
		boolean      using_int_menu_resource{ false };
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
	public:
		WindowCreatingArgs() = default;
		WindowCreatingArgs(TStringView title, const Size2DI32& size = default_size, const Position2DI32& position = default_position, dword style = default_style, dword ex_style = default_ex_style, win32hmenu menu = default_menu);
		WindowCreatingArgs(const WindowCreatingArgs&) = default;
		WindowCreatingArgs(WindowCreatingArgs&&) noexcept = default;
		~WindowCreatingArgs() = default;
	public:
		static constexpr Position2DI32 default_position{ CW_USEDEFAULT, CW_USEDEFAULT };
		static constexpr Size2DI32     default_size{ 640, 480 };
		static constexpr dword         default_ex_style{ 0L };
		static constexpr dword         default_style{ WS_OVERLAPPEDWINDOW };
		static constexpr win32hmenu    default_menu{ nullptr };
	private:
		TString window_title{ Petal_TStr("Petal~Window") };
	public:
		Position2DI32 position{ default_position };
		Size2DI32     size{ default_size };
		dword         ex_style{ default_ex_style };
		dword         style{ default_style };
		win32hmenu    menu{ default_menu };
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
		[[nodiscard]] win32hwnd WindowHandle() const noexcept;
		[[nodiscard]] boolean Valid() const noexcept;
		CreateResult Create(win32atom class_atom, const WindowCreatingArgs& args = {});
		DestroyResult Destroy() noexcept;
	protected:
		[[nodiscard]] dword WindowStyle() const noexcept;
		[[nodiscard]] dword WindowExStyle() const noexcept;
	public:
		Window() = default;
		Window(const Window&) = delete;
		Window(Window&&) noexcept = delete;
		virtual ~Window() = default;
		Window& operator= (const Window&) = delete;
		Window& operator= (Window&&) = delete;
	private:
		win32hwnd window_handle{};
	};
}

namespace Petal
{
	class WindowClassSet final
	{
	public:
		using RegisterResult = WindowClassRegisteringResult;
		using UnregisterResult = WindowClassUnregisteringResult;
		using Hash = WindowClassNameAtomHash;
		using Pair = WindowClassNameAtomPair;
	private:
		class Set final
		{
		public:
			using Unique = ::std::unique_ptr<Pair>;
			using AtomToData = ::std::unordered_map<Pair::Atom, Unique>;
			using NameToData = ::std::unordered_map<Pair::NameView, ptrc<Pair>>;
		public:
			void Insert(Unique&& data_ptr);
			tsize Erase(Pair::Atom atom);
			tsize Erase(Pair::Name name);
			ptrc<Pair> Find(Pair::Atom atom) const;
			ptrc<Pair> Find(Pair::NameView name) const;
		public:
			AtomToData atom_to_data;
			NameToData name_to_data;
		};
	public:
		[[nodiscard]] RegisterResult Register(const WindowClassArgs& wrapped_window_class) noexcept(false);
		[[nodiscard]] UnregisterResult Unregister(Pair::Atom class_atom) noexcept;
		tsize UnregisterAll() noexcept;
		[[nodiscard]] boolean Check(Pair::Atom class_atom) const noexcept;
		[[nodiscard]] boolean Empty() const noexcept;
		[[nodiscard]] const Pair::Name& operator[](Pair::Atom class_atom) const noexcept;
		[[nodiscard]] Pair::Atom operator[](Pair::NameView class_name) const noexcept;
	private:
		static [[nodiscard]] WindowClassSet& Instance();
	private:
		WindowClassSet() = default;
		~WindowClassSet();
	public:
		WindowClassSet(const WindowClassSet&) = delete;
		WindowClassSet(WindowClassSet&&) noexcept = delete;
		WindowClassSet& operator= (const WindowClassSet&) = delete;
		WindowClassSet& operator= (WindowClassSet&&) = delete;
	private:
		Set pt_set;
	public:
		boolean unregister_all_when_deconstruction{ true };
		friend WindowClassSet& IWindowClassSet();
	};
}

namespace Petal
{
	template <typename Callable>
		requires requires (Callable callable, win32hwnd window_handle)
	{
		{ callable(window_handle) } -> std::convertible_to<ptr<Abstract::Window>>;
	}
	win32lres CommonWindowProcess(Callable callable, win32hwnd window_handle, win32msg message, win32wprm w_param, win32lprm l_param)
	{
		ptr<Abstract::Window> win{ IWindow::WindowFromCreateEvent(message, l_param) };
		if (win) win->Bind(window_handle);
		win = callable(window_handle);
		if (win && window_handle == win->WindowHandle()) return win->Process(message, w_param, l_param);
		return IWindow::DefaultSysWndProc(window_handle, message, w_param, l_param);
	}
	template <typename Callable>
		requires requires (Callable callable, win32hwnd window_handle)
	{
		{ callable(window_handle) } -> std::convertible_to<Abstract::Window&>;
	}
	win32lres CommonWindowProcess(Callable callable, win32hwnd window_handle, win32msg message, win32wprm w_param, win32lprm l_param)
	{
		ptr<Abstract::Window> win{ IWindow::WindowFromCreateEvent(message, l_param) };
		if (win) win->Bind(window_handle);
		Abstract::Window& user_win = callable(window_handle);
		if (window_handle == user_win.WindowHandle()) return user_win.Process(message, w_param, l_param);
		return IWindow::DefaultSysWndProc(window_handle, message, w_param, l_param);
	}
	template <typename Callable>
		requires requires (Callable callable)
	{
		{ callable() } -> std::convertible_to<ptr<Abstract::Window>>;
	}
	win32lres CommonWindowProcess(Callable callable, win32hwnd window_handle, win32msg message, win32wprm w_param, win32lprm l_param)
	{
		ptr<Abstract::Window> win{ IWindow::WindowFromCreateEvent(message, l_param) };
		if (win) win->Bind(window_handle);
		win = callable();
		if (win && window_handle == win->WindowHandle()) return win->Process(message, w_param, l_param);
		return IWindow::DefaultSysWndProc(window_handle, message, w_param, l_param);
	}
	template <typename Callable>
		requires requires (Callable callable)
	{
		{ callable() } -> std::convertible_to<Abstract::Window&>;
	}
	win32lres CommonWindowProcess(Callable callable, win32hwnd window_handle, win32msg message, win32wprm w_param, win32lprm l_param)
	{
		ptr<Abstract::Window> win{ IWindow::WindowFromCreateEvent(message, l_param) };
		if (win) win->Bind(window_handle);
		Abstract::Window& user_win = callable();
		if (window_handle == user_win.WindowHandle()) return user_win.Process(message, w_param, l_param);
		return IWindow::DefaultSysWndProc(window_handle, message, w_param, l_param);
	}
}

#endif // !Petal_Header_WindowManger
