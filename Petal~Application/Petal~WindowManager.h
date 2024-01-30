#pragma once

#ifndef Petal_Header_WindowManger
#define Petal_Header_WindowManger

#include "Petal~WinTypes.h"
#include "Petal~Types.h"
#include "Petal~String.h"
#include "Petal~Process.h"

#ifndef Petal_Enable_IWindowSetImplByHashMap
#include <map>
#endif
#include <unordered_map>
#include <unordered_set>
#include <list>
#include <memory>

namespace Petal
{
	class WindowClassArgs;
	class WindowCreatingArgs;
	class WindowClassSet;
	class WindowSet;
	class WindowClassNameAtomHash;
	enum class WindowOperationResultCondition : i16
	{
		Unknown = 0,
		Success = 1,
		Win32,
		PetalFramework,
	};
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
		using Condition = WindowOperationResultCondition;
		enum class Error : i16
		{
			Unknown = 0,
			CannotFindPair,
		};
		Condition condition{ Condition::Success };
		Error framework_error{ Error::Unknown };
		win32bool value{ win32_true };
		win32error win32_error{ win32_no_error };
	};
	struct WindowCreatingResult final
	{
		using Condition = WindowOperationResultCondition;
		enum class Error : i16
		{
			Unknown = 0,
			CannotFindWindowClassFromIWindowClassSet,
			WindowHasBeenCreated,
		};
		Condition condition{ Condition::Success };
		Error framework_error{ Error::Unknown };
		win32error win32_error{ win32_no_error };
		win32hwnd window_handle{ nullptr };
	};
	struct WindowDestroyingResult final
	{
		using Condition = WindowOperationResultCondition;
		enum class Error : i16
		{
			Unknown = 0,
			CannotFindWindowFromIWindowSet,
			FailedWhenEraseFromIWindowSet,
		};
		Condition condition{ Condition::Success };
		Error framework_error{ Error::Unknown };
		win32bool return_value{ win32_false };
		win32error win32_error{ win32_no_error };
	};
	WindowClassSet& IWindowClassSet();
	WindowSet& IWindowSet();
	i32 MessageLoop(win32hwnd window_handle = nullptr, win32msg message_filter_min = 0, win32msg message_filter_max = 0);
	i32 MessageLoop(Abstract::Process<>& user_process, boolean remove = true, boolean yield = true, win32hwnd window_handle = nullptr, win32msg message_filter_min = 0, win32msg message_filter_max = 0);
	void ExitMessageLoop(i32 exit_code = 0) noexcept;
}

namespace Petal::IWin32
{
	win32lres DefaultWindowProcess(const win32hwnd& hwnd, const win32msg& msg, const win32wprm& w, const win32lprm& l) noexcept;
	[[nodiscard]] win32ctstr ToWinResource(word integer) noexcept;
	[[nodiscard]] win32hicon LoadDefaultWinAppIcon() noexcept;
	[[nodiscard]] win32hcursor LoadDefaultWinAppCursor() noexcept;
	[[nodiscard]] win32lptr WindowLongPtrGet(win32hwnd hwnd, i32 index) noexcept;
}

namespace Petal::Framework::Impl
{
	class WindowAccessor; // non export, used only for .cpp impl
}

namespace Petal
{
	class WindowClassNameAtomHash final
	{
	public:
		::std::size_t operator() (const ::std::unique_ptr<WindowClassNameAtomPair>& o) const noexcept(noexcept(::std::declval<::std::hash<win32atom>>().operator()({})));
	};
	class WindowClassArgs final
	{
	public:
		using RegisterResult = WindowClassRegisteringResult;
	private:
		static constexpr [[nodiscard]] auto DefaultWindowProcess() noexcept -> decltype(WindowClass::lpfnWndProc);
	public:
		[[nodiscard]] WindowClass BuildWindowClass() const noexcept;
		[[nodiscard]] const TString& ClassName() const noexcept;
		[[nodiscard]] const TString& MenuName() const noexcept;
		void UpdateClassName(const TString& class_name) noexcept(noexcept(StringToCStyleString(class_name)));
		void UpdateMenuName(const TString& menu_name) noexcept(noexcept(StringToCStyleString(menu_name)));
		void UsingMenuName(const TString& menu_name) noexcept(noexcept(::std::declval<WindowClassArgs>().UpdateMenuName(menu_name)));
		void UsingMenuName(word menu_resource) noexcept;
		void EnableDoubleClickMessage(boolean enable = true) noexcept;
		RegisterResult Register() const noexcept(false);
	public:
		WindowClassArgs() = default;
		WindowClassArgs(const TString& class_name, win32hicon icon = default_icon, win32hcursor cursor = default_cursor, win32hbrush background_brush = default_background_brush, win32hicon icon_sm = default_icon_sm, dword style = default_style, i32 class_extra = default_class_extra, i32 window_extra = default_window_extra);
		WindowClassArgs(const TString& class_name, const TString& menu_name, win32hicon icon = default_icon, win32hcursor cursor = default_cursor, win32hbrush background_brush = default_background_brush, win32hicon icon_sm = default_icon_sm, dword style = default_style, i32 class_extra = default_class_extra, i32 window_extra = default_window_extra);
		WindowClassArgs(const TString& class_name, word menu_resource, win32hicon icon = default_icon, win32hcursor cursor = default_cursor, win32hbrush background_brush = default_background_brush, win32hicon icon_sm = default_icon_sm, dword style = default_style, i32 class_extra = default_class_extra, i32 window_extra = default_window_extra);
		WindowClassArgs(const WindowClassArgs&) = default;
		WindowClassArgs(WindowClassArgs&&) noexcept = default;
		~WindowClassArgs() = default;
	public:
		static constexpr dword default_style{ CS_HREDRAW | CS_VREDRAW };
		static constexpr i32 default_class_extra{ 0 };
		static constexpr i32 default_window_extra{ 0 };
		static inline const win32hicon default_icon{ IWin32::LoadDefaultWinAppIcon() };
		static inline const win32hcursor default_cursor{ IWin32::LoadDefaultWinAppCursor() };
		static constexpr win32hbrush default_background_brush{ reinterpret_cast<win32hbrush>(COLOR_WINDOW) };
		static constexpr win32hicon default_icon_sm{ nullptr };
	private:
		// WIN32-RegisterClassEx requires that string WindowClass::lpszClassName not be nullptr and not equal to null_tstr.
		// WIN32-RegisterClassEx requires class_name's length less than 256 after processing(StringToCStyleString).
		TString class_name;
		// When using_int_menu_res == false:
		// If menu_name != null_tstr, method BuildWindowClass will assign menu_name.c_str() to lpszMenuName. 
		// If menu_name == null_tstr, method BuildWindowClass will assign nullptr to lpszMenuName. 
		TString menu_name;
	public:
		dword        style{ default_style };
		i32          class_extra{ default_class_extra };
		i32          window_extra{ default_window_extra };
		win32hicon   icon{ default_icon };
		win32hcursor cursor{ default_cursor };
		win32hbrush  background_brush{ default_background_brush };
		win32hicon   icon_sm{ default_icon_sm };
		word         menu_resource{ 0 };
		// Switch to fill WindowClass::lpszMenuName by menu_resource but not string menu_name when building WindowClass.
		boolean using_int_menu_res{ false };
		friend class WindowClassSet;
	};
}

namespace Petal
{
	class WindowCreatingArgs final
	{
	public:
		boolean UpdateTitle(const TString& new_title) noexcept;
		[[nodiscard]] const TString& Title() const noexcept;
	public:
		WindowCreatingArgs() = default;
		WindowCreatingArgs(const TString& title, const Size2DI32& size = default_size, const Position2DI32& position = default_position, dword style = default_style, dword ex_style = default_ex_style, win32hmenu menu = default_menu);
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
		virtual win32lres Process(win32msg message, win32wprm w, win32lprm l) noexcept = 0;
	public:
		[[nodiscard]] win32hwnd WindowHandle() const noexcept;
		[[nodiscard]] boolean Valid() const noexcept;
		CreateResult Create(win32atom class_atom, const WindowCreatingArgs& args = {}) noexcept(false);
		DestroyResult Destroy() noexcept;
	protected:
		[[nodiscard]] dword WindowStyle() const noexcept;
		[[nodiscard]] dword WindowExStyle() const noexcept;
	public:
		Window() = default;
		Window(const Window&) = delete;
		Window(Window&&) noexcept = delete;
		virtual ~Window() noexcept;
		Window& operator= (const Window&) = delete;
		Window& operator= (Window&&) = delete;
	private:
		win32hwnd window_handle{ nullptr };
		friend class Framework::Impl::WindowAccessor;
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
		[[nodiscard]] UnregisterResult Unregister(Pair::Atom class_atom) noexcept(false);
		tsize UnregisterAll() noexcept(false);
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
		Set set;
		friend class WindowSet;
		friend WindowClassSet& IWindowClassSet();
	};
}

namespace Petal
{
	class WindowSet final
	{
	public:
		using CreateResult = WindowCreatingResult;
		using DestroyResult = WindowDestroyingResult;
	private:
#ifdef Petal_Enable_IWindowSetImplByHashMap
		using Set = ::std::unordered_map<win32hwnd, ptr<Abstract::Window>>;
#else
		using Set = ::std::map<win32hwnd, ptr<Abstract::Window>>;
#endif
		using AutoDestroyFailedRecordList = ::std::list<DestroyResult>;
	public:
		[[nodiscard]] CreateResult Create(Abstract::Window& target_window, win32atom class_atom, const WindowCreatingArgs& args = {}) noexcept(false);
		[[nodiscard]] DestroyResult Destroy(Abstract::Window& window) noexcept;
		tsize DestroyAll() noexcept(false);
		[[nodiscard]] boolean Check(const Abstract::Window& window) const noexcept;
		[[nodiscard]] boolean Empty() const noexcept;
		[[nodiscard]] ptr<Abstract::Window> operator[] (win32hwnd index) const noexcept;
	public:
		void UpdateQuitWhenEmpty(boolean should_quit_when_empty) noexcept;
		boolean QuitWhenEmpty() const noexcept;
		boolean ShouldQuit() const noexcept;
	private:
		static [[nodiscard]] WindowSet& Instance();
	private:
		WindowSet() = default;
		~WindowSet() = default;
	public:
		WindowSet(const WindowSet&) = delete;
		WindowSet(WindowSet&&) noexcept = delete;
		WindowSet& operator= (const WindowSet&) = delete;
		WindowSet& operator= (WindowSet&&) = delete;
	public:
		AutoDestroyFailedRecordList auto_destroy_failed_record{};
	private:
		Set set{};
		boolean quit_when_empty{ true };
		friend WindowSet& IWindowSet();
	};
}

#endif // !Petal_Header_WindowManger
