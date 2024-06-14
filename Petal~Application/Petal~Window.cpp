#include "Petal~Window.h"

#include "Petal~VSDebugOutput.h"

namespace Petal
{
	win32bool Window::ShowNoBorder(boolean no_border_mode) noexcept
	{
		constexpr win32dword no_border_style{ WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP };
		if (this->no_border_mode == no_border_mode)
		{
			return win32_true;
		}
		
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

		this->no_border_mode = no_border_mode;

		return ::SetWindowPos(this->WindowHandle(), nullptr, 0, 0, client_rect.right - client_rect.left, client_rect.bottom - client_rect.top, SWP_NOMOVE);
	}
	win32lres Window::Process(win32msg msg, win32wprm w, win32lprm l)
	{
		switch (msg)
		{
		case WM_CREATE:
		{
			CreatingMessage wrapped_msg{ msg, w, l };
			this->CreatingEvent(wrapped_msg);
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
}
