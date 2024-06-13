#pragma once

#ifndef Petal_Header_Window
#define Petal_Header_Window

#include "Petal~WindowMessage.h"
#include "Petal~WindowBase.h"
#include "Petal~Types.h"
#include "Petal~String.h"

#include <optional>

#ifdef CreateEvent
#undef CreateEvent
#endif

namespace Petal
{
	enum class WindowShowCode : int
	{
		Hide = SW_HIDE,
		Normal = SW_NORMAL,
		Max = SW_MAXIMIZE,
		NoActiveNormal = SW_SHOWNOACTIVATE,
		Show = SW_SHOW,
		Min = SW_MINIMIZE,
		NoActiveMin = SW_SHOWMINNOACTIVE,
		NoActiveShow = SW_SHOWNA,
		Restore = SW_RESTORE,
		DefaultShow = SW_SHOWDEFAULT,
		ForceMin = SW_FORCEMINIMIZE,
	};

	class Window : public Abstract::Window
	{
	public:
		using ShowCode = WindowShowCode;
	public:
		win32bool Show(ShowCode show_code = ShowCode::Show) noexcept;
		win32bool ShowNormal() noexcept { return this->Show(ShowCode::Normal); }
		win32bool ShowMaximize() noexcept { return this->Show(ShowCode::Max); }
		win32bool ShowMinimize() noexcept { return this->Show(ShowCode::Min); }
		win32bool Hide() noexcept { return this->Show(ShowCode::Hide); }
		win32bool ShowNoBorder(boolean no_border_mode = true) noexcept;
		win32bool Repaint(win32uint flags) noexcept;
		win32bool UpdateWindow() noexcept { return ::UpdateWindow(this->WindowHandle()); }
		auto WindowRect() const noexcept -> ::std::optional<Win32Rect>;
		auto ClientRect() const noexcept -> ::std::optional<Win32Rect>;
		auto ClientSize() const noexcept -> ::std::optional<Size2DI32>;
		win32bool WindowRect(Win32Rect& rect) const noexcept;
		win32bool ClientRect(Win32Rect& rect) const noexcept;
		win32bool ClientSize(Size2DI32& size) const noexcept;
		win32bool UpdateTitle(ptrc<TChar> new_title) noexcept;
		win32bool UpdateTitleByString(TStringView new_title);
		win32bool Resize(i32 width, i32 height) noexcept;
		win32bool Resize(const Size2DI32& new_size) noexcept { return this->Resize(new_size.width, new_size.height); }
		win32bool ResizeClient(i32 width, i32 height) noexcept;
		win32bool ResizeClient(const Size2DI32& new_size) noexcept { return ResizeClient(new_size.width, new_size.height); }
		win32bool MoveTo(i32 x, i32 y) noexcept;
		win32bool MoveTo(const Position2DI32& new_pos) noexcept { return this->MoveTo(new_pos.x, new_pos.y); }
	public:
		virtual win32lres Process(win32msg message, win32wprm w_param, win32lprm l_param) override;
	protected:
		virtual void CreateEvent(CreateMessage& e) {}
		virtual void ActiveEvent(ActiveMessage& e) {}
		virtual void InactiveEvent(InactiveMessage& e) {}
		virtual void EnterSizeEvent(EnterSizeMessage& e) {}
		virtual void EnterMoveEvent(EnterMoveMessage& e) {}
		virtual void SizingEvent(SizingMessage& e) {}
		virtual void MovingEvent(MovingMessage& e) {}
		virtual void ExitSizeEvent(ExitSizeMessage& e) {}
		virtual void ExitMoveEvent(ExitMoveMessage& e) {}
		virtual void ResizeEvent(ResizeMessage& e) {}
		virtual void MaximizedEvent(MaximizedMessage& e) {}
		virtual void MinimizedEvent(MinimizedMessage& e) {}
		virtual void MovedEvent(MovedMessage& e) {}
		virtual void CloseEvent(CloseMessage& e) { this->Destroy(); ExitMessageLoop(); }
		virtual void PaintEvent(PaintMessage& e) { this->DefaultDraw(e); }
		virtual void DestroyEvent(DestroyMessage& e) {}
		virtual void MouseMoveEvent(MouseMoveMessage& e) {}
		virtual void MouseLButtonDownEvent(MouseLButtonDownMessage& e) {}
		virtual void MouseLButtonUpEvent(MouseLButtonUpMessage& e) {}
		virtual void MouseLButtonDoubleClickEvent(MouseLButtonDoubleClickMessage& e) {}
		virtual void MouseRButtonDownEvent(MouseRButtonDownMessage& e) {}
		virtual void MouseRButtonUpEvent(MouseRButtonUpMessage& e) {}
		virtual void MouseRButtonDoubleClickEvent(MouseRButtonDoubleClickMessage& e) {}
		virtual void MouseMButtonDownEvent(MouseMButtonDownMessage& e) {}
		virtual void MouseMButtonUpEvent(MouseMButtonUpMessage& e) {}
		virtual void MouseMButtonDoubleClickEvent(MouseMButtonDoubleClickMessage& e) {}
		virtual void MouseXButton1DownEvent(MouseXButton1DownMessage& e) {}
		virtual void MouseXButton1UpEvent(MouseXButton1UpMessage& e) {}
		virtual void MouseXButton1DoubleClickEvent(MouseXButton1DoubleClickMessage& e) {}
		virtual void MouseXButton2DownEvent(MouseXButton2DownMessage& e) {}
		virtual void MouseXButton2UpEvent(MouseXButton2UpMessage& e) {}
		virtual void MouseXButton2DoubleClickEvent(MouseXButton2DoubleClickMessage& e) {}
		virtual void MouseWheelEvent(MouseWheelMessage& e) {}
		virtual void MouseHWheelEvent(MouseHWheelMessage& e) {}
		virtual void KeyDownEvent(KeyDownMessage& e) {}
		virtual void KeyUpEvent(KeyUpMessage& e) {}
		virtual void SysKeyDownEvent(SysKeyDownMessage& e) {}
		virtual void SysKeyUpEvent(SysKeyUpMessage& e) {}
		virtual void CharEvent(CharMessage& e) {}
		virtual void DeadCharEvent(DeadCharMessage& e) {}
		virtual void SysDeadCharEvent(SysDeadCharMessage& e) {}
	public:
		win32lres DefaultDraw(PaintMessage& e) noexcept;
	public:
		Window() = default;
		Window(const Window&) = delete;
		virtual ~Window()
		{
#ifdef Petal_Enable_BindWindowLifeCycle
			if (this->Valid())
			{
				this->Destroy();
			}
#endif
		}
		Window& operator= (const Window&) = delete;
	public:
		Size2DI32 minimum_client_size{ 120, 90 };
	private:
		win32dword former_style{};
		boolean enter_size{ false };
		boolean enter_move{ false };
		boolean no_border_mode{ false };
	public:
		boolean using_default_get_min_max_info_process{ false };
	};

	inline win32bool Window::Show(ShowCode show_code) noexcept
	{
		return ::ShowWindow(this->WindowHandle(), static_cast<int>(show_code));
	}
	inline win32bool Window::Repaint(win32uint flags) noexcept
	{
		return ::RedrawWindow(this->WindowHandle(), nullptr, nullptr, flags);
	}
	inline auto Window::WindowRect() const noexcept -> ::std::optional<Win32Rect>
	{
		Win32Rect rect{};
		win32bool result{ this->WindowRect(rect) };
		return result ? ::std::make_optional<Win32Rect>(rect) : ::std::nullopt;
	}
	inline auto Window::ClientRect() const noexcept -> ::std::optional<Win32Rect>
	{
		Win32Rect rect{};
		win32bool result{ this->ClientRect(rect) };
		return result ? ::std::make_optional<Win32Rect>(rect) : ::std::nullopt;
	}
	inline auto Window::ClientSize() const noexcept -> ::std::optional<Size2DI32>
	{
		Size2DI32 size{};
		win32bool result{ this->ClientSize(size) };
		return result ? ::std::make_optional<Size2DI32>(size) : ::std::nullopt;
	}
	inline win32bool Window::WindowRect(Win32Rect& rect) const noexcept
	{
		return ::GetWindowRect(this->WindowHandle(), &rect);
	}
	inline win32bool Window::ClientRect(Win32Rect& rect) const noexcept
	{
		return ::GetClientRect(this->WindowHandle(), &rect);
	}
	inline win32bool Window::ClientSize(Size2DI32& size) const noexcept
	{
		Win32Rect rect{};
		win32bool result{ this->ClientRect(rect) };
		size = { rect.right - rect.left, rect.bottom - rect.top };
		return result;
	}
	inline win32bool Window::ResizeClient(i32 width, i32 height) noexcept
	{
		Win32Rect rect{ 0, 0, width, height };
		win32bool result{ ::AdjustWindowRectEx(&rect, this->GWLP_Style(), ::GetMenu(this->WindowHandle()) != nullptr, this->GWLP_ExStyle()) };
		return result ? this->Resize(rect.right - rect.left, rect.bottom - rect.top) : result;
	}
	inline win32bool Window::UpdateTitle(ptrc<TChar> new_title) noexcept
	{
#ifdef Petal_Enable_Unicode
		return ::SetWindowTextW(this->WindowHandle(), new_title);
#else
		return ::SetWindowTextA(this->WindowHandle(), new_title);
#endif
	}
	inline win32bool Window::UpdateTitleByString(TStringView new_title)
	{
		return this->UpdateTitle(StringToCStyleString(new_title).c_str());
	}
	inline win32bool Window::Resize(i32 width, i32 height) noexcept
	{
		return ::SetWindowPos(this->WindowHandle(), nullptr, 0, 0, width, height, SWP_NOMOVE);
	}
	inline win32bool Window::MoveTo(i32 x, i32 y) noexcept
	{
		return ::SetWindowPos(this->WindowHandle(), nullptr, x, y, 0, 0, SWP_NOSIZE);
	}
	inline win32lres Window::DefaultDraw(PaintMessage& e) noexcept
	{
		return IWindow::SystemDefWndProc(this->WindowHandle(), e.Message(), e.Word(), e.Long());
	}
}

#endif // !Petal_Header_Window
