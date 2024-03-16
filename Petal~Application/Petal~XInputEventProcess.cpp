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
		target_count{ target_count },
		loop_triggering{ loop_mode }
	{

	}
	void XHoldProcess::UpdateTargetCount(Tick target_count) noexcept
	{
		this->target_count = target_count;
	}
	XHoldProcess::Tick XHoldProcess::TargetCount() const noexcept
	{
		return this->target_count;
	}
	void XHoldProcess::UpdateLoopMode(boolean loop_mode) noexcept
	{
		this->loop_triggering = loop_mode;
	}
	boolean XHoldProcess::LoopMode() const noexcept
	{
		return this->loop_triggering;
	}
	boolean XHoldProcess::Check(const Resource& resource)
	{
		if (this->in_holding)
		{
			if (this->ThisPositive(resource.controller))
			{
				this->total_count += resource.delta_count;
			}
			else
			{
				this->in_holding = false;
				this->total_count = 0;
				return false;
			}
		}
		else
		{
			if (!this->LastPositive(resource.controller) && this->ThisPositive(resource.controller))
			{
				this->total_count = 0;
				this->in_holding = true;
			}
			else
			{
				return false;
			}
		}
		if (this->total_count >= this->target_count)
		{
			this->total_count = 0;
			if (!this->loop_triggering)
			{
				this->in_holding = false;
			}
			return true;
		}
		return false;
	}

	// XInputButtonProcess
	ButtonProcess::ButtonProcess(Button::Type buttons) :
		BasicMiddleProcess(),
		buttons{ buttons }
	{

	}
	void ButtonProcess::UpdateButtons(Button::Type buttons) noexcept
	{
		this->buttons = buttons;
	}
	Button::Type ButtonProcess::Buttons() const noexcept
	{
		return this->buttons;
	}
	boolean ButtonProcess::GamepadPositive(const WrappedGamepad& gamepad) const
	{
		return gamepad.Pushed(this->buttons);
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
		trigger_dimension{ trigger_dimension },
		target_trigger_value{ target_value }
	{

	}
	void TriggerProcess::UpdateTargetTriggerValue(TriggerValue::Type target_value) noexcept
	{
		this->target_trigger_value = target_value;
	}
	TriggerValue::Type TriggerProcess::TargetTriggerValue() const noexcept
	{
		return this->target_trigger_value;
	}
	void TriggerProcess::UpdateTriggerDimension(XInput::TriggerDimension dimension) noexcept
	{
		this->trigger_dimension = dimension;
	}
	TriggerDimension TriggerProcess::TriggerDimension() const noexcept
	{
		return this->trigger_dimension;
	}
	boolean TriggerProcess::GamepadPositive(const WrappedGamepad& gamepad) const
	{
		switch (this->trigger_dimension)
		{
		case Petal::XInput::TriggerDimension::Left:
			if (gamepad.LeftTrigger() >= this->target_trigger_value) return true;
			return false;
			break;
		case Petal::XInput::TriggerDimension::Right:
			if (gamepad.RightTrigger() >= this->target_trigger_value) return true;
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
		stick_dimension{ stick_dimension },
		direction_dimension{ direction_dimension },
		target_stick_value{ target_value }
	{

	}
	void StickProcess::UpdateTargetStickValue(StickValue::Type target_value) noexcept
	{
		this->target_stick_value = target_value;
	}
	StickValue::Type StickProcess::TargetStickValue() const noexcept
	{
		return this->target_stick_value;
	}
	void StickProcess::UpdateStickDimension(XInput::StickDimension dimension) noexcept
	{
		this->stick_dimension = dimension;
	}
	StickDimension StickProcess::StickDimension() const noexcept
	{
		return this->stick_dimension;
	}
	void StickProcess::UpdateDirectionDimension(XInput::DirectionDimension dimension) noexcept
	{
		this->direction_dimension = dimension;
	}
	DirectionDimension StickProcess::DirectionDimension() const noexcept
	{
		return this->direction_dimension;
	}
	boolean StickProcess::GamepadPositive(const WrappedGamepad& gamepad) const
	{
		switch (this->stick_dimension)
		{
		case XInput::StickDimension::Left:
			switch (this->direction_dimension)
			{
			case Petal::XInput::DirectionDimension::Up:
				if (gamepad.LeftStickY() >= this->target_stick_value) return true;
				return false;
				break;
			case Petal::XInput::DirectionDimension::Down:
				if (gamepad.LeftStickY() < -this->target_stick_value) return true;
				return false;
				break;
			case Petal::XInput::DirectionDimension::Left:
				if (gamepad.LeftStickX() < -this->target_stick_value) return true;
				return false;
				break;
			case Petal::XInput::DirectionDimension::Right:
				if (gamepad.LeftStickX() >= this->target_stick_value) return true;
				return false;
				break;
			default:
				break;
			}
			break;
		case XInput::StickDimension::Right:
			switch (this->direction_dimension)
			{
			case Petal::XInput::DirectionDimension::Up:
				if (gamepad.RightStickY() >= this->target_stick_value) return true;
				return false;
				break;
			case Petal::XInput::DirectionDimension::Down:
				if (gamepad.RightStickY() < -this->target_stick_value) return true;
				return false;
				break;
			case Petal::XInput::DirectionDimension::Left:
				if (gamepad.RightStickX() < -this->target_stick_value) return true;
				return false;
				break;
			case Petal::XInput::DirectionDimension::Right:
				if (gamepad.RightStickX() >= this->target_stick_value) return true;
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
