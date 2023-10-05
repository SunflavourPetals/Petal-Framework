#pragma once

#ifndef Petal_Header_XInputEventProcess
#define Petal_Header_XInputEventProcess

#include "Petal~XInputController.h"

namespace Petal::XInput
{
	enum class TriggerDimension : i32
	{
		Left = 0x1,
		Right = 0x2,
	};
	enum class StickDimension : i16
	{
		Left = 0x1,
		Right = 0x2,
	};
	enum class DirectionDimension : i16
	{
		Up = 0x1,
		Down = 0x2,
		Left = 0x3,
		Right = 0x4,
	};
}

namespace Petal::XInput::MiddleProcess
{
	using Petal::boolean;
	class BasicMiddleProcess : public Abstract::XInputEventProcess
	{
	protected:
		virtual boolean GamepadPositive(const Gamepad& gamepad) const = 0;
		virtual boolean LastPositive() const = 0;
		virtual boolean ThisPositive() const = 0;
	public:
		BasicMiddleProcess() = default;
		BasicMiddleProcess(const BasicMiddleProcess&) = default;
		BasicMiddleProcess(BasicMiddleProcess&&) noexcept = default;
		~BasicMiddleProcess() = default;
	};

	class XHoldProcess : virtual public BasicMiddleProcess
	{
	public:
		virtual boolean Check();
	public:
		virtual void UpdateTargetTime(f64 target_time) noexcept final; // hold target_time to triggering
		virtual f64 TargetTime() const noexcept final;
		virtual void UpdateLoopMode(boolean loop_mode) noexcept final; // loop triggering target_time to target_time
		virtual boolean LoopMode() const noexcept final;
	public:
		XHoldProcess(f64 target_time = 1.0, boolean loop_mode = false);
		XHoldProcess(const XHoldProcess&) = default;
		XHoldProcess(XHoldProcess&&) noexcept = default;
		~XHoldProcess() = default;
	private:
		f64 pt_target_time{ 1.0 };
		i64 pt_total_count{};
		boolean pt_loop_triggering{ false };
		boolean pt_in_holding{ false };
	};

	class ButtonProcess : virtual public BasicMiddleProcess
	{
	protected:
		virtual boolean GamepadPositive(const Gamepad& gamepad) const override;
		virtual boolean LastPositive() const override;
		virtual boolean ThisPositive() const override;
	public:
		virtual void UpdateButtons(Button::Type buttons) noexcept final;
		virtual Button::Type Buttons() const noexcept final;
		virtual boolean Check() = 0;
	public:
		ButtonProcess(Button::Type buttons = Button::A);
		ButtonProcess(const ButtonProcess&) = default;
		ButtonProcess(ButtonProcess&&) noexcept = default;
		~ButtonProcess() = default;
	protected:
		Button::Type pt_buttons{ Button::A };
	};

	class TriggerProcess : virtual public BasicMiddleProcess
	{
	protected:
		virtual boolean GamepadPositive(const Gamepad& gamepad) const override;
		virtual boolean LastPositive() const override;
		virtual boolean ThisPositive() const override;
	public:
		virtual void UpdateTargetTriggerValue(TriggerValue::Type target_value) noexcept final;
		virtual TriggerValue::Type TargetTriggerValue() const noexcept final;
		virtual void UpdateTriggerDimension(TriggerDimension dimension) noexcept final;
		virtual TriggerDimension TriggerDimension() const noexcept final;
		virtual boolean Check() = 0;
	public:
		TriggerProcess(XInput::TriggerDimension trigger_dimension = XInput::TriggerDimension::Left, TriggerValue::Type target_value = TriggerValue::threshold);
		TriggerProcess(const TriggerProcess&) = default;
		TriggerProcess(TriggerProcess&&) noexcept = default;
		~TriggerProcess() = default;
	protected:
		XInput::TriggerDimension pt_trigger_dimension{ XInput::TriggerDimension::Left };
		TriggerValue::Type pt_target_trigger_value{ TriggerValue::threshold };
	};

	class StickProcess : virtual public BasicMiddleProcess
	{
	protected:
		virtual boolean GamepadPositive(const Gamepad& gamepad) const override;
		virtual boolean LastPositive() const override;
		virtual boolean ThisPositive() const override;
	public:
		virtual void UpdateTargetStickValue(StickValue::Type target_value) noexcept final;
		virtual StickValue::Type TargetStickValue() const noexcept final;
		virtual void UpdateStickDimension(XInput::StickDimension dimension) noexcept final;
		virtual StickDimension StickDimension() const noexcept final;
		virtual void UpdateDirectionDimension(XInput::DirectionDimension dimension) noexcept final;
		virtual DirectionDimension DirectionDimension() const noexcept final;
		virtual boolean Check() = 0;
	public:
		StickProcess(XInput::StickDimension stick_dimension = XInput::StickDimension::Left, XInput::DirectionDimension direction_dimension = XInput::DirectionDimension::Left, StickValue::Type target_value = StickValue::threshold);
		StickProcess(const StickProcess& rhs) = default;
		StickProcess(StickProcess&& rhs) = default;
		~StickProcess() = default;
	protected:
		XInput::StickDimension pt_stick_dimension{ XInput::StickDimension::Left };
		XInput::DirectionDimension pt_direction_dimension{ XInput::DirectionDimension::Left };
		StickValue::Type pt_target_stick_value{ StickValue::threshold };
	};
}

namespace Petal::XInput
{
	// ButtonProcess Derived
	class ButtonPushProcess : public MiddleProcess::ButtonProcess
	{
	public:
		virtual boolean Check() override;
	public:
		ButtonPushProcess(Button::Type buttons = Button::A);
	};
	// ButtonProcess Derived
	class ButtonReleaseProcess : public MiddleProcess::ButtonProcess
	{
	public:
		virtual boolean Check() override;
	public:
		ButtonReleaseProcess(Button::Type buttons = Button::A);
	};
	// ButtonProcess Derived
	class ButtonPositiveProcess : public MiddleProcess::ButtonProcess
	{
	public:
		virtual boolean Check() override;
	public:
		ButtonPositiveProcess(Button::Type buttons = Button::A);
	};
	// ButtonProcess Derived
	class ButtonNegativeProcess : public MiddleProcess::ButtonProcess
	{
	public:
		virtual boolean Check() override;
	public:
		ButtonNegativeProcess(Button::Type buttons = Button::A);
	};
	// ButtonProcess Derived & XHoldProcess Derived
	class ButtonHoldProcess : public MiddleProcess::ButtonProcess, public MiddleProcess::XHoldProcess
	{
	protected:
		virtual boolean GamepadPositive(const Gamepad& gamepad) const override final;
		virtual boolean LastPositive() const override final;
		virtual boolean ThisPositive() const override final;
	public:
		virtual boolean Check() override;
	public:
		ButtonHoldProcess(Button::Type buttons = Button::A, f64 target_time = 1.0, boolean loop_mode = false);
	};
	// TriggerProcess Derived
	class TriggerPushProcess : public MiddleProcess::TriggerProcess
	{
	public:
		virtual boolean Check() override;
	public:
		TriggerPushProcess(XInput::TriggerDimension dimension = XInput::TriggerDimension::Left, TriggerValue::Type target_value = TriggerValue::threshold);
	};
	// TriggerProcess Derived
	class TriggerReleaseProcess : public MiddleProcess::TriggerProcess
	{
	public:
		virtual boolean Check() override;
	public:
		TriggerReleaseProcess(XInput::TriggerDimension dimension = XInput::TriggerDimension::Left, TriggerValue::Type target_value = TriggerValue::threshold);
	};
	// TriggerProcess Derived
	class TriggerPositiveProcess : public MiddleProcess::TriggerProcess
	{
	public:
		virtual boolean Check() override;
	public:
		TriggerPositiveProcess(XInput::TriggerDimension dimension = XInput::TriggerDimension::Left, TriggerValue::Type target_value = TriggerValue::threshold);
	};
	// TriggerProcess Derived
	class TriggerNegativeProcess : public MiddleProcess::TriggerProcess
	{
	public:
		virtual boolean Check() override;
	public:
		TriggerNegativeProcess(XInput::TriggerDimension dimension = XInput::TriggerDimension::Left, TriggerValue::Type target_value = TriggerValue::threshold);
	};
	// TriggerProcess Derived & XHoldProcess Derived
	class TriggerHoldProcess : public MiddleProcess::TriggerProcess, public MiddleProcess::XHoldProcess
	{
	protected:
		virtual boolean GamepadPositive(const Gamepad& gamepad) const override final;
		virtual boolean LastPositive() const override final;
		virtual boolean ThisPositive() const override final;
	public:
		virtual boolean Check() override;
	public:
		TriggerHoldProcess(XInput::TriggerDimension dimension = XInput::TriggerDimension::Left, TriggerValue::Type target_value = TriggerValue::threshold, f64 target_time = 1.0, boolean loop_mode = false);
	};
	// StickProcess Derived
	class StickPushProcess : public MiddleProcess::StickProcess
	{
	public:
		virtual boolean Check() override;
	public:
		StickPushProcess(XInput::StickDimension dimension = XInput::StickDimension::Left, XInput::DirectionDimension direction = XInput::DirectionDimension::Up, StickValue::Type target_value = StickValue::threshold);
	};
	// StickProcess Derived
	class StickReleaseProcess : public MiddleProcess::StickProcess
	{
	public:
		virtual boolean Check() override;
	public:
		StickReleaseProcess(XInput::StickDimension dimension = XInput::StickDimension::Left, XInput::DirectionDimension direction = XInput::DirectionDimension::Up, StickValue::Type target_value = StickValue::threshold);
	};
	// StickProcess Derived
	class StickPositiveProcess : public MiddleProcess::StickProcess
	{
	public:
		virtual boolean Check() override;
	public:
		StickPositiveProcess(XInput::StickDimension dimension = XInput::StickDimension::Left, XInput::DirectionDimension direction = XInput::DirectionDimension::Up, StickValue::Type target_value = StickValue::threshold);
	};
	// StickProcess Derived
	class StickNegativeProcess : public MiddleProcess::StickProcess
	{
	public:
		virtual boolean Check() override;
	public:
		StickNegativeProcess(XInput::StickDimension dimension = XInput::StickDimension::Left, XInput::DirectionDimension direction = XInput::DirectionDimension::Up, StickValue::Type target_value = StickValue::threshold);
	};
	// StickProcess Derived & XHoldProcess Derived
	class StickHoldProcess : public MiddleProcess::StickProcess, public MiddleProcess::XHoldProcess
	{
	protected:
		virtual boolean GamepadPositive(const Gamepad& gamepad) const override final;
		virtual boolean LastPositive() const override final;
		virtual boolean ThisPositive() const override final;
	public:
		virtual boolean Check() override;
	public:
		StickHoldProcess(XInput::StickDimension dimension = XInput::StickDimension::Left, XInput::DirectionDimension direction = XInput::DirectionDimension::Up, StickValue::Type target_value = StickValue::threshold, f64 target_time = 1.0, boolean loop_mode = false);
	};
}

#endif // !Petal_Header_XInputEventProcess
