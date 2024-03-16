#include "Petal~PerformanceCounter.h"
#include "Petal~WinTypes.h"

namespace
{
	constexpr Petal::PerformanceCounter::Tick zero{ 0LL };
}

namespace Petal
{
	PerformanceCounter::PerformanceCounter() noexcept
	{
		this->Reset();
	}
	void PerformanceCounter::Reset() noexcept
	{
		::QueryPerformanceFrequency(reinterpret_cast<ptr<::LARGE_INTEGER>>(&this->frequency));
		::QueryPerformanceCounter(reinterpret_cast<ptr<::LARGE_INTEGER>>(&this->current));
		this->base = this->current;
		this->delta = zero;
		this->delta_paused = zero;
		this->base_paused = zero;
		this->total_paused = zero;
		this->state_paused = false;
	}
	void PerformanceCounter::Count() noexcept
	{
		Tick prev = this->current;
		if (this->state_paused == true)
		{
			::QueryPerformanceCounter(reinterpret_cast<ptr<::LARGE_INTEGER>>(&this->current));
			this->delta_paused = this->current - prev;
			if (this->delta_paused < zero)
			{
				this->delta = zero;
			}
			return;
		}
		else
		{
			::QueryPerformanceCounter(reinterpret_cast<ptr<::LARGE_INTEGER>>(&this->current));
			this->delta = this->current - prev;
			if (this->delta < zero)
			{
				this->delta = zero;
			}
			return;
		}
	}
	void PerformanceCounter::Pause() noexcept
	{
		if (this->state_paused == false)
		{
			::QueryPerformanceCounter(reinterpret_cast<ptr<::LARGE_INTEGER>>(&this->current));
			this->base_paused = this->current;
			this->delta = zero;
			this->delta_paused = zero;
			this->state_paused = true;
		}
	}
	void PerformanceCounter::Start() noexcept
	{
		if (this->state_paused == true)
		{
			::QueryPerformanceCounter(reinterpret_cast<ptr<::LARGE_INTEGER>>(&this->current));
			this->total_paused += (this->current - this->base_paused);
			this->delta = zero;
			this->delta_paused = zero;
			this->state_paused = false;
		}
	}
	PerformanceCounter::Tick PerformanceCounter::Frequency() const noexcept
	{
		return this->frequency;
	}
	PerformanceCounter::Tick PerformanceCounter::TotalCounts() const noexcept
	{
		if (this->state_paused == true)
		{
			return (this->base_paused - this->total_paused - this->base);
		}
		return (this->current - this->total_paused - this->base);
	}
	PerformanceCounter::Tick PerformanceCounter::DeltaCounts() const noexcept
	{
		return this->delta;
	}
	PerformanceCounter::Tick PerformanceCounter::DeltaCountsPaused() const noexcept
	{
		return this->delta_paused;
	}
	PerformanceCounter::Second PerformanceCounter::TotalTime() const noexcept
	{
		return this->TotalCounts() / static_cast<f64>(this->frequency);
	}
	PerformanceCounter::Second PerformanceCounter::DeltaTime() const noexcept
	{
		return this->DeltaCounts() / static_cast<f64>(this->frequency);
	}
	PerformanceCounter::Second PerformanceCounter::DeltaTimePaused() const noexcept
	{
		return this->DeltaCountsPaused() / static_cast<f64>(this->frequency);
	}
	boolean PerformanceCounter::PauseState() const noexcept
	{
		return this->state_paused;
	}
}
