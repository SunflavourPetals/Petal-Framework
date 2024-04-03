#include "Petal~KeyboardEventProcess.h"

namespace Petal::Keyboard::MiddleProcess
{
	boolean XKeyProcess::LastPositive(const Controller& controller) const
	{
		return this->KeyPositive(controller.GetLastState());
	}
	boolean XKeyProcess::ThisPositive(const Controller& controller) const
	{
		return this->KeyPositive(controller.GetState());
	}
}

namespace Petal::Keyboard
{
	boolean KeyPushProcess::Check(const Resource& resource)
	{
		return (!this->LastPositive(resource.controller) && this->ThisPositive(resource.controller));
	}
	boolean KeyReleaseProcess::Check(const Resource& resource)
	{
		return (this->LastPositive(resource.controller) && !this->ThisPositive(resource.controller));
	}
	boolean KeyPositiveProcess::Check(const Resource& resource)
	{
		return this->ThisPositive(resource.controller);
	}
	boolean KeyNegativeProcess::Check(const Resource& resource)
	{
		return !this->ThisPositive(resource.controller);
	}
	boolean KeyHoldProcess::Check(const Resource& resource)
	{
		return this->hold_config.Check(resource, *this);
	}
}
