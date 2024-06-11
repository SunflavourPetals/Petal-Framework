#include "Petal~Window.h"

#include "Petal~VSDebugOutput.h"

namespace Petal
{
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
		constexpr win32dword no_border_style{ WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP };
		if (this->no_border_mode == no_border_mode)
		{
			return win32_true;
		}
		this->no_border_mode = no_border_mode;
		if (no_border_mode == true)
		{
			this->former_style = static_cast<win32dword>(this->GWLP_Style());
		}

		win32dword target_style{ (no_border_mode == true) ? no_border_style : this->former_style };

		Win32Rect client_rect{};
		if (this->ClientRect(client_rect) == win32_false)
		{
			return win32_false;
		}
		if (::AdjustWindowRectEx(&client_rect, target_style, ::GetMenu(this->WindowHandle()) != nullptr, static_cast<win32dword>(this->GWLP_ExStyle())) == win32_false)
		{
			return win32_false;
		}
		IWindow::UpdateWindowLongPtr(this->WindowHandle(), GWL_STYLE, target_style);
		return ::SetWindowPos(this->WindowHandle(), nullptr, 0, 0, client_rect.right - client_rect.left, client_rect.bottom - client_rect.top, SWP_NOMOVE);
	}
	win32bool Window::Repaint(win32uint flags) noexcept
	{
		return ::RedrawWindow(this->WindowHandle(), nullptr, nullptr, flags);
	}
	win32bool Window::UpdateWindow() noexcept
	{
		return ::UpdateWindow(this->WindowHandle());
	}
	::std::optional<Win32Rect> Window::WindowRect() const noexcept
	{
		Win32Rect rect{};
		win32bool result{ this->WindowRect(rect) };
		if (result == win32_false)
		{
			return ::std::nullopt;
		}
		return ::std::make_optional<Win32Rect>(rect);
	}
	::std::optional<Win32Rect> Window::ClientRect() const noexcept
	{
		Win32Rect rect{};
		win32bool result{ this->ClientRect(rect) };
		if (result == win32_false)
		{
			return ::std::nullopt;
		}
		return ::std::make_optional<Win32Rect>(rect);
	}
	::std::optional<Size2DI32> Window::ClientSize() const noexcept
	{
		Size2DI32 size{};
		win32bool result{ this->ClientSize(size) };
		if (result == win32_false)
		{
			return ::std::nullopt;
		}
		return ::std::make_optional<Size2DI32>(size);
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
		win32bool result{ ::AdjustWindowRectEx(&rect, this->GWLP_Style(), ::GetMenu(this->WindowHandle()) != nullptr, this->GWLP_ExStyle()) };
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
	void Window::CreateEvent(CreateMessage& e) {}
	void Window::ActiveEvent(ActiveMessage& e) {}
	void Window::InactiveEvent(InactiveMessage& e) {}
	void Window::EnterSizeEvent(EnterSizeMessage& e) {}
	void Window::EnterMoveEvent(EnterMoveMessage& e) {}
	void Window::SizingEvent(SizingMessage& e) {}
	void Window::MovingEvent(MovingMessage& e) {}
	void Window::ExitSizeEvent(ExitSizeMessage& e) {}
	void Window::ExitMoveEvent(ExitMoveMessage& e) {}
	void Window::ResizeEvent(ResizeMessage& e) {}
	void Window::MaximizedEvent(MaximizedMessage& e) {}
	void Window::MinimizedEvent(MinimizedMessage& e) {}
	void Window::MovedEvent(MovedMessage& e) {}
	void Window::CloseEvent(CloseMessage& e) { this->Destroy(); ExitMessageLoop(); }
	void Window::PaintEvent(PaintMessage& e) { this->DefaultDraw(e); }
	void Window::DestroyEvent(DestroyMessage& e) {}
	void Window::MouseMoveEvent(MouseMoveMessage& e) {}
	void Window::MouseLButtonDownEvent(MouseLButtonDownMessage& e) {}
	void Window::MouseLButtonUpEvent(MouseLButtonUpMessage& e) {}
	void Window::MouseLButtonDoubleClickEvent(MouseLButtonDoubleClickMessage& e) {}
	void Window::MouseRButtonDownEvent(MouseRButtonDownMessage& e) {}
	void Window::MouseRButtonUpEvent(MouseRButtonUpMessage& e) {}
	void Window::MouseRButtonDoubleClickEvent(MouseRButtonDoubleClickMessage& e) {}
	void Window::MouseMButtonDownEvent(MouseMButtonDownMessage& e) {}
	void Window::MouseMButtonUpEvent(MouseMButtonUpMessage& e) {}
	void Window::MouseMButtonDoubleClickEvent(MouseMButtonDoubleClickMessage& e) {}
	void Window::MouseXButton1DownEvent(MouseXButton1DownMessage& e) {}
	void Window::MouseXButton1UpEvent(MouseXButton1UpMessage& e) {}
	void Window::MouseXButton1DoubleClickEvent(MouseXButton1DoubleClickMessage& e) {}
	void Window::MouseXButton2DownEvent(MouseXButton2DownMessage& e) {}
	void Window::MouseXButton2UpEvent(MouseXButton2UpMessage& e) {}
	void Window::MouseXButton2DoubleClickEvent(MouseXButton2DoubleClickMessage& e) {}
	void Window::MouseWheelEvent(MouseWheelMessage& e) {}
	void Window::MouseHWheelEvent(MouseHWheelMessage& e) {}
	void Window::KeyDownEvent(KeyDownMessage& e) {}
	void Window::KeyUpEvent(KeyUpMessage& e) {}
	void Window::SysKeyDownEvent(SysKeyDownMessage& e) {}
	void Window::SysKeyUpEvent(SysKeyUpMessage& e) {}
	void Window::CharEvent(CharMessage& e) {}
	void Window::DeadCharEvent(DeadCharMessage& e) {}
	void Window::SysDeadCharEvent(SysDeadCharMessage& e) {}
	win32lres Window::Process(win32msg msg, win32wprm w, win32lprm l)
	{
		switch (msg)
		{
		case WM_CREATE:
		{
			CreateMessage wrapped_msg{ msg, w, l };
			this->CreateEvent(wrapped_msg);
			return wrapped_msg.continue_creation ? 0 : -1;
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
				return IWindow::SystemDefWndProc(this->WindowHandle(), msg, w, l);
				break;
			}
			return 0;
		}
		break;
		case WM_SIZING:
		{
			if (this->enter_size == false)
			{
				EnterSizeMessage wrapped_msg{ msg, w, l };
				this->enter_size = true;
				this->EnterSizeEvent(wrapped_msg);
			}
			SizingMessage wrapped_msg{ msg, w, l };
			this->SizingEvent(wrapped_msg);
			return win32_true;
		}
		break;
		case WM_MOVING:
		{
			if (this->enter_move == false)
			{
				EnterMoveMessage wrapped_msg{ msg, w, l };
				this->enter_move = true;
				this->EnterMoveEvent(wrapped_msg);
			}
			MovingMessage wrapped_msg{ msg, w, l };
			this->MovingEvent(wrapped_msg);
			return win32_true;
		}
		break;
		case WM_EXITSIZEMOVE:
		{
			if (this->enter_size == true)
			{
				ExitSizeMessage wrapped_msg{ msg, w, l };
				this->enter_size = false;
				this->ExitSizeEvent(wrapped_msg);
			}
			if (this->enter_move == true)
			{
				ExitMoveMessage wrapped_msg{ msg, w, l };
				this->enter_move = false;
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
				return IWindow::SystemDefWndProc(this->WindowHandle(), msg, w, l);
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
			if (this->using_default_get_min_max_info_process)
			{
				return IWindow::SystemDefWndProc(this->WindowHandle(), msg, w, l);
			}
			Win32Rect rect{ 0, 0, this->minimum_client_size.width, this->minimum_client_size.height };
			::AdjustWindowRectEx(&rect, this->GWLP_Style(), ::GetMenu(this->WindowHandle()) != NULL, this->GWLP_ExStyle());
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
		{
			DestroyMessage wrapped_msg{ msg, w, l };
			this->DestroyEvent(wrapped_msg);
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
				return win32_true;
			}
			break;
			case XBUTTON2:
			{
				MouseXButton2DownMessage wrapped_msg{ msg, w, l };
				this->MouseXButton2DownEvent(wrapped_msg);
				return win32_true;
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
				return win32_true;
			}
			break;
			case XBUTTON2:
			{
				MouseXButton2UpMessage wrapped_msg{ msg, w, l };
				this->MouseXButton2UpEvent(wrapped_msg);
				return win32_true;
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
				return win32_true;
			}
			break;
			case XBUTTON2:
			{
				MouseXButton2DoubleClickMessage wrapped_msg{ msg, w, l };
				this->MouseXButton2DoubleClickEvent(wrapped_msg);
				return win32_true;
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
		return IWindow::SystemDefWndProc(this->WindowHandle(), msg, w, l);
	}
	win32lres Window::DefaultDraw(PaintMessage& e) noexcept
	{
		return IWindow::SystemDefWndProc(this->WindowHandle(), e.Message(), 0, 0);
	}
}
