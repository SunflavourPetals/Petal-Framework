#pragma once

#ifndef Petal_Header_KeyboardEventProcess
#define Petal_Header_KeyboardEventProcess

#include "Petal~KeyboardController.h"
#include "Petal~XEventProcessComponents.h"

namespace Petal::Keyboard::MiddleProcess
{
	using Abstract::Keyboard::Controller;

	class BasicProcess : public Abstract::KeyboardEventProcess
	{
	public:
		virtual boolean LastPositive(const Controller& controller) const = 0;
		virtual boolean ThisPositive(const Controller& controller) const = 0;
	public:
		BasicProcess() = default;
		~BasicProcess() = default;
	};

	class XKeyProcess : virtual public BasicProcess
	{
	public:
		boolean LastPositive(const Controller& controller) const override
		{
			return KeyPositive(controller.GetLastState());
		}
		boolean ThisPositive(const Controller& controller) const override
		{
			return KeyPositive(controller.GetState());
		}
	private:
		boolean KeyPositive(const WrappedState& state) const { return state.Pushed(this->key); }
	public:
		XKeyProcess(VirtualKey::Type target_key = 0) :
			BasicProcess(),
			key{ target_key } {}
		~XKeyProcess() = default;
	public:
		VirtualKey::Type key{};
	};
}

namespace Petal::Keyboard
{
	class KeyPushProcess : public MiddleProcess::XKeyProcess
	{
	public:
		boolean Check(const Resource& resource) override
		{
			return (!LastPositive(resource.controller) && this->ThisPositive(resource.controller));
		}
	public:
		KeyPushProcess(VirtualKey::Type target_key) :
			XKeyProcess(target_key) {}
	};
	class KeyReleaseProcess : public MiddleProcess::XKeyProcess
	{
	public:
		boolean Check(const Resource& resource) override
		{
			return (LastPositive(resource.controller) && !this->ThisPositive(resource.controller));
		}
	public:
		KeyReleaseProcess(VirtualKey::Type target_key) :
			XKeyProcess(target_key) {}
	};
	class KeyPositiveProcess : public MiddleProcess::XKeyProcess
	{
	public:
		boolean Check(const Resource& resource) override { return ThisPositive(resource.controller); }
	public:
		KeyPositiveProcess(VirtualKey::Type target_key) :
			XKeyProcess(target_key) {}
	};
	class KeyNegativeProcess : public MiddleProcess::XKeyProcess
	{
	public:
		boolean Check(const Resource& resource) override { return !ThisPositive(resource.controller); }
	public:
		KeyNegativeProcess(VirtualKey::Type target_key) :
			XKeyProcess(target_key) {}
	};
	class KeyHoldProcess : public MiddleProcess::XKeyProcess
	{
	public:
		boolean Check(const Resource& resource) override
		{ return hold_config.Check(resource, *this); }
	public:
		KeyHoldProcess(
			VirtualKey::Type target_key,
			XEventProcessComponents::Hold::Tick target_count = 1,
			boolean loop_mode = false) :
			XKeyProcess(target_key),
			hold_config(target_count, loop_mode) {}
	public:
		XEventProcessComponents::Hold hold_config{};
	};
}

#endif // !Petal_Header_KeyboardEventProcess
