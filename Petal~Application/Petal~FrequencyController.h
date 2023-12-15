#pragma once

#ifndef Petal_Header_FrequencyController
#define Petal_Header_FrequencyController

#include "Petal~BasicTypes.h"
#include "Petal~PerformanceCounter.h"
#include "Petal~Process.h"

#include <thread>
#include <utility>

namespace Petal
{
	class FrequencyController final
	{
	public:
		struct ResourceDelta
		{
			f64 delta_time{};
			f64 total_time{};
		};
		struct ResourceStatistics
		{
			i64 frame_counts{};
			f64 statistics_time{};
		};
	private:
		static constexpr bool noexcept_pc_frequency{ noexcept(::std::declval<PerformanceCounter>().Frequency()) };
		static constexpr bool noexcept_fc_sleep{ noexcept(::std::this_thread::sleep_for(::std::chrono::nanoseconds{ 1 })) };
		static constexpr bool noexcept_fc_count
		{
			noexcept(::std::declval<PerformanceCounter>().Count()) &&
			noexcept(::std::declval<PerformanceCounter>().DeltaCounts())
		};
	public:
		void ResetCounter() noexcept(noexcept(::std::declval<PerformanceCounter>().Reset()));
		void Pause() noexcept(noexcept(::std::declval<PerformanceCounter>().Pause()));
		void Start() noexcept(noexcept(::std::declval<PerformanceCounter>().Start()));
		i64 CounterFrequency() const noexcept(noexcept_pc_frequency);
		const PerformanceCounter& Counter() const noexcept;
	public:
		void UpdateFrequencyAutoSFT(f64 frequency) noexcept(noexcept_pc_frequency);
		void UpdateTargetDeltaTimeAutoSFT(f64 target_delta_time) noexcept(noexcept_pc_frequency);
		void UpdateTargetDeltaCountAutoSFT(i64 count) noexcept;
		void UpdateFrequency(f64 frequency) noexcept(noexcept_pc_frequency);
		void UpdateTargetDeltaTime(f64 target_delta_time_second) noexcept(noexcept_pc_frequency);
		void UpdateTargetDeltaCount(i64 target_delta_count) noexcept;
		void UpdateSleepFailureTimeByDelta(f64 ratio_of_delta) noexcept;
		void UpdateSleepFailureTime(f64 time_second) noexcept(noexcept_pc_frequency);
		void UpdateSleepFailureCount(i64 count) noexcept;
		void UpdateStatisticsTime(f64 time_second) noexcept(noexcept_pc_frequency);
		void UpdateStatisticsCount(i64 count) noexcept;
		void UpdateSleepCountLimitation(i64 sleep_limitation) noexcept;
	public:
		f64 Frequency() const noexcept(noexcept_pc_frequency);
		f64 TargetDeltaTime() const noexcept(noexcept_pc_frequency);
		i64 TargetDeltaCount() const noexcept;
		f64 TargetStatisticsTime() const noexcept(noexcept_pc_frequency);
		i64 TargetStatisticsCount() const noexcept;
		f64 SleepFailureTime() const noexcept(noexcept_pc_frequency);
		i64 SleepFailureCount() const noexcept;
		i64 SleepCountLimitation() const noexcept;
	public:
		boolean LimitedDo(Abstract::Process<ResourceDelta>& user_process, Abstract::Process<ResourceStatistics>& user_statistics_process);
		boolean LimitedDo(Abstract::Process<ResourceDelta>& user_process);
		boolean LimitedDo(fptr<void, ResourceDelta&> user_process, fptr<void, ResourceStatistics&> user_statistics_process);
		boolean LimitedDo(fptr<void, ResourceDelta&> user_process);
	private:
		void Count() noexcept(noexcept_fc_count);
		boolean Sleep() noexcept(noexcept_fc_sleep);
		void Statistics(Abstract::Process<ResourceStatistics>& process);
		void Statistics(fptr<void, ResourceStatistics&> process);
		void Do(Abstract::Process<ResourceDelta>& process);
		void Do(fptr<void, ResourceDelta&> process);
	public:
		FrequencyController(f64 target_frequency = 1.0, f64 sleep_failure_second = 1.0, f64 statistics_interval_second = 1.0);
		FrequencyController(const FrequencyController&) = default;
		FrequencyController(FrequencyController&&) noexcept = default;
		~FrequencyController() = default;
	private:
		static constexpr i64 pt_max_value{ static_cast<i64>((~0LLU) >> 1) };
	private:
		PerformanceCounter pt_performance_counter;
		i64 pt_frame_count{}; // 帧数: 实际间隔时间内执行的次数
		i64 pt_sleep_failure_threshold_count{ pt_max_value }; // 睡眠失效阈值，当距离 actual_delta_count 的 tick 数不足此值时，不进行睡眠，但是成员 Sleep 函数依旧返回 false
		i64 pt_sleep_count{}; // 间隔时间内剩余的睡眠次数，每个间隔会重置
		i64 pt_sleep_limitation{ pt_max_value }; // 目标间隔时间内允许的睡眠次数
		i64 pt_delta_count{}; // 间隔时间
		i64 pt_target_delta_count{}; // 目标间隔时间
		i64 pt_actual_delta_count{}; // 经修正的目标间隔时间
		i64 pt_statistics_count{}; // 每次统计的间隔时间
		i64 pt_target_statistics_count{}; // 每次统计的目标间隔时间
		i64 pt_actual_statistics_count{}; // 每次统计的实际间隔时间
	};
}

#endif // !Petal_Header_FrequencyController
