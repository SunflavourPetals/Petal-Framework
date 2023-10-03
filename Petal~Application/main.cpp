#include "Petal~Main.h"
#include "Petal~VSDebugOutput.h"
#include "Petal~XInputController.h"
#include "Petal~FrequencyController.h"
#include <vector>
#include <memory>

class XInputControllerDemo
{
public:
	static inline Petal::boolean flag_exit_loop{ false };
	static int main()
	{
		using namespace Petal;
		using Petal::boolean;
		
		// 派生XInputEventProcess
		class BasicT : public Abstract::XInputEventProcess
		{
		public:
			// 实现事件触发检测方法：之前没按下并且现在按下了，触发 pushed 事件
			boolean Check() override
			{
				return (this->Controller().GetWrappedGamepad().Pushed(button) == true &&
					this->Controller().GetLastWrappedGamepad().Pushed(button) == false);
			}
			BasicT(XInput::Button::Type button_) : button(button_) {}
			XInput::Button::Type button;
		};
		class TA : public BasicT
		{
		public:
			// 实现响应事件方法
			void Execution() override
			{
				dout + "A pushed" + ln;
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
				flag_exit_loop = true;
			}
			TX() : BasicT(XInput::Button::X) {}
		};

		std::vector<std::unique_ptr<Abstract::XInputEventProcess>> proc_vector{}; // 创建事件容器
		proc_vector.push_back(std::make_unique<TA>());
		proc_vector.push_back(std::make_unique<TX>());

		XInput::Controller c{ 0 }; // 创建控制器对象，并将用户索引设置为 0

		FrequencyController f{ 100.0, 0.005 }; // 设置频率控制器 100 Hz，并允许线程休眠
		
		class DeltaProc : public Abstract::Process<FrequencyController::ResourceDelta>
		{
		public:
			DeltaProc(XInput::Controller& c_, decltype(proc_vector)& v_) :
				Process<FrequencyController::ResourceDelta>(), c(c_), v(v_) {}
			decltype(proc_vector)& v;
			XInput::Controller& c;
			void Execution() override
			{
				c.Update(this->Resource().delta_time, v.begin(), v.end());
			}
		} delta_proc{ c, proc_vector };

		while (flag_exit_loop == false)
		{
			f.LimitedDo(delta_proc);
		}

		return 0;
	}
};

Petal_SetMainClass(XInputControllerDemo);
