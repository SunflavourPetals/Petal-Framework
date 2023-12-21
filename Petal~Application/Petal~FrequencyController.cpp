#include "Petal~FrequencyController.h"

#include <thread>
#include <utility>

namespace Petal
{
	FrequencyController::FrequencyController(f64 target_frequency, f64 sleep_failure_second, f64 statistics_interval_second)
	{
		this->UpdateFrequency(target_frequency);
		this->UpdateSleepFailureTime(sleep_failure_second);
		this->UpdateStatisticsTime(statistics_interval_second);
	}
	void FrequencyController::ResetCounter() noexcept(noexcept(::std::declval<PerformanceCounter>().Reset()))
	{
		this->pt_performance_counter.Reset();
	}
	void FrequencyController::Pause() noexcept(noexcept(::std::declval<PerformanceCounter>().Pause()))
	{
		this->pt_performance_counter.Pause();
	}
	void FrequencyController::Start() noexcept(noexcept(::std::declval<PerformanceCounter>().Start()))
	{
		this->pt_performance_counter.Start();
	}
	i64 FrequencyController::CounterFrequency() const noexcept(noexcept_pc_frequency)
	{
		return this->pt_performance_counter.Frequency();
	}
	const PerformanceCounter& FrequencyController::Counter() const noexcept
	{
		return this->pt_performance_counter;
	}
	void FrequencyController::UpdateFrequencyAutoSFT(f64 frequency) noexcept(noexcept_pc_frequency)
	{
		f64 ratio{ static_cast<f64>(this->pt_sleep_failure_threshold_count) / this->pt_target_delta_count };
		this->UpdateFrequency(frequency);
		this->UpdateSleepFailureTimeByDelta(ratio);
	}
	void FrequencyController::UpdateTargetDeltaTimeAutoSFT(f64 target_delta_time) noexcept(noexcept_pc_frequency)
	{
		f64 ratio{ static_cast<f64>(this->pt_sleep_failure_threshold_count) / this->pt_target_delta_count };
		this->UpdateTargetDeltaTime(target_delta_time);
		this->UpdateSleepFailureTimeByDelta(ratio);
	}
	void FrequencyController::UpdateTargetDeltaCountAutoSFT(i64 count) noexcept
	{
		f64 ratio{ static_cast<f64>(this->pt_sleep_failure_threshold_count) / this->pt_target_delta_count };
		this->UpdateTargetDeltaCount(count);
		this->UpdateSleepFailureTimeByDelta(ratio);
	}
	void FrequencyController::UpdateFrequency(f64 frequency) noexcept(noexcept_pc_frequency)
	{
		if (frequency <= 0.0) this->pt_target_delta_count = 0;
		else this->pt_target_delta_count = static_cast<i64>(this->pt_performance_counter.Frequency() / frequency);
	}
	void FrequencyController::UpdateTargetDeltaTime(f64 target_delta_time_second) noexcept(noexcept_pc_frequency)
	{
		this->pt_target_delta_count = static_cast<i64>(target_delta_time_second * this->pt_performance_counter.Frequency());
	}
	void FrequencyController::UpdateTargetDeltaCount(i64 target_delta_count) noexcept
	{
		this->pt_target_delta_count = target_delta_count;
	}
	void FrequencyController::UpdateSleepFailureTimeByDelta(f64 ratio_of_delta) noexcept
	{
		this->pt_sleep_failure_threshold_count = static_cast<i64>(ratio_of_delta * this->pt_target_delta_count);
	}
	void FrequencyController::UpdateSleepFailureTime(f64 time_second) noexcept(noexcept_pc_frequency)
	{
		this->pt_sleep_failure_threshold_count = static_cast<i64>(time_second * this->pt_performance_counter.Frequency());
	}
	void FrequencyController::UpdateSleepFailureCount(i64 count) noexcept
	{
		this->pt_sleep_failure_threshold_count = count;
	}
	void FrequencyController::UpdateStatisticsTime(f64 time_second) noexcept(noexcept_pc_frequency)
	{
		this->pt_target_statistics_count = static_cast<i64>(time_second * this->pt_performance_counter.Frequency());
	}
	void FrequencyController::UpdateStatisticsCount(i64 count) noexcept
	{
		this->pt_target_statistics_count = count;
	}
	void FrequencyController::UpdateSleepCountLimitation(i64 sleep_limitation) noexcept
	{
		this->pt_sleep_limitation = sleep_limitation;
	}
	void FrequencyController::UpdateSleepFunction(fptr<void> sleep_fn) noexcept
	{
		this->pt_sleep_fn = sleep_fn;
	}
	f64 FrequencyController::Frequency() const noexcept(noexcept_pc_frequency)
	{
		return this->pt_performance_counter.Frequency() / static_cast<f64>(this->pt_target_delta_count);
	}
	f64 FrequencyController::TargetDeltaTime() const noexcept(noexcept_pc_frequency)
	{
		return static_cast<f64>(this->pt_target_delta_count) / this->pt_performance_counter.Frequency();
	}
	i64 FrequencyController::TargetDeltaCount() const noexcept
	{
		return this->pt_target_delta_count;
	}
	f64 FrequencyController::TargetStatisticsTime() const noexcept(noexcept_pc_frequency)
	{
		return static_cast<f64>(this->pt_target_statistics_count) / this->pt_performance_counter.Frequency();
	}
	i64 FrequencyController::TargetStatisticsCount() const noexcept
	{
		return this->pt_target_statistics_count;
	}
	f64 FrequencyController::SleepFailureTime() const noexcept(noexcept_pc_frequency)
	{
		return static_cast<f64>(this->pt_sleep_failure_threshold_count) / this->pt_performance_counter.Frequency();
	}
	i64 FrequencyController::SleepFailureCount() const noexcept
	{
		return this->pt_sleep_failure_threshold_count;
	}
	i64 FrequencyController::SleepCountLimitation() const noexcept
	{
		return this->pt_sleep_limitation;
	}
	fptr<void> FrequencyController::SleepFunction() const noexcept
	{
		return this->pt_sleep_fn;
	}
	boolean FrequencyController::LimitedDo(Abstract::Process<ResourceDelta>& user_process, Abstract::Process<ResourceStatistics>& user_statistics_process)
	{
		this->Count();
		if (this->Sleep())
		{
			this->Statistics(user_statistics_process);
			return false;
		}
		this->Do(user_process);
		this->Count();
		this->Statistics(user_statistics_process);
		return true;
	}
	boolean FrequencyController::LimitedDo(Abstract::Process<ResourceDelta>& user_process)
	{
		this->Count();
		if (this->Sleep())
		{
			return false;
		}
		this->Do(user_process);
		this->Count();
		return true;
	}
	boolean FrequencyController::LimitedDo(fptr<void, ResourceDelta&> user_process, fptr<void, ResourceStatistics&> user_statistics_process)
	{
		this->Count();
		if (this->Sleep())
		{
			this->Statistics(user_statistics_process);
			return false;
		}
		this->Do(user_process);
		this->Count();
		this->Statistics(user_statistics_process);
		return true;
	}
	boolean FrequencyController::LimitedDo(fptr<void, ResourceDelta&> user_process)
	{
		this->Count();
		if (this->Sleep())
		{
			return false;
		}
		this->Do(user_process);
		this->Count();
		return true;
	}
	void FrequencyController::Count() noexcept(noexcept_fc_count)
	{
		this->pt_performance_counter.Count();
		this->pt_delta_count += this->pt_performance_counter.DeltaCounts(); // 增加间隔时间
		this->pt_statistics_count += this->pt_performance_counter.DeltaCounts(); // 增加统计间隔时间
	}
	boolean FrequencyController::Sleep() noexcept(noexcept_fc_sleep)
	{
		if (this->pt_delta_count < this->pt_target_delta_count)
		{
			if ((this->pt_sleep_count > 0) && ((this->pt_target_delta_count - this->pt_delta_count) > this->pt_sleep_failure_threshold_count))
			{
				this->pt_sleep_fn();
				--(this->pt_sleep_count);
			}
			return true;
		}
		return false;
	}
	void FrequencyController::Statistics(Abstract::Process<ResourceStatistics>& process)
	{
		if (this->pt_statistics_count >= this->pt_target_statistics_count)
		{
			this->pt_actual_statistics_count = this->pt_statistics_count - this->pt_actual_statistics_count;
			ResourceStatistics resource{ this->pt_frame_count, static_cast<f64>(this->pt_actual_statistics_count) / this->pt_performance_counter.Frequency() };
			process.Execution(resource);
			this->pt_statistics_count -= this->pt_target_statistics_count;
			this->pt_actual_statistics_count = this->pt_statistics_count;
			this->pt_frame_count = 0;
		}
	}
	void FrequencyController::Statistics(fptr<void, ResourceStatistics&> process)
	{
		if (this->pt_statistics_count >= this->pt_target_statistics_count)
		{
			this->pt_actual_statistics_count = this->pt_statistics_count - this->pt_actual_statistics_count;
			ResourceStatistics resource{ this->pt_frame_count, static_cast<f64>(this->pt_actual_statistics_count) / this->pt_performance_counter.Frequency() };
			process(resource);
			this->pt_statistics_count -= this->pt_target_statistics_count;
			this->pt_actual_statistics_count = this->pt_statistics_count;
			this->pt_frame_count = 0;
		}
	}
	void FrequencyController::Do(Abstract::Process<ResourceDelta>& process)
	{
		this->pt_actual_delta_count = this->pt_delta_count - this->pt_actual_delta_count;
		ResourceDelta resource{ static_cast<f64>(this->pt_actual_delta_count) / this->pt_performance_counter.Frequency(), this->pt_performance_counter.TotalTime() };
		process.Execution(resource);
		this->pt_delta_count -= this->pt_target_delta_count;
		this->pt_actual_delta_count = this->pt_delta_count;
		++(this->pt_frame_count);
		this->pt_sleep_count = this->pt_sleep_limitation;
	}
	void FrequencyController::Do(fptr<void, ResourceDelta&> process)
	{
		this->pt_actual_delta_count = this->pt_delta_count - this->pt_actual_delta_count;
		ResourceDelta resource{ static_cast<f64>(this->pt_actual_delta_count) / this->pt_performance_counter.Frequency(), this->pt_performance_counter.TotalTime() };
		process(resource);
		this->pt_delta_count -= this->pt_target_delta_count;
		this->pt_actual_delta_count = this->pt_delta_count;
		++(this->pt_frame_count);
		this->pt_sleep_count = this->pt_sleep_limitation;
	}
}
