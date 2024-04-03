#pragma once

#ifndef Petal_Header_XEventProcessComponents
#define Petal_Header_XEventProcessComponents

#include "Petal~PerformanceCounter.h"

namespace Petal::XEventProcess::Component
{
	class Hold final
	{
	public:
		using Tick = typename PerformanceCounter::Tick;
	public:
		template <typename Resource, typename  Process>
		boolean Check(const Resource& resource, const Process& process);
	public:
		Hold(Tick target_count = 1, boolean loop_mode = false) :
			target_count{ target_count },
			loop_triggering{ loop_mode } {}
		~Hold() = default;
	public:
		Tick target_count{ 1 }; // hold target_count to triggering
	private:
		Tick total_count{ 0 };
	public:
		boolean loop_triggering{ false }; // loop triggering target_count to target_count
	private:
		boolean in_holding{ false };
	};

	template <typename Resource, typename  Process>
	inline boolean Hold::Check(const Resource& resource, const Process& process)
	{
		if (this->in_holding)
		{
			if (process.ThisPositive(resource.controller))
			{
				this->total_count += resource.delta_count;
			}
			else
			{
				this->in_holding = false;
				this->total_count = 0;
				return false;
			}
		}
		else
		{
			if (!process.LastPositive(resource.controller) && process.ThisPositive(resource.controller))
			{
				this->total_count = 0;
				this->in_holding = true;
			}
			else
			{
				return false;
			}
		}
		if (this->total_count >= this->target_count)
		{
			this->total_count = 0;
			if (!this->loop_triggering)
			{
				this->in_holding = false;
			}
			return true;
		}
		return false;
	}
}

#endif // !Petal_Header_XEventProcessComponents
