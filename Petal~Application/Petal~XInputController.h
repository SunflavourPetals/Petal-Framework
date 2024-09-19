#pragma once

#ifndef Petal_Header_XInputController
#define Petal_Header_XInputController

#include "Petal~BasicTypes.h"
#include "Petal~WinTypes.h"
#include "Petal~String.h"
#include "Petal~Process.h"
#include "Petal~PointerTraits.h"
#include "Petal~PerformanceCounter.h"

#include <Xinput.h>

namespace Petal::XInput
{
	using Tick = typename PerformanceCounter::Tick;
	class WrappedGamepad;
	class Controller;
	struct ResourceOfController
	{
		using ControllerType = Controller;
		using DeltaCountType = Tick;
		const DeltaCountType delta_count{};
		const DeltaCountType frequency{};
		const ControllerType& controller;
	};
}

namespace Petal::Abstract
{
	class XInputEventProcess : public Process<Petal::XInput::ResourceOfController>
	{
	public:
		using Resource = Petal::XInput::ResourceOfController;
	public:
		virtual boolean Check(const Resource&) = 0;
	public:
		XInputEventProcess() = default;
		virtual ~XInputEventProcess() = default;
	};
}

namespace Petal::Concept
{
	template <typename Ty>
	concept GenericXInputEventProcessIterator = requires
	{
		typename ::std::iterator_traits<Ty>::value_type;
		requires ::std::is_base_of_v<Abstract::XInputEventProcess, typename TypeTraits::RemoveAllAnyPointerType<typename Ty::value_type>>;
		::std::declval<typename TypeTraits::RemoveAllAnyPointerType<typename Ty::value_type>>().Execution(::std::declval<const Abstract::XInputEventProcess::Resource&>());
	};

	template <typename Ty>
	concept GenericXInputEventProcessPointer = TypeTraits::is_any_pointer<Ty> && ::std::is_base_of_v<Abstract::XInputEventProcess, typename TypeTraits::RemoveAllAnyPointerType<Ty>>;
}

namespace Petal::XInput
{
	using State = typename ::XINPUT_STATE;
	using Gamepad = typename ::XINPUT_GAMEPAD;
	using BatteryInformation = typename ::XINPUT_BATTERY_INFORMATION;
	using Capabilities = typename ::XINPUT_CAPABILITIES;
	using Keystroke = typename ::XINPUT_KEYSTROKE;
	struct Button
	{
		using Type = win32word;
		static constexpr Type Up{ XINPUT_GAMEPAD_DPAD_UP };
		static constexpr Type Down{ XINPUT_GAMEPAD_DPAD_DOWN };
		static constexpr Type Left{ XINPUT_GAMEPAD_DPAD_LEFT };
		static constexpr Type Right{ XINPUT_GAMEPAD_DPAD_RIGHT };
		static constexpr Type Start{ XINPUT_GAMEPAD_START };
		static constexpr Type Back{ XINPUT_GAMEPAD_BACK };
		static constexpr Type LeftStick{ XINPUT_GAMEPAD_LEFT_THUMB };
		static constexpr Type RightStick{ XINPUT_GAMEPAD_RIGHT_THUMB };
		static constexpr Type LeftShoulder{ XINPUT_GAMEPAD_LEFT_SHOULDER };
		static constexpr Type RightShoulder{ XINPUT_GAMEPAD_RIGHT_SHOULDER };
		static constexpr Type A{ XINPUT_GAMEPAD_A };
		static constexpr Type B{ XINPUT_GAMEPAD_B };
		static constexpr Type X{ XINPUT_GAMEPAD_X };
		static constexpr Type Y{ XINPUT_GAMEPAD_Y };
	};
	struct StickValue
	{
		using Type = win32short;
		static constexpr Type full{ +0x7fff };
		static constexpr Type half{ +0x3fff };
		static constexpr Type full_up{ +0x7fff };
		static constexpr Type full_right{ +0x7fff };
		static constexpr Type full_down{ -0x8000 };
		static constexpr Type full_left{ -0x8000 };
		static constexpr Type half_up{ +0x3fff };
		static constexpr Type half_right{ +0x3fff };
		static constexpr Type half_down{ -0x4000 };
		static constexpr Type half_left{ -0x4000 };
		static constexpr Type left_dead_zone{ XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE };
		static constexpr Type right_dead_zone{ XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE };
		static constexpr Type threshold{ static_cast<Type>(0.265625 * full) };
	};
	struct TriggerValue
	{
		using Type = win32byte;
		static constexpr Type full{ 0xffu };
		static constexpr Type half{ 0x7fu };
		static constexpr Type threshold{ XINPUT_GAMEPAD_TRIGGER_THRESHOLD };
	};
	struct VibrationValue
	{
		using Type = win32word;
		static constexpr Type full{ 0xffffu };
		static constexpr Type half{ 0x7fffu };
	};
	struct UserIndexValue
	{
		using Type = win32dword;
		static constexpr Type player1{ 0 };
		static constexpr Type player2{ 1 };
		static constexpr Type player3{ 2 };
		static constexpr Type player4{ 3 };
	};

	class WrappedGamepad final
	{
	public:
		boolean Connected() const noexcept { return gamepad_connected; }
		boolean CheckConnection() const noexcept; // Check connection, and no calling function QueryState before required.
		UserIndexValue::Type UserIndex() const noexcept { return user_index; }
		const State& GetState() const noexcept { return gamepad_state; }
		const Gamepad& GetGamepad() const noexcept { return gamepad_state.Gamepad; }
		boolean StateChanged() const noexcept { return state_changed; }
		void ClearState() noexcept { gamepad_state = {}; }
		boolean Pushed(Button::Type buttons) const noexcept { return (gamepad_state.Gamepad.wButtons & buttons) == buttons; }
		TriggerValue::Type LeftTrigger() const noexcept { return gamepad_state.Gamepad.bLeftTrigger; }
		TriggerValue::Type RightTrigger() const noexcept { return gamepad_state.Gamepad.bRightTrigger; }
		StickValue::Type LeftStickX() const noexcept { return gamepad_state.Gamepad.sThumbLX; }
		StickValue::Type LeftStickY() const noexcept { return gamepad_state.Gamepad.sThumbLY; }
		StickValue::Type RightStickX() const noexcept { return gamepad_state.Gamepad.sThumbRX; }
		StickValue::Type RightStickY() const noexcept { return gamepad_state.Gamepad.sThumbRY; }
		StickValue::Type CalcLStickUp() const noexcept { return LeftStickY() > 0 ? LeftStickY() : 0; }
		StickValue::Type CalcLStickDown() const noexcept { return LeftStickY() < -1 ? -(LeftStickY() + 1) : 0; }
		StickValue::Type CalcLStickLeft() const noexcept { return LeftStickX() < -1 ? -(LeftStickX() + 1) : 0; }
		StickValue::Type CalcLStickRight() const noexcept { return LeftStickX() > 0 ? LeftStickX() : 0; }
		StickValue::Type CalcRStickUp() const noexcept { return RightStickY() > 0 ? RightStickY() : 0; }
		StickValue::Type CalcRStickDown() const noexcept { return RightStickY() < -1 ? -(RightStickY() + 1) : 0; }
		StickValue::Type CalcRStickLeft() const noexcept { return RightStickX() < -1 ? -(RightStickX() + 1) : 0; }
		StickValue::Type CalcRStickRight() const noexcept { return RightStickX() > 0 ? RightStickX() : 0; }
		boolean UpdateUserIndex(UserIndexValue::Type user_index) noexcept;
		win32dword QueryState() noexcept;
		win32dword Vibration(VibrationValue::Type left_motor_speed, VibrationValue::Type right_motor_speed) const noexcept;	// Set vibration, arguments in range [0, 65535] integer
		win32dword AudioDeviceIDs(ptr<WChar> render_device_id, win32uint& render_count, ptr<WChar> capture_device_id, win32uint& capture_count) const noexcept;
		win32dword GetBatteryInformation(BatteryInformation& battery_info) const noexcept;
		win32dword GetCapabilities(Capabilities& capabilities) const noexcept;
		win32dword GetKeystroke(Keystroke& keystroke) const noexcept;
	public:
		WrappedGamepad(UserIndexValue::Type user_index = 0) { this->UpdateUserIndex(user_index); }
		~WrappedGamepad() = default;
	private:
		State gamepad_state{}; // XINPUT_STATE, updates after QueryState is called
		UserIndexValue::Type user_index{ 0 }; // User index, 0 to 3
		mutable boolean gamepad_connected{}; // Connection state, updates after QueryState is called
		boolean state_changed{}; // is state changing, updates after QueryState is called
	};

	class Controller final
	{
	public:
		using Resource = ResourceOfController;
	public:
		void ClearState() noexcept { gamepad.ClearState(); }
		void ClearLastState() noexcept { last_gamepad.ClearState(); }
		boolean UpdateUserIndex(UserIndexValue::Type user_index) noexcept { return gamepad.UpdateUserIndex(user_index); }
		win32dword Update(Concept::GenericXInputEventProcessIterator auto begin, Concept::GenericXInputEventProcessIterator auto end, Tick delta_count = 0, Tick frequency = 1);
		const WrappedGamepad& GetWrappedGamepad() const noexcept { return gamepad; }
		const WrappedGamepad& GetLastWrappedGamepad() const noexcept { return last_gamepad; }
	private:
		win32dword QueryState() noexcept;
		static void ExecuteEventProcess(Concept::GenericXInputEventProcessPointer auto& pointer, Resource& resource);
		static void ExecuteEventProcess(Abstract::XInputEventProcess& proc, Resource& resource);
	public:
		Controller() = default;
		Controller(UserIndexValue::Type user_index) :
			gamepad{ user_index },
			last_gamepad{ user_index } {}
		~Controller() = default;
	private:
		WrappedGamepad gamepad;
		WrappedGamepad last_gamepad;
	};
}

// Implementation

#pragma comment(lib, "Xinput.lib")

namespace Petal::XInput
{
	inline boolean WrappedGamepad::UpdateUserIndex(UserIndexValue::Type new_user_index) noexcept
	{
		if (new_user_index <= 3 && new_user_index >= 0)
		{
			user_index = new_user_index;
			gamepad_connected = false;
			state_changed = false;
			ClearState();
			return true;
		}
		return false;
	}
	inline boolean WrappedGamepad::CheckConnection() const noexcept
	{
		XINPUT_STATE state{};
		win32dword result{ ::XInputGetState(user_index, &state) };
		gamepad_connected = result != ERROR_DEVICE_NOT_CONNECTED;
		return Connected();
	}
	inline win32dword WrappedGamepad::QueryState() noexcept
	{
		win32dword last_state = gamepad_state.dwPacketNumber;
		win32dword result = ::XInputGetState(user_index, &gamepad_state);
		if (result == ERROR_DEVICE_NOT_CONNECTED)
		{
			gamepad_connected = false;
			state_changed = false;
		}
		else
		{
			if ((gamepad_connected == true) && (gamepad_state.dwPacketNumber != last_state))
			{
				state_changed = true;
			}
			else
			{
				state_changed = false;
			}
			gamepad_connected = true;
		}
		return result;
	}
	inline win32dword WrappedGamepad::Vibration(VibrationValue::Type left_motor_speed, VibrationValue::Type right_motor_speed) const noexcept
	{
		::XINPUT_VIBRATION vibration{ left_motor_speed, right_motor_speed };
		return ::XInputSetState(user_index, &vibration);
	}
	inline win32dword WrappedGamepad::AudioDeviceIDs(ptr<WChar> render_device_id, win32uint& render_count, ptr<WChar> capture_device_id, win32uint& capture_count) const noexcept
	{
		return ::XInputGetAudioDeviceIds(user_index, render_device_id, &render_count, capture_device_id, &capture_count);
	}
	inline win32dword WrappedGamepad::GetBatteryInformation(BatteryInformation& battery_info) const noexcept
	{
		return ::XInputGetBatteryInformation(user_index, BATTERY_DEVTYPE_GAMEPAD, &battery_info);
	}
	inline win32dword WrappedGamepad::GetCapabilities(Capabilities& capabilities) const noexcept
	{
		return ::XInputGetCapabilities(user_index, XINPUT_FLAG_GAMEPAD, &capabilities);
	}
	inline win32dword WrappedGamepad::GetKeystroke(Keystroke& keystroke) const noexcept
	{
		return ::XInputGetKeystroke(user_index, 0, &keystroke);
	}
	
	inline win32dword Controller::QueryState() noexcept
	{
		last_gamepad = gamepad;
		return gamepad.QueryState();
	}
	inline win32dword Controller::Update(Concept::GenericXInputEventProcessIterator auto begin, Concept::GenericXInputEventProcessIterator auto end, Tick delta_count, Tick frequency)
	{
		auto result{ QueryState() };
		Resource resource{ delta_count, frequency, *this };
		for (; begin != end; ++begin)
		{
			ExecuteEventProcess(*begin, resource);
		}
		return result;
	}
	inline void Controller::ExecuteEventProcess(Abstract::XInputEventProcess& proc, Resource& resource)
	{
		if (proc.Check(resource))
		{
			proc.Execution(resource);
		}
	}
	inline void Controller::ExecuteEventProcess(Concept::GenericXInputEventProcessPointer auto& pointer, Resource& resource)
	{
		if (pointer != nullptr)
		{
			Controller::ExecuteEventProcess(*pointer, resource);
		}
	}
}

#endif // !Petal_Header_XInputController
