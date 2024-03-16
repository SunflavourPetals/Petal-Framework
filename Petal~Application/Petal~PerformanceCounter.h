#pragma once

#ifndef Petal_Header_PerformanceCounter
#define Petal_Header_PerformanceCounter

#include "Petal~BasicTypes.h"

namespace Petal
{
	class PerformanceCounter final
	{
	public:
		using Tick = i64;
		using Second = f64;
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

#endif // !Petal_Header_PerformanceCounter
