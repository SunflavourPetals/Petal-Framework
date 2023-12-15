#include "Petal~KeyboardEventProcess.h"

namespace Petal::Keyboard::MiddleProcess
{
	XHoldProcess::XHoldProcess(Tick target_count, boolean loop_mode) :
		BasicMiddleProcess(),
		pt_target_count{ target_count },
		pt_loop_triggering{ loop_mode }
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

	XKeyProcess::XKeyProcess(VirtualKey::Type target_key) :
		BasicMiddleProcess(),
		pt_key{ target_key }
	{

	}
	void XKeyProcess::UpdateKey(VirtualKey::Type target_key) noexcept
	{
		this->pt_key = target_key;
	}
	VirtualKey::Type XKeyProcess::Key() const noexcept
	{
		return this->pt_key;
	}
	boolean XKeyProcess::KeyPositive(const WrappedState& state) const
	{
		return state.Pushed(this->pt_key);
	}
	boolean XKeyProcess::LastPositive(const BasicController& controller) const
	{
		return this->KeyPositive(controller.GetLastState());
	}
	boolean XKeyProcess::ThisPositive(const BasicController& controller) const
	{
		return this->KeyPositive(controller.GetState());
	}
}

namespace Petal::Keyboard
{
	KeyPushProcess::KeyPushProcess(VirtualKey::Type target_key) :
		XKeyProcess(target_key)
	{

	}
	boolean KeyPushProcess::Check(const Resource& resource)
	{
		return (!this->LastPositive(resource.controller) && this->ThisPositive(resource.controller));
	}
	KeyReleaseProcess::KeyReleaseProcess(VirtualKey::Type target_key) :
		XKeyProcess(target_key)
	{

	}
	boolean KeyReleaseProcess::Check(const Resource& resource)
	{
		return (this->LastPositive(resource.controller) && !this->ThisPositive(resource.controller));
	}
	KeyPositiveProcess::KeyPositiveProcess(VirtualKey::Type target_key) :
		XKeyProcess(target_key)
	{

	}
	boolean KeyPositiveProcess::Check(const Resource& resource)
	{
		return this->ThisPositive(resource.controller);
	}
	KeyNegativeProcess::KeyNegativeProcess(VirtualKey::Type target_key) :
		XKeyProcess(target_key)
	{

	}
	boolean KeyNegativeProcess::Check(const Resource& resource)
	{
		return !this->ThisPositive(resource.controller);
	}
	KeyHoldProcess::KeyHoldProcess(VirtualKey::Type target_key, i64 target_count, boolean loop_mode) :
		XKeyProcess(target_key),
		XHoldProcess(target_count, loop_mode)
	{

	}
	boolean KeyHoldProcess::KeyPositive(const WrappedState& state) const
	{
		return this->XKeyProcess::KeyPositive(state);
	}
	boolean KeyHoldProcess::LastPositive(const BasicController& controller) const
	{
		return this->XKeyProcess::LastPositive(controller);
	}
	boolean KeyHoldProcess::ThisPositive(const BasicController& controller) const
	{
		return this->XKeyProcess::ThisPositive(controller);
	}
	boolean KeyHoldProcess::Check(const Resource& resource)
	{
		return this->XHoldProcess::Check(resource);
	}
}
