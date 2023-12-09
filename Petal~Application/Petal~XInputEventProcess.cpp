#include "Petal~XInputEventProcess.h"

namespace Petal::XInput
{
	// = = XInputButtonProcess Derived = =
	// XInputButtonPushProcess
	ButtonPushProcess::ButtonPushProcess(Button::Type buttons) :
		ButtonProcess(buttons)
	{

	}
	boolean ButtonPushProcess::Check(const Resource& resource)
	{
		return (!this->LastPositive(resource.controller) && this->ThisPositive(resource.controller));
	}
	// XInputButtonReleaseProcess
	ButtonReleaseProcess::ButtonReleaseProcess(Button::Type buttons) :
		ButtonProcess(buttons)
	{

	}
	boolean ButtonReleaseProcess::Check(const Resource& resource)
	{
		return (this->LastPositive(resource.controller) && !this->ThisPositive(resource.controller));
	}
	// XInputButtonPositiveProcess
	ButtonPositiveProcess::ButtonPositiveProcess(Button::Type buttons) :
		ButtonProcess(buttons)
	{

	}
	boolean ButtonPositiveProcess::Check(const Resource& resource)
	{
		return this->ThisPositive(resource.controller);
	}
	// XInputButtonNegativeProcess
	ButtonNegativeProcess::ButtonNegativeProcess(Button::Type buttons) :
		ButtonProcess(buttons)
	{

	}
	boolean ButtonNegativeProcess::Check(const Resource& resource)
	{
		return !this->ThisPositive(resource.controller);
	}
	// XInputButtonHoldProcess
	ButtonHoldProcess::ButtonHoldProcess(Button::Type buttons, i64 target_count, boolean loop_mode) :
		ButtonProcess(buttons),
		XHoldProcess(target_count, loop_mode)
	{

	}
	boolean ButtonHoldProcess::GamepadPositive(const WrappedGamepad& gamepad) const
	{
		return this->ButtonProcess::GamepadPositive(gamepad);
	}
	boolean ButtonHoldProcess::LastPositive(const Controller& controller) const
	{
		return this->ButtonProcess::LastPositive(controller);
	}
	boolean ButtonHoldProcess::ThisPositive(const Controller& controller) const
	{
		return this->ButtonProcess::ThisPositive(controller);
	}
	boolean ButtonHoldProcess::Check(const Resource& resource)
	{
		return this->XHoldProcess::Check(resource);
	}

	// = = XInputTriggerProcess Derived = =
	// XInputTriggerPushProcess
	TriggerPushProcess::TriggerPushProcess(XInput::TriggerDimension dimension, TriggerValue::Type target_value) :
		TriggerProcess(dimension, target_value)
	{

	}
	boolean TriggerPushProcess::Check(const Resource& resource)
	{
		return (!this->LastPositive(resource.controller) && this->ThisPositive(resource.controller));
	}
	// XInputTriggerReleaseProcess
	TriggerReleaseProcess::TriggerReleaseProcess(XInput::TriggerDimension dimension, TriggerValue::Type target_value) :
		TriggerProcess(dimension, target_value)
	{

	}
	boolean TriggerReleaseProcess::Check(const Resource& resource)
	{
		return (this->LastPositive(resource.controller) && !this->ThisPositive(resource.controller));
	}
	// XInputTriggerPositiveProcess
	TriggerPositiveProcess::TriggerPositiveProcess(XInput::TriggerDimension dimension, TriggerValue::Type target_value) :
		TriggerProcess(dimension, target_value)
	{

	}
	boolean TriggerPositiveProcess::Check(const Resource& resource)
	{
		return this->ThisPositive(resource.controller);
	}
	// XInputTriggerNegativeProcess
	TriggerNegativeProcess::TriggerNegativeProcess(XInput::TriggerDimension dimension, TriggerValue::Type target_value) :
		TriggerProcess(dimension, target_value)
	{

	}
	boolean TriggerNegativeProcess::Check(const Resource& resource)
	{
		return !this->ThisPositive(resource.controller);
	}
	// XInputTriggerHoldProcess
	TriggerHoldProcess::TriggerHoldProcess(XInput::TriggerDimension dimension, TriggerValue::Type target_value, i64 target_count, boolean loop_mode) :
		TriggerProcess(dimension, target_value),
		XHoldProcess(target_count, loop_mode)
	{

	}
	boolean TriggerHoldProcess::GamepadPositive(const WrappedGamepad& gamepad) const
	{
		return this->TriggerProcess::GamepadPositive(gamepad);
	}
	boolean TriggerHoldProcess::LastPositive(const Controller& controller) const
	{
		return this->TriggerProcess::LastPositive(controller);
	}
	boolean TriggerHoldProcess::ThisPositive(const Controller& controller) const
	{
		return this->TriggerProcess::ThisPositive(controller);
	}
	boolean TriggerHoldProcess::Check(const Resource& resource)
	{
		return this->XHoldProcess::Check(resource);
	}

	// = = XInputStickProcess Derived = =
	// XInputStickPushProcess
	StickPushProcess::StickPushProcess(XInput::StickDimension dimension, XInput::DirectionDimension direction, StickValue::Type target_value) :
		StickProcess(dimension, direction, target_value)
	{

	}
	boolean StickPushProcess::Check(const Resource& resource)
	{
		return (!this->LastPositive(resource.controller) && this->ThisPositive(resource.controller));
	}
	// XInputStickReleaseProcess
	StickReleaseProcess::StickReleaseProcess(XInput::StickDimension dimension, XInput::DirectionDimension direction, StickValue::Type target_value) :
		StickProcess(dimension, direction, target_value)
	{

	}
	boolean StickReleaseProcess::Check(const Resource& resource)
	{
		return (this->LastPositive(resource.controller) && !this->ThisPositive(resource.controller));
	}
	// XInputStickPositiveProcess
	StickPositiveProcess::StickPositiveProcess(XInput::StickDimension dimension, XInput::DirectionDimension direction, StickValue::Type target_value) :
		StickProcess(dimension, direction, target_value)
	{

	}
	boolean StickPositiveProcess::Check(const Resource& resource)
	{
		return this->ThisPositive(resource.controller);
	}
	// XInputStickNegativeProcess
	StickNegativeProcess::StickNegativeProcess(XInput::StickDimension dimension, XInput::DirectionDimension direction, StickValue::Type target_value) :
		StickProcess(dimension, direction, target_value)
	{

	}
	boolean StickNegativeProcess::Check(const Resource& resource)
	{
		return !this->ThisPositive(resource.controller);
	}
	// XInputStickHoldProcess
	StickHoldProcess::StickHoldProcess(XInput::StickDimension dimension, XInput::DirectionDimension direction, StickValue::Type target_value, i64 target_count, boolean loop_mode) :
		StickProcess(dimension, direction, target_value),
		XHoldProcess(target_count, loop_mode)
	{

	}
	boolean StickHoldProcess::GamepadPositive(const WrappedGamepad& gamepad) const
	{
		return this->StickProcess::GamepadPositive(gamepad);
	}
	boolean StickHoldProcess::LastPositive(const Controller& controller) const
	{
		return this->StickProcess::LastPositive(controller);
	}
	boolean StickHoldProcess::ThisPositive(const Controller& controller) const
	{
		return this->StickProcess::ThisPositive(controller);
	}
	boolean StickHoldProcess::Check(const Resource& resource)
	{
		return this->XHoldProcess::Check(resource);
	}
}

namespace Petal::XInput::MiddleProcess
{
	// XInputXHold
	XHoldProcess::XHoldProcess(Tick target_count, boolean loop_mode) :
		BasicMiddleProcess(),
		pt_target_count(target_count),
		pt_loop_triggering(loop_mode)
	{

	}
	void XHoldProcess::UpdateTargetCount(Tick target_count) noexcept
	{
		this->pt_target_count = target_count;
	}
	XHoldProcess::Tick XHoldProcess::TargetCount() const noexcept
	{
		return this->pt_target_count;
	}
	void XHoldProcess::UpdateLoopMode(boolean loop_mode) noexcept
	{
		this->pt_loop_triggering = loop_mode;
	}
	boolean XHoldProcess::LoopMode() const noexcept
	{
		return this->pt_loop_triggering;
	}
	boolean XHoldProcess::Check(const Resource& resource)
	{
		if (this->pt_in_holding)
		{
			if (this->ThisPositive(resource.controller))
			{
				this->pt_total_count += resource.delta_count;
			}
			else
			{
				this->pt_in_holding = false;
				this->pt_total_count = 0;
				return false;
			}
		}
		else
		{
			if (!this->LastPositive(resource.controller) && this->ThisPositive(resource.controller))
			{
				this->pt_total_count = 0;
				this->pt_in_holding = true;
			}
			else
			{
				return false;
			}
		}
		if (this->pt_total_count >= this->pt_target_count)
		{
			this->pt_total_count = 0;
			if (!this->pt_loop_triggering)
			{
				this->pt_in_holding = false;
			}
			return true;
		}
		return false;
	}

	// XInputButtonProcess
	ButtonProcess::ButtonProcess(Button::Type buttons) :
		BasicMiddleProcess(),
		pt_buttons(buttons)
	{

	}
	void ButtonProcess::UpdateButtons(Button::Type buttons) noexcept
	{
		this->pt_buttons = buttons;
	}
	Button::Type ButtonProcess::Buttons() const noexcept
	{
		return this->pt_buttons;
	}
	boolean ButtonProcess::GamepadPositive(const WrappedGamepad& gamepad) const
	{
		return gamepad.Pushed(this->pt_buttons);
	}
	boolean ButtonProcess::LastPositive(const Controller& controller) const
	{
		return this->GamepadPositive(controller.GetLastWrappedGamepad());
	}
	boolean ButtonProcess::ThisPositive(const Controller& controller) const
	{
		return this->GamepadPositive(controller.GetWrappedGamepad());
	}

	// XInputTriggerProcess
	TriggerProcess::TriggerProcess(XInput::TriggerDimension trigger_dimension, TriggerValue::Type target_value) :
		BasicMiddleProcess(),
		pt_trigger_dimension(trigger_dimension),
		pt_target_trigger_value(target_value)
	{

	}
	void TriggerProcess::UpdateTargetTriggerValue(TriggerValue::Type target_value) noexcept
	{
		this->pt_target_trigger_value = target_value;
	}
	TriggerValue::Type TriggerProcess::TargetTriggerValue() const noexcept
	{
		return this->pt_target_trigger_value;
	}
	void TriggerProcess::UpdateTriggerDimension(XInput::TriggerDimension dimension) noexcept
	{
		this->pt_trigger_dimension = dimension;
	}
	TriggerDimension TriggerProcess::TriggerDimension() const noexcept
	{
		return this->pt_trigger_dimension;
	}
	boolean TriggerProcess::GamepadPositive(const WrappedGamepad& gamepad) const
	{
		switch (this->pt_trigger_dimension)
		{
		case Petal::XInput::TriggerDimension::Left:
			if (gamepad.LeftTrigger() >= this->pt_target_trigger_value) return true;
			return false;
			break;
		case Petal::XInput::TriggerDimension::Right:
			if (gamepad.RightTrigger() >= this->pt_target_trigger_value) return true;
			return false;
			break;
		default:
			break;
		}
		return false;
	}
	boolean TriggerProcess::LastPositive(const Controller& controller) const
	{
		return this->GamepadPositive(controller.GetLastWrappedGamepad());
	}
	boolean TriggerProcess::ThisPositive(const Controller& controller) const
	{
		return this->GamepadPositive(controller.GetWrappedGamepad());
	}

	// XInputStickProcess
	StickProcess::StickProcess(XInput::StickDimension stick_dimension, XInput::DirectionDimension direction_dimension, StickValue::Type target_value) :
		BasicMiddleProcess(),
		pt_stick_dimension(stick_dimension),
		pt_direction_dimension(direction_dimension),
		pt_target_stick_value(target_value)
	{

	}
	void StickProcess::UpdateTargetStickValue(StickValue::Type target_value) noexcept
	{
		this->pt_target_stick_value = target_value;
	}
	StickValue::Type StickProcess::TargetStickValue() const noexcept
	{
		return this->pt_target_stick_value;
	}
	void StickProcess::UpdateStickDimension(XInput::StickDimension dimension) noexcept
	{
		this->pt_stick_dimension = dimension;
	}
	StickDimension StickProcess::StickDimension() const noexcept
	{
		return this->pt_stick_dimension;
	}
	void StickProcess::UpdateDirectionDimension(XInput::DirectionDimension dimension) noexcept
	{
		this->pt_direction_dimension = dimension;
	}
	DirectionDimension StickProcess::DirectionDimension() const noexcept
	{
		return this->pt_direction_dimension;
	}
	boolean StickProcess::GamepadPositive(const WrappedGamepad& gamepad) const
	{
		switch (this->pt_stick_dimension)
		{
		case XInput::StickDimension::Left:
			switch (this->pt_direction_dimension)
			{
			case Petal::XInput::DirectionDimension::Up:
				if (gamepad.LeftStickY() >= this->pt_target_stick_value) return true;
				return false;
				break;
			case Petal::XInput::DirectionDimension::Down:
				if (gamepad.LeftStickY() < -this->pt_target_stick_value) return true;
				return false;
				break;
			case Petal::XInput::DirectionDimension::Left:
				if (gamepad.LeftStickX() < -this->pt_target_stick_value) return true;
				return false;
				break;
			case Petal::XInput::DirectionDimension::Right:
				if (gamepad.LeftStickX() >= this->pt_target_stick_value) return true;
				return false;
				break;
			default:
				break;
			}
			break;
		case XInput::StickDimension::Right:
			switch (this->pt_direction_dimension)
			{
			case Petal::XInput::DirectionDimension::Up:
				if (gamepad.RightStickY() >= this->pt_target_stick_value) return true;
				return false;
				break;
			case Petal::XInput::DirectionDimension::Down:
				if (gamepad.RightStickY() < -this->pt_target_stick_value) return true;
				return false;
				break;
			case Petal::XInput::DirectionDimension::Left:
				if (gamepad.RightStickX() < -this->pt_target_stick_value) return true;
				return false;
				break;
			case Petal::XInput::DirectionDimension::Right:
				if (gamepad.RightStickX() >= this->pt_target_stick_value) return true;
				return false;
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
		return false;
	}
	boolean StickProcess::LastPositive(const Controller& controller) const
	{
		return this->GamepadPositive(controller.GetLastWrappedGamepad());
	}
	boolean StickProcess::ThisPositive(const Controller& controller) const
	{
		return this->GamepadPositive(controller.GetWrappedGamepad());
	}
}
