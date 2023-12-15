#include "Petal~Main.h"

#include "Petal~RawInputWindow.h"

class MyWin : public Petal::RawInputWindow
{
public:
	void RawMouseEvent(Petal::RawMouseMessage& e, Petal::Win32RawInput& r, Petal::tsize size) noexcept override
	{
		const auto& ms{ r.data.mouse };
		Petal::Debug::println("MOUS|   F:{:<4}|  BF:{:<4}|  BD:{:<4}|  RB:{:<4}|  LX:{:<4}|  LY:{:<4}|  EX:{:<4}|",
			ms.usFlags, ms.usButtonFlags, ms.usButtonData,
			ms.ulRawButtons, ms.lLastX, ms.lLastY, ms.ulExtraInformation);
	}
	void RawKeyboardEvent(Petal::RawKeyboardMessage& e, Petal::Win32RawInput& r, Petal::tsize size) noexcept override
	{
		const auto& kb{ r.data.keyboard };
		Petal::Debug::println("KB  |  MC:{:<4}|   F:{:<4}| RSV:{:<4}|  VK:{:<4}| MSG:{:<4}|  EX:{:<4}|",
			kb.MakeCode, kb.Flags, kb.Reserved, kb.VKey, kb.Message, kb.ExtraInformation);
	}
	void RawHidEvent(Petal::RawHidMessage& e, Petal::Win32RawInput& r, Petal::tsize size) noexcept override
	{
		Petal::dout + "INPUT-HID" + Petal::ln;
	}
	MyWin()
	{
		using namespace Petal;
		this->Create(WindowClassArgs{ Petal_TStr("my window class") }.Register().class_atom);

		constexpr tsize ridev_count{ 2 };
		::RAWINPUTDEVICE ridev[ridev_count]{};

		ridev[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
		ridev[0].usUsage = HID_USAGE_GENERIC_KEYBOARD;
		ridev[0].dwFlags = 0; // RIDEV_NOLEGACY;
		ridev[0].hwndTarget = 0;

		ridev[1].usUsagePage = HID_USAGE_PAGE_GENERIC;
		ridev[1].usUsage = HID_USAGE_GENERIC_MOUSE;
		ridev[1].dwFlags = 0; // RIDEV_NOLEGACY;
		ridev[1].hwndTarget = 0;

		if (this->RegisterRawInputDevices(ridev) == win32_false)
		{
			dout + "FAILED IN REGISTER RAW INPUT" + ln;
		}

		this->Show();
	}
};

class Main
{
public:
	static int main()
	{
		using namespace Petal;
		MyWin w;
		return MessageLoop();
	}
};

Petal_SetMainClass(Main);

