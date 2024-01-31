#pragma once

#ifndef Petal_Header_Window
#define Petal_Header_Window

#include "Petal~WindowManager.h"
#include "Petal~WindowMessage.h"
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
		boolean MainWindowState() const noexcept;
		void UpdateMainWindowState(boolean new_state) noexcept;

		const Size2DI32& MinimumSize() const noexcept;
		void UpdateMinimumSize(const Size2DI32& new_size) noexcept;
		void UpdateMinimumSize(i32 new_width, i32 new_height) noexcept;

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
	protected:
		virtual win32lres Process(win32msg msg, win32wprm w, win32lprm l) noexcept override;
		virtual void CreateEvent(CreateMessage& e) noexcept;
		virtual void ActiveEvent(ActiveMessage& e) noexcept;
		virtual void InactiveEvent(InactiveMessage& e) noexcept;
		virtual void EnterSizeEvent(EnterSizeMessage& e) noexcept;
		virtual void EnterMoveEvent(EnterMoveMessage& e) noexcept;
		virtual void SizingEvent(SizingMessage& e) noexcept;
		virtual void MovingEvent(MovingMessage& e) noexcept;
		virtual void ExitSizeEvent(ExitSizeMessage& e) noexcept;
		virtual void ExitMoveEvent(ExitMoveMessage& e) noexcept;
		virtual void ResizeEvent(ResizeMessage& e) noexcept;
		virtual void MaximizedEvent(MaximizedMessage& e) noexcept;
		virtual void MinimizedEvent(MinimizedMessage& e) noexcept;
		virtual void MovedEvent(MovedMessage& e) noexcept;
		virtual void CloseEvent(CloseMessage& e) noexcept;
		virtual void PaintEvent(PaintMessage& e) noexcept;
		virtual void MouseMoveEvent(MouseMoveMessage& e) noexcept;
		virtual void MouseLButtonDownEvent(MouseLButtonDownMessage& e) noexcept;
		virtual void MouseLButtonUpEvent(MouseLButtonUpMessage& e) noexcept;
		virtual void MouseLButtonDoubleClickEvent(MouseLButtonDoubleClickMessage& e) noexcept;
		virtual void MouseRButtonDownEvent(MouseRButtonDownMessage& e) noexcept;
		virtual void MouseRButtonUpEvent(MouseRButtonUpMessage& e) noexcept;
		virtual void MouseRButtonDoubleClickEvent(MouseRButtonDoubleClickMessage& e) noexcept;
		virtual void MouseMButtonDownEvent(MouseMButtonDownMessage& e) noexcept;
		virtual void MouseMButtonUpEvent(MouseMButtonUpMessage& e) noexcept;
		virtual void MouseMButtonDoubleClickEvent(MouseMButtonDoubleClickMessage& e) noexcept;
		virtual void MouseXButton1DownEvent(MouseXButton1DownMessage& e) noexcept;
		virtual void MouseXButton1UpEvent(MouseXButton1UpMessage& e) noexcept;
		virtual void MouseXButton1DoubleClickEvent(MouseXButton1DoubleClickMessage& e) noexcept;
		virtual void MouseXButton2DownEvent(MouseXButton2DownMessage& e) noexcept;
		virtual void MouseXButton2UpEvent(MouseXButton2UpMessage& e) noexcept;
		virtual void MouseXButton2DoubleClickEvent(MouseXButton2DoubleClickMessage& e) noexcept;
		virtual void MouseWheelEvent(MouseWheelMessage& e) noexcept;
		virtual void MouseHWheelEvent(MouseHWheelMessage& e) noexcept;
		virtual void KeyDownEvent(KeyDownMessage& e) noexcept;
		virtual void KeyUpEvent(KeyUpMessage& e) noexcept;
		virtual void SysKeyDownEvent(SysKeyDownMessage& e) noexcept;
		virtual void SysKeyUpEvent(SysKeyUpMessage& e) noexcept;
		virtual void CharEvent(CharMessage& e) noexcept;
		virtual void DeadCharEvent(DeadCharMessage& e) noexcept;
		virtual void SysDeadCharEvent(SysDeadCharMessage& e) noexcept;
	public:
		win32lres DefaultDraw(PaintMessage& e) noexcept;
	public:
		Window() = default;
		Window(const Window&) = delete;
		Window(Window&&) noexcept = delete;
		virtual ~Window() = default;
		Window& operator= (const Window&) = delete;
		Window& operator= (Window&&) = delete;
	private:
		Size2DI32 pt_limit_client_size{ 120, 90 };
	private:
		dword pt_former_style{};
		boolean pt_main_window{ false };
		boolean pt_enter_size{ false };
		boolean pt_enter_move{ false };
		boolean pt_no_border_mode{ false };
	};
}

#endif // !Petal_Header_Window
