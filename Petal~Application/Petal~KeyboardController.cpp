#include "Petal~KeyboardController.h"

namespace Petal::Keyboard
{
	win32short StoredState::Value() const noexcept
	{
		return this->pt_state;
	}
	boolean StoredState::Pushed() const noexcept
	{
		return this->pt_pushed;
	}
	void StoredState::Update(win32short new_state) noexcept
	{
		this->pt_state = new_state;
		this->pt_pushed = (new_state & 0x8000) == 0x8000;
	}
	
	const StoredState& Controller::GetState(VirtualKey::Type vk) const noexcept
	{
		auto result{ this->FindStoredState(vk) };
		if (result != this->key_state_container.end())
		{
			return result->second.state;
		}
		return this->NullState();
	}
	const StoredState& Controller::GetLastState(VirtualKey::Type vk) const noexcept
	{
		auto result{ this->FindStoredState(vk) };
		if (result != this->key_state_container.end())
		{
			return result->second.last_state;
		}
		return this->NullState();
	}
	StateContainer& Controller::GetStateContainer() noexcept
	{
		return this->key_state_container;
	}
	const StateContainer& Controller::GetStateContainer() const noexcept
	{
		return this->key_state_container;
	}
	boolean Controller::Register(VirtualKey::Type key) noexcept
	{
		auto result{ this->FindStoredState(key) };
		if (result == this->key_state_container.end())
		{
			this->key_state_container.insert({ key, {} });
			return true;
		}
		return false;
	}
	boolean Controller::Unregister(VirtualKey::Type key) noexcept
	{
		auto result{ this->FindStoredState(key) };
		if (result != this->key_state_container.end())
		{
			this->key_state_container.erase(key);
			return true;
		}
		return false;
	}
	StateContainer::const_iterator Controller::FindStoredState(VirtualKey::Type vk) const noexcept
	{
		try
		{
			auto result{ this->key_state_container.find(vk) };
			return result;
		}
		catch (const ::std::exception&) {}
		return this->key_state_container.end();
	}
	void Controller::QueryState() noexcept
	{
		for (auto& kv_pair : this->key_state_container)
		{
			kv_pair.second.last_state = kv_pair.second.state;
			kv_pair.second.state.Update(::GetAsyncKeyState(kv_pair.first));
		}
	}
	const StoredState& Controller::NullState() noexcept
	{
		static StoredState state{};
		return state;
	}
	void Controller::ExecuteEventProcess(Abstract::KeyboardEventProcess& proc, Resource& resource)
	{
		if (proc.Check(resource))
		{
			proc.Execution(resource);
		}
	}
}
