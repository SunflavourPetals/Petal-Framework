#pragma once

#ifndef Petal_Header_PerformanceCounter
#define Petal_Header_PerformanceCounter

#include <utility>

#include "Petal~BasicTypes.h"
#include "Petal~WinTypes.h"

namespace Petal
{
	class PerformanceCounter final
	{
	public:
		using Tick = decltype(::std::declval<Win32LargeInteger>().QuadPart);
		using Second = f64;
	public:
		static Tick QueryFrequency() noexcept;
		static Tick QueryCounter() noexcept;
	public:
		void Reset() noexcept; // Reset this counter
		void Count() noexcept; // Count
		void Pause() noexcept; // Make counter pause(it only works when counter is working)
		void Start() noexcept; // Start(it only works when counter paused)
		Tick Frequency() const noexcept; // Frequency of counter(counts number per second)
		Tick TotalCounts() const noexcept; // Total counts except pause state
		Tick DeltaCounts() const noexcept; // Counts between this count and last count
		Tick DeltaCountsPaused() const noexcept; // Counts between this count and last count(paused state)
		Second TotalTime() const noexcept; // Total time
		Second DeltaTime() const noexcept; // Time between this count and last count
		Second DeltaTimePaused() const noexcept; // Time between this count and last count(paused state)
		boolean PauseState() const noexcept; // Counter state of pause
	public:
		PerformanceCounter() noexcept;
		PerformanceCounter(const PerformanceCounter& rhs) noexcept = default;
		PerformanceCounter(PerformanceCounter&& rhs) noexcept = default;
		~PerformanceCounter() noexcept = default;
	private:
		Tick frequency{}; // Frequency of counter(counts number per second)
		Tick base{}; // When the performance counter start
		Tick current{}; // Now value of performance counter
		Tick delta{}; // Duration of this count and last count(current - prev)
		Tick delta_paused{}; // Duration of this count and last count in pause state(current - prev)
		Tick base_paused{}; // The value of performance counter queried when suspended
		Tick total_paused{}; // The total duration of pause
		boolean state_paused{ false }; // Pause state of this performance
	};
}

// Implementation

namespace Petal
{
	inline PerformanceCounter::PerformanceCounter() noexcept
	{
		this->Reset();
	}
	inline void PerformanceCounter::Reset() noexcept
	{
		this->frequency = QueryFrequency();
		this->current = QueryCounter();
		this->base = this->current;
		this->delta = 0;
		this->delta_paused = 0;
		this->base_paused = 0;
		this->total_paused = 0;
		this->state_paused = false;
	}
	inline void PerformanceCounter::Count() noexcept
	{
		Tick prev = this->current;
		if (this->state_paused == true)
		{
			this->current = QueryCounter();
			this->delta_paused = this->current - prev;
			if (this->delta_paused < 0)
			{
				this->delta = 0;
			}
			return;
		}
		else
		{
			this->current = QueryCounter();
			this->delta = this->current - prev;
			if (this->delta < 0)
			{
				this->delta = 0;
			}
			return;
		}
	}
	inline void PerformanceCounter::Pause() noexcept
	{
		if (this->state_paused == false)
		{
			this->current = QueryCounter();
			this->base_paused = this->current;
			this->delta = 0;
			this->delta_paused = 0;
			this->state_paused = true;
		}
	}
	inline void PerformanceCounter::Start() noexcept
	{
		if (this->state_paused == true)
		{
			this->current = QueryCounter();
			this->total_paused += (this->current - this->base_paused);
			this->delta = 0;
			this->delta_paused = 0;
			this->state_paused = false;
		}
	}
	inline PerformanceCounter::Tick PerformanceCounter::Frequency() const noexcept
	{
		return this->frequency;
	}
	inline PerformanceCounter::Tick PerformanceCounter::TotalCounts() const noexcept
	{
		if (this->state_paused == true)
		{
			return (this->base_paused - this->total_paused - this->base);
		}
		return (this->current - this->total_paused - this->base);
	}
	inline PerformanceCounter::Tick PerformanceCounter::DeltaCounts() const noexcept
	{
		return this->delta;
	}
	inline PerformanceCounter::Tick PerformanceCounter::DeltaCountsPaused() const noexcept
	{
		return this->delta_paused;
	}
	inline PerformanceCounter::Second PerformanceCounter::TotalTime() const noexcept
	{
		return this->TotalCounts() / static_cast<f64>(this->frequency);
	}
	inline PerformanceCounter::Second PerformanceCounter::DeltaTime() const noexcept
	{
		return this->DeltaCounts() / static_cast<f64>(this->frequency);
	}
	inline PerformanceCounter::Second PerformanceCounter::DeltaTimePaused() const noexcept
	{
		return this->DeltaCountsPaused() / static_cast<f64>(this->frequency);
	}
	inline boolean PerformanceCounter::PauseState() const noexcept
	{
		return this->state_paused;
	}
	inline auto PerformanceCounter::QueryFrequency() noexcept -> Tick
	{
		Win32LargeInteger counts{};
		::QueryPerformanceFrequency(&counts);
		return counts.QuadPart;
	}
	inline auto PerformanceCounter::QueryCounter() noexcept -> Tick
	{
		Win32LargeInteger counts{};
		::QueryPerformanceCounter(&counts);
		return counts.QuadPart;
	}
}

#endif // !Petal_Header_PerformanceCounter
