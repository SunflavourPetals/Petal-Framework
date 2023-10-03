#pragma once

#ifndef Petal_Header_PerformanceCounter
#define Petal_Header_PerformanceCounter

#include "Petal~BasicTypes.h"

namespace Petal
{
	class PerformanceCounter final
	{
	public:
		void Reset() noexcept; // Reset this counter
		void Count() noexcept; // Count
		void Pause() noexcept; // Make counter pause(it only works when counter is working)
		void Start() noexcept; // Start(it only works when counter paused)
		i64 Frequency() const noexcept; // Frequency of counter(counts number per second)
		i64 TotalCounts() const noexcept; // Total counts except pause state
		i64 DeltaCounts() const noexcept; // Counts between this count and last count
		i64 DeltaCountsPaused() const noexcept; // Counts between this count and last count(paused state)
		f64 TotalTime() const noexcept; // Total time
		f64 DeltaTime() const noexcept; // Time between this count and last count
		f64 DeltaTimePaused() const noexcept; // Time between this count and last count(paused state)
		boolean PauseState() const noexcept; // Counter state of pause
	public:
		PerformanceCounter() noexcept;
		PerformanceCounter(const PerformanceCounter& rhs) noexcept = default;
		PerformanceCounter(PerformanceCounter&& rhs) noexcept = default;
		~PerformanceCounter() noexcept = default;
	private:
		i64 pt_frequency; // Frequency of counter(counts number per second)
		i64 pt_base; // When the performance counter start
		i64 pt_current; // Now value of performance counter
		i64 pt_delta; // Duration of this count and last count(pt_current - pt_prev)
		i64 pt_delta_paused; // Duration of this count and last count in pause state(pt_current - pt_prev)
		i64 pt_base_paused; // The value of performance counter queried when suspended
		i64 pt_total_paused; // The total duration of pause
		boolean pt_state_paused; // Pause state of this performance
	};
}

#endif // !Petal_Header_PerformanceCounter