#pragma once

#ifndef Petal_Header_XInputEventProcess
#define Petal_Header_XInputEventProcess

#include "Petal~XInputController.h"
#include "Petal~XEventProcessComponents.h"

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
	class BasicProcess : public Abstract::XInputEventProcess
	{
	public:
		virtual boolean LastPositive(const Controller& controller) const = 0;
		virtual boolean ThisPositive(const Controller& controller) const = 0;
	public:
		BasicProcess() = default;
		~BasicProcess() = default;
	};

	class ButtonProcess : public BasicProcess
	{
	public:
		boolean LastPositive(const Controller& controller) const override
		{
			return GamepadPositive(controller.GetLastWrappedGamepad());
		}
		boolean ThisPositive(const Controller& controller) const override
		{
			return GamepadPositive(controller.GetWrappedGamepad());
		}
	private:
		boolean GamepadPositive(const WrappedGamepad& gamepad) const { return gamepad.Pushed(this->buttons); }
	public:
		ButtonProcess(Button::Type buttons = Button::A) :
			BasicProcess(),
			buttons{ buttons } {}
		~ButtonProcess() = default;
	public:
		Button::Type buttons{ Button::A };
	};

	class TriggerProcess : public BasicProcess
	{
	public:
		boolean LastPositive(const Controller& controller) const override
		{
			return GamepadPositive(controller.GetLastWrappedGamepad());
		}
		boolean ThisPositive(const Controller& controller) const override
		{
			return GamepadPositive(controller.GetWrappedGamepad());
		}
	private:
		boolean GamepadPositive(const WrappedGamepad& gamepad) const;
	public:
		TriggerProcess(
			XInput::TriggerDimension trigger_dimension = XInput::TriggerDimension::Left,
			TriggerValue::Type target_value = TriggerValue::threshold) :
			BasicProcess(),
			trigger_dimension{ trigger_dimension },
			target_trigger_value{ target_value } {}
		~TriggerProcess() = default;
	public:
		XInput::TriggerDimension trigger_dimension{ XInput::TriggerDimension::Left };
		TriggerValue::Type target_trigger_value{ TriggerValue::threshold };
	};

	class StickProcess : public BasicProcess
	{
	public:
		boolean LastPositive(const Controller& controller) const override
		{
			return GamepadPositive(controller.GetLastWrappedGamepad());
		}
		boolean ThisPositive(const Controller& controller) const override
		{
			return GamepadPositive(controller.GetWrappedGamepad());
		}
	private:
		boolean GamepadPositive(const WrappedGamepad& gamepad) const;
	public:
		StickProcess(
			XInput::StickDimension stick_dimension = XInput::StickDimension::Left,
			XInput::DirectionDimension direction_dimension = XInput::DirectionDimension::Left,
			StickValue::Type target_value = StickValue::threshold) :
			BasicProcess(),
			stick_dimension{ stick_dimension },
			direction_dimension{ direction_dimension },
			target_stick_value{ target_value } {}
		~StickProcess() = default;
	public:
		XInput::StickDimension stick_dimension{ XInput::StickDimension::Left };
		XInput::DirectionDimension direction_dimension{ XInput::DirectionDimension::Left };
		StickValue::Type target_stick_value{ StickValue::threshold };
	};
}

namespace Petal::XInput
{
	class ButtonPushProcess : public MiddleProcess::ButtonProcess
	{
	public:
		boolean Check(const Resource& resource) override
		{
			return (!LastPositive(resource.controller) && ThisPositive(resource.controller));
		}
	public:
		ButtonPushProcess(Button::Type buttons = Button::A) :
			ButtonProcess(buttons) {}
	};

	class ButtonReleaseProcess : public MiddleProcess::ButtonProcess
	{
	public:
		boolean Check(const Resource& resource) override
		{
			return (LastPositive(resource.controller) && !ThisPositive(resource.controller));
		}
	public:
		ButtonReleaseProcess(Button::Type buttons = Button::A) :
			ButtonProcess(buttons) {}
	};

	class ButtonPositiveProcess : public MiddleProcess::ButtonProcess
	{
	public:
		boolean Check(const Resource& resource) override { return ThisPositive(resource.controller); }
	public:
		ButtonPositiveProcess(Button::Type buttons = Button::A) :
			ButtonProcess(buttons) {}
	};

	class ButtonNegativeProcess : public MiddleProcess::ButtonProcess
	{
	public:
		boolean Check(const Resource& resource) override { return !ThisPositive(resource.controller); }
	public:
		ButtonNegativeProcess(Button::Type buttons = Button::A) :
			ButtonProcess(buttons) {}
	};

	class ButtonHoldProcess : public MiddleProcess::ButtonProcess
	{
	public:
		boolean Check(const Resource& resource) override { return hold_config.Check(resource, *this); }
	public:
		ButtonHoldProcess(
			Button::Type buttons = Button::A,
			XEventProcessComponents::Hold::Tick target_count = 1,
			boolean loop_mode = false) :
			ButtonProcess(buttons),
			hold_config(target_count, loop_mode) {}
	public:
		XEventProcessComponents::Hold hold_config{};
	};
}

namespace Petal::XInput
{
	class TriggerPushProcess : public MiddleProcess::TriggerProcess
	{
	public:
		boolean Check(const Resource& resource) override
		{
			return (!LastPositive(resource.controller) && ThisPositive(resource.controller));
		}
	public:
		TriggerPushProcess(
			XInput::TriggerDimension dimension = XInput::TriggerDimension::Left,
			TriggerValue::Type target_value = TriggerValue::threshold) :
			TriggerProcess(dimension, target_value) {}
	};

	class TriggerReleaseProcess : public MiddleProcess::TriggerProcess
	{
	public:
		boolean Check(const Resource& resource) override
		{
			return (LastPositive(resource.controller) && !ThisPositive(resource.controller));
		}
	public:
		TriggerReleaseProcess(
			XInput::TriggerDimension dimension = XInput::TriggerDimension::Left,
			TriggerValue::Type target_value = TriggerValue::threshold) :
			TriggerProcess(dimension, target_value) {}
	};

	class TriggerPositiveProcess : public MiddleProcess::TriggerProcess
	{
	public:
		boolean Check(const Resource& resource) override { return ThisPositive(resource.controller); }
	public:
		TriggerPositiveProcess(
			XInput::TriggerDimension dimension = XInput::TriggerDimension::Left,
			TriggerValue::Type target_value = TriggerValue::threshold) :
			TriggerProcess(dimension, target_value) {}
	};

	class TriggerNegativeProcess : public MiddleProcess::TriggerProcess
	{
	public:
		boolean Check(const Resource& resource) override { return !ThisPositive(resource.controller); }
	public:
		TriggerNegativeProcess(
			XInput::TriggerDimension dimension = XInput::TriggerDimension::Left,
			TriggerValue::Type target_value = TriggerValue::threshold) :
			TriggerProcess(dimension, target_value) {}
	};

	class TriggerHoldProcess : public MiddleProcess::TriggerProcess
	{
	public:
		boolean Check(const Resource& resource) override { return hold_config.Check(resource, *this); }
	public:
		TriggerHoldProcess(
			XInput::TriggerDimension dimension = XInput::TriggerDimension::Left,
			TriggerValue::Type target_value = TriggerValue::threshold,
			XEventProcessComponents::Hold::Tick target_count = 1,
			boolean loop_mode = false) :
			TriggerProcess(dimension, target_value),
			hold_config(target_count, loop_mode) {}
	public:
		XEventProcessComponents::Hold hold_config{};
	};
}

namespace Petal::XInput
{
	class StickPushProcess : public MiddleProcess::StickProcess
	{
	public:
		boolean Check(const Resource& resource) override
		{
			return (!LastPositive(resource.controller) && ThisPositive(resource.controller));
		}
	public:
		StickPushProcess(
			XInput::StickDimension dimension = XInput::StickDimension::Left,
			XInput::DirectionDimension direction = XInput::DirectionDimension::Up,
			StickValue::Type target_value = StickValue::threshold) :
			StickProcess(dimension, direction, target_value) {}
	};

	class StickReleaseProcess : public MiddleProcess::StickProcess
	{
	public:
		boolean Check(const Resource& resource) override
		{
			return (LastPositive(resource.controller) && !ThisPositive(resource.controller));
		}
	public:
		StickReleaseProcess(
			XInput::StickDimension dimension = XInput::StickDimension::Left,
			XInput::DirectionDimension direction = XInput::DirectionDimension::Up,
			StickValue::Type target_value = StickValue::threshold) :
			StickProcess(dimension, direction, target_value) {}
	};

	class StickPositiveProcess : public MiddleProcess::StickProcess
	{
	public:
		boolean Check(const Resource& resource) override { return ThisPositive(resource.controller); }
	public:
		StickPositiveProcess(
			XInput::StickDimension dimension = XInput::StickDimension::Left,
			XInput::DirectionDimension direction = XInput::DirectionDimension::Up,
			StickValue::Type target_value = StickValue::threshold) :
			StickProcess(dimension, direction, target_value) {}
	};

	class StickNegativeProcess : public MiddleProcess::StickProcess
	{
	public:
		boolean Check(const Resource& resource) override { return !ThisPositive(resource.controller); }
	public:
		StickNegativeProcess(
			XInput::StickDimension dimension = XInput::StickDimension::Left,
			XInput::DirectionDimension direction = XInput::DirectionDimension::Up,
			StickValue::Type target_value = StickValue::threshold) :
			StickProcess(dimension, direction, target_value) {}
	};

	class StickHoldProcess : public MiddleProcess::StickProcess
	{
	public:
		boolean Check(const Resource& resource) override { return hold_config.Check(resource, *this); }
	public:
		StickHoldProcess(
			XInput::StickDimension dimension = XInput::StickDimension::Left,
			XInput::DirectionDimension direction = XInput::DirectionDimension::Up,
			StickValue::Type target_value = StickValue::threshold,
			XEventProcessComponents::Hold::Tick target_count = 1,
			boolean loop_mode = false) :
			StickProcess(dimension, direction, target_value),
			hold_config(target_count, loop_mode) {}
	public:
		XEventProcessComponents::Hold hold_config{};
	};
}

// Implementation

namespace Petal::XInput::MiddleProcess
{
	inline boolean TriggerProcess::GamepadPositive(const WrappedGamepad& gamepad) const
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
	
	inline boolean StickProcess::GamepadPositive(const WrappedGamepad& gamepad) const
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
}


#endif // !Petal_Header_XInputEventProcess
