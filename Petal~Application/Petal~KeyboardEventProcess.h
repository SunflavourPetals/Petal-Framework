#pragma once

#ifndef Petal_Header_KeyboardEventProcess
#define Petal_Header_KeyboardEventProcess

#include "Petal~KeyboardController.h"

namespace Petal::Keyboard::MiddleProcess
{
	using Petal::boolean;
	class BasicMiddleProcess : public Abstract::KeyboardEventProcess
	{
	protected:
		virtual boolean KeyPositive(const StoredState& state) const = 0;
		virtual boolean LastPositive(const Controller& controller) const = 0;
		virtual boolean ThisPositive(const Controller& controller) const = 0;
	public:
		BasicMiddleProcess() = default;
		BasicMiddleProcess(const BasicMiddleProcess&) = default;
		BasicMiddleProcess(BasicMiddleProcess&&) noexcept = default;
		~BasicMiddleProcess() = default;
	};

	class XHoldProcess : virtual public BasicMiddleProcess
	{
	public:
		virtual boolean Check(const Resource&) override;
	public:
		virtual void UpdateTargetCount(i64 target_count) noexcept final; // hold target_count to triggering
		virtual i64 TargetCount() const noexcept final;
		virtual void UpdateLoopMode(boolean loop_mode) noexcept final; // loop triggering target_count to target_count
		virtual boolean LoopMode() const noexcept final;
	public:
		XHoldProcess(i64 target_count = 1, boolean loop_mode = false);
		XHoldProcess(const XHoldProcess&) = default;
		XHoldProcess(XHoldProcess&&) noexcept = default;
		~XHoldProcess() = default;
	private:
		i64 pt_target_count{ 1 };
		i64 pt_total_count{};
		boolean pt_loop_triggering{ false };
		boolean pt_in_holding{ false };
	};

	class XKeyProcess : virtual public BasicMiddleProcess
	{
	protected:
		virtual boolean KeyPositive(const StoredState& state) const override;
		virtual boolean LastPositive(const Controller& controller) const override;
		virtual boolean ThisPositive(const Controller& controller) const override;
	public:
		virtual void UpdateKey(VirtualKey::Type target_key) noexcept final;
		virtual VirtualKey::Type Key() const noexcept final;
		virtual boolean Check(const Resource&) = 0;
	public:
		XKeyProcess(VirtualKey::Type key = 0);
		XKeyProcess(const XKeyProcess&) = default;
		XKeyProcess(XKeyProcess&&) noexcept = default;
		~XKeyProcess() = default;
	protected:
		VirtualKey::Type pt_key{};
	};
}

namespace Petal::Keyboard
{
	class KeyPushProcess : public MiddleProcess::XKeyProcess
	{
	public:
		virtual boolean Check(const Resource&) override;
	public:
		KeyPushProcess(VirtualKey::Type target_key);
	};
	class KeyReleaseProcess : public MiddleProcess::XKeyProcess
	{
	public:
		virtual boolean Check(const Resource&) override;
	public:
		KeyReleaseProcess(VirtualKey::Type target_key);
	};
	class KeyPositiveProcess : public MiddleProcess::XKeyProcess
	{
	public:
		virtual boolean Check(const Resource&) override;
	public:
		KeyPositiveProcess(VirtualKey::Type target_key);
	};
	class KeyNegativeProcess : public MiddleProcess::XKeyProcess
	{
	public:
		virtual boolean Check(const Resource&) override;
	public:
		KeyNegativeProcess(VirtualKey::Type target_key);
	};
	class KeyHoldProcess : public MiddleProcess::XKeyProcess, public MiddleProcess::XHoldProcess
	{
	protected:
		virtual boolean KeyPositive(const StoredState& gamepad) const override final;
		virtual boolean LastPositive(const Controller& controller) const override final;
		virtual boolean ThisPositive(const Controller& controller) const override final;
	public:
		virtual boolean Check(const Resource&) override;
	public:
		KeyHoldProcess(VirtualKey::Type target_key, i64 target_count = 1, boolean loop_mode = false);
	};
}

#endif // !Petal_Header_KeyboardEventProcess
