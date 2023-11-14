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
		[[nodiscard]] win32atom RegisterPetalWindowClass(const WindowClass& window_class) noexcept;
		[[nodiscard]] win32bool UnregisterPetalWindowClass(word class_atom) noexcept;
		[[nodiscard]] win32hwnd CreatePetalWindow(win32atom class_atom, Abstract::Window& window, const WindowCreatingArgs& args) noexcept;
		[[nodiscard]] win32bool GetWinMessage(Win32Message& message, win32hwnd hwnd, win32msg filter_min, win32msg filter_max) noexcept;
		[[nodiscard]] win32bool PeekWinMessage(Win32Message& message, win32hwnd hwnd, win32msg filter_min, win32msg filter_max, win32msg remove) noexcept;
		void MessageProcess(Win32Message& message) noexcept;
		win32lres CALLBACK PetalWindowProcess(win32hwnd window_handle, win32msg message, win32wprm w, win32lprm l) noexcept;
	}
}

namespace Petal::Abstract
{
	Window::~Window()
	{
		if (this->WindowHandle() != nullptr)
		{
			this->Destroy();
		}
	}
	[[nodiscard]] win32hwnd Window::WindowHandle() const noexcept
	{
		return this->window_handle;
	}
	[[nodiscard]] boolean Window::Valid() const noexcept
	{
		return this->window_handle != nullptr;
	}
	win32error Window::Create(win32atom class_atom, const WindowCreatingArgs& args) noexcept(false)
	{
		return IWindowSet().Create(*this, class_atom, args).error;
	}
	Window::DestroyResult Window::Destroy() noexcept(false)
	{
		return IWindowSet().Destroy(*this);
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
	::std::size_t WindowClassNameAtomHash::operator() (const ::std::unique_ptr<WindowClassNameAtomPair>& o) const noexcept(noexcept(::std::declval<::std::hash<win32atom>>().operator()({})))
	{
		::std::hash<win32atom> hash{};
		if (o.get() == nullptr) return hash(0);
		return hash(o->atom);
	}
	class WindowAccessor final
	{
	public:
		static void Assign(Abstract::Window& target_window, win32hwnd resource)
		{
			target_window.window_handle = resource;
		}
	};
}

namespace Petal
{
	WindowClassArgs::WindowClassArgs(const TString& class_name, win32hicon icon, win32hcursor cursor, win32hbrush background_brush, win32hicon icon_sm, dword style, i32 class_extra, i32 window_extra) :
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
	WindowClassArgs::WindowClassArgs(const TString& class_name, const TString& menu_name, win32hicon icon, win32hcursor cursor, win32hbrush background_brush, win32hicon icon_sm, dword style, i32 class_extra, i32 window_extra) :
		WindowClassArgs(class_name, icon, cursor, background_brush, icon_sm, style, class_extra, window_extra)
	{
		this->UsingMenuName(menu_name);
	}
	WindowClassArgs::WindowClassArgs(const TString& class_name, word menu_resource, win32hicon icon, win32hcursor cursor, win32hbrush background_brush, win32hicon icon_sm, dword style, i32 class_extra, i32 window_extra) :
		WindowClassArgs(class_name, icon, cursor, background_brush, icon_sm, style, class_extra, window_extra)
	{
		this->UsingMenuName(menu_resource);
	}
	constexpr [[nodiscard]] auto WindowClassArgs::DefaultWindowProcess() noexcept -> decltype(WindowClass::lpfnWndProc)
	{
		return &PetalUnnamed::IWin32::PetalWindowProcess;
	}
	[[nodiscard]] WindowClass WindowClassArgs::BuildWindowClass() const noexcept
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
	[[nodiscard]] const TString& WindowClassArgs::ClassName() const noexcept
	{
		return this->class_name;
	}
	[[nodiscard]] const TString& WindowClassArgs::MenuName() const noexcept
	{
		return this->menu_name;
	}
	void WindowClassArgs::UpdateClassName(const TString& class_name) noexcept(noexcept(StringToCStyleString(class_name)))
	{
		this->class_name = StringToCStyleString(class_name);
	}
	void WindowClassArgs::UpdateMenuName(const TString& menu_name) noexcept(noexcept(StringToCStyleString(menu_name)))
	{
		this->menu_name = StringToCStyleString(menu_name);
	}
	void WindowClassArgs::UsingMenuName(const TString& menu_name) noexcept(noexcept(::std::declval<WindowClassArgs>().UpdateMenuName(menu_name)))
	{
		this->UpdateMenuName(menu_name);
		this->using_int_menu_res = false;
	}
	void WindowClassArgs::UsingMenuName(word menu_resource) noexcept
	{
		this->menu_resource = menu_resource;
		this->using_int_menu_res = true;
	}
	void WindowClassArgs::EnableDoubleClickMessage(boolean enable) noexcept
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
	WindowClassArgs::RegisterResult WindowClassArgs::Register() noexcept(false)
	{
		return IWindowClassSet().Register(*this);
	}
}

namespace Petal
{
	WindowCreatingArgs::WindowCreatingArgs(const TString& title, const Size2DI32& size, const Position2DI32& position, dword style, dword ex_style, win32hmenu menu) :
		position(position),
		size(size),
		ex_style(ex_style),
		style(style),
		menu(menu)
	{
		this->UpdateTitle(title);
	}
	boolean WindowCreatingArgs::UpdateTitle(const TString& new_title) noexcept
	{
		try
		{
			this->window_title = StringToCStyleString(new_title);
		}
#ifdef Petal_Enable_VSDebugOutput
		catch (const ::std::exception& e)
#else
		catch (const ::std::exception&)
#endif
		{
			Petal_VSDbgT("[Petal] std::exception: ");
			Petal_VSDbgA(e.what());
			Petal_VSDbgT("\r\n");
			return false;
		}
		return true;
	}
	[[nodiscard]] const TString& WindowCreatingArgs::Title() const noexcept
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
	[[nodiscard]] WindowClassSet::RegisterResult WindowClassSet::Register(const WindowClassArgs& wrapped_window_class) noexcept(false)
	{
		RegisterResult result{};
		WindowClass window_class{ wrapped_window_class.BuildWindowClass() };
		result.class_atom = PetalUnnamed::IWin32::RegisterPetalWindowClass(window_class);

		if (result.class_atom == 0)
		{
			result.error = ::GetLastError();
			try
			{
				Petal_VSDbgT("[Petal] Failed in WindowClassSet::Register method!\r\n");
				Petal_VSDbgT("\t\tclass_name: \"");
				Petal_VSDebugOutput(wrapped_window_class.ClassName().c_str());
				Petal_VSDbgT("\"\r\n");
				Petal_VSDbg(::std::format(Petal_DbgStr("\t\terror code: {}\r\n"), result.error).c_str());
			}
			catch (const ::std::exception&) {}

			return result;
		}

		::std::unique_ptr<Pair> data_ptr{ ::std::make_unique<Pair>(wrapped_window_class.ClassName(), result.class_atom) };

		this->set.Insert(::std::move(data_ptr));

		try
		{
			Petal_VSDbg(::std::format(Petal_DbgStr("[Petal] Window class(atom:{}) has been registered"), result.class_atom).c_str());
			Petal_VSDbgT("\r\n");
		}
		catch (const ::std::exception&) {}

		return result;
	}
	[[nodiscard]] WindowClassSet::UnregisterResult WindowClassSet::Unregister(Pair::Atom class_atom) noexcept(false)
	{
		UnregisterResult result{};

		auto debug_output
		{
			[](const Pair::Atom& class_atom, const UnregisterResult& result) noexcept
			{
				try
				{
					Petal_VSDbgT("[Petal] Failed in WindowClassSet::Unregister series method!\r\n");
					Petal_VSDbg(::std::format(Petal_DbgStr("\t\tin class_atom: {}\r\n"), class_atom).c_str());
					switch (result.condition)
					{
					case WindowClassSet::UnregisterResult::Condition::Win32:
						Petal_VSDbg(::std::format(Petal_DbgStr("\t\tWin32: error code {}\r\n"), result.win32_error).c_str());
						break;
					case WindowClassSet::UnregisterResult::Condition::PetalFramework:
						switch (result.framework_error)
						{
						case WindowClassSet::UnregisterResult::Error::Unknown:
							Petal_VSDbgT("\t\tPetal: unknown reason\r\n");
							break;
						case WindowClassSet::UnregisterResult::Error::CannotFindPair:
							Petal_VSDbgT("\t\tPetal: cannot find atom in IWindowClassSet()\r\n");
							break;
						default:
							Petal_VSDbg(::std::format(Petal_DbgStr("\t\tPetal: error code {}\r\n"), static_cast<i16>(result.framework_error)).c_str());
							break;
						}
						break;
					default:
						Petal_VSDbgT("\t\tUnknown reason\r\n");
						break;
					}
					
				}
				catch (const ::std::exception&) {}
			}
		};

		auto data{ this->set.Find(class_atom) };
		if (data == nullptr)
		{
			result.condition = UnregisterResult::Condition::PetalFramework;
			result.framework_error = UnregisterResult::Error::CannotFindPair;
			result.value = FALSE;
			debug_output(class_atom, result);
			return result;
		}

		result.value = PetalUnnamed::IWin32::UnregisterPetalWindowClass(class_atom);

		if (result.value == FALSE)
		{
			result.condition = UnregisterResult::Condition::Win32;
			result.win32_error = ::GetLastError();
			debug_output(class_atom, result);
			return result;
		}

		this->set.Erase(class_atom);

		try
		{
			Petal_VSDbg(::std::format(Petal_DbgStr("[Petal] Window class(atom:{}) has been unregistered"), class_atom).c_str());
			Petal_VSDbgT("\r\n");
		}
		catch (const ::std::exception&) {}

		return result;
	}
	tsize WindowClassSet::UnregisterAll() noexcept(false)
	{
		::std::vector<win32atom> atom_set;
		for (const auto& e : this->set.atom_to_data)
		{
			atom_set.push_back(e.first);
		}
		for (auto&& e : atom_set)
		{
			auto result{ this->Unregister(e) };
		}
		return this->set.atom_to_data.size();
	}
	[[nodiscard]] boolean WindowClassSet::Check(Pair::Atom class_atom) const noexcept
	{
		try
		{
			auto data{ this->set.Find(class_atom) };
			if (data == nullptr)
			{
				return false;
			}
			return true;
		}
#ifdef Petal_Enable_VSDebugOutput
		catch (const ::std::exception& e)
#else
		catch (const ::std::exception&)
#endif
		{
			try
			{
				Petal_VSDbgT("[Petal] std::exception: ");
				Petal_VSDbgA(e.what());
				Petal_VSDbgT("\r\n");
			}
			catch (const ::std::exception&) {}
			return false;
		}
	}
	[[nodiscard]] boolean WindowClassSet::Empty() const noexcept
	{
		return this->set.atom_to_data.empty();
	}
	[[nodiscard]] const TString& WindowClassSet::operator[](Pair::Atom class_atom) const noexcept
	{
		static const TString null_tstr{};
		try
		{
			auto info_pair{ this->set.Find(class_atom) };
			if (info_pair == nullptr)
			{
				return null_tstr;
			}
			return info_pair->name;
		}
#ifdef Petal_Enable_VSDebugOutput
		catch (const ::std::exception& e)
#else
		catch (const ::std::exception&)
#endif
		{
			try
			{
				Petal_VSDbgT("[Petal] std::exception: ");
				Petal_VSDbgA(e.what());
				Petal_VSDbgT("\r\n");
			}
			catch (const ::std::exception&) {}
			return null_tstr;
		}
		return null_tstr;
	}
	[[nodiscard]] WindowClassSet::Pair::Atom WindowClassSet::operator[](Pair::NameView class_name) const noexcept
	{
		try
		{
			auto info_pair{ this->set.Find(class_name) };
			if (info_pair == nullptr)
			{
				return 0;
			}
			return info_pair->atom;
		}
#ifdef Petal_Enable_VSDebugOutput
		catch (const ::std::exception& e)
#else
		catch (const ::std::exception&)
#endif
		{
			try
			{
				Petal_VSDbgT("[Petal] std::exception: ");
				Petal_VSDbgA(e.what());
				Petal_VSDbgT("\r\n");
			}
			catch (const ::std::exception&) {}
			return 0;
		}
		return 0;
	}
	[[nodiscard]] WindowClassSet& WindowClassSet::Instance()
	{
		static WindowClassSet window_class_set{};
		return window_class_set;
	}

	void WindowClassSet::Set::Insert(WindowClassSet::Set::Unique&& data_ptr)
	{
		if (data_ptr == nullptr)
		{
			Petal_VSDbgT("[Petal] Failed in Petal::WindowClassSet::Set::Insert: data_ptr is nullptr\r\n");
			return;
		}
		this->name_to_data[data_ptr->name] = data_ptr.get();
		this->atom_to_data[data_ptr->atom] = ::std::move(data_ptr);
	}
	tsize WindowClassSet::Set::Erase(WindowClassSet::Pair::Atom atom)
	{
		auto result = this->atom_to_data.find(atom);
		if (result == this->atom_to_data.end())
		{
			return 0;
		}
		this->name_to_data.erase(result->second->name);
		return this->atom_to_data.erase(atom);
	}
	tsize WindowClassSet::Set::Erase(WindowClassSet::Pair::Name name)
	{
		auto result = this->name_to_data.find(name);
		if (result == this->name_to_data.end())
		{
			return 0;
		}
		Pair::Atom target_atom = result->second->atom;
		this->name_to_data.erase(result->second->name);
		return this->atom_to_data.erase(target_atom);
	}
	ptrc<WindowClassSet::Pair> WindowClassSet::Set::Find(WindowClassSet::Pair::Atom atom) const
	{
		auto result{ this->atom_to_data.find(atom) };
		if (result == this->atom_to_data.end())
		{
			return nullptr;
		}
		return result->second.get();
	}
	ptrc<WindowClassSet::Pair> WindowClassSet::Set::Find(WindowClassSet::Pair::NameView name) const
	{
		auto result{ this->name_to_data.find(name) };
		if (result == this->name_to_data.end())
		{
			return nullptr;
		}
		return result->second;
	}
}

namespace Petal
{
	[[nodiscard]] WindowSet::CreateResult WindowSet::Create(Abstract::Window& target_window, win32atom class_atom, const WindowCreatingArgs& args) noexcept(false)
	{
		CreateResult result{};
		if (IWindowClassSet().Check(class_atom) == false)
		{
			result.error = ERROR_CANNOT_FIND_WND_CLASS;
			try
			{
				Petal_VSDbgT("[Petal] Failed in WindowSet::Create method!\r\n");
				Petal_VSDbg(::std::format(Petal_DbgStr("\t\tCan not find class_atom: {} in window_class_set\r\n"), class_atom).c_str());
			}
			catch (const ::std::exception&) {}

			goto return_label;
		}

		result.window_handle = PetalUnnamed::IWin32::CreatePetalWindow(class_atom, target_window, args);

		if (result.window_handle == nullptr)
		{
			result.error = ::GetLastError();
			try
			{
				Petal_VSDbgT("[Petal] Failed in WindowSet::Create method!\r\n");
				Petal_VSDbg(::std::format(Petal_DbgStr("\t\tclass_atom: \"{}\"\r\n"), class_atom).c_str());
				Petal_VSDbg(::std::format(Petal_DbgStr("\t\terror code: {}\r\n"), result.error).c_str());
			}
			catch (const ::std::exception&) {}
			
			goto return_label;
		}

		WindowAccessor::Assign(target_window, result.window_handle);

		this->set[result.window_handle] = &target_window;

		try
		{
			Petal_VSDbg(::std::format(Petal_DbgStr("[Petal] Window(handle:{}) has been created\r\n"), static_cast<void*>(result.window_handle)).c_str());
		}
		catch (const ::std::exception&) {}

		return_label:

		if (IWindowSet().Empty())
		{
			ExitMessageLoop();
		}

		return result;
	}
	[[nodiscard]] WindowSet::DestroyResult WindowSet::Destroy(Abstract::Window& window) noexcept(false)
	{
		DestroyResult result{};

		auto debug_output
		{
			[](const Abstract::Window& window, const DestroyResult& result) noexcept
			{
				try
				{
					Petal_VSDbgT("[Petal] Failed in WindowSet::Destroy series method!\r\n");
					Petal_VSDbg(::std::format(Petal_DbgStr("\t\tWindow_handle: {}\r\n"), static_cast<ptr<void>>(window.WindowHandle())).c_str());
					Petal_VSDbg(::std::format(Petal_DbgStr("\t\terror code: {}\r\n"), result.error).c_str());
				}
				catch (const ::std::exception&) {}
			}
		};

		decltype(this->set.find(window.WindowHandle())) window_pair;
		try
		{
			window_pair = this->set.find(window.WindowHandle());
		}
		catch (const ::std::exception&)
		{
			window_pair = this->set.end();
		}

		if (window_pair == this->set.end())
		{
			result.error = ERROR_INVALID_WINDOW_HANDLE;
			result.value = FALSE;
			debug_output(window, result);

			goto return_label;
		}
		result.value = ::DestroyWindow(window.WindowHandle());

		if (result.value == FALSE)
		{
			result.error = ::GetLastError();
			debug_output(window, result);

			goto return_label;
		}

		decltype(this->set.erase(window.WindowHandle())) erase_count;

		if constexpr (noexcept(this->set.erase(window.WindowHandle())) == false)
		{
			try
			{
				erase_count = this->set.erase(window.WindowHandle());
			}
			catch (const ::std::exception&)
			{
				erase_count = 0;
			}
		}
		else
		{
			erase_count = this->set.erase(window.WindowHandle());
		}

		try
		{
			Petal_VSDbg(::std::format(Petal_DbgStr("[Petal] Window(handle:{}) has been destroyed\r\n"), static_cast<void*>(window.WindowHandle())).c_str());
		}
		catch (const ::std::exception&) {}

		WindowAccessor::Assign(window, nullptr);

		return_label:

		if (IWindowSet().Empty() == true)
		{
			ExitMessageLoop();
		}

		return result;
	}
	tsize WindowSet::DestroyAll() noexcept(false)
	{
		::std::vector<ptr<Abstract::Window>> win_ptr_set;
		for (const auto& e : this->set)
		{
			win_ptr_set.push_back(e.second);
		}
		for (auto& e : win_ptr_set)
		{
			auto result{ this->Destroy(*e) };
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
		catch (const ::std::exception& e)
#else
		catch (const ::std::exception&)
#endif
		{
			try
			{
				Petal_VSDbgT("[Petal] std::exception: ");
				Petal_VSDbgA(e.what());
				Petal_VSDbgT("\r\n");
			}
			catch (const ::std::exception&) {}
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
		catch (const ::std::exception& e)
#else
		catch (const ::std::exception&)
#endif
		{
			try
			{
				Petal_VSDbgT("[Petal] std::exception: ");
				Petal_VSDbgA(e.what());
				Petal_VSDbgT("\r\n");
			}
			catch (const ::std::exception&) {}
			return nullptr;
		}
	}
	[[nodiscard]] WindowSet& WindowSet::Instance()
	{
		static WindowSet window_set{};
		return window_set;
	}
}

namespace Petal
{
	WindowClassSet& IWindowClassSet()
	{
		return WindowClassSet::Instance();
	}

	WindowSet& IWindowSet()
	{
		return WindowSet::Instance();
	}

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
		win32lres CALLBACK PetalWindowProcess(win32hwnd window_handle, win32msg message, win32wprm w, win32lprm l) noexcept
		{
			try
			{
				ptr<Abstract::Window> window{ IWindowSet()[window_handle] };
				if (window != nullptr)
				{
					return window->Process(message, w, l);
				}
				else
				{
					if (message == WM_CREATE)
					{
						ptrc<Win32CreateStruct> create_struct_ptr{ reinterpret_cast<ptr<Win32CreateStruct>>(l) };
						if (create_struct_ptr != nullptr && create_struct_ptr->lpCreateParams != nullptr)
						{
							Abstract::Window& window{ *reinterpret_cast<ptr<Abstract::Window>>(create_struct_ptr->lpCreateParams) };
							WindowAccessor::Assign(window, window_handle);
							auto result{ window.Process(message, w, l) };
							WindowAccessor::Assign(window, {});
							return result;
						}
					}
				}
			}
			catch (const ::std::exception&) {}
			return Petal::IWin32::DefaultWindowProcess(window_handle, message, w, l);
		}
#ifdef Petal_Enable_Unicode
		[[nodiscard]] win32atom RegisterPetalWindowClass(const WindowClass& window_class) noexcept
		{
			return ::RegisterClassExW(&window_class);
		}
		[[nodiscard]] win32bool UnregisterPetalWindowClass(word class_atom) noexcept
		{
			return ::UnregisterClassW(Petal::IWin32::ToWinResource(class_atom), WinMain::hins);
		}
		[[nodiscard]] win32hwnd CreatePetalWindow(win32atom class_atom, Abstract::Window& window, const WindowCreatingArgs& args) noexcept
		{
			Win32Rect rect{ 0, 0, args.size.width, args.size.height };
			::AdjustWindowRectEx(&rect, args.style, args.menu != nullptr, args.ex_style);
			i32 width{ rect.right - rect.left };
			i32 height{ rect.bottom - rect.top };
			return ::CreateWindowExW
			(
				args.ex_style,
				Petal::IWin32::ToWinResource(class_atom),
				args.Title().c_str(),
				args.style,
				args.position.x,
				args.position.y,
				width,
				height,
				nullptr,
				args.menu,
				WinMain::hins,
				static_cast<::LPVOID>(&window)
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
		[[nodiscard]] win32hwnd CreatePetalWindow(win32atom class_atom, Abstract::Window& window, const WindowCreatingArgs& args) noexcept
		{
			Win32Rect rect{ 0, 0, args.size.width, args.size.height };
			::AdjustWindowRectEx(&rect, args.style, args.menu != nullptr, args.ex_style);
			i32 width{ rect.right - rect.left };
			i32 height{ rect.bottom - rect.top };
			return ::CreateWindowExA
			(
				args.ex_style,
				Petal::IWin32::ToWinResource(class_atom),
				args.Title().c_str(),
				args.style,
				args.position.x,
				args.position.y,
				width,
				height,
				nullptr,
				args.menu,
				WinMain::hins,
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
