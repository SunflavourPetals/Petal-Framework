#include "Petal~KeyboardEventProcess.h"

namespace Petal::Keyboard::MiddleProcess
{
	XHoldProcess::XHoldProcess(i64 target_count, boolean loop_mode) :
		BasicMiddleProcess(),
		pt_target_count(target_count),
		pt_loop_triggering(loop_mode)
	{

	}
	void XHoldProcess::UpdateTargetCount(i64 target_count) noexcept
	{
		this->pt_target_count = target_count;
	}
	i64 XHoldProcess::TargetCount() const noexcept
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
	boolean XHoldProcess::Check()
	{
		if (this->pt_in_holding)
		{
			if (this->ThisPositive())
			{
				this->pt_total_count += this->Resource().delta_count;
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
			if (!this->LastPositive() && this->ThisPositive())
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
		pt_key(target_key)
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
	boolean XKeyProcess::KeyPositive(const StoredState& state) const
	{
		return state.Pushed();
	}
	boolean XKeyProcess::LastPositive() const
	{
		return this->KeyPositive(this->Resource().controller.GetLastState(this->pt_key));
	}
	boolean XKeyProcess::ThisPositive() const
	{
		return this->KeyPositive(this->Resource().controller.GetState(this->pt_key));
	}
}

namespace Petal::Keyboard
{
	KeyPushProcess::KeyPushProcess(VirtualKey::Type target_key) :
		XKeyProcess(target_key)
	{

	}
	boolean KeyPushProcess::Check()
	{
		return (!this->LastPositive() && this->ThisPositive());
	}
	KeyReleaseProcess::KeyReleaseProcess(VirtualKey::Type target_key) :
		XKeyProcess(target_key)
	{

	}
	boolean KeyReleaseProcess::Check()
	{
		return (this->LastPositive() && !this->ThisPositive());
	}
	KeyPositiveProcess::KeyPositiveProcess(VirtualKey::Type target_key) :
		XKeyProcess(target_key)
	{

	}
	boolean KeyPositiveProcess::Check()
	{
		return this->ThisPositive();
	}
	KeyNegativeProcess::KeyNegativeProcess(VirtualKey::Type target_key) :
		XKeyProcess(target_key)
	{

	}
	boolean KeyNegativeProcess::Check()
	{
		return !this->ThisPositive();
	}
	KeyHoldProcess::KeyHoldProcess(VirtualKey::Type target_key, i64 target_count, boolean loop_mode) :
		XKeyProcess(target_key),
		XHoldProcess(target_count, loop_mode)
	{

	}
	boolean KeyHoldProcess::KeyPositive(const StoredState& state) const
	{
		return this->XKeyProcess::KeyPositive(state);
	}
	boolean KeyHoldProcess::LastPositive() const
	{
		return this->XKeyProcess::LastPositive();
	}
	boolean KeyHoldProcess::ThisPositive() const
	{
		return this->XKeyProcess::ThisPositive();
	}
	boolean KeyHoldProcess::Check()
	{
		return this->XHoldProcess::Check();
	}
}
