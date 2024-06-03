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
		win32bool ShowNormal() noexcept;
		win32bool ShowMaximize() noexcept;
		win32bool ShowMinimize() noexcept;
		win32bool Hide() noexcept;
		win32bool ShowNoBorder(boolean no_border_mode = true) noexcept;
		win32bool Repaint(win32uint flags = RDW_INTERNALPAINT || RDW_UPDATENOW || RDW_ALLCHILDREN) noexcept;
		win32bool UpdateWindow() noexcept;
		::std::optional<Win32Rect> WindowRect() const noexcept;
		::std::optional<Win32Rect> ClientRect() const noexcept;
		::std::optional<Size2DI32> ClientSize() const noexcept;
		win32bool WindowRect(Win32Rect& rect) const noexcept;
		win32bool ClientRect(Win32Rect& rect) const noexcept;
		win32bool ClientSize(Size2DI32& size) const noexcept;
		win32bool UpdateTitle(ptrc<TChar> new_title) noexcept;
		win32bool UpdateTitle(const TString& new_title) noexcept;
		win32bool Resize(i32 width, i32 height) noexcept;
		win32bool Resize(const Size2DI32& new_size) noexcept;
		win32bool MoveTo(i32 x, i32 y) noexcept;
		win32bool MoveTo(const Position2DI32& new_pos) noexcept;
	public:
		virtual win32lres Process(win32msg message, win32wprm w_param, win32lprm l_param) override;
	protected:
		virtual void CreateEvent(CreateMessage& e);
		virtual void ActiveEvent(ActiveMessage& e);
		virtual void InactiveEvent(InactiveMessage& e);
		virtual void EnterSizeEvent(EnterSizeMessage& e);
		virtual void EnterMoveEvent(EnterMoveMessage& e);
		virtual void SizingEvent(SizingMessage& e);
		virtual void MovingEvent(MovingMessage& e);
		virtual void ExitSizeEvent(ExitSizeMessage& e);
		virtual void ExitMoveEvent(ExitMoveMessage& e);
		virtual void ResizeEvent(ResizeMessage& e);
		virtual void MaximizedEvent(MaximizedMessage& e);
		virtual void MinimizedEvent(MinimizedMessage& e);
		virtual void MovedEvent(MovedMessage& e);
		virtual void CloseEvent(CloseMessage& e);
		virtual void PaintEvent(PaintMessage& e);
		virtual void DestroyEvent(DestroyMessage& e);
		virtual void MouseMoveEvent(MouseMoveMessage& e);
		virtual void MouseLButtonDownEvent(MouseLButtonDownMessage& e);
		virtual void MouseLButtonUpEvent(MouseLButtonUpMessage& e);
		virtual void MouseLButtonDoubleClickEvent(MouseLButtonDoubleClickMessage& e);
		virtual void MouseRButtonDownEvent(MouseRButtonDownMessage& e);
		virtual void MouseRButtonUpEvent(MouseRButtonUpMessage& e);
		virtual void MouseRButtonDoubleClickEvent(MouseRButtonDoubleClickMessage& e);
		virtual void MouseMButtonDownEvent(MouseMButtonDownMessage& e);
		virtual void MouseMButtonUpEvent(MouseMButtonUpMessage& e);
		virtual void MouseMButtonDoubleClickEvent(MouseMButtonDoubleClickMessage& e);
		virtual void MouseXButton1DownEvent(MouseXButton1DownMessage& e);
		virtual void MouseXButton1UpEvent(MouseXButton1UpMessage& e);
		virtual void MouseXButton1DoubleClickEvent(MouseXButton1DoubleClickMessage& e);
		virtual void MouseXButton2DownEvent(MouseXButton2DownMessage& e);
		virtual void MouseXButton2UpEvent(MouseXButton2UpMessage& e);
		virtual void MouseXButton2DoubleClickEvent(MouseXButton2DoubleClickMessage& e);
		virtual void MouseWheelEvent(MouseWheelMessage& e);
		virtual void MouseHWheelEvent(MouseHWheelMessage& e);
		virtual void KeyDownEvent(KeyDownMessage& e);
		virtual void KeyUpEvent(KeyUpMessage& e);
		virtual void SysKeyDownEvent(SysKeyDownMessage& e);
		virtual void SysKeyUpEvent(SysKeyUpMessage& e);
		virtual void CharEvent(CharMessage& e);
		virtual void DeadCharEvent(DeadCharMessage& e);
		virtual void SysDeadCharEvent(SysDeadCharMessage& e);
	public:
		win32lres DefaultDraw(PaintMessage& e) noexcept;
	public:
		Window() = default;
		Window(const Window&) = delete;
		Window(Window&&) noexcept = delete;
		virtual ~Window() = default;
		Window& operator= (const Window&) = delete;
		Window& operator= (Window&&) = delete;
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
}

#endif // !Petal_Header_Window
