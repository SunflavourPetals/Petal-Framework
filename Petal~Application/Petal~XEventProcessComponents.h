#pragma once

#ifndef Petal_Header_XEventProcessComponents
#define Petal_Header_XEventProcessComponents

#include "Petal~PerformanceCounter.h"

#include <type_traits>

namespace Petal::Concepts
{
	template <typename Resource>
	concept XEventResource = requires(Resource resource) // as (XInput|Keyboard)::ResourceOfController
	{
		typename Resource::ControllerType;
		{ resource.controller } -> ::std::convertible_to<::std::add_lvalue_reference_t<::std::add_const_t<typename Resource::ControllerType>>>;
	};

	template <typename Process, typename Controller>
	concept XEventProcess = requires(Process process, Controller controller) // as (XInput|Keyboard)::MiddleProcess::BasicProcess
	{
		{ process.ThisPositive(controller) } -> ::std::convertible_to<bool>;
		{ process.LastPositive(controller) } -> ::std::convertible_to<bool>;
	};
}

namespace Petal::XEventProcessComponents
{
	class Hold final
	{
	public:
		using Tick = typename PerformanceCounter::Tick;
	public:
		template <Concepts::XEventResource Resource, Concepts::XEventProcess<typename Resource::ControllerType> Process>
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

	template <Concepts::XEventResource Resource, Concepts::XEventProcess<typename Resource::ControllerType> Process>
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
