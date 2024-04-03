#include "Petal~XInputEventProcess.h"

namespace Petal::XInput
{
	// ButtonProcess derived
	boolean ButtonPushProcess::Check(const Resource& resource)
	{
		return (!this->LastPositive(resource.controller) && this->ThisPositive(resource.controller));
	}
	boolean ButtonReleaseProcess::Check(const Resource& resource)
	{
		return (this->LastPositive(resource.controller) && !this->ThisPositive(resource.controller));
	}
	boolean ButtonPositiveProcess::Check(const Resource& resource)
	{
		return this->ThisPositive(resource.controller);
	}
	boolean ButtonNegativeProcess::Check(const Resource& resource)
	{
		return !this->ThisPositive(resource.controller);
	}
	boolean ButtonHoldProcess::Check(const Resource& resource)
	{
		return this->hold_config.Check(resource, *this);
	}

	// TriggerProcess Derived
	boolean TriggerPushProcess::Check(const Resource& resource)
	{
		return (!this->LastPositive(resource.controller) && this->ThisPositive(resource.controller));
	}
	boolean TriggerReleaseProcess::Check(const Resource& resource)
	{
		return (this->LastPositive(resource.controller) && !this->ThisPositive(resource.controller));
	}
	boolean TriggerPositiveProcess::Check(const Resource& resource)
	{
		return this->ThisPositive(resource.controller);
	}
	boolean TriggerNegativeProcess::Check(const Resource& resource)
	{
		return !this->ThisPositive(resource.controller);
	}
	boolean TriggerHoldProcess::Check(const Resource& resource)
	{
		return this->hold_config.Check(resource, *this);
	}

	// StickProcess Derived
	boolean StickPushProcess::Check(const Resource& resource)
	{
		return (!this->LastPositive(resource.controller) && this->ThisPositive(resource.controller));
	}
	boolean StickReleaseProcess::Check(const Resource& resource)
	{
		return (this->LastPositive(resource.controller) && !this->ThisPositive(resource.controller));
	}
	boolean StickPositiveProcess::Check(const Resource& resource)
	{
		return this->ThisPositive(resource.controller);
	}
	boolean StickNegativeProcess::Check(const Resource& resource)
	{
		return !this->ThisPositive(resource.controller);
	}
	boolean StickHoldProcess::Check(const Resource& resource)
	{
		return this->hold_config.Check(resource, *this);
	}
}

namespace Petal::XInput::MiddleProcess
{
	// ButtonProcess
	boolean ButtonProcess::LastPositive(const Controller& controller) const
	{
		return this->GamepadPositive(controller.GetLastWrappedGamepad());
	}
	boolean ButtonProcess::ThisPositive(const Controller& controller) const
	{
		return this->GamepadPositive(controller.GetWrappedGamepad());
	}

	// TriggerProcess
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

	// StickProcess
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
