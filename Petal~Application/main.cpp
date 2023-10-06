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
		// 派生 XInputEventProcess
		class BasicT : public Abstract::XInputEventProcess
		{
		public:
			// 实现事件触发检测方法：之前没按下并且现在按下了，触发 pushed 事件
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
			// 实现响应事件方法
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
			// 实现响应事件方法
			void Execution() override
			{
				dout + "X pushed and Exit" + ln;
				std::cout << "X pushed and Exit\n";
				flag_exit_loop = true;
			}
			TX() : BasicT(XInput::Button::X) {}
		};
		// 派生本框架提供的处理过程
		class TY : public XInput::ButtonHoldProcess
		{
		public:
			// 实现响应事件方法
			void Execution() override
			{
				dout + "Y holding" + ln;
				std::cout << "Y holding\n";
			}
			TY() : ButtonHoldProcess(XInput::Button::Y, 1.0, true) {}
		};

		// 创建存储 XInput 事件处理过程的容器
		std::vector<std::unique_ptr<Abstract::XInputEventProcess>> proc_vector{};
		proc_vector.push_back(std::make_unique<TA>());
		proc_vector.push_back(std::make_unique<TY>());
		proc_vector.push_back(std::make_unique<TX>());

		// 创建控制器对象，并将用户索引设置为 0
		XInput::Controller c{ 0 };

		while (flag_exit_loop == false)
		{
			c.Update(proc_vector.begin(), proc_vector.end());
			::Sleep(1);
		}

		return 0;
	}
};

Petal_SetMainClass(XInputControllerDemo);
