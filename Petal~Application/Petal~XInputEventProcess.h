#pragma once

#ifndef Petal_Header_XInputEventProcess
#define Petal_Header_XInputEventProcess

#include "Petal~XInputController.h"
#include "Petal~PerformanceCounter.h"

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
		virtual boolean GamepadPositive(const WrappedGamepad& gamepad) const = 0;
		virtual boolean LastPositive(const Controller& controller) const = 0;
		virtual boolean ThisPositive(const Controller& controller) const = 0;
	public:
		BasicMiddleProcess() = default;
		BasicMiddleProcess(const BasicMiddleProcess&) = default;
		BasicMiddleProcess(BasicMiddleProcess&&) noexcept = default;
		~BasicMiddleProcess() = default;
	};

	class XHoldProcess : virtual public BasicMiddleProcess
	{
	public:
		using Tick = typename PerformanceCounter::Tick;
	public:
		virtual boolean Check(const Resource&) override;
	public:
		virtual void UpdateTargetCount(Tick target_count) noexcept final; // hold target_count to triggering
		virtual Tick TargetCount() const noexcept final;
		virtual void UpdateLoopMode(boolean loop_mode) noexcept final; // loop triggering target_count to target_count
		virtual boolean LoopMode() const noexcept final;
	public:
		XHoldProcess(Tick target_count = 1, boolean loop_mode = false);
		XHoldProcess(const XHoldProcess&) = default;
		XHoldProcess(XHoldProcess&&) noexcept = default;
		~XHoldProcess() = default;
	private:
		Tick pt_target_count{ 1 };
		Tick pt_total_count{};
		boolean pt_loop_triggering{ false };
		boolean pt_in_holding{ false };
	};

	class ButtonProcess : virtual public BasicMiddleProcess
	{
	protected:
		virtual boolean GamepadPositive(const WrappedGamepad& gamepad) const override;
		virtual boolean LastPositive(const Controller& controller) const override;
		virtual boolean ThisPositive(const Controller& controller) const override;
	public:
		virtual void UpdateButtons(Button::Type buttons) noexcept final;
		virtual Button::Type Buttons() const noexcept final;
		virtual boolean Check(const Resource&) = 0;
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
		virtual boolean GamepadPositive(const WrappedGamepad& gamepad) const override;
		virtual boolean LastPositive(const Controller& controller) const override;
		virtual boolean ThisPositive(const Controller& controller) const override;
	public:
		virtual void UpdateTargetTriggerValue(TriggerValue::Type target_value) noexcept final;
		virtual TriggerValue::Type TargetTriggerValue() const noexcept final;
		virtual void UpdateTriggerDimension(TriggerDimension dimension) noexcept final;
		virtual TriggerDimension TriggerDimension() const noexcept final;
		virtual boolean Check(const Resource&) = 0;
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
		virtual boolean GamepadPositive(const WrappedGamepad& gamepad) const override;
		virtual boolean LastPositive(const Controller& controller) const override;
		virtual boolean ThisPositive(const Controller& controller) const override;
	public:
		virtual void UpdateTargetStickValue(StickValue::Type target_value) noexcept final;
		virtual StickValue::Type TargetStickValue() const noexcept final;
		virtual void UpdateStickDimension(XInput::StickDimension dimension) noexcept final;
		virtual StickDimension StickDimension() const noexcept final;
		virtual void UpdateDirectionDimension(XInput::DirectionDimension dimension) noexcept final;
		virtual DirectionDimension DirectionDimension() const noexcept final;
		virtual boolean Check(const Resource&) = 0;
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
		virtual boolean Check(const Resource&) override;
	public:
		ButtonPushProcess(Button::Type buttons = Button::A);
	};
	// ButtonProcess Derived
	class ButtonReleaseProcess : public MiddleProcess::ButtonProcess
	{
	public:
		virtual boolean Check(const Resource&) override;
	public:
		ButtonReleaseProcess(Button::Type buttons = Button::A);
	};
	// ButtonProcess Derived
	class ButtonPositiveProcess : public MiddleProcess::ButtonProcess
	{
	public:
		virtual boolean Check(const Resource&) override;
	public:
		ButtonPositiveProcess(Button::Type buttons = Button::A);
	};
	// ButtonProcess Derived
	class ButtonNegativeProcess : public MiddleProcess::ButtonProcess
	{
	public:
		virtual boolean Check(const Resource&) override;
	public:
		ButtonNegativeProcess(Button::Type buttons = Button::A);
	};
	// ButtonProcess Derived & XHoldProcess Derived
	class ButtonHoldProcess : public MiddleProcess::ButtonProcess, public MiddleProcess::XHoldProcess
	{
	protected:
		virtual boolean GamepadPositive(const WrappedGamepad& gamepad) const override final;
		virtual boolean LastPositive(const Controller& controller) const override final;
		virtual boolean ThisPositive(const Controller& controller) const override final;
	public:
		virtual boolean Check(const Resource&) override;
	public:
		ButtonHoldProcess(Button::Type buttons = Button::A, i64 target_count = 1, boolean loop_mode = false);
	};
	// TriggerProcess Derived
	class TriggerPushProcess : public MiddleProcess::TriggerProcess
	{
	public:
		virtual boolean Check(const Resource&) override;
	public:
		TriggerPushProcess(XInput::TriggerDimension dimension = XInput::TriggerDimension::Left, TriggerValue::Type target_value = TriggerValue::threshold);
	};
	// TriggerProcess Derived
	class TriggerReleaseProcess : public MiddleProcess::TriggerProcess
	{
	public:
		virtual boolean Check(const Resource&) override;
	public:
		TriggerReleaseProcess(XInput::TriggerDimension dimension = XInput::TriggerDimension::Left, TriggerValue::Type target_value = TriggerValue::threshold);
	};
	// TriggerProcess Derived
	class TriggerPositiveProcess : public MiddleProcess::TriggerProcess
	{
	public:
		virtual boolean Check(const Resource&) override;
	public:
		TriggerPositiveProcess(XInput::TriggerDimension dimension = XInput::TriggerDimension::Left, TriggerValue::Type target_value = TriggerValue::threshold);
	};
	// TriggerProcess Derived
	class TriggerNegativeProcess : public MiddleProcess::TriggerProcess
	{
	public:
		virtual boolean Check(const Resource&) override;
	public:
		TriggerNegativeProcess(XInput::TriggerDimension dimension = XInput::TriggerDimension::Left, TriggerValue::Type target_value = TriggerValue::threshold);
	};
	// TriggerProcess Derived & XHoldProcess Derived
	class TriggerHoldProcess : public MiddleProcess::TriggerProcess, public MiddleProcess::XHoldProcess
	{
	protected:
		virtual boolean GamepadPositive(const WrappedGamepad& gamepad) const override final;
		virtual boolean LastPositive(const Controller& controller) const override final;
		virtual boolean ThisPositive(const Controller& controller) const override final;
	public:
		virtual boolean Check(const Resource&) override;
	public:
		TriggerHoldProcess(XInput::TriggerDimension dimension = XInput::TriggerDimension::Left, TriggerValue::Type target_value = TriggerValue::threshold, i64 target_count = 1, boolean loop_mode = false);
	};
	// StickProcess Derived
	class StickPushProcess : public MiddleProcess::StickProcess
	{
	public:
		virtual boolean Check(const Resource&) override;
	public:
		StickPushProcess(XInput::StickDimension dimension = XInput::StickDimension::Left, XInput::DirectionDimension direction = XInput::DirectionDimension::Up, StickValue::Type target_value = StickValue::threshold);
	};
	// StickProcess Derived
	class StickReleaseProcess : public MiddleProcess::StickProcess
	{
	public:
		virtual boolean Check(const Resource&) override;
	public:
		StickReleaseProcess(XInput::StickDimension dimension = XInput::StickDimension::Left, XInput::DirectionDimension direction = XInput::DirectionDimension::Up, StickValue::Type target_value = StickValue::threshold);
	};
	// StickProcess Derived
	class StickPositiveProcess : public MiddleProcess::StickProcess
	{
	public:
		virtual boolean Check(const Resource&) override;
	public:
		StickPositiveProcess(XInput::StickDimension dimension = XInput::StickDimension::Left, XInput::DirectionDimension direction = XInput::DirectionDimension::Up, StickValue::Type target_value = StickValue::threshold);
	};
	// StickProcess Derived
	class StickNegativeProcess : public MiddleProcess::StickProcess
	{
	public:
		virtual boolean Check(const Resource&) override;
	public:
		StickNegativeProcess(XInput::StickDimension dimension = XInput::StickDimension::Left, XInput::DirectionDimension direction = XInput::DirectionDimension::Up, StickValue::Type target_value = StickValue::threshold);
	};
	// StickProcess Derived & XHoldProcess Derived
	class StickHoldProcess : public MiddleProcess::StickProcess, public MiddleProcess::XHoldProcess
	{
	protected:
		virtual boolean GamepadPositive(const WrappedGamepad& gamepad) const override final;
		virtual boolean LastPositive(const Controller& controller) const override final;
		virtual boolean ThisPositive(const Controller& controller) const override final;
	public:
		virtual boolean Check(const Resource&) override;
	public:
		StickHoldProcess(XInput::StickDimension dimension = XInput::StickDimension::Left, XInput::DirectionDimension direction = XInput::DirectionDimension::Up, StickValue::Type target_value = StickValue::threshold, i64 target_count = 1, boolean loop_mode = false);
	};
}

#endif // !Petal_Header_XInputEventProcess
