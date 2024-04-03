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
	protected:
		virtual boolean LastPositive(const Controller& controller) const = 0;
		virtual boolean ThisPositive(const Controller& controller) const = 0;
	public:
		BasicProcess() = default;
		~BasicProcess() = default;
	};

	class XKeyProcess : virtual public BasicProcess
	{
	protected:
		virtual boolean LastPositive(const Controller& controller) const override;
		virtual boolean ThisPositive(const Controller& controller) const override;
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
		virtual boolean Check(const Resource&) override;
	public:
		KeyPushProcess(VirtualKey::Type target_key) :
			XKeyProcess(target_key) {}
	};
	class KeyReleaseProcess : public MiddleProcess::XKeyProcess
	{
	public:
		virtual boolean Check(const Resource&) override;
	public:
		KeyReleaseProcess(VirtualKey::Type target_key) :
			XKeyProcess(target_key) {}
	};
	class KeyPositiveProcess : public MiddleProcess::XKeyProcess
	{
	public:
		virtual boolean Check(const Resource&) override;
	public:
		KeyPositiveProcess(VirtualKey::Type target_key) :
			XKeyProcess(target_key) {}
	};
	class KeyNegativeProcess : public MiddleProcess::XKeyProcess
	{
	public:
		virtual boolean Check(const Resource&) override;
	public:
		KeyNegativeProcess(VirtualKey::Type target_key) :
			XKeyProcess(target_key) {}
	};
	class KeyHoldProcess : public MiddleProcess::XKeyProcess
	{
	public:
		virtual boolean Check(const Resource&) override;
	public:
		KeyHoldProcess(
			VirtualKey::Type target_key,
			XEventProcess::Component::Hold::Tick target_count = 1,
			boolean loop_mode = false) :
			XKeyProcess(target_key),
			hold_config(target_count, loop_mode) {}
	public:
		XEventProcess::Component::Hold hold_config{};
		friend class XEventProcess::Component::Hold;
	};
}

#endif // !Petal_Header_KeyboardEventProcess
