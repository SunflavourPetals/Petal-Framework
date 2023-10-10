#pragma once

#ifndef Petal_Header_VirtualKey
#define Petal_Header_VirtualKey

#include <WinUser.h>

namespace Petal
{
	//	https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
	//	https://learn.microsoft.com/zh-cn/windows/win32/inputdev/virtual-key-codes

	class VirtualKey
	{
	private:
		using vk = int;
	public:
		using Type = vk;
	public:
		class Mouse
		{
		public:
			static constexpr vk LButton{ VK_LBUTTON };
			static constexpr vk RButton{ VK_RBUTTON };
			static constexpr vk MButton{ VK_MBUTTON };
			static constexpr vk XButton1{ VK_XBUTTON1 };
			static constexpr vk XButton2{ VK_XBUTTON2 };
		};
		class Browser
		{
		public:
			static constexpr vk Back{ VK_BROWSER_BACK };
			static constexpr vk Forward{ VK_BROWSER_FORWARD };
			static constexpr vk Refresh{ VK_BROWSER_REFRESH };
			static constexpr vk Stop{ VK_BROWSER_STOP };
			static constexpr vk Search{ VK_BROWSER_SEARCH };
			static constexpr vk Favorites{ VK_BROWSER_FAVORITES };
			static constexpr vk Home{ VK_BROWSER_HOME };
		};
		class Volume
		{
		public:
			static constexpr vk Mute{ VK_VOLUME_MUTE };
			static constexpr vk Down{ VK_VOLUME_DOWN };
			static constexpr vk Up{ VK_VOLUME_UP };
		};
		class Media
		{
		public:
			static constexpr vk Next{ VK_MEDIA_NEXT_TRACK };
			static constexpr vk Prev{ VK_MEDIA_PREV_TRACK };
			static constexpr vk Stop{ VK_MEDIA_STOP };
			static constexpr vk PlayPause{ VK_MEDIA_PLAY_PAUSE };
		};
		class Numpad
		{
		public:
			static constexpr vk Num0{ VK_NUMPAD0 }; // numpad 0
			static constexpr vk Num1{ VK_NUMPAD1 }; // numpad 1
			static constexpr vk Num2{ VK_NUMPAD2 }; // numpad 2
			static constexpr vk Num3{ VK_NUMPAD3 }; // numpad 3
			static constexpr vk Num4{ VK_NUMPAD4 }; // numpad 4
			static constexpr vk Num5{ VK_NUMPAD5 }; // numpad 5
			static constexpr vk Num6{ VK_NUMPAD6 }; // numpad 6
			static constexpr vk Num7{ VK_NUMPAD7 }; // numpad 7
			static constexpr vk Num8{ VK_NUMPAD8 }; // numpad 8
			static constexpr vk Num9{ VK_NUMPAD9 }; // numpad 9
			static constexpr vk Multiply{ VK_MULTIPLY }; // numpad *
			static constexpr vk Add{ VK_ADD }; // numpad +
			static constexpr vk Separator{ VK_SEPARATOR };
			static constexpr vk Subtract{ VK_SUBTRACT }; // numpad -
			static constexpr vk Decimal{ VK_DECIMAL };
			static constexpr vk Divide{ VK_DIVIDE }; // numpad /
		};
		static constexpr vk Backspace{ VK_BACK };
		static constexpr vk Tab{ VK_TAB };
		static constexpr vk Enter{ VK_RETURN }; // Carriage Return
		static constexpr vk Shift{ VK_SHIFT };
		static constexpr vk Ctrl{ VK_CONTROL };
		static constexpr vk Alt{ VK_MENU };
		static constexpr vk Pause{ VK_PAUSE };
		static constexpr vk CapsLock{ VK_CAPITAL };
		static constexpr vk Esc{ VK_ESCAPE };
		static constexpr vk Space{ VK_SPACE };
		static constexpr vk PageUp{ VK_PRIOR };
		static constexpr vk PageDown{ VK_NEXT };
		static constexpr vk End{ VK_END };
		static constexpr vk Home{ VK_HOME };
		static constexpr vk Left{ VK_LEFT };
		static constexpr vk Up{ VK_UP };
		static constexpr vk Right{ VK_RIGHT };
		static constexpr vk Down{ VK_DOWN };
		static constexpr vk Select{ VK_SELECT };
		static constexpr vk Print{ VK_PRINT };
		static constexpr vk Execute{ VK_EXECUTE };
		static constexpr vk PrintScreen{ VK_SNAPSHOT };
		static constexpr vk Insert{ VK_INSERT };
		static constexpr vk Delete{ VK_DELETE };
		static constexpr vk Help{ VK_HELP };
		static constexpr vk Key0{ 0x30 }; // key 0
		static constexpr vk Key1{ 0x31 }; // key 1
		static constexpr vk Key2{ 0x32 }; // key 2
		static constexpr vk Key3{ 0x33 }; // key 3
		static constexpr vk Key4{ 0x34 }; // key 4
		static constexpr vk Key5{ 0x35 }; // key 5
		static constexpr vk Key6{ 0x36 }; // key 6
		static constexpr vk Key7{ 0x37 }; // key 7
		static constexpr vk Key8{ 0x38 }; // key 8
		static constexpr vk Key9{ 0x39 }; // key 9
		static constexpr vk A{ 0x41 };
		static constexpr vk B{ 0x42 };
		static constexpr vk C{ 0x43 };
		static constexpr vk D{ 0x44 };
		static constexpr vk E{ 0x45 };
		static constexpr vk F{ 0x46 };
		static constexpr vk G{ 0x47 };
		static constexpr vk H{ 0x48 };
		static constexpr vk I{ 0x49 };
		static constexpr vk J{ 0x4a };
		static constexpr vk K{ 0x4b };
		static constexpr vk L{ 0x4c };
		static constexpr vk M{ 0x4d };
		static constexpr vk N{ 0x4e };
		static constexpr vk O{ 0x4f };
		static constexpr vk P{ 0x50 };
		static constexpr vk Q{ 0x51 };
		static constexpr vk R{ 0x52 };
		static constexpr vk S{ 0x53 };
		static constexpr vk T{ 0x54 };
		static constexpr vk U{ 0x55 };
		static constexpr vk V{ 0x56 };
		static constexpr vk W{ 0x57 };
		static constexpr vk X{ 0x58 };
		static constexpr vk Y{ 0x59 };
		static constexpr vk Z{ 0x5a };
		static constexpr vk LWin{ VK_LWIN };
		static constexpr vk RWin{ VK_RWIN };
		static constexpr vk Apps{ VK_APPS };
		static constexpr vk Sleep{ VK_SLEEP };
		static constexpr vk F1{ VK_F1 };
		static constexpr vk F2{ VK_F2 };
		static constexpr vk F3{ VK_F3 };
		static constexpr vk F4{ VK_F4 };
		static constexpr vk F5{ VK_F5 };
		static constexpr vk F6{ VK_F6 };
		static constexpr vk F7{ VK_F7 };
		static constexpr vk F8{ VK_F8 };
		static constexpr vk F9{ VK_F9 };
		static constexpr vk F10{ VK_F10 };
		static constexpr vk F11{ VK_F11 };
		static constexpr vk F12{ VK_F12 };
		static constexpr vk F13{ VK_F13 };
		static constexpr vk F14{ VK_F14 };
		static constexpr vk F15{ VK_F15 };
		static constexpr vk F16{ VK_F16 };
		static constexpr vk F17{ VK_F17 };
		static constexpr vk F18{ VK_F18 };
		static constexpr vk F19{ VK_F19 };
		static constexpr vk F20{ VK_F20 };
		static constexpr vk F21{ VK_F21 };
		static constexpr vk F22{ VK_F22 };
		static constexpr vk F23{ VK_F23 };
		static constexpr vk F24{ VK_F24 };
		static constexpr vk NumLock{ VK_NUMLOCK };
		static constexpr vk ScrollLock{ VK_SCROLL };
		static constexpr vk LShift{ VK_LSHIFT };
		static constexpr vk RShift{ VK_RSHIFT };
		static constexpr vk LCtrl{ VK_LCONTROL };
		static constexpr vk RCtrl{ VK_RCONTROL };
		static constexpr vk LAlt{ VK_LMENU };
		static constexpr vk RAlt{ VK_RMENU };
		static constexpr vk Colon{ VK_OEM_1 }; // ----------- // ;:
		static constexpr vk Plus{ VK_OEM_PLUS }; // --------- // =+
		static constexpr vk Comma{ VK_OEM_COMMA }; // ------- // ,<
		static constexpr vk Minus{ VK_OEM_MINUS }; // ------- // -_
		static constexpr vk Period{ VK_OEM_PERIOD }; // ----- // .>
		static constexpr vk QuestionMark{ VK_OEM_2 }; // ---- // /?
		static constexpr vk Tilde{ VK_OEM_3 }; // ----------- // `~
		static constexpr vk LBraces{ VK_OEM_4 }; // --------- // [{
		static constexpr vk VerticalBar{ VK_OEM_5 }; // ----- // \|
		static constexpr vk RBraces{ VK_OEM_6 }; // --------- // ]}
		static constexpr vk DoubleQuote{ VK_OEM_7 }; // ----- // '"
		static constexpr vk Semicolon{ Colon }; // ---------- // ;:
		static constexpr vk Equal{ Plus }; // --------------- // =+
		static constexpr vk Less{ Comma }; // --------------- // ,<
		static constexpr vk Underline{ Minus }; // ---------- // -_
		static constexpr vk Greater{ Period }; // ----------- // .>
		static constexpr vk Slash{ QuestionMark }; // ------- // /?
		static constexpr vk BackQuote{ Tilde }; // ---------- // `~
		static constexpr vk LBrackets{ LBraces }; // -------- // [{
		static constexpr vk Backslash{ VerticalBar }; // ---- // \|
		static constexpr vk RBrackets{ RBraces }; // -------- // ]}
		static constexpr vk SingleQuote{ DoubleQuote }; // -- // '"
	};
}

#endif // !Petal_Header_VirtualKey
