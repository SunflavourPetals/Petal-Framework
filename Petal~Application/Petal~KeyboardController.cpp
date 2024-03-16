#include "Petal~KeyboardController.h"

namespace Petal::Keyboard
{
	const State& WrappedState::GetState() const noexcept
	{
		return this->state;
	}
	void WrappedState::ClearState() noexcept
	{
		this->state = State{ 0 };
	}
	boolean WrappedState::Pushed(VirtualKey::Type vk_code) const noexcept
	{
		return this->state[vk_code];
	}
	void WrappedState::Set(VirtualKey::Type vk_code, boolean pushed) noexcept
	{
		this->state[vk_code] = pushed;
	}
}

namespace Petal::Keyboard
{
	void BasicController::ClearState() noexcept
	{
		this->state = WrappedState{};
	}
	void BasicController::ClearLastState() noexcept
	{
		this->last_state = WrappedState{};
	}
	const WrappedState& BasicController::GetState() const noexcept
	{
		return this->state;
	}
	const WrappedState& BasicController::GetLastState() const noexcept
	{
		return this->last_state;
	}
	void BasicController::ExecuteEventProcess(Abstract::KeyboardEventProcess& proc, Resource& resource)
	{
		if (proc.Check(resource))
		{
			proc.Execution(resource);
		}
	}
}

namespace Petal::Keyboard
{
	void Controller::QueryState() noexcept
	{
		this->last_state = this->state;
		for (const auto& vk_code : this->registry)
		{
			this->state.Set(vk_code, (::GetAsyncKeyState(vk_code) & 0x8000) == 0x8000);
		}
	}
	void Controller::Register(VirtualKey::Type key) noexcept
	{
		this->registry.insert(key);
	}
	tsize Controller::Unregister(VirtualKey::Type key) noexcept
	{
		return this->registry.erase(key);
	}
}
