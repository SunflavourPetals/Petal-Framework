#pragma once

#ifndef Petal_Header_KeyboardController
#define Petal_Header_KeyboardController

#include "Petal~BasicTypes.h"
#include "Petal~WinTypes.h"
#include "Petal~String.h"
#include "Petal~Process.h"
#include "Petal~VirtualKey.h"
#include "Petal~PointerTraits.h"
#include "Petal~PerformanceCounter.h"

#include <bitset>
#include <set>
#include <initializer_list>

static_assert(::std::is_same_v<Petal::VirtualKey::Type, Petal::u8>);

namespace Petal::Abstract::Keyboard
{
	class Controller;
}

namespace Petal::Keyboard
{
	constexpr tsize vk_table_size{ 256 };
	using Tick = typename PerformanceCounter::Tick;
	using State = ::std::bitset<vk_table_size>;
	class Controller;
	struct ResourceOfController
	{
		using ControllerType = Abstract::Keyboard::Controller;
		using DeltaCountType = Tick;
		const DeltaCountType delta_count{};
		const ControllerType& controller;
	};
}

namespace Petal::Abstract
{
	class KeyboardEventProcess : public Process<Petal::Keyboard::ResourceOfController>
	{
	public:
		using Resource = Petal::Keyboard::ResourceOfController;
	public:
		virtual boolean Check(const Resource&) = 0;
	public:
		KeyboardEventProcess() = default;
		virtual ~KeyboardEventProcess() = default;
	};
}

namespace Petal::Keyboard
{
	class WrappedState final
	{
	public:
		const State& GetState() const noexcept { return state; }
		void ClearState() noexcept { state = State{ 0 }; }
		boolean Pushed(VirtualKey::Type vk_code) const noexcept { return state[vk_code]; }
		void Set(VirtualKey::Type vk_code, boolean pushed) noexcept { this->state[vk_code] = pushed; }
	public:
		WrappedState() = default;
		~WrappedState() = default;
	private:
		State state{ 0 };
	};
}

namespace Petal::Concept
{
	template <typename Ty>
	concept GenericKeyboardEventProcessIterator = requires
	{
		typename ::std::iterator_traits<Ty>::value_type;
		requires ::std::is_base_of_v<Abstract::KeyboardEventProcess, typename TypeTraits::RemoveAllAnyPointerType<typename Ty::value_type>>;
		::std::declval<typename TypeTraits::RemoveAllAnyPointerType<typename Ty::value_type>>().Execution(::std::declval<const Abstract::KeyboardEventProcess::Resource&>());
	};
	template <typename Ty>
	concept GenericKeyboardEventProcessPointer = TypeTraits::is_any_pointer<Ty> && ::std::is_base_of_v<Abstract::KeyboardEventProcess, typename TypeTraits::RemoveAllAnyPointerType<Ty>>;
}

namespace Petal::Abstract::Keyboard
{
	using namespace Petal::Keyboard;

	class Controller
	{
	public:
		using Resource = ResourceOfController;
	protected:
		virtual void QueryState() noexcept = 0;
	public:
		void ClearState() noexcept { state = WrappedState{}; }
		void ClearLastState() noexcept { last_state = WrappedState{}; }
		const WrappedState& GetState() const noexcept { return state; }
		const WrappedState& GetLastState() const noexcept { return last_state; }
	public:
		void Update(Concept::GenericKeyboardEventProcessIterator auto begin, Concept::GenericKeyboardEventProcessIterator auto end, Tick delta_count = 0);
	private:
		static void ExecuteEventProcess(Abstract::KeyboardEventProcess& proc, Resource& resource);
		static void ExecuteEventProcess(Concept::GenericKeyboardEventProcessPointer auto& pointer, Resource& resource);
	protected:
		WrappedState state{};
		WrappedState last_state{};
	};
}

namespace Petal::Keyboard
{
	class Controller final : public Abstract::Keyboard::Controller
	{
	public:
		using Registry = ::std::set<typename VirtualKey::Type>;
	private:
		void QueryState() noexcept override;
	public:
		void Register(VirtualKey::Type key) noexcept { registry.insert(key); }
		void Register(::std::initializer_list<VirtualKey::Type> keys) noexcept { registry.insert(keys); }
		template <typename Iterator>
		void Register(Iterator begin, Iterator end) { registry.insert(begin, end); }
		tsize Unregister(VirtualKey::Type key) noexcept { return registry.erase(key); }
	public:
		Controller() = default;
		~Controller() = default;
	private:
		Registry registry{};
	};
}

namespace Petal::Abstract::Keyboard
{
	inline void Controller::ExecuteEventProcess(Abstract::KeyboardEventProcess& proc, Resource& resource)
	{
		if (proc.Check(resource))
		{
			proc.Execution(resource);
		}
	}
	inline void Controller::Update(Concept::GenericKeyboardEventProcessIterator auto begin, Concept::GenericKeyboardEventProcessIterator auto end, Keyboard::Tick delta_count)
	{
		this->QueryState();
		Resource resource{ delta_count, *this };
		for (; begin != end; ++begin)
		{
			this->ExecuteEventProcess(*begin, resource);
		}
	}
	inline void Controller::ExecuteEventProcess(Concept::GenericKeyboardEventProcessPointer auto& pointer, Resource& resource)
	{
		if (pointer != nullptr)
		{
			Controller::ExecuteEventProcess(*pointer, resource);
		}
	}
}

namespace Petal::Keyboard
{
	inline void Controller::QueryState() noexcept
	{
		this->last_state = this->state;
		for (const auto& vk_code : this->registry)
		{
			this->state.Set(vk_code, (::GetAsyncKeyState(vk_code) & 0x8000) == 0x8000);
		}
	}
}


#endif // !Petal_Header_KeyboardController
