#include "Petal~Main.h"
#include "Petal~VSDebugOutput.h"
#include "Petal~XInputController.h"
#include "Petal~XInputEventProcess.h"
#include "Petal~FrequencyController.h"
#include "Petal~KeyboardController.h"
#include "Petal~KeyboardEventProcess.h"

#include <vector>
#include <memory>
#include <format>
#include <iostream>

class XInputControllerDemo
{
public:
	static inline bool flag_exit_loop{ false };
	static int main()
	{
		using namespace Petal;
		PerformanceCounter pc;
		// ���� XInputEventProcess
		class BasicT : public Abstract::XInputEventProcess
		{
		public:
			// ʵ���¼�������ⷽ����֮ǰû���²������ڰ����ˣ����� pushed �¼�
			Petal::boolean Check() override
			{
				return (this->Resource().controller.GetWrappedGamepad().Pushed(button) == true &&
					this->Resource().controller.GetLastWrappedGamepad().Pushed(button) == false);
			}
			BasicT(XInput::Button::Type button_) : XInputEventProcess(), button(button_) {}
			XInput::Button::Type button;
		};
		class TA : public BasicT
		{
		public:
			// ʵ����Ӧ�¼�����
			void Execution() override
			{
				dout + "A pushed" + ln;
				std::cout << "A pushed\n";
			}
			TA() : BasicT(XInput::Button::A) {}
		};
		class TX : public BasicT
		{
		public:
			// ʵ����Ӧ�¼�����
			void Execution() override
			{
				dout + "X pushed and Exit" + ln;
				std::cout << "X pushed and Exit\n";
				flag_exit_loop = true;
			}
			TX() : BasicT(XInput::Button::X) {}
		};
		// ����������ṩ�Ĵ������
		class TY : public XInput::ButtonHoldProcess
		{
		public:
			// ʵ����Ӧ�¼�����
			void Execution() override
			{
				dout + "Y holding" + ln;
				std::cout << "Y holding\n";
			}
			TY(const PerformanceCounter& pc) : ButtonHoldProcess(XInput::Button::Y, pc.Frequency() * 1, true) {}
		};



		class BasicK : public Abstract::KeyboardEventProcess
		{
		public:
			// ʵ���¼�������ⷽ����֮ǰû���²������ڰ����ˣ����� pushed �¼�
			Petal::boolean Check() override
			{
				return (this->Resource().controller.GetState(key).Pushed() == true &&
					this->Resource().controller.GetLastState(key).Pushed() == false);
			}
			BasicK(VirtualKey::Type key_) : KeyboardEventProcess(), key(key_) {}
			VirtualKey::Type key;
		};
		class KA : public BasicK
		{
		public:
			// ʵ����Ӧ�¼�����
			void Execution() override
			{
				dout + "Key A pushed" + ln;
				std::cout << "Key A pushed\n";
			}
			KA() : BasicK(VirtualKey::A) {}
		};
		class KW : public BasicK
		{
		public:
			// ʵ����Ӧ�¼�����
			void Execution() override
			{
				dout + "Key W pushed" + ln;
				std::cout << "Key W pushed and Exit\n";
				flag_exit_loop = true;
			}
			KW() : BasicK(VirtualKey::W) {}
		};
		class KD : public BasicK
		{
		public:
			// ʵ����Ӧ�¼�����
			void Execution() override
			{
				kc.Unregister('A');
				kc.Unregister('S');
				dout + "Key D pushed" + ln;
				std::cout << "Key D pushed and Unregister 'A' and 'S'\n";
			}
			KD(Keyboard::Controller& kc_) : BasicK(VirtualKey::D), kc(kc_) {}
			Keyboard::Controller& kc;
		};
		class KS : public Keyboard::KeyHoldProcess
		{
		public:
			// ʵ����Ӧ�¼�����
			void Execution() override
			{
				dout + "Key S rel" + ln;
				std::cout << "Key S rel\n";
			}
			KS(const PerformanceCounter& pc) : KeyHoldProcess(VirtualKey::S, pc.Frequency() * 1, true) {}
		};


		// �����洢 XInput �¼�������̵�����
		std::vector<std::unique_ptr<Abstract::XInputEventProcess>> proc_vector{};
		proc_vector.push_back(std::make_unique<TA>());
		proc_vector.push_back(std::make_unique<TY>(pc));
		proc_vector.push_back(std::make_unique<TX>());
		

		std::vector<std::unique_ptr<Abstract::KeyboardEventProcess>> k_proc_vector{};
		k_proc_vector.push_back(std::make_unique<KA>());
		k_proc_vector.push_back(std::make_unique<KS>(pc));
		k_proc_vector.push_back(std::make_unique<KW>());

		// �������������󣬲����û���������Ϊ 0
		XInput::Controller c{ 0 };

		Keyboard::Controller kc{};
		kc.Register('A');
		kc.Register('S');
		kc.Register('W');
		kc.Register('D');

		k_proc_vector.push_back(std::make_unique<KD>(kc));

		while (flag_exit_loop == false)
		{
			pc.Count();
			c.Update(proc_vector.begin(), proc_vector.end(), pc.DeltaCounts());
			kc.Update(k_proc_vector.begin(), k_proc_vector.end(), pc.DeltaCounts());
			::Sleep(1);
		}

		return 0;
	}
};

/*
void* operator new(std::size_t sz)
{
	std::printf("�ѵ���ȫ�� new ���������СΪ %zu\n", sz);
	if (sz == 0)
		++sz; // ���� std::malloc(0)�������ܻ��ڳɹ�ʱ���� nullptr

	if (void* ptr = std::malloc(sz))
		return ptr;

	throw std::bad_alloc{}; // �� [new.delete.single]/3 Ҫ��
}

void operator delete(void* ptr) noexcept
{
	std::puts("�ѵ���ȫ�� delete �����");
	std::free(ptr);
}
*/

#include "Petal~Window.h"

class Test
{
public:
	static int main()
	{
		using namespace Petal;
		//	std::cout << "����ʼ" << '\n';
		dout + "����ʼ" + ln;
		try
		{
		//	int* p1 = new int;
		//	delete p1;
		//	int* p2 = new int[10]; // C++11 �б�֤�����滻
		//	delete[] p2;

			WrappedWindowClass wc{ Petal_TStr("my-class") };

			auto [atom, error] = wc.Register();

			Window w;

			w.Show();

			MessageLoop();
		}
		catch (const ::std::exception& e) { dout + "std::exception: " + e.what() + ln; }
		//	std::cout << "�������" << '\n';
		dout + "�������" + ln;

		return 0;
	}
};

// Petal_SetMainClass(XInputControllerDemo);

// Petal_SetMainClass(Test);
