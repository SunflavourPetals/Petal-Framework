#include "Petal~Window.h"

#include "Petal~Exception.h"
#include "Petal~VSDebugOutput.h"

namespace Petal
{
	Window::Window(ShowCode show_code, win32atom class_atom, const WindowCreatingParameters& parameters, CreateResult& create_result)
	{
		create_result = this->Create(class_atom, parameters);
		this->Show(show_code);
		::UpdateWindow(this->WindowHandle());
	}
	Window::Window(win32atom class_atom, const WindowCreatingParameters& parameters, ShowCode show_code)
	{
		auto create_result{ this->Create(class_atom, parameters) };
		this->Show(show_code);
		::UpdateWindow(this->WindowHandle());
	}
	Window::CreateResult Window::Create(win32atom class_atom, const WindowCreatingParameters& parameters)
	{
		return IWindowSet().Create(*this, class_atom, parameters);
	}
	Window::CreateResult Window::Create(const WrappedWindowClass& wrapped_window_class, const WindowCreatingParameters& parameters, RegisterResult& register_result)
	{
		register_result = IWindowClassSet().Register(wrapped_window_class);
		return this->Create(register_result.class_atom, parameters);
	}
	Window::CreateResult Window::Create(const WrappedWindowClass& wrapped_window_class, const WindowCreatingParameters& parameters)
	{
		RegisterResult register_result{};
		return this->Create(wrapped_window_class, parameters, register_result);
	}
	auto Window::Destroy() noexcept(noexcept(IWindowSet().Destroy(*(static_cast<ptr<Window>>(nullptr))))) -> decltype(IWindowSet().Destroy(*this))
	{
		return IWindowSet().Destroy(*this);
	}
	win32bool Window::Show(ShowCode show_code) noexcept
	{
		return ::ShowWindow(this->WindowHandle(), static_cast<int>(show_code));
	}
	win32bool Window::ShowNormal() noexcept
	{
		return this->Show(ShowCode::Normal);
	}
	win32bool Window::ShowMaximize() noexcept
	{
		return this->Show(ShowCode::Max);
	}
	win32bool Window::ShowMinimize() noexcept
	{
		return this->Show(ShowCode::Min);
	}
	win32bool Window::Hide() noexcept
	{
		return this->Show(ShowCode::Hide);
	}
	win32bool Window::ShowNoBorder(boolean no_border_mode) noexcept
	{
		constexpr dword no_border_style{ WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP };
		if (this->pt_no_border_mode == no_border_mode)
		{
			return TRUE;
		}
		this->pt_no_border_mode = no_border_mode;
		if (no_border_mode == true)
		{
			this->pt_former_style = static_cast<dword>(this->WindowStyle());
		}

		dword target_style{ (no_border_mode == true) ? no_border_style : this->pt_former_style };

		Win32Rect client_rect{};
		if (this->ClientRect(client_rect) == FALSE)
		{
			return FALSE;
		}
		if (::AdjustWindowRectEx(&client_rect, target_style, ::GetMenu(this->WindowHandle()) != nullptr, static_cast<dword>(this->WindowExStyle())) == FALSE)
		{
			return FALSE;
		}
#ifdef Petal_Enable_Unicode
		::SetWindowLongPtrW(this->WindowHandle(), GWL_STYLE, target_style);
#else
		::SetWindowLongPtrA(this->WindowHandle(), GWL_STYLE, target_style);
#endif
		return ::SetWindowPos(this->WindowHandle(), nullptr, 0, 0, client_rect.right - client_rect.left, client_rect.bottom - client_rect.top, SWP_NOMOVE);
	}
	void Window::Repaint() noexcept
	{
		this->Process(WM_PAINT, NULL, NULL);
	}
	win32bool Window::UpdateWindow() noexcept
	{
		return ::UpdateWindow(this->WindowHandle());
	}
	boolean Window::MainWindowState() const noexcept
	{
		return this->pt_main_window;
	}
	void Window::UpdateMainWindowState(boolean new_state) noexcept
	{
		this->pt_main_window = new_state;
	}
	const Size2DI32& Window::MinimumSize() const noexcept
	{
		return this->pt_limit_client_size;
	}
	void Window::UpdateMinimumSize(const Size2DI32& new_size) noexcept
	{
		this->pt_limit_client_size = new_size;
	}
	void Window::UpdateMinimumSize(i32 new_width, i32 new_height) noexcept
	{
		this->pt_limit_client_size = { new_width, new_height };
	}
	win32bool Window::WindowRect(Win32Rect& rect) const noexcept
	{
		return ::GetWindowRect(this->WindowHandle(), &rect);
	}
	win32bool Window::ClientRect(Win32Rect& rect) const noexcept
	{
		return ::GetClientRect(this->WindowHandle(), &rect);
	}
	win32bool Window::ClientSize(Size2DI32& size) const noexcept
	{
		Win32Rect rect{};
		win32bool result{ this->ClientRect(rect) };
		size = { rect.right - rect.left, rect.bottom - rect.top };
		return result;
	}
	win32bool Window::UpdateTitle(ptrc<TChar> new_title) noexcept
	{
#ifdef Petal_Enable_Unicode
		return ::SetWindowTextW(this->WindowHandle(), new_title);
#else
		return ::SetWindowTextA(this->WindowHandle(), new_title);
#endif
	}
	win32bool Window::UpdateTitle(const TString& new_title) noexcept
	{
		return UpdateTitle(new_title.c_str());
	}
	win32bool Window::Resize(i32 width, i32 height) noexcept
	{
		Win32Rect rect{ 0, 0, width, height };
		win32bool result{ ::AdjustWindowRectEx(&rect, this->WindowStyle(), ::GetMenu(this->WindowHandle()) != nullptr, this->WindowExStyle()) };
		if (result == FALSE)
		{
			return result;
		}
		i32 new_width{ rect.right - rect.left };
		i32 new_height{ rect.bottom - rect.top };
		return ::SetWindowPos(this->WindowHandle(), nullptr, 0, 0, new_width, new_height, SWP_NOMOVE);
	}
	win32bool Window::Resize(const Size2DI32& new_size) noexcept
	{
		return this->Resize(new_size.width, new_size.height);
	}
	win32bool Window::MoveTo(i32 x, i32 y) noexcept
	{
		return ::SetWindowPos(this->WindowHandle(), nullptr, x, y, 0, 0, SWP_NOSIZE);
	}
	win32bool Window::MoveTo(const Position2DI32& new_pos) noexcept
	{
		return this->MoveTo(new_pos.x, new_pos.y);
	}
	void Window::CreateEvent(CreateMessage& e) noexcept {}
	void Window::ActiveEvent(ActiveMessage& e) noexcept {}
	void Window::InactiveEvent(InactiveMessage& e) noexcept {}
	void Window::EnterSizeEvent(EnterSizeMessage& e) noexcept {}
	void Window::EnterMoveEvent(EnterMoveMessage& e) noexcept {}
	void Window::SizingEvent(SizingMessage& e) noexcept {}
	void Window::MovingEvent(MovingMessage& e) noexcept {}
	void Window::ExitSizeEvent(ExitSizeMessage& e) noexcept {}
	void Window::ExitMoveEvent(ExitMoveMessage& e) noexcept {}
	void Window::ResizeEvent(ResizeMessage& e) noexcept {}
	void Window::MaximizedEvent(MaximizedMessage& e) noexcept {}
	void Window::MinimizedEvent(MinimizedMessage& e) noexcept {}
	void Window::MovedEvent(MovedMessage& e) noexcept {}
	void Window::CloseEvent(CloseMessage& e) noexcept { try { this->Destroy(); } catch (const Exception& e) { e; Petal_VSDbgExcep(e.Desc()); } }
	void Window::PaintEvent(PaintMessage& e) noexcept { IWin32::DefaultWindowProcess(this->WindowHandle(), e.Message(), 0, 0); }
	void Window::MouseMoveEvent(MouseMoveMessage& e) noexcept {}
	void Window::MouseLButtonDownEvent(MouseLButtonDownMessage& e) noexcept {}
	void Window::MouseLButtonUpEvent(MouseLButtonUpMessage& e) noexcept {}
	void Window::MouseLButtonDoubleClickEvent(MouseLButtonDoubleClickMessage& e) noexcept {}
	void Window::MouseRButtonDownEvent(MouseRButtonDownMessage& e) noexcept {}
	void Window::MouseRButtonUpEvent(MouseRButtonUpMessage& e) noexcept {}
	void Window::MouseRButtonDoubleClickEvent(MouseRButtonDoubleClickMessage& e) noexcept {}
	void Window::MouseMButtonDownEvent(MouseMButtonDownMessage& e) noexcept {}
	void Window::MouseMButtonUpEvent(MouseMButtonUpMessage& e) noexcept {}
	void Window::MouseMButtonDoubleClickEvent(MouseMButtonDoubleClickMessage& e) noexcept {}
	void Window::MouseXButton1DownEvent(MouseXButton1DownMessage& e) noexcept {}
	void Window::MouseXButton1UpEvent(MouseXButton1UpMessage& e) noexcept {}
	void Window::MouseXButton1DoubleClickEvent(MouseXButton1DoubleClickMessage& e) noexcept {}
	void Window::MouseXButton2DownEvent(MouseXButton2DownMessage& e) noexcept {}
	void Window::MouseXButton2UpEvent(MouseXButton2UpMessage& e) noexcept {}
	void Window::MouseXButton2DoubleClickEvent(MouseXButton2DoubleClickMessage& e) noexcept {}
	void Window::MouseWheelEvent(MouseWheelMessage& e) noexcept {}
	void Window::MouseHWheelEvent(MouseHWheelMessage& e) noexcept {}
	void Window::KeyDownEvent(KeyDownMessage& e) noexcept {}
	void Window::KeyUpEvent(KeyUpMessage& e) noexcept {}
	void Window::SysKeyDownEvent(SysKeyDownMessage& e) noexcept {}
	void Window::SysKeyUpEvent(SysKeyUpMessage& e) noexcept {}
	void Window::CharEvent(CharMessage& e) noexcept {}
	void Window::DeadCharEvent(DeadCharMessage& e) noexcept {}
	void Window::SysDeadCharEvent(SysDeadCharMessage& e) noexcept {}
	win32lres Window::Window::Process(win32msg msg, win32wprm w, win32lprm l) noexcept
	{
		//	dout + std::format("msg {:x}", msg) + lna;
		switch (msg)
		{
		case WM_CREATE:
		{
			CreateMessage wrapped_msg{ msg, w, l };
			this->CreateEvent(wrapped_msg);
			return 0; // Return -1 to block creating.
		}
		break;
		case WM_ACTIVATE:
		{
			switch (w)
			{
			case WA_ACTIVE:
			case WA_CLICKACTIVE:
			{
				ActiveMessage wrapped_msg{ msg, w, l };
				this->ActiveEvent(wrapped_msg);
				return 0;
			}
			break;
			case WA_INACTIVE:
			{
				InactiveMessage wrapped_msg{ msg, w, l };
				this->InactiveEvent(wrapped_msg);
				return 0;
			}
			break;
			default:
				return IWin32::DefaultWindowProcess(this->WindowHandle(), msg, w, l);
				break;
			}
			return 0;
		}
		break;
		case WM_SIZING:
		{
			if (this->pt_enter_size == false)
			{
				EnterSizeMessage wrapped_msg{ msg, w, l };
				this->pt_enter_size = true;
				this->EnterSizeEvent(wrapped_msg);
			}
			SizingMessage wrapped_msg{ msg, w, l };
			this->SizingEvent(wrapped_msg);
			return TRUE;
		}
		break;
		case WM_MOVING:
		{
			if (this->pt_enter_move == false)
			{
				EnterMoveMessage wrapped_msg{ msg, w, l };
				this->pt_enter_move = true;
				this->EnterMoveEvent(wrapped_msg);
			}
			MovingMessage wrapped_msg{ msg, w, l };
			this->MovingEvent(wrapped_msg);
			return TRUE;
		}
		break;
		case WM_EXITSIZEMOVE:
		{
			if (this->pt_enter_size == true)
			{
				ExitSizeMessage wrapped_msg{ msg, w, l };
				this->pt_enter_size = false;
				this->ExitSizeEvent(wrapped_msg);
			}
			if (this->pt_enter_move == true)
			{
				ExitMoveMessage wrapped_msg{ msg, w, l };
				this->pt_enter_move = false;
				this->ExitMoveEvent(wrapped_msg);
			}
			return 0;
		}
		break;
		case WM_SIZE:
		{
			switch (w)
			{
			case SIZE_RESTORED:
			{
				ResizeMessage wrapped_msg{ msg, w, l };
				this->ResizeEvent(wrapped_msg);
				return 0;
			}
			break;
			case SIZE_MINIMIZED:
			{
				MinimizedMessage wrapped_msg{ msg, w, l };
				this->MinimizedEvent(wrapped_msg);
				return 0;
			}
			break;
			case SIZE_MAXIMIZED:
			{
				MaximizedMessage wrapped_msg{ msg, w, l };
				this->MaximizedEvent(wrapped_msg);
				return 0;
			}
			break;
			case SIZE_MAXSHOW:
			case SIZE_MAXHIDE:
			default:
				return IWin32::DefaultWindowProcess(this->WindowHandle(), msg, w, l);
				break;
			}
			return 0;
		}
		break;
		case WM_MOVE:
		{
			MovedMessage wrapped_msg{ msg, w, l };
			this->MovedEvent(wrapped_msg);
			return 0;
		}
		break;
		case WM_GETMINMAXINFO:
		{
			Win32Rect rect{ 0, 0, pt_limit_client_size.width, pt_limit_client_size.height };
			::AdjustWindowRectEx(&rect, this->WindowStyle(), ::GetMenu(this->WindowHandle()) != NULL, this->WindowExStyle());
			reinterpret_cast<::MINMAXINFO*>(l)->ptMinTrackSize.x = rect.right - rect.left;
			reinterpret_cast<::MINMAXINFO*>(l)->ptMinTrackSize.y = rect.bottom - rect.top;
			return 0;
		}
		break;
		case WM_CLOSE:
		{
			CloseMessage wrapped_msg{ msg, w, l };
			this->CloseEvent(wrapped_msg);
			return 0;
		}
		break;
		case WM_DESTROY:
			if (this->MainWindowState() == true)
			{
				ExitMessageLoop();
				return 0;
			}
			break;
		case WM_PAINT:
		{
			PaintMessage wrapped_msg{ msg, w, l };
			this->PaintEvent(wrapped_msg);
			return 0;
		}
		break;
		case WM_MOUSEMOVE:
		{
			MouseMoveMessage wrapped_msg{ msg, w, l };
			this->MouseMoveEvent(wrapped_msg);
			return 0;
		}
		break;
		case WM_LBUTTONDOWN:
		{
			MouseLButtonDownMessage wrapped_msg{ msg, w, l };
			this->MouseLButtonDownEvent(wrapped_msg);
			return 0;
		}
		break;
		case WM_LBUTTONUP:
		{
			MouseLButtonUpMessage wrapped_msg{ msg, w, l };
			this->MouseLButtonUpEvent(wrapped_msg);
			return 0;
		}
		break;
		case WM_LBUTTONDBLCLK:
		{
			MouseLButtonDoubleClickMessage wrapped_msg{ msg, w, l };
			this->MouseLButtonDoubleClickEvent(wrapped_msg);
			return 0;
		}
		break;
		case WM_RBUTTONDOWN:
		{
			MouseRButtonDownMessage wrapped_msg{ msg, w, l };
			this->MouseRButtonDownEvent(wrapped_msg);
			return 0;
		}
		break;
		case WM_RBUTTONUP:
		{
			MouseRButtonUpMessage wrapped_msg{ msg, w, l };
			this->MouseRButtonUpEvent(wrapped_msg);
			return 0;
		}
		break;
		case WM_RBUTTONDBLCLK:
		{
			MouseRButtonDoubleClickMessage wrapped_msg{ msg, w, l };
			this->MouseRButtonDoubleClickEvent(wrapped_msg);
			return 0;
		}
		break;
		case WM_MBUTTONDOWN:
		{
			MouseMButtonDownMessage wrapped_msg{ msg, w, l };
			this->MouseMButtonDownEvent(wrapped_msg);
			return 0;
		}
		break;
		case WM_MBUTTONUP:
		{
			MouseMButtonUpMessage wrapped_msg{ msg, w, l };
			this->MouseMButtonUpEvent(wrapped_msg);
			return 0;
		}
		break;
		case WM_MBUTTONDBLCLK:
		{
			MouseMButtonDoubleClickMessage wrapped_msg{ msg, w, l };
			this->MouseMButtonDoubleClickEvent(wrapped_msg);
			return 0;
		}
		break;
		case WM_XBUTTONDOWN:
		{
			switch (GET_XBUTTON_WPARAM(w))
			{
			case XBUTTON1:
			{
				MouseXButton1DownMessage wrapped_msg{ msg, w, l };
				this->MouseXButton1DownEvent(wrapped_msg);
				return TRUE;
			}
			break;
			case XBUTTON2:
			{
				MouseXButton2DownMessage wrapped_msg{ msg, w, l };
				this->MouseXButton2DownEvent(wrapped_msg);
				return TRUE;
			}
			break;
			default:
				break;
			}
		}
		break;
		case WM_XBUTTONUP:
		{
			switch (GET_XBUTTON_WPARAM(w))
			{
			case XBUTTON1:
			{
				MouseXButton1UpMessage wrapped_msg{ msg, w, l };
				this->MouseXButton1UpEvent(wrapped_msg);
				return TRUE;
			}
			break;
			case XBUTTON2:
			{
				MouseXButton2UpMessage wrapped_msg{ msg, w, l };
				this->MouseXButton2UpEvent(wrapped_msg);
				return TRUE;
			}
			break;
			default:
				break;
			}
		}
		break;
		case WM_XBUTTONDBLCLK:
		{
			switch (GET_XBUTTON_WPARAM(w))
			{
			case XBUTTON1:
			{
				MouseXButton1DoubleClickMessage wrapped_msg{ msg, w, l };
				this->MouseXButton1DoubleClickEvent(wrapped_msg);
				return TRUE;
			}
			break;
			case XBUTTON2:
			{
				MouseXButton2DoubleClickMessage wrapped_msg{ msg, w, l };
				this->MouseXButton2DoubleClickEvent(wrapped_msg);
				return TRUE;
			}
			break;
			default:
				break;
			}
		}
		break;
		case WM_MOUSEWHEEL:
		{
			MouseWheelMessage wrapped_msg{ msg, w, l };
			this->MouseWheelEvent(wrapped_msg);
			return 0;
		}
		break;
		case WM_MOUSEHWHEEL:
		{
			MouseHWheelMessage wrapped_msg{ msg, w, l };
			this->MouseHWheelEvent(wrapped_msg);
			return 0;
		}
		break;
		case WM_KEYDOWN:
		{
			KeyDownMessage wrapped_msg{ msg, w, l };
			this->KeyDownEvent(wrapped_msg);
			return 0;
		}
		break;
		case WM_KEYUP:
		{
			KeyUpMessage wrapped_msg{ msg, w, l };
			this->KeyUpEvent(wrapped_msg);
			return 0;
		}
		break;
		case WM_SYSKEYDOWN:
		{
			SysKeyDownMessage wrapped_msg{ msg, w, l };
			this->SysKeyDownEvent(wrapped_msg);
			return 0;
		}
		break;
		case WM_SYSKEYUP:
		{
			SysKeyUpMessage wrapped_msg{ msg, w, l };
			this->SysKeyUpEvent(wrapped_msg);
			return 0;
		}
		break;
		case WM_CHAR:
		{
			CharMessage wrapped_msg{ msg, w, l };
			this->CharEvent(wrapped_msg);
			return 0;
		}
		break;
		case WM_DEADCHAR:
		{
			DeadCharMessage wrapped_msg{ msg, w, l };
			this->DeadCharEvent(wrapped_msg);
			return 0;
		}
		case WM_SYSDEADCHAR:
		{
			SysDeadCharMessage wrapped_msg{ msg, w, l };
			this->SysDeadCharEvent(wrapped_msg);
			return 0;
		}
		break;
		default:
			break;
		}
		return IWin32::DefaultWindowProcess(this->WindowHandle(), msg, w, l);
	}
}
