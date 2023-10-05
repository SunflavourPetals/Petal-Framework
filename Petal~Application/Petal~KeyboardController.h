#pragma once

#ifndef Petal_Header_KeyboardController
#define Petal_Header_KeyboardController

#include "Petal~BasicTypes.h"
#include "Petal~WinTypes.h"
#include "Petal~String.h"
#include "Petal~Process.h"
#include "Petal~VirtualKey.h"

#include <unordered_map>

namespace Petal::Keyboard
{
	using Petal::boolean;
	struct StoredState final
	{
		win32short state{};
		win32short last_state{};
		boolean pushed{};
		u8 unused_pad1{};
		boolean last_pushed{};
		u8 unused_pad2{};
	};
	using StateContainer = ::std::unordered_map<VirtualKey::Type, StoredState>;
	class Controller;
	struct ResourceOfController
	{
		const f64 delta_time;
		const Controller& controller;
	};
}

namespace Petal::Abstract
{
	class KeyboardEventProcess : public Process<Petal::Keyboard::ResourceOfController>
	{
	public:
		virtual const Keyboard::Controller& Controller() const final;
		virtual f64 DeltaTime() const final;
		virtual boolean Check() = 0;
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
	concept KeyboardEventProcessIterator = requires
	{
		typename ::std::iterator_traits<Ty>::value_type;
		requires ::std::is_base_of_v<Abstract::KeyboardEventProcess, typename Ty::value_type>;
	};
	template <typename Ty>
	concept KeyboardtEventProcessPointerIterator = requires (Ty o)
	{
		typename ::std::iterator_traits<Ty>::value_type;
		requires ::std::is_pointer_v<typename Ty::value_type>;
		requires ::std::is_base_of_v<Abstract::KeyboardEventProcess, typename ::std::remove_pointer_t<typename Ty::value_type>>;
		(*o)->Check();
	} || requires (Ty o)
	{
		typename ::std::iterator_traits<Ty>::value_type;
		typename Ty::value_type::element_type;
		requires ::std::is_base_of_v<Abstract::KeyboardEventProcess, typename Ty::value_type::element_type>;
		(*o)->Check();
	};
}

namespace Petal::Keyboard
{
	class Controller final
	{
	public:
		using Resource = ResourceOfController;
	public:
		const StateContainer& KeyStateContainer() const noexcept;
		win32short KeyState(VirtualKey::Type vk) const noexcept;
		win32short LastKeyState(VirtualKey::Type vk) const noexcept;
		boolean Pushed(VirtualKey::Type vk) const noexcept;
		boolean LastPushed(VirtualKey::Type vk) const noexcept;
	public:
		Controller() = default;
		Controller(const StateContainer& o);
		Controller(StateContainer&& o);
		Controller(const Controller&) = default;
		Controller(Controller&&) noexcept = default;
		~Controller() = default;
		Controller& operator = (const Controller&) noexcept = default;
		Controller& operator = (Controller&&) noexcept = default;
	private:
		StateContainer pt_key_state;
	};
}

#endif // !Petal_Header_KeyboardController
