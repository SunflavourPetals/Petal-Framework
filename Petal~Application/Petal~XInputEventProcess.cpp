#include "Petal~XInputEventProcess.h"

namespace Petal::XInput
{
	// = = XInputButtonProcess Derived = =
	// XInputButtonPushProcess
	ButtonPushProcess::ButtonPushProcess(Button::Type buttons) :
		ButtonProcess(buttons)
	{

	}
	boolean ButtonPushProcess::Check()
	{
		return (!this->LastPositive() && this->ThisPositive());
	}
	// XInputButtonReleaseProcess
	ButtonReleaseProcess::ButtonReleaseProcess(Button::Type buttons) :
		ButtonProcess(buttons)
	{

	}
	boolean ButtonReleaseProcess::Check()
	{
		return (this->LastPositive() && !this->ThisPositive());
	}
	// XInputButtonPositiveProcess
	ButtonPositiveProcess::ButtonPositiveProcess(Button::Type buttons) :
		ButtonProcess(buttons)
	{

	}
	boolean ButtonPositiveProcess::Check()
	{
		return this->ThisPositive();
	}
	// XInputButtonNegativeProcess
	ButtonNegativeProcess::ButtonNegativeProcess(Button::Type buttons) :
		ButtonProcess(buttons)
	{

	}
	boolean ButtonNegativeProcess::Check()
	{
		return !this->ThisPositive();
	}
	// XInputButtonHoldProcess
	ButtonHoldProcess::ButtonHoldProcess(Button::Type buttons, f64 target_time, boolean loop_mode) :
		ButtonProcess(buttons),
		XHoldProcess(target_time, loop_mode)
	{

	}
	boolean ButtonHoldProcess::GamepadPositive(const Gamepad& gamepad) const
	{
		return this->ButtonProcess::GamepadPositive(gamepad);
	}
	boolean ButtonHoldProcess::LastPositive() const
	{
		return this->ButtonProcess::LastPositive();
	}
	boolean ButtonHoldProcess::ThisPositive() const
	{
		return this->ButtonProcess::ThisPositive();
	}
	boolean ButtonHoldProcess::Check()
	{
		return this->XHoldProcess::Check();
	}

	// = = XInputTriggerProcess Derived = =
	// XInputTriggerPushProcess
	TriggerPushProcess::TriggerPushProcess(XInput::TriggerDimension dimension, TriggerValue::Type target_value) :
		TriggerProcess(dimension, target_value)
	{

	}
	boolean TriggerPushProcess::Check()
	{
		return (!this->LastPositive() && this->ThisPositive());
	}
	// XInputTriggerReleaseProcess
	TriggerReleaseProcess::TriggerReleaseProcess(XInput::TriggerDimension dimension, TriggerValue::Type target_value) :
		TriggerProcess(dimension, target_value)
	{

	}
	boolean TriggerReleaseProcess::Check()
	{
		return (this->LastPositive() && !this->ThisPositive());
	}
	// XInputTriggerPositiveProcess
	TriggerPositiveProcess::TriggerPositiveProcess(XInput::TriggerDimension dimension, TriggerValue::Type target_value) :
		TriggerProcess(dimension, target_value)
	{

	}
	boolean TriggerPositiveProcess::Check()
	{
		return this->ThisPositive();
	}
	// XInputTriggerNegativeProcess
	TriggerNegativeProcess::TriggerNegativeProcess(XInput::TriggerDimension dimension, TriggerValue::Type target_value) :
		TriggerProcess(dimension, target_value)
	{

	}
	boolean TriggerNegativeProcess::Check()
	{
		return !this->ThisPositive();
	}
	// XInputTriggerHoldProcess
	TriggerHoldProcess::TriggerHoldProcess(XInput::TriggerDimension dimension, TriggerValue::Type target_value, f64 target_time, boolean loop_mode) :
		TriggerProcess(dimension, target_value),
		XHoldProcess(target_time, loop_mode)
	{

	}
	boolean TriggerHoldProcess::GamepadPositive(const Gamepad& gamepad) const
	{
		return this->TriggerProcess::GamepadPositive(gamepad);
	}
	boolean TriggerHoldProcess::LastPositive() const
	{
		return this->TriggerProcess::LastPositive();
	}
	boolean TriggerHoldProcess::ThisPositive() const
	{
		return this->TriggerProcess::ThisPositive();
	}
	boolean TriggerHoldProcess::Check()
	{
		return this->XHoldProcess::Check();
	}

	// = = XInputStickProcess Derived = =
	// XInputStickPushProcess
	StickPushProcess::StickPushProcess(XInput::StickDimension dimension, XInput::DirectionDimension direction, StickValue::Type target_value) :
		StickProcess(dimension, direction, target_value)
	{

	}
	boolean StickPushProcess::Check()
	{
		return (!this->LastPositive() && this->ThisPositive());
	}
	// XInputStickReleaseProcess
	StickReleaseProcess::StickReleaseProcess(XInput::StickDimension dimension, XInput::DirectionDimension direction, StickValue::Type target_value) :
		StickProcess(dimension, direction, target_value)
	{

	}
	boolean StickReleaseProcess::Check()
	{
		return (this->LastPositive() && !this->ThisPositive());
	}
	// XInputStickPositiveProcess
	StickPositiveProcess::StickPositiveProcess(XInput::StickDimension dimension, XInput::DirectionDimension direction, StickValue::Type target_value) :
		StickProcess(dimension, direction, target_value)
	{

	}
	boolean StickPositiveProcess::Check()
	{
		return this->ThisPositive();
	}
	// XInputStickNegativeProcess
	StickNegativeProcess::StickNegativeProcess(XInput::StickDimension dimension, XInput::DirectionDimension direction, StickValue::Type target_value) :
		StickProcess(dimension, direction, target_value)
	{

	}
	boolean StickNegativeProcess::Check()
	{
		return !this->ThisPositive();
	}
	// XInputStickHoldProcess
	StickHoldProcess::StickHoldProcess(XInput::StickDimension dimension, XInput::DirectionDimension direction, StickValue::Type target_value, f64 target_time, boolean loop_mode) :
		StickProcess(dimension, direction, target_value),
		XHoldProcess(target_time, loop_mode)
	{

	}
	boolean StickHoldProcess::GamepadPositive(const Gamepad& gamepad) const
	{
		return this->StickProcess::GamepadPositive(gamepad);
	}
	boolean StickHoldProcess::LastPositive() const
	{
		return this->StickProcess::LastPositive();
	}
	boolean StickHoldProcess::ThisPositive() const
	{
		return this->StickProcess::ThisPositive();
	}
	boolean StickHoldProcess::Check()
	{
		return this->XHoldProcess::Check();
	}
}

namespace Petal::XInput::MiddleProcess
{
	// XInputXHold
	XHoldProcess::XHoldProcess(f64 target_time, boolean loop_mode) :
		BasicMiddleProcess(),
		pt_target_time(target_time),
		pt_loop_triggering(loop_mode)
	{

	}
	void XHoldProcess::UpdateTargetTime(f64 target_time) noexcept
	{
		this->pt_target_time = target_time;
	}
	f64 XHoldProcess::TargetTime() const noexcept
	{
		return this->pt_target_time;
	}
	void XHoldProcess::UpdateLoopMode(boolean loop_mode) noexcept
	{
		this->pt_loop_triggering = loop_mode;
	}
	boolean XHoldProcess::LoopMode() const noexcept
	{
		return this->pt_loop_triggering;
	}
	boolean XHoldProcess::Check()
	{
		if (this->pt_in_holding)
		{
			if (this->ThisPositive())
			{
				this->pt_total_time += this->DeltaTime();
			}
			else
			{
				this->pt_in_holding = false;
				this->pt_total_time = 0.0;
				return false;
			}
		}
		else
		{
			if (!this->LastPositive() && this->ThisPositive())
			{
				this->pt_total_time = 0.0;
				this->pt_in_holding = true;
			}
			else
			{
				return false;
			}
		}
		if (this->pt_total_time >= this->pt_target_time)
		{
			this->pt_total_time = 0.0;
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
	boolean ButtonProcess::GamepadPositive(const Gamepad& gamepad) const
	{
		return ((gamepad.wButtons & this->pt_buttons) == this->pt_buttons);
	}
	boolean ButtonProcess::LastPositive() const
	{
		return this->GamepadPositive(this->Resource().controller.GetLastWrappedGamepad().GetGamepad());
	}
	boolean ButtonProcess::ThisPositive() const
	{
		return this->GamepadPositive(this->Resource().controller.GetWrappedGamepad().GetGamepad());
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
	boolean TriggerProcess::GamepadPositive(const Gamepad& gamepad) const
	{
		switch (this->pt_trigger_dimension)
		{
		case Petal::XInput::TriggerDimension::Left:
			if (gamepad.bLeftTrigger >= this->pt_target_trigger_value) return true;
			return false;
			break;
		case Petal::XInput::TriggerDimension::Right:
			if (gamepad.bRightTrigger >= this->pt_target_trigger_value) return true;
			return false;
			break;
		default:
			break;
		}
		return false;
	}
	boolean TriggerProcess::LastPositive() const
	{
		return this->GamepadPositive(this->Resource().controller.GetLastWrappedGamepad().GetGamepad());
	}
	boolean TriggerProcess::ThisPositive() const
	{
		return this->GamepadPositive(this->Resource().controller.GetWrappedGamepad().GetGamepad());
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
	boolean StickProcess::GamepadPositive(const Gamepad& gamepad) const
	{
		switch (this->pt_stick_dimension)
		{
		case XInput::StickDimension::Left:
			switch (this->pt_direction_dimension)
			{
			case Petal::XInput::DirectionDimension::Up:
				if (gamepad.sThumbLY >= this->pt_target_stick_value) return true;
				return false;
				break;
			case Petal::XInput::DirectionDimension::Down:
				if (gamepad.sThumbLY < -this->pt_target_stick_value) return true;
				return false;
				break;
			case Petal::XInput::DirectionDimension::Left:
				if (gamepad.sThumbLX < -this->pt_target_stick_value) return true;
				return false;
				break;
			case Petal::XInput::DirectionDimension::Right:
				if (gamepad.sThumbLX >= this->pt_target_stick_value) return true;
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
				if (gamepad.sThumbRY >= this->pt_target_stick_value) return true;
				return false;
				break;
			case Petal::XInput::DirectionDimension::Down:
				if (gamepad.sThumbRY < -this->pt_target_stick_value) return true;
				return false;
				break;
			case Petal::XInput::DirectionDimension::Left:
				if (gamepad.sThumbRX < -this->pt_target_stick_value) return true;
				return false;
				break;
			case Petal::XInput::DirectionDimension::Right:
				if (gamepad.sThumbRX >= this->pt_target_stick_value) return true;
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
	boolean StickProcess::LastPositive() const
	{
		return this->GamepadPositive(this->Resource().controller.GetLastWrappedGamepad().GetGamepad());
	}
	boolean StickProcess::ThisPositive() const
	{
		return this->GamepadPositive(this->Resource().controller.GetWrappedGamepad().GetGamepad());
	}
}
