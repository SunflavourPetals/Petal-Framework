#include "Petal~Main.h"
#include "Petal~VSDebugOutput.h"
#include "Petal~XInputController.h"
#include "Petal~XInputEventProcess.h"
#include "Petal~FrequencyController.h"

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
			TY() : ButtonHoldProcess(XInput::Button::Y, 1.0, true) {}
		};

		// �����洢 XInput �¼�������̵�����
		std::vector<std::unique_ptr<Abstract::XInputEventProcess>> proc_vector{};
		proc_vector.push_back(std::make_unique<TA>());
		proc_vector.push_back(std::make_unique<TY>());
		proc_vector.push_back(std::make_unique<TX>());

		// �������������󣬲����û���������Ϊ 0
		XInput::Controller c{ 0 };

		while (flag_exit_loop == false)
		{
			c.Update(proc_vector.begin(), proc_vector.end());
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
		
		std::cout << "����ʼ" << '\n';
		{
		//	int* p1 = new int;
		//	delete p1;
		//	int* p2 = new int[10]; // C++11 �б�֤�����滻
		//	delete[] p2;

			WrappedWindowClass wc{ Petal_TStr("my-class") };
			auto [atom, error] = wc.Register();

			Window w;
			w.Create(atom);

			w.Show();

			MessageLoop();
		}
		std::cout << "�������" << '\n';

		return 0;
	}
};

// Petal_SetMainClass(XInputControllerDemo);

Petal_SetMainClass(Test);
