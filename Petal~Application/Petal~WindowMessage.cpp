#include "Petal~WindowMessage.h"

#include <type_traits>

#include <Windows.h>
#include <windowsx.h>

namespace Petal
{
	BasicWindowMessage::BasicWindowMessage(win32msg msg, win32wprm w_prm, win32lprm l_prm) :
		message{ msg },
		w{ w_prm },
		l{ l_prm }
	{

	}
	win32msg BasicWindowMessage::Message() const noexcept
	{
		return this->message;
	}
	win32wprm BasicWindowMessage::Word() const noexcept
	{
		return this->w;
	}
	win32lprm BasicWindowMessage::Long() const noexcept
	{
		return this->l;
	}
	word BasicWindowMessage::HWLong() const noexcept
	{
		return HIWORD(this->Long());
	}
	word BasicWindowMessage::LWLong() const noexcept
	{
		return LOWORD(this->Long());
	}
	word BasicWindowMessage::HWWord() const noexcept
	{
		return HIWORD(this->Word());
	}
	word BasicWindowMessage::LWWord() const noexcept
	{
		return LOWORD(this->Word());
	}
}

namespace Petal
{
	BasicSizeMoveMessage::BasicSizeMoveMessage(win32msg msg, win32wprm w, win32lprm l) :
		BasicWindowMessage(msg, w, l)
	{

	}
	Win32Rect& BasicSizeMoveMessage::WinRect() noexcept
	{
		return *(reinterpret_cast<ptr<Win32Rect>>(this->Long()));
	}
	const Win32Rect& BasicSizeMoveMessage::WinRect() const noexcept
	{
		return *(reinterpret_cast<ptr<Win32Rect>>(this->Long()));
	}
}

namespace Petal
{
	BasicResizeMessage::BasicResizeMessage(win32msg msg, win32wprm w, win32lprm l) :
		BasicWindowMessage(msg, w, l),
		client_size{ static_cast<i32>(this->LWLong()), static_cast<i32>(this->HWLong()) }
	{

	}
	const Size2DI32& BasicResizeMessage::NewClientSize() const noexcept
	{
		return client_size;
	}
}

namespace Petal
{
	BasicMouseMessage::BasicMouseMessage(win32msg msg, win32wprm w, win32lprm l) :
		BasicWindowMessage(msg, w, l),
		cursor_pos{ GET_X_LPARAM(l), GET_Y_LPARAM(l) },
		key_state{ GET_KEYSTATE_WPARAM(w) }
	{

	}
	word BasicMouseMessage::VirtualKeys() const noexcept
	{
		return this->key_state;
	}
	boolean BasicMouseMessage::LButtonDown() const noexcept
	{
		return static_cast<boolean>(this->VirtualKeys() & static_cast<i32>(VirtualKey::LButton));
	}
	boolean BasicMouseMessage::RButtonDown() const noexcept
	{
		return static_cast<boolean>(this->VirtualKeys() & static_cast<i32>(VirtualKey::RButton));
	}
	boolean BasicMouseMessage::ShiftDown() const noexcept
	{
		return static_cast<boolean>(this->VirtualKeys() & static_cast<i32>(VirtualKey::Shift));
	}
	boolean BasicMouseMessage::ControlDown() const noexcept
	{
		return static_cast<boolean>(this->VirtualKeys() & static_cast<i32>(VirtualKey::Control));
	}
	boolean BasicMouseMessage::MButtonDown() const noexcept
	{
		return static_cast<boolean>(this->VirtualKeys() & static_cast<i32>(VirtualKey::MButton));
	}
	boolean BasicMouseMessage::XButton1Down() const noexcept
	{
		return static_cast<boolean>(this->VirtualKeys() & static_cast<i32>(VirtualKey::XButton1));
	}
	boolean BasicMouseMessage::XButton2Down() const noexcept
	{
		return static_cast<boolean>(this->VirtualKeys() & static_cast<i32>(VirtualKey::XButton2));
	}
	const Position2DI32& BasicMouseMessage::CursorPosition() const noexcept
	{
		return this->cursor_pos;
	}
}

namespace Petal
{
	i16 BasicMouseWheelMessage::WheelDelta() const noexcept
	{
		return this->wheel_delta;
	}
	BasicMouseWheelMessage::BasicMouseWheelMessage(win32msg msg, win32wprm w, win32lprm l) :
		BasicMouseMessage(msg, w, l),
		wheel_delta{ GET_WHEEL_DELTA_WPARAM(w) }
	{

	}
}

namespace Petal
{
	BasicKeyMessage::BasicKeyMessage(win32msg msg, win32wprm w, win32lprm l) :
		BasicWindowMessage(msg, w, l)
	{

	}
	u16 BasicKeyMessage::RepeatCount() const noexcept
	{
		return (this->Long() & 0xffffu);
	}
	u8 BasicKeyMessage::ScanCode() const noexcept
	{
		return ((this->Long() >> 16) & 0xffu);
	}
	u8 BasicKeyMessage::LongParam25To32Bits() const noexcept
	{
		return ((this->Long() >> 24) & 0xffu);
	}
}

namespace Petal
{
	BasicKeyPressMessage::BasicKeyPressMessage(win32msg msg, win32wprm w, win32lprm l) :
		BasicKeyMessage(msg, w, l)
	{

	}
	int BasicKeyPressMessage::VirtualKey() const noexcept
	{
		return static_cast<int>(this->Word());
	}
}

namespace Petal
{
	BasicCharMessage::BasicCharMessage(win32msg msg, win32wprm w, win32lprm l) :
		BasicKeyMessage(msg, w, l)
	{

	}
	TChar BasicCharMessage::GetTChar() const noexcept
	{
		return static_cast<TChar>(this->Word());
	}
	achar BasicCharMessage::GetCharA() const noexcept
	{
		return static_cast<achar>(this->Word());
	}
	u8char BasicCharMessage::GetCharU8() const noexcept
	{
		return static_cast<u8char>(this->Word());
	}
	wchar BasicCharMessage::GetCharW() const noexcept
	{
		return static_cast<wchar>(this->Word());
	}
	u16char BasicCharMessage::GetCharU16() const noexcept
	{
		return static_cast<u16char>(this->Word());
	}
	boolean BasicCharMessage::IsLowSurrogate() const noexcept
	{
		return IS_LOW_SURROGATE(this->GetCharU16());
	}
	boolean BasicCharMessage::IsHighSurrogate() const noexcept
	{
		return IS_HIGH_SURROGATE(this->GetCharU16());
	}
}

namespace Petal
{
	CreateMessage::CreateMessage(win32msg msg, win32wprm w, win32lprm l) :
		BasicWindowMessage(msg, w, l)
	{

	}
	const Win32CreateStruct& CreateMessage::CreateStruct() const noexcept
	{
		return *reinterpret_cast<ptr<Win32CreateStruct>>(this->Long());
	}
}

namespace Petal
{
	ActiveMessage::ActiveMessage(win32msg msg, win32wprm w, win32lprm l) :
		BasicWindowMessage(msg, w, l),
		clicked{ w == WA_CLICKACTIVE }
	{

	}
	boolean ActiveMessage::TriggeringByClick() const noexcept
	{
		return this->clicked;
	}
	win32hwnd ActiveMessage::Deactivated() const noexcept
	{
		return reinterpret_cast<win32hwnd>(this->Long());
	}
	boolean ActiveMessage::ValidDeactivated() const noexcept
	{
		return this->Long() != NULL;
	}
}

namespace Petal
{
	InactiveMessage::InactiveMessage(win32msg msg, win32wprm w, win32lprm l) :
		BasicWindowMessage(msg, w, l)
	{

	}
	win32hwnd InactiveMessage::Activate() const noexcept
	{
		return reinterpret_cast<win32hwnd>(this->Long());
	}
	boolean InactiveMessage::ValidActivate() const noexcept
	{
		return this->Long() != NULL;
	}
}

namespace Petal
{
	SizingMessage::SizingMessage(win32msg msg, win32wprm w, win32lprm l) :
		BasicSizeMoveMessage(msg, w, l)
	{

	}
	SizingMessage::Edge SizingMessage::SizingEdge() noexcept
	{
		return static_cast<Edge>(this->Word());
	}
}

namespace Petal
{
	const Position2DI32& MovedMessage::NewClientPosition() const noexcept
	{
		return this->pos;
	}
	MovedMessage::MovedMessage(win32msg msg, win32wprm w, win32lprm l) :
		BasicWindowMessage(msg, w, l),
		pos{ GET_X_LPARAM(l), GET_Y_LPARAM(l) }
	{

	}
}
