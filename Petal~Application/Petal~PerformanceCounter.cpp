#include "Petal~PerformanceCounter.h"
#include "Petal~WinTypes.h"

#include <Windows.h>

namespace Petal
{
	static constexpr i64 zero{ 0LL };

	PerformanceCounter::PerformanceCounter() noexcept
	{
		this->Reset();
	}
	void PerformanceCounter::Reset() noexcept
	{
		::QueryPerformanceFrequency(reinterpret_cast<ptr<::LARGE_INTEGER>>(&this->pt_frequency));
		::QueryPerformanceCounter(reinterpret_cast<ptr<::LARGE_INTEGER>>(&this->pt_current));
		this->pt_base = this->pt_current;
		this->pt_delta = zero;
		this->pt_delta_paused = zero;
		this->pt_base_paused = zero;
		this->pt_total_paused = zero;
		this->pt_state_paused = false;
	}
	void PerformanceCounter::Count() noexcept
	{
		i64 prev = this->pt_current;
		if (this->pt_state_paused == true)
		{
			::QueryPerformanceCounter(reinterpret_cast<ptr<::LARGE_INTEGER>>(&this->pt_current));
			this->pt_delta_paused = this->pt_current - prev;
			if (this->pt_delta_paused < zero)
			{
				this->pt_delta = zero;
			}
			return;
		}
		else
		{
			::QueryPerformanceCounter(reinterpret_cast<ptr<::LARGE_INTEGER>>(&this->pt_current));
			this->pt_delta = this->pt_current - prev;
			if (this->pt_delta < zero)
			{
				this->pt_delta = zero;
			}
			return;
		}
	}
	void PerformanceCounter::Pause() noexcept
	{
		if (this->pt_state_paused == false)
		{
			::QueryPerformanceCounter(reinterpret_cast<ptr<::LARGE_INTEGER>>(&this->pt_current));
			this->pt_base_paused = this->pt_current;
			this->pt_delta = zero;
			this->pt_delta_paused = zero;
			this->pt_state_paused = true;
		}
	}
	void PerformanceCounter::Start() noexcept
	{
		if (this->pt_state_paused == true)
		{
			::QueryPerformanceCounter(reinterpret_cast<ptr<::LARGE_INTEGER>>(&this->pt_current));
			this->pt_total_paused += (this->pt_current - this->pt_base_paused);
			this->pt_delta = zero;
			this->pt_delta_paused = zero;
			this->pt_state_paused = false;
		}
	}
	i64 PerformanceCounter::Frequency() const noexcept
	{
		return this->pt_frequency;
	}
	i64 PerformanceCounter::TotalCounts() const noexcept
	{
		if (this->pt_state_paused == true)
		{
			return (this->pt_base_paused - this->pt_total_paused - this->pt_base);
		}
		return (this->pt_current - this->pt_total_paused - this->pt_base);
	}
	i64 PerformanceCounter::DeltaCounts() const noexcept
	{
		return this->pt_delta;
	}
	i64 PerformanceCounter::DeltaCountsPaused() const noexcept
	{
		return this->pt_delta_paused;
	}
	f64 PerformanceCounter::TotalTime() const noexcept
	{
		return this->TotalCounts() / static_cast<f64>(this->pt_frequency);
	}
	f64 PerformanceCounter::DeltaTime() const noexcept
	{
		return this->DeltaCounts() / static_cast<f64>(this->pt_frequency);
	}
	f64 PerformanceCounter::DeltaTimePaused() const noexcept
	{
		return this->DeltaCountsPaused() / static_cast<f64>(this->pt_frequency);
	}
	boolean PerformanceCounter::PauseState() const noexcept
	{
		return this->pt_state_paused;
	}
}
