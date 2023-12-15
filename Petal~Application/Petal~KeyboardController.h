#pragma once

#ifndef Petal_Header_KeyboardController
#define Petal_Header_KeyboardController

#include "Petal~BasicTypes.h"
#include "Petal~WinTypes.h"
#include "Petal~String.h"
#include "Petal~Process.h"
#include "Petal~VirtualKey.h"
#include "Petal~PointerTraits.h"

#include <bitset>
#include <unordered_set>

static_assert(::std::is_same_v<Petal::VirtualKey::Type, Petal::u8>);

namespace Petal::Keyboard
{
	constexpr tsize vk_table_size{ 256 };
	using State = ::std::bitset<vk_table_size>;
	class BasicController;
	struct ResourceOfController
	{
		const i64 delta_count{};
		const BasicController& controller;
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
		KeyboardEventProcess(const KeyboardEventProcess&) = default;
		KeyboardEventProcess(KeyboardEventProcess&&) noexcept = default;
		virtual ~KeyboardEventProcess() = default;
	};
}

namespace Petal::Keyboard
{
	class WrappedState final
	{
	public:
		const State& GetState() const noexcept;
		void ClearState() noexcept;
		boolean Pushed(VirtualKey::Type vk_code) const noexcept;
		void Set(VirtualKey::Type vk_code, boolean pushed) noexcept;
	public:
		WrappedState() = default;
		WrappedState(const WrappedState&) = default;
		WrappedState(WrappedState&&) noexcept = default;
		~WrappedState() = default;
		WrappedState& operator=(const WrappedState&) noexcept = default;
		WrappedState& operator=(WrappedState&&) noexcept = default;
	private:
		State pt_state{ 0 };
	};
}

namespace Petal::Concept
{
	template <typename Ty>
	concept GenericKeyboardEventProcessIterator = requires
	{
		typename ::std::iterator_traits<Ty>::value_type;
		requires ::std::is_const_v<typename TypeTraits::RemoveAllGenericPointer<typename Ty::value_type>::Type> == false;
		requires ::std::is_volatile_v<typename TypeTraits::RemoveAllGenericPointer<typename Ty::value_type>::Type> == false;
		requires ::std::is_base_of_v<Abstract::KeyboardEventProcess, typename TypeTraits::RemoveAllGenericPointer<typename Ty::value_type>::Type>;
	};
	template <typename Ty>
	concept GenericKeyboardEventProcessPointer = TypeTraits::is_generic_pointer<Ty> && ::std::is_base_of_v<Abstract::KeyboardEventProcess, typename TypeTraits::RemoveAllGenericPointer<Ty>::Type>;
}

namespace Petal::Keyboard
{
	class BasicController
	{
	public:
		using Resource = ResourceOfController;
	protected:
		virtual void QueryState() noexcept = 0;
	public:
		void ClearState() noexcept;
		void ClearLastState() noexcept;
		const WrappedState& GetState() const noexcept;
		const WrappedState& GetLastState() const noexcept;
	public:
		void Update(Concept::GenericKeyboardEventProcessIterator auto begin, Concept::GenericKeyboardEventProcessIterator auto end, i64 delta_count = 0);
	private:
		static void ExecuteEventProcess(Abstract::KeyboardEventProcess& proc, Resource& resource);
		static void ExecuteEventProcess(Concept::GenericKeyboardEventProcessPointer auto& pointer, Resource& resource);
	protected:
		WrappedState pt_state{};
		WrappedState pt_last_state{};
	};
}

namespace Petal::Keyboard
{
	class Controller final : public BasicController
	{
	public:
		using Registry = ::std::unordered_set<typename VirtualKey::Type>;
	private:
		void QueryState() noexcept override;
	public:
		void Register(VirtualKey::Type key) noexcept;
		tsize Unregister(VirtualKey::Type key) noexcept;
	public:
		Controller() = default;
		Controller(const Controller&) = default;
		Controller(Controller&&) noexcept = default;
		~Controller() = default;
		Controller& operator=(const Controller&) = default;
		Controller& operator=(Controller&&) noexcept = default;
	private:
		Registry pt_registry{};
	};
}

namespace Petal::Keyboard
{
	inline void BasicController::Update(Concept::GenericKeyboardEventProcessIterator auto begin, Concept::GenericKeyboardEventProcessIterator auto end, i64 delta_count)
	{
		this->QueryState();
		Resource resource{ delta_count, *this };
		for (; begin != end; ++begin)
		{
			this->ExecuteEventProcess(*begin, resource);
		}
	}
	inline void BasicController::ExecuteEventProcess(Concept::GenericKeyboardEventProcessPointer auto& pointer, Resource& resource)
	{
		if (pointer != nullptr)
		{
			BasicController::ExecuteEventProcess(*pointer, resource);
		}
	}
}

#endif // !Petal_Header_KeyboardController
