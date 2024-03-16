#pragma once

#ifndef Petal_Header_WindowMessage
#define Petal_Header_WindowMessage

#include "Petal~Types.h"
#include "Petal~WinTypes.h"
#include "Petal~String.h"

namespace Petal
{
	enum class Win32SizingEdge : i32
	{
		Left = WMSZ_LEFT,
		Right = WMSZ_RIGHT,
		Top = WMSZ_TOP,
		TopLeft = WMSZ_TOPLEFT,
		TopRight = WMSZ_TOPRIGHT,
		Bottom = WMSZ_BOTTOM,
		BottomLeft = WMSZ_BOTTOMLEFT,
		BottomRight = WMSZ_BOTTOMRIGHT,
	};
}

namespace Petal
{
	class BasicWindowMessage
	{
	public:
		win32msg Message() const noexcept;
	protected:
		win32wprm Word() const noexcept;
		win32lprm Long() const noexcept;
		word HWLong() const noexcept;
		word LWLong() const noexcept;
		word HWWord() const noexcept;
		word LWWord() const noexcept;
	public:
		BasicWindowMessage(win32msg msg, win32wprm w, win32lprm l);
		BasicWindowMessage(const BasicWindowMessage&) = default;
		BasicWindowMessage(BasicWindowMessage&&) noexcept = default;
		virtual ~BasicWindowMessage() = default;
	private:
		win32msg message{};
		win32wprm w{};
		win32lprm l{};
	};

	class BasicSizeMoveMessage : public BasicWindowMessage
	{
	public:
		Win32Rect& WinRect() noexcept;
		const Win32Rect& WinRect() const noexcept;
	public:
		BasicSizeMoveMessage(win32msg msg, win32wprm w, win32lprm l);
		BasicSizeMoveMessage(const BasicSizeMoveMessage&) = default;
		BasicSizeMoveMessage(BasicSizeMoveMessage&&) noexcept = default;
		virtual ~BasicSizeMoveMessage() = default;
	};

	class BasicResizeMessage : public BasicWindowMessage
	{
	public:
		const Size2DI32& NewClientSize() const noexcept;
	public:
		BasicResizeMessage(win32msg msg, win32wprm w, win32lprm l);
		BasicResizeMessage(const BasicResizeMessage&) = default;
		BasicResizeMessage(BasicResizeMessage&&) noexcept = default;
		virtual ~BasicResizeMessage() = default;
	private:
		Size2DI32 client_size{};
	};

	class BasicMouseMessage : public BasicWindowMessage
	{
	public:
		enum class VirtualKey : i32
		{
			LButton = MK_LBUTTON,
			RButton = MK_RBUTTON,
			Shift = MK_SHIFT,
			Control = MK_CONTROL,
			MButton = MK_MBUTTON,
			XButton1 = MK_XBUTTON1,
			XButton2 = MK_XBUTTON2,
		};
	public:
		word VirtualKeys() const noexcept;
		boolean LButtonDown() const noexcept;
		boolean RButtonDown() const noexcept;
		boolean ShiftDown() const noexcept;
		boolean ControlDown() const noexcept;
		boolean MButtonDown() const noexcept;
		boolean XButton1Down() const noexcept;
		boolean XButton2Down() const noexcept;
		const Position2DI32& CursorPosition() const noexcept;
	public:
		BasicMouseMessage(win32msg msg, win32wprm w, win32lprm l);
		BasicMouseMessage(const BasicMouseMessage&) = default;
		BasicMouseMessage(BasicMouseMessage&&) noexcept = default;
		virtual ~BasicMouseMessage() = default;
	private:
		Position2DI32 cursor_pos{};
		word key_state{};
	};

	class BasicMouseWheelMessage : public BasicMouseMessage
	{
	public:
		i16 WheelDelta() const noexcept;
	public:
		BasicMouseWheelMessage(win32msg msg, win32wprm w, win32lprm l);
		BasicMouseWheelMessage(const BasicMouseWheelMessage&) = default;
		BasicMouseWheelMessage(BasicMouseWheelMessage&&) noexcept = default;
		virtual ~BasicMouseWheelMessage() = default;
	private:
		i16 wheel_delta{};
	};

	class BasicKeyMessage : public BasicWindowMessage
	{
	public:
		u16 RepeatCount() const noexcept;
		u8 ScanCode() const noexcept;
		u8 LongParam25To32Bits() const noexcept;
	public:
		BasicKeyMessage(win32msg msg, win32wprm w, win32lprm l);
		BasicKeyMessage(const BasicKeyMessage&) = default;
		BasicKeyMessage(BasicKeyMessage&&) noexcept = default;
		virtual ~BasicKeyMessage() = default;
	};

	class BasicKeyPressMessage : public BasicKeyMessage
	{
	public:
		int VirtualKey() const noexcept;
	public:
		BasicKeyPressMessage(win32msg msg, win32wprm w, win32lprm l);
		BasicKeyPressMessage(const BasicKeyPressMessage&) = default;
		BasicKeyPressMessage(BasicKeyPressMessage&&) noexcept = default;
		virtual ~BasicKeyPressMessage() = default;
	};

	class BasicCharMessage : public BasicKeyMessage
	{
	public:
		TChar GetTChar() const noexcept;
		achar GetCharA() const noexcept;
		u8char GetCharU8() const noexcept;
		wchar GetCharW() const noexcept;
		u16char GetCharU16() const noexcept;
		boolean IsLowSurrogate() const noexcept;
		boolean IsHighSurrogate() const noexcept;
	public:
		BasicCharMessage(win32msg msg, win32wprm w, win32lprm l);
		BasicCharMessage(const BasicCharMessage&) = default;
		BasicCharMessage(BasicCharMessage&&) noexcept = default;
		virtual ~BasicCharMessage() = default;
	};
}

namespace Petal
{
	class CreateMessage : public BasicWindowMessage
	{
	public:
		const Win32CreateStruct& CreateStruct() const noexcept;
	public:
		CreateMessage(win32msg msg, win32wprm w, win32lprm l);
		CreateMessage(const CreateMessage&) = default;
		CreateMessage(CreateMessage&&) noexcept = default;
		virtual ~CreateMessage() = default;
	public:
		boolean continue_creation{ true };
	};
	class ActiveMessage : public BasicWindowMessage
	{
	public:
		boolean TriggeringByClick() const noexcept;
		win32hwnd Deactivated() const noexcept;
		boolean ValidDeactivated() const noexcept;
	public:
		ActiveMessage(win32msg msg, win32wprm w, win32lprm l);
		ActiveMessage(const ActiveMessage&) = default;
		ActiveMessage(ActiveMessage&&) noexcept = default;
		virtual ~ActiveMessage() = default;
	private:
		boolean clicked{ false };
	};

	class InactiveMessage : public BasicWindowMessage
	{
	public:
		win32hwnd Activate() const noexcept;
		boolean ValidActivate() const noexcept;
	public:
		InactiveMessage(win32msg msg, win32wprm w, win32lprm l);
		InactiveMessage(const InactiveMessage&) = default;
		InactiveMessage(InactiveMessage&&) noexcept = default;
		virtual ~InactiveMessage() = default;
	};

	using DestroyMessage = BasicWindowMessage;

	using EnterSizeMessage = BasicWindowMessage;

	using EnterMoveMessage = BasicWindowMessage;

	class SizingMessage : public BasicSizeMoveMessage
	{
	public:
		using Edge = Win32SizingEdge;
	public:
		Edge SizingEdge() noexcept;
	public:
		SizingMessage(win32msg msg, win32wprm w, win32lprm l);
		SizingMessage(const SizingMessage&) = default;
		SizingMessage(SizingMessage&&) noexcept = default;
		virtual ~SizingMessage() = default;
	};

	using MovingMessage = BasicSizeMoveMessage;

	using ExitSizeMessage = BasicWindowMessage;

	using ExitMoveMessage = BasicWindowMessage;

	using CloseMessage = BasicWindowMessage;

	using ResizeMessage = BasicResizeMessage;

	using MaximizedMessage = BasicResizeMessage;

	using MinimizedMessage = BasicResizeMessage;

	class MovedMessage : public BasicWindowMessage
	{
	public:
		const Position2DI32& NewClientPosition() const noexcept;
	public:
		MovedMessage(win32msg msg, win32wprm w, win32lprm l);
		MovedMessage(const MovedMessage&) = default;
		MovedMessage(MovedMessage&&) noexcept = default;
		virtual ~MovedMessage() = default;
	private:
		Position2DI32 pos{};
	};

	using PaintMessage = BasicWindowMessage;

	using MouseMoveMessage = BasicMouseMessage;

	using MouseLButtonDownMessage = BasicMouseMessage;

	using MouseLButtonUpMessage = BasicMouseMessage;

	using MouseLButtonDoubleClickMessage = BasicMouseMessage;

	using MouseRButtonDownMessage = BasicMouseMessage;

	using MouseRButtonUpMessage = BasicMouseMessage;

	using MouseRButtonDoubleClickMessage = BasicMouseMessage;

	using MouseMButtonDownMessage = BasicMouseMessage;

	using MouseMButtonUpMessage = BasicMouseMessage;

	using MouseMButtonDoubleClickMessage = BasicMouseMessage;

	using MouseXButton1DownMessage = BasicMouseMessage;

	using MouseXButton1UpMessage = BasicMouseMessage;

	using MouseXButton1DoubleClickMessage = BasicMouseMessage;

	using MouseXButton2DownMessage = BasicMouseMessage;

	using MouseXButton2UpMessage = BasicMouseMessage;

	using MouseXButton2DoubleClickMessage = BasicMouseMessage;

	using MouseWheelMessage = BasicMouseWheelMessage;

	using MouseHWheelMessage = BasicMouseWheelMessage;

	using KeyDownMessage = BasicKeyPressMessage;

	using KeyUpMessage = BasicKeyPressMessage;

	using SysKeyDownMessage = BasicKeyPressMessage;

	using SysKeyUpMessage = BasicKeyPressMessage;

	using CharMessage = BasicCharMessage;

	using DeadCharMessage = BasicCharMessage;

	using SysDeadCharMessage = BasicCharMessage;
}

#endif // !Petal_Header_WindowMessage
