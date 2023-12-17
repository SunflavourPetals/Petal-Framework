# Petal~XInputController

源代码有更改，这部分文档还未进行修改。  

## 概述

命名空间 Petal::XInput

此标头内容使用XInput实现了用于与手柄交互的包装类：WrappedGamepad 和 Controller。  
类 WrappedGamepad 封装了 XInputGetState 等函数，并提供了更简单的手柄状态判断方法，更易于检测控制器状态  
类 Controller 通过保留上一次查询的结果进一步提高了对 push button、release button 等事件的支持；为事件的检测和响应事件，在命名空间`Petal::Abstract`提供了基类`XInputEventProcess`，开发者派生并实现其`boolean Check()`和`void Execution()`方法，分别用于检测事件触发条件和响应事件。  
为使开发者专注于响应事件，本框架的标头`"Petal~XInputEventProcess.h"`提供了更多实现了`Check()`的派生类供开发者使用。  
调用 Controller 对象的`Update`方法，  
开发者提供存储  
__无cv限定的`XInputEventProcess`派生类对象  
____或  
__无cv限定`XInputEventProcess`派生类对象的n层可带cv限定的指针(包括智能指针)  
的容器的迭代器完成事件的触发和响应。(n ≥ 1 and n ∈ N)  

示例
```cpp
#include "Petal~Main.h"
#include "Petal~VSDebugOutput.h"
#include "Petal~XInputController.h"

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
				XInputControllerDemo::flag_exit_loop = true;
			}
			TX() : BasicT(XInput::Button::X) {}
		};

		// 创建存储 XInput 事件处理过程的容器
		std::vector<std::unique_ptr<Abstract::XInputEventProcess>> proc_vector{};
		proc_vector.push_back(std::make_unique<TA>());
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

```

## 参考

### 命名空间 Petal::Abstract

#### 类 XInputEventProcess

派生自[Petal::Abstract::Process&lt;Petal::XInput::Controller::Resource>]()

##### 函数Check

返回值 Petal::boolean
根据传入的资源，检测事件是否被触发

### 命名空间 Petal::XInput

#### 类Button

本类为类枚举类，列举了XInput中不同按钮所对应的码，开发者可以使用位运算随意组合它们。
各值所代表的按钮可参照[MSDN](https://learn.microsoft.com/zh-cn/windows/win32/api/xinput/ns-xinput-xinput_gamepad)。

类型 Type 指示了本类列举的值的类型。

#### 类StickValue

本类为类枚举类，列举了XInput中摇杆的值的参照，对于 
WrappedGamepad::LeftStickX  
WrappedGamepad::LeftStickY  
WrappedGamepad::RightStickX  
WrappedGamepad::RightStickY  
函数，拿到的值为未经处理的值，开发者应当对XInput的XInputGetState和GAMEPAD结构有所了解，
本枚举类专为未经处理的摇杆的值提供了值  
full_up{ +0x7fff }  
full_right{ +0x7fff }  
full_down{ -0x8000 }  
full_left{ -0x8000 }  
half_up{ +0x3fff }  
half_right{ +0x3fff }  
half_down{ -0x4000 }  
half_left{ -0x4000 }  

通常使用WrappedGamepad::CalcXStickDirection系列的方法拿到经过处理的摇杆的值，对于这些值，可以直接使用本枚举类的full、half、threshold等值。

关于死区的值可参照[MSDN](https://learn.microsoft.com/zh-cn/windows/win32/api/xinput/ns-xinput-xinput_gamepad)。

类型 Type 指示了本类列举的值的类型。

### 命名空间 Petal::Concept

