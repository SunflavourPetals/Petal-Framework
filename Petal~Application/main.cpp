#include "Petal~RawInputWindow.h"
#include "Petal~VSDebugOutput.h"
#include "Petal~Log.h"
#include "Petal~XInputController.h"
#include "Petal~XInputEventProcess.h"
#include "Petal~KeyboardController.h"
#include "Petal~KeyboardEventProcess.h"

#include <thread>
#include <memory>
#include <vector>

namespace App
{
	class AppWindow : public Petal::RawInputWindow // Test raw input window
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
		//	RegisterRawInputDevices(rid);
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
		
		// Petal::LogA log("test.txt");
		// log + "test log" + Petal::ln;
		// Test operator+ and operator- for Petal::Debug::VSDebugOutputA/W
		// Petal::dout + "test A+ " - Petal::ln - "test A- " + Petal::ln;
		// Petal::dowt + L"test W+" - Petal::ln - L"test W-" + Petal::ln;

		volatile bool not_end{ true };

		std::thread t([](volatile bool& not_end) {
			Petal::Debug::println("input thread: begin");

			Petal::PerformanceCounter counter;

			Petal::XInput::Controller c{};

			std::vector<std::unique_ptr<Petal::Abstract::XInputEventProcess>> v;

			class PushA : public Petal::XInput::ButtonPushProcess
			{
			public:
				void Execution(const decltype(c)::Resource& r) override
				{
					Petal::Debug::println("按下了 A 键!");
				}
				PushA() : ButtonPushProcess(Petal::XInput::Button::A) {}
			};
			class HoldA : public Petal::XInput::ButtonHoldProcess
			{
			public:
				void Execution(const decltype(c)::Resource& r) override
				{
					Petal::Debug::println("按住了 A 键!");
				}
				HoldA() : ButtonHoldProcess(Petal::XInput::Button::A, Petal::PerformanceCounter{}.Frequency()) {}
			};
			class ReleaseA : public Petal::XInput::ButtonReleaseProcess
			{
			public:
				void Execution(const decltype(c)::Resource& r) override
				{
					Petal::Debug::println("松开了 A 键!");
				}
				ReleaseA() : ButtonReleaseProcess(Petal::XInput::Button::A) {}
			};

			v.push_back(std::make_unique<PushA>());
			v.push_back(std::make_unique<HoldA>());
			v.push_back(std::make_unique<ReleaseA>());

			Petal::Keyboard::Controller kc{};

			std::vector<std::unique_ptr<Petal::Abstract::KeyboardEventProcess>> kv;

			class KPushA : public Petal::Keyboard::KeyPushProcess
			{
			public:
				void Execution(const decltype(kc)::Resource& r) override
				{
					Petal::Debug::println("按下了 A 键(键盘)!");
				}
				KPushA() : KeyPushProcess(Petal::VirtualKey::A) {}
			};
			class KHoldA : public Petal::Keyboard::KeyHoldProcess
			{
			public:
				void Execution(const decltype(kc)::Resource& r) override
				{
					Petal::Debug::println("按住了 A 键(键盘)!");
				}
				KHoldA() : KeyHoldProcess(Petal::VirtualKey::A, Petal::PerformanceCounter{}.Frequency()) {}
			};
			class KReleaseA : public Petal::Keyboard::KeyReleaseProcess
			{
			public:
				void Execution(const decltype(kc)::Resource& r) override
				{
					Petal::Debug::println("松开了 A 键(键盘)!");
				}
				KReleaseA() : KeyReleaseProcess(Petal::VirtualKey::A) {}
			};

			kv.push_back(std::make_unique<KPushA>());
			kv.push_back(std::make_unique<KHoldA>());
			kv.push_back(std::make_unique<KReleaseA>());

			kc.Register({ Petal::VirtualKey::A, Petal::VirtualKey::B });

			counter.Count();

			while (not_end)
			{
				Sleep(1);
				counter.Count();
				c.Update(v.begin(), v.end(), counter.DeltaCounts());
				kc.Update(kv.begin(), kv.end(), counter.DeltaCounts());
			}
			Petal::Debug::println("input thread: exit");
			},
			::std::ref(not_end));

		Petal::MessageLoop();

		not_end = false;

		t.join();

		return 0;
	}
}
