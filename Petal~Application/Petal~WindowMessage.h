#pragma once

#ifndef Petal_Header_WindowMessage
#define Petal_Header_WindowMessage

#include "Petal~Types.h"
#include "Petal~WinTypes.h"
#include "Petal~String.h"

#include <windowsx.h>

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
		win32msg Message() const noexcept { return message; }
	public:
		win32wprm Word() const noexcept { return w; }
		win32lprm Long() const noexcept { return l; }
		win32word HWLong() const noexcept { return HIWORD(Long()); }
		win32word LWLong() const noexcept { return LOWORD(Long()); }
		win32word HWWord() const noexcept { return HIWORD(Word()); }
		win32word LWWord() const noexcept { return LOWORD(Word()); }
	public:
		BasicWindowMessage(win32msg msg, win32wprm w_prm, win32lprm l_prm) :
			message{ msg },
			w{ w_prm },
			l{ l_prm } {}
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
		Win32Rect& WinRect() noexcept { return *(reinterpret_cast<ptr<Win32Rect>>(Long())); }
		const Win32Rect& WinRect() const noexcept { return *(reinterpret_cast<ptr<Win32Rect>>(Long())); }
	public:
		BasicSizeMoveMessage(win32msg msg, win32wprm w, win32lprm l) :
			BasicWindowMessage(msg, w, l) {}
		BasicSizeMoveMessage(const BasicSizeMoveMessage&) = default;
		BasicSizeMoveMessage(BasicSizeMoveMessage&&) noexcept = default;
		virtual ~BasicSizeMoveMessage() = default;
	};

	class BasicResizeMessage : public BasicWindowMessage
	{
	public:
		const Size2DI32& NewClientSize() const noexcept { return client_size; }
	public:
		BasicResizeMessage(win32msg msg, win32wprm w, win32lprm l) :
			BasicWindowMessage(msg, w, l),
			client_size{ static_cast<i32>(LWLong()), static_cast<i32>(HWLong()) } {}
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
		win32word VirtualKeys() const noexcept { return key_state; }
		boolean LButtonDown() const noexcept;
		boolean RButtonDown() const noexcept;
		boolean ShiftDown() const noexcept;
		boolean ControlDown() const noexcept;
		boolean MButtonDown() const noexcept;
		boolean XButton1Down() const noexcept;
		boolean XButton2Down() const noexcept;
		const Position2DI32& CursorPosition() const noexcept { return cursor_pos; }
	public:
		BasicMouseMessage(win32msg msg, win32wprm w, win32lprm l) :
			BasicWindowMessage(msg, w, l),
			cursor_pos{ GET_X_LPARAM(l), GET_Y_LPARAM(l) },
			key_state{ GET_KEYSTATE_WPARAM(w) } {}
		BasicMouseMessage(const BasicMouseMessage&) = default;
		BasicMouseMessage(BasicMouseMessage&&) noexcept = default;
		virtual ~BasicMouseMessage() = default;
	private:
		Position2DI32 cursor_pos{};
		win32word key_state{};
	};

	class BasicMouseWheelMessage : public BasicMouseMessage
	{
	public:
		i16 WheelDelta() const noexcept { return wheel_delta; }
	public:
		BasicMouseWheelMessage(win32msg msg, win32wprm w, win32lprm l) :
			BasicMouseMessage(msg, w, l),
			wheel_delta{ GET_WHEEL_DELTA_WPARAM(w) } {}
		BasicMouseWheelMessage(const BasicMouseWheelMessage&) = default;
		BasicMouseWheelMessage(BasicMouseWheelMessage&&) noexcept = default;
		virtual ~BasicMouseWheelMessage() = default;
	private:
		i16 wheel_delta{};
	};

	class BasicKeyMessage : public BasicWindowMessage
	{
	public:
		u16 RepeatCount() const noexcept { return (Long() & 0xffffu); }
		u8 ScanCode() const noexcept { return ((Long() >> 16) & 0xffu); }
		u8 LongParam25To32Bits() const noexcept { return ((Long() >> 24) & 0xffu); }
	public:
		BasicKeyMessage(win32msg msg, win32wprm w, win32lprm l) :
			BasicWindowMessage(msg, w, l) {}
		BasicKeyMessage(const BasicKeyMessage&) = default;
		BasicKeyMessage(BasicKeyMessage&&) noexcept = default;
		virtual ~BasicKeyMessage() = default;
	};

	class BasicKeyPressMessage : public BasicKeyMessage
	{
	public:
		int VirtualKey() const noexcept { return static_cast<int>(Word()); }
	public:
		BasicKeyPressMessage(win32msg msg, win32wprm w, win32lprm l) :
			BasicKeyMessage(msg, w, l) {}
		BasicKeyPressMessage(const BasicKeyPressMessage&) = default;
		BasicKeyPressMessage(BasicKeyPressMessage&&) noexcept = default;
		virtual ~BasicKeyPressMessage() = default;
	};

	class BasicCharMessage : public BasicKeyMessage
	{
	public:
		TChar GetTChar() const noexcept { return static_cast<TChar>(Word()); }
		achar GetCharA() const noexcept { return static_cast<achar>(Word()); }
		u8char GetCharU8() const noexcept { return static_cast<u8char>(Word()); }
		wchar GetCharW() const noexcept { return static_cast<wchar>(Word()); }
		u16char GetCharU16() const noexcept { return static_cast<u16char>(Word()); }
		boolean IsLowSurrogate() const noexcept { return IS_LOW_SURROGATE(GetCharU16()); }
		boolean IsHighSurrogate() const noexcept { return IS_HIGH_SURROGATE(GetCharU16()); }
	public:
		BasicCharMessage(win32msg msg, win32wprm w, win32lprm l) :
			BasicKeyMessage(msg, w, l) {}
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
		const Win32CreateStruct& CreateStruct() const noexcept
		{
			return *reinterpret_cast<ptr<Win32CreateStruct>>(Long());
		}
	public:
		CreateMessage(win32msg msg, win32wprm w, win32lprm l) :
			BasicWindowMessage(msg, w, l) {}
		CreateMessage(const CreateMessage&) = default;
		CreateMessage(CreateMessage&&) noexcept = default;
		virtual ~CreateMessage() = default;
	public:
		boolean continue_creation{ true };
	};
	class ActiveMessage : public BasicWindowMessage
	{
	public:
		boolean TriggeringByClick() const noexcept { return clicked; }
		win32hwnd Deactivated() const noexcept { return reinterpret_cast<win32hwnd>(Long()); }
		boolean ValidDeactivated() const noexcept { return Long() != NULL; }
	public:
		ActiveMessage(win32msg msg, win32wprm w, win32lprm l) :
			BasicWindowMessage(msg, w, l),
			clicked{ w == WA_CLICKACTIVE } {}
		ActiveMessage(const ActiveMessage&) = default;
		ActiveMessage(ActiveMessage&&) noexcept = default;
		virtual ~ActiveMessage() = default;
	private:
		boolean clicked{ false };
	};

	class InactiveMessage : public BasicWindowMessage
	{
	public:
		win32hwnd Activate() const noexcept { return reinterpret_cast<win32hwnd>(Long()); }
		boolean ValidActivate() const noexcept { return Long() != NULL; }
	public:
		InactiveMessage(win32msg msg, win32wprm w, win32lprm l) :
			BasicWindowMessage(msg, w, l) {}
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
		Edge SizingEdge() noexcept { return static_cast<Edge>(Word()); }
	public:
		SizingMessage(win32msg msg, win32wprm w, win32lprm l) :
			BasicSizeMoveMessage(msg, w, l) {}
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
		const Position2DI32& NewClientPosition() const noexcept { return pos; }
	public:
		MovedMessage(win32msg msg, win32wprm w, win32lprm l) :
			BasicWindowMessage(msg, w, l),
			pos{ GET_X_LPARAM(l), GET_Y_LPARAM(l) } {}
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

namespace Petal
{
	inline boolean BasicMouseMessage::LButtonDown() const noexcept
	{
		return static_cast<boolean>(VirtualKeys() & static_cast<i32>(VirtualKey::LButton));
	}
	inline boolean BasicMouseMessage::RButtonDown() const noexcept
	{
		return static_cast<boolean>(VirtualKeys() & static_cast<i32>(VirtualKey::RButton));
	}
	inline boolean BasicMouseMessage::ShiftDown() const noexcept
	{
		return static_cast<boolean>(VirtualKeys() & static_cast<i32>(VirtualKey::Shift));
	}
	inline boolean BasicMouseMessage::ControlDown() const noexcept
	{
		return static_cast<boolean>(VirtualKeys() & static_cast<i32>(VirtualKey::Control));
	}
	inline boolean BasicMouseMessage::MButtonDown() const noexcept
	{
		return static_cast<boolean>(VirtualKeys() & static_cast<i32>(VirtualKey::MButton));
	}
	inline boolean BasicMouseMessage::XButton1Down() const noexcept
	{
		return static_cast<boolean>(VirtualKeys() & static_cast<i32>(VirtualKey::XButton1));
	}
	inline boolean BasicMouseMessage::XButton2Down() const noexcept
	{
		return static_cast<boolean>(VirtualKeys() & static_cast<i32>(VirtualKey::XButton2));
	}
}

#endif // !Petal_Header_WindowMessage
