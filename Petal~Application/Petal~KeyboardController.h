#pragma once

#ifndef Petal_Header_KeyboardController
#define Petal_Header_KeyboardController

#include "Petal~BasicTypes.h"
#include "Petal~WinTypes.h"
#include "Petal~String.h"
#include "Petal~Process.h"
#include "Petal~VirtualKey.h"
#include "Petal~PointerTraits.h"

#include <unordered_map>

namespace Petal::Keyboard
{
	using Petal::boolean;
	class Controller;
	class StoredState final
	{
	public:
		win32short Value() const noexcept;
		boolean Pushed() const noexcept;
	private:
		void Update(win32short new_state) noexcept;
	public:
		StoredState() = default;
		StoredState(const StoredState&) = default;
		StoredState(StoredState&&) = default;
		~StoredState() = default;
		StoredState& operator=(const StoredState&) = default;
	private:
		win32short pt_state{};
		boolean pt_pushed{};
		u8 pt_unused_pad{};
		friend class Controller;
	};
	struct StoredPairState
	{
		StoredState state;
		StoredState last_state;
	};
	using StateContainer = ::std::unordered_map<VirtualKey::Type, StoredPairState>;
	struct ResourceOfController
	{
		const i64 delta_count{};
		const Controller& controller;
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

namespace Petal::Concept
{
	template <typename Ty>
	concept KeyboardEventProcessGeneralIterator = requires
	{
		typename ::std::iterator_traits<Ty>::value_type;
		requires ::std::is_const_v<typename TypeTraits::RemoveAllGenericPointer<typename Ty::value_type>::Type> == false;
		requires ::std::is_volatile_v<typename TypeTraits::RemoveAllGenericPointer<typename Ty::value_type>::Type> == false;
		requires ::std::is_base_of_v<Abstract::KeyboardEventProcess, typename TypeTraits::RemoveAllGenericPointer<typename Ty::value_type>::Type>;
	};
	template <typename Ty>
	concept KeyboardEventProcessGeneralPointer = TypeTraits::is_generic_pointer<Ty> && ::std::is_base_of_v<Abstract::KeyboardEventProcess, typename TypeTraits::RemoveAllGenericPointer<Ty>::Type>;
}

namespace Petal::Keyboard
{
	class Controller final
	{
	public:
		using Resource = ResourceOfController;
	public:
		const StoredState& GetState(VirtualKey::Type vk) const noexcept;
		const StoredState& GetLastState(VirtualKey::Type vk) const noexcept;
		StateContainer& GetStateContainer() noexcept;
		const StateContainer& GetStateContainer() const noexcept;
		boolean Register(VirtualKey::Type key) noexcept;
		boolean Unregister(VirtualKey::Type key) noexcept;
		void Update(Concept::KeyboardEventProcessGeneralIterator auto begin, Concept::KeyboardEventProcessGeneralIterator auto end, i64 delta_count = 0);
	private:
		StateContainer::const_iterator FindStoredState(VirtualKey::Type vk) const noexcept;
		void QueryState() noexcept;
		static const StoredState& NullState() noexcept;
		static void ExecuteEventProcess(Abstract::KeyboardEventProcess& proc, Resource& resource);
		static void ExecuteEventProcess(Concept::KeyboardEventProcessGeneralPointer auto& pointer, Resource& resource);
	public:
		Controller() = default;
		Controller(const Controller&) = default;
		Controller(Controller&&) noexcept = default;
		~Controller() = default;
		Controller& operator = (const Controller&) = default;
		Controller& operator = (Controller&&) noexcept = default;
	public:
		StateContainer key_state_container;
	};
	inline void Controller::Update(Concept::KeyboardEventProcessGeneralIterator auto begin, Concept::KeyboardEventProcessGeneralIterator auto end, i64 delta_count)
	{
		this->QueryState();
		Resource resource{ delta_count, *this };
		for (; begin != end; ++begin)
		{
			this->ExecuteEventProcess(*begin, resource);
		}
	}
	inline void Controller::ExecuteEventProcess(Concept::KeyboardEventProcessGeneralPointer auto& pointer, Resource& resource)
	{
		if (pointer != nullptr)
		{
			Controller::ExecuteEventProcess(*pointer, resource);
		}
	}
}

#endif // !Petal_Header_KeyboardController
