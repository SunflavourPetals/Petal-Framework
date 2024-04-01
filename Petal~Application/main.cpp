#include "Petal~RawInputWindow.h"
#include "Petal~VSDebugOutput.h"

namespace App
{
	class AppWindow : public Petal::RawInputWindow
	{
	public:
		void RawKeyboardEvent(
			Petal::RawKeyboardMessage& e,
			Petal::Win32RawInput& raw_input,
			Petal::tsize raw_input_size) noexcept
			override
		{
			Petal::Debug::println("KEYBOARD");
		}
		void RawHidEvent(
			Petal::RawHidMessage& e,
			Petal::Win32RawInput& raw_input,
			Petal::tsize raw_input_size) noexcept
			override
		{
			Petal::Debug::println("HID");
		}
		AppWindow() : RawInputWindow()
		{
			window_class.Register();
			Create(window_class.ClassAtom());
			UpdateTitle(Petal_TStr("Hello Visual Studio 2022 Community Preview"));
			Show();
			UpdateWindow();
			Petal::Win32RawInputDevice rid[]
			{
				{
					.usUsagePage = HID_USAGE_PAGE_GENERIC,
					.usUsage = HID_USAGE_GENERIC_KEYBOARD,
					.dwFlags = RIDEV_NOLEGACY,
					.hwndTarget = nullptr,
				},
				{
					.usUsagePage = HID_USAGE_PAGE_GENERIC,
					.usUsage = HID_USAGE_GENERIC_GAMEPAD,
					.dwFlags = 0,
					.hwndTarget = nullptr,
				},
				{
					.usUsagePage = HID_USAGE_PAGE_GENERIC,
					.usUsage = HID_USAGE_GENERIC_JOYSTICK,
					.dwFlags = 0,
					.hwndTarget = nullptr,
				}
			};
			RegisterRawInputDevices(rid);
			Petal::Debug::println("Hello Visual Studio 2022 Community Preview");
		}
		~AppWindow()
		{
			if (this->Valid())
			{
				this->Destroy(); // bind life circle
				Petal::ExitMessageLoop();
			}
		}
	private:
		Petal::WindowClass window_class;
	};

	int main()
	{
		AppWindow app{};
		return Petal::MessageLoop();
	}
}
