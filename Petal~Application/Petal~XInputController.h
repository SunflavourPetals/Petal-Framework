#pragma once

#ifndef Petal_Header_XInputController
#define Petal_Header_XInputController

#include "Petal~BasicTypes.h"
#include "Petal~WinTypes.h"
#include "Petal~String.h"
#include "Petal~Process.h"

#include <Xinput.h>

namespace Petal::XInput
{
	using Petal::boolean;
	class WrappedGamepad;
	class Controller;
	struct ResourceOfController
	{
		const f64 delta_time;
		const Controller& controller;
	};
}

namespace Petal::Abstract
{
	class XInputEventProcess : public Process<Petal::XInput::ResourceOfController>
	{
	public:
		virtual const XInput::Controller& Controller() const final;
		virtual f64 DeltaTime() const final;
		virtual boolean Check() = 0;
	public:
		XInputEventProcess() = default;
		XInputEventProcess(const XInputEventProcess&) = default;
		XInputEventProcess(XInputEventProcess&&) noexcept = default;
		virtual ~XInputEventProcess() = default;
	};
}

namespace Petal::Concept
{
	template <typename Ty>
	concept XInputEventProcessIterator = requires
	{
		typename ::std::iterator_traits<Ty>::value_type;
		requires ::std::is_base_of_v<Abstract::XInputEventProcess, typename Ty::value_type>;
	};
	template <typename Ty>
	concept XInputEventProcessPointerIterator = requires (Ty o)
	{
		typename ::std::iterator_traits<Ty>::value_type;
		requires ::std::is_pointer_v<typename Ty::value_type>;
		requires ::std::is_base_of_v<Abstract::XInputEventProcess, typename ::std::remove_pointer_t<typename Ty::value_type>>;
		(*o)->Check();
	} || requires (Ty o)
	{
		typename ::std::iterator_traits<Ty>::value_type;
		typename Ty::value_type::element_type;
		requires ::std::is_base_of_v<Abstract::XInputEventProcess, typename Ty::value_type::element_type>;
		(*o)->Check();
	};
}

namespace Petal::XInput
{
	using State = typename ::XINPUT_STATE;
	using Gamepad = typename ::XINPUT_GAMEPAD;
	using BatteryInformation = typename ::XINPUT_BATTERY_INFORMATION;
	using Capabilities = typename ::XINPUT_CAPABILITIES;
	using Keystroke = typename ::XINPUT_KEYSTROKE;
	class Button
	{
	public:
		using Type = win32word;
		static constexpr Type Up{ XINPUT_GAMEPAD_DPAD_UP };
		static constexpr Type Down{ XINPUT_GAMEPAD_DPAD_DOWN };
		static constexpr Type Left{ XINPUT_GAMEPAD_DPAD_LEFT };
		static constexpr Type Right{ XINPUT_GAMEPAD_DPAD_RIGHT };
		static constexpr Type Start{ XINPUT_GAMEPAD_START };
		static constexpr Type Back{ XINPUT_GAMEPAD_BACK };
		static constexpr Type LeftStick{ XINPUT_GAMEPAD_LEFT_THUMB };
		static constexpr Type RightStick{ XINPUT_GAMEPAD_RIGHT_THUMB };
		static constexpr Type LeftShoulder{ XINPUT_GAMEPAD_LEFT_SHOULDER };
		static constexpr Type RightShoulder{ XINPUT_GAMEPAD_RIGHT_SHOULDER };
		static constexpr Type A{ XINPUT_GAMEPAD_A };
		static constexpr Type B{ XINPUT_GAMEPAD_B };
		static constexpr Type X{ XINPUT_GAMEPAD_X };
		static constexpr Type Y{ XINPUT_GAMEPAD_Y };
	};
	class StickValue
	{
	public:
		using Type = win32short;
		static constexpr Type full{ +0x7fff };
		static constexpr Type half{ +0x3fff };
		static constexpr Type full_up{ +0x7fff };
		static constexpr Type full_right{ +0x7fff };
		static constexpr Type full_down{ -0x8000 };
		static constexpr Type full_left{ -0x8000 };
		static constexpr Type half_up{ +0x3fff };
		static constexpr Type half_right{ +0x3fff };
		static constexpr Type half_down{ -0x4000 };
		static constexpr Type half_left{ -0x4000 };
		static constexpr Type left_dead_zone{ XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE };
		static constexpr Type right_dead_zone{ XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE };
		static constexpr Type threshold{ static_cast<Type>(0.265625 * full) };
	};
	class TriggerValue
	{
	public:
		using Type = win32byte;
		static constexpr Type full{ 0xffu };
		static constexpr Type half{ 0x7fu };
		static constexpr Type threshold{ XINPUT_GAMEPAD_TRIGGER_THRESHOLD };
	};
	class VibrationValue
	{
	public:
		using Type = win32word;
		static constexpr Type full{ 0xffffu };
		static constexpr Type half{ 0x7fffu };
	};
	class UserIndexValue
	{
	public:
		using Type = win32dword;
		static constexpr Type player1{ 0 };
		static constexpr Type player2{ 1 };
		static constexpr Type player3{ 2 };
		static constexpr Type player4{ 3 };
	};

	class WrappedGamepad final
	{
	public:
		boolean Connected() const noexcept; // 查看控制器是否已连接 需要提前调用 QueryState 获取或更新状态
		boolean CheckConnection() noexcept; // 检查控制器是否已连接 无需提前调用 QueryState
		boolean CheckConnection() const noexcept;
		UserIndexValue::Type UserIndex() const noexcept; // 查看控制器的用户索引
		const State& GetState() const noexcept; // 获得 XINPUT_GAMEPAD 信息 需要提前调用 QueryState 获取或更新状态
		const Gamepad& GetGamepad() const noexcept;
		boolean StateChanged() const noexcept; // 查看输入状态是否有变		需要提前调用 QueryState 获取或更新状态
		void ClearState() noexcept; // 清空属性
		boolean Pushed(Button::Type buttons) const noexcept; // 输入 XInputGamepad::Button 枚举的组合 查看该组合是否被激活(列举的所有按钮被按下)
		TriggerValue::Type LeftTrigger() const noexcept;
		TriggerValue::Type RightTrigger() const noexcept;
		StickValue::Type LeftStickX() const noexcept;
		StickValue::Type LeftStickY() const noexcept;
		StickValue::Type RightStickX() const noexcept;
		StickValue::Type RightStickY() const noexcept;
		StickValue::Type CalcLStickUp() const noexcept;
		StickValue::Type CalcLStickDown() const noexcept;
		StickValue::Type CalcLStickLeft() const noexcept;
		StickValue::Type CalcLStickRight() const noexcept;
		StickValue::Type CalcRStickUp() const noexcept;
		StickValue::Type CalcRStickDown() const noexcept;
		StickValue::Type CalcRStickLeft() const noexcept;
		StickValue::Type CalcRStickRight() const noexcept;
		boolean UpdateUserIndex(UserIndexValue::Type user_index) noexcept; // 指定新的用户索引
		win32dword QueryState() noexcept; // 查询控制器输入状态
		win32dword Vibration(VibrationValue::Type left_motor_speed, VibrationValue::Type right_motor_speed) const noexcept;	// 设置控制器震动 最小值 0 最大值 65535
		win32dword AudioDeviceIDs(ptr<WChar> render_device_id, win32uint& render_count, ptr<WChar> capture_device_id, win32uint& capture_count) const noexcept;
		win32dword GetBatteryInformation(BatteryInformation& battery_info) const noexcept;
		win32dword GetCapabilities(Capabilities& capabilities) const noexcept;
		win32dword GetKeystroke(Keystroke& keystroke) const noexcept;
	public:
		WrappedGamepad() = default;
		WrappedGamepad(UserIndexValue::Type user_index);
		WrappedGamepad(const WrappedGamepad&) = default;
		WrappedGamepad(WrappedGamepad&&) noexcept = default;
		~WrappedGamepad() = default;
		WrappedGamepad& operator = (const WrappedGamepad&) noexcept = default;
	private:
		UserIndexValue::Type pt_user_index{ 0 }; // 用户索引 数值为 0 到 3
		State pt_gamepad_state{}; // 控制器状态 调用 QueryState 后更新
		boolean pt_gamepad_connected{}; // 控制器是否已连接 调用 QueryState 后更新
		boolean pt_state_changed{}; // 控制器状态有无变化 调用 QueryState 后更新
	};

	class Controller final
	{
	public:
		using Resource = ResourceOfController;
	public:
		void ClearState() noexcept;
		void ClearLastState() noexcept;
		boolean UpdateUserIndex(UserIndexValue::Type user_index) noexcept;
		win32dword QueryState() noexcept;
		win32dword Update(f64 delta_time, Concept::XInputEventProcessIterator auto begin, Concept::XInputEventProcessIterator auto end);
		win32dword Update(f64 delta_time, Concept::XInputEventProcessPointerIterator auto begin, Concept::XInputEventProcessPointerIterator auto end);
		const WrappedGamepad& GetWrappedGamepad() const noexcept;
		const WrappedGamepad& GetLastWrappedGamepad() const noexcept;
	public:
		Controller() = default;
		Controller(UserIndexValue::Type user_index);
		Controller(const Controller&) = default;
		Controller(Controller&&) noexcept = default;
		~Controller() = default;
		Controller& operator = (const Controller&) noexcept = default;
	private:
		WrappedGamepad pt_gamepad;
		WrappedGamepad pt_last_gamepad;
	};

	inline win32dword Controller::Update(f64 delta_time, Concept::XInputEventProcessIterator auto begin, Concept::XInputEventProcessIterator auto end)
	{
		auto result{ this->QueryState() };
		Resource resource{ delta_time, *this };
		for (; begin != end; ++begin)
		{
			begin->AssignResource(resource);
			if (begin->Check())
			{
				begin->Execution();
			}
			begin->RemoveResource();
		}
		return result;
	}

	inline win32dword Controller::Update(f64 delta_time, Concept::XInputEventProcessPointerIterator auto begin, Concept::XInputEventProcessPointerIterator auto end)
	{
		auto result{ this->QueryState() };
		Resource resource{ delta_time, *this };
		for (; begin != end; ++begin)
		{
			(*begin)->AssignResource(resource);
			if ((*begin) != nullptr && (*begin)->Check())
			{
				(*begin)->Execution();
			}
			(*begin)->RemoveResource();
		}
		return result;
	}
}

#endif // !Petal_Header_XInputController
