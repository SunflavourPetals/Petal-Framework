#include "Petal~FrequencyController.h"

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
		this->performance_counter.Reset();
	}
	void FrequencyController::Pause() noexcept(noexcept(::std::declval<PerformanceCounter>().Pause()))
	{
		this->performance_counter.Pause();
	}
	void FrequencyController::Start() noexcept(noexcept(::std::declval<PerformanceCounter>().Start()))
	{
		this->performance_counter.Start();
	}
	i64 FrequencyController::CounterFrequency() const noexcept(noexcept_pc_frequency)
	{
		return this->performance_counter.Frequency();
	}
	const PerformanceCounter& FrequencyController::Counter() const noexcept
	{
		return this->performance_counter;
	}
	void FrequencyController::UpdateFrequencyAutoSFT(f64 frequency) noexcept(noexcept_pc_frequency)
	{
		f64 ratio{ static_cast<f64>(this->sleep_failure_threshold_count) / this->target_delta_count };
		this->UpdateFrequency(frequency);
		this->UpdateSleepFailureTimeByDelta(ratio);
	}
	void FrequencyController::UpdateTargetDeltaTimeAutoSFT(f64 target_delta_time) noexcept(noexcept_pc_frequency)
	{
		f64 ratio{ static_cast<f64>(this->sleep_failure_threshold_count) / this->target_delta_count };
		this->UpdateTargetDeltaTime(target_delta_time);
		this->UpdateSleepFailureTimeByDelta(ratio);
	}
	void FrequencyController::UpdateTargetDeltaCountAutoSFT(i64 count) noexcept
	{
		f64 ratio{ static_cast<f64>(this->sleep_failure_threshold_count) / this->target_delta_count };
		this->UpdateTargetDeltaCount(count);
		this->UpdateSleepFailureTimeByDelta(ratio);
	}
	void FrequencyController::UpdateFrequency(f64 frequency) noexcept(noexcept_pc_frequency)
	{
		if (frequency <= 0.0) this->target_delta_count = 0;
		else this->target_delta_count = static_cast<i64>(this->performance_counter.Frequency() / frequency);
	}
	void FrequencyController::UpdateTargetDeltaTime(f64 target_delta_time_second) noexcept(noexcept_pc_frequency)
	{
		this->target_delta_count = static_cast<i64>(target_delta_time_second * this->performance_counter.Frequency());
	}
	void FrequencyController::UpdateTargetDeltaCount(i64 target_delta_count) noexcept
	{
		this->target_delta_count = target_delta_count;
	}
	void FrequencyController::UpdateSleepFailureTimeByDelta(f64 ratio_of_delta) noexcept
	{
		this->sleep_failure_threshold_count = static_cast<i64>(ratio_of_delta * this->target_delta_count);
	}
	void FrequencyController::UpdateSleepFailureTime(f64 time_second) noexcept(noexcept_pc_frequency)
	{
		this->sleep_failure_threshold_count = static_cast<i64>(time_second * this->performance_counter.Frequency());
	}
	void FrequencyController::UpdateSleepFailureCount(i64 count) noexcept
	{
		this->sleep_failure_threshold_count = count;
	}
	void FrequencyController::UpdateStatisticsTime(f64 time_second) noexcept(noexcept_pc_frequency)
	{
		this->target_statistics_count = static_cast<i64>(time_second * this->performance_counter.Frequency());
	}
	void FrequencyController::UpdateStatisticsCount(i64 count) noexcept
	{
		this->target_statistics_count = count;
	}
	void FrequencyController::UpdateSleepCountLimitation(i64 sleep_limitation) noexcept
	{
		this->sleep_limitation = sleep_limitation;
	}
	void FrequencyController::UpdateSleepFunction(fptr<void> sleep_fn) noexcept
	{
		this->sleep_fn = (sleep_fn == nullptr) ? this->default_sleep_fn : sleep_fn;
	}
	f64 FrequencyController::Frequency() const noexcept(noexcept_pc_frequency)
	{
		return this->performance_counter.Frequency() / static_cast<f64>(this->target_delta_count);
	}
	f64 FrequencyController::TargetDeltaTime() const noexcept(noexcept_pc_frequency)
	{
		return static_cast<f64>(this->target_delta_count) / this->performance_counter.Frequency();
	}
	i64 FrequencyController::TargetDeltaCount() const noexcept
	{
		return this->target_delta_count;
	}
	f64 FrequencyController::TargetStatisticsTime() const noexcept(noexcept_pc_frequency)
	{
		return static_cast<f64>(this->target_statistics_count) / this->performance_counter.Frequency();
	}
	i64 FrequencyController::TargetStatisticsCount() const noexcept
	{
		return this->target_statistics_count;
	}
	f64 FrequencyController::SleepFailureTime() const noexcept(noexcept_pc_frequency)
	{
		return static_cast<f64>(this->sleep_failure_threshold_count) / this->performance_counter.Frequency();
	}
	i64 FrequencyController::SleepFailureCount() const noexcept
	{
		return this->sleep_failure_threshold_count;
	}
	i64 FrequencyController::SleepCountLimitation() const noexcept
	{
		return this->sleep_limitation;
	}
	fptr<void> FrequencyController::SleepFunction() const noexcept
	{
		return this->sleep_fn;
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
		this->performance_counter.Count();
		this->delta_count += this->performance_counter.DeltaCounts();
		this->statistics_count += this->performance_counter.DeltaCounts();
	}
	boolean FrequencyController::Sleep() noexcept(noexcept_fc_sleep)
	{
		if (this->delta_count < this->target_delta_count)
		{
			if ((this->sleep_count > 0) && ((this->target_delta_count - this->delta_count) > this->sleep_failure_threshold_count))
			{
				this->sleep_fn();
				--(this->sleep_count);
			}
			return true;
		}
		return false;
	}
	void FrequencyController::Statistics(Abstract::Process<ResourceStatistics>& process)
	{
		if (this->statistics_count >= this->target_statistics_count)
		{
			this->actual_statistics_count = this->statistics_count - this->actual_statistics_count;
			ResourceStatistics resource{ this->frame_count, static_cast<f64>(this->actual_statistics_count) / this->performance_counter.Frequency() };
			process.Execution(resource);
			this->statistics_count -= this->target_statistics_count;
			this->actual_statistics_count = this->statistics_count;
			this->frame_count = 0;
		}
	}
	void FrequencyController::Statistics(fptr<void, ResourceStatistics&> process)
	{
		if (this->statistics_count >= this->target_statistics_count)
		{
			this->actual_statistics_count = this->statistics_count - this->actual_statistics_count;
			ResourceStatistics resource{ this->frame_count, static_cast<f64>(this->actual_statistics_count) / this->performance_counter.Frequency() };
			process(resource);
			this->statistics_count -= this->target_statistics_count;
			this->actual_statistics_count = this->statistics_count;
			this->frame_count = 0;
		}
	}
	void FrequencyController::Do(Abstract::Process<ResourceDelta>& process)
	{
		this->actual_delta_count = this->delta_count - this->actual_delta_count;
		ResourceDelta resource{ static_cast<f64>(this->actual_delta_count) / this->performance_counter.Frequency(), this->performance_counter.TotalTime() };
		process.Execution(resource);
		this->delta_count -= this->target_delta_count;
		this->actual_delta_count = this->delta_count;
		++(this->frame_count);
		this->sleep_count = this->sleep_limitation;
	}
	void FrequencyController::Do(fptr<void, ResourceDelta&> process)
	{
		this->actual_delta_count = this->delta_count - this->actual_delta_count;
		ResourceDelta resource{ static_cast<f64>(this->actual_delta_count) / this->performance_counter.Frequency(), this->performance_counter.TotalTime() };
		process(resource);
		this->delta_count -= this->target_delta_count;
		this->actual_delta_count = this->delta_count;
		++(this->frame_count);
		this->sleep_count = this->sleep_limitation;
	}
}
