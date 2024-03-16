#include "Petal~XInputController.h"

#pragma comment(lib, "Xinput.lib")

namespace Petal::XInput
{
	WrappedGamepad::WrappedGamepad(UserIndexValue::Type user_index)
	{
		this->UpdateUserIndex(user_index);
	}
	boolean WrappedGamepad::Connected() const noexcept
	{
		return this->gamepad_connected;
	}
	boolean WrappedGamepad::CheckConnection() const noexcept
	{
		XINPUT_STATE state{};
		win32dword result{ ::XInputGetState(this->user_index, &state) };
		this->gamepad_connected = result != ERROR_DEVICE_NOT_CONNECTED;
		return this->Connected();
	}
	UserIndexValue::Type WrappedGamepad::UserIndex() const noexcept
	{
		return this->user_index;
	}
	const State& WrappedGamepad::GetState() const noexcept
	{
		return this->gamepad_state;
	}
	const Gamepad& WrappedGamepad::GetGamepad() const noexcept
	{
		return this->gamepad_state.Gamepad;
	}
	boolean WrappedGamepad::StateChanged() const noexcept
	{
		return this->state_changed;
	}
	void WrappedGamepad::ClearState() noexcept
	{
		this->gamepad_state = {};
	}
	boolean WrappedGamepad::Pushed(Button::Type buttons) const noexcept
	{
		return (this->gamepad_state.Gamepad.wButtons & buttons) == buttons;
	}
	TriggerValue::Type WrappedGamepad::LeftTrigger() const noexcept
	{
		return this->gamepad_state.Gamepad.bLeftTrigger;
	}
	TriggerValue::Type WrappedGamepad::RightTrigger() const noexcept
	{
		return this->gamepad_state.Gamepad.bRightTrigger;
	}
	StickValue::Type WrappedGamepad::LeftStickX() const noexcept
	{
		return this->gamepad_state.Gamepad.sThumbLX;
	}
	StickValue::Type WrappedGamepad::LeftStickY() const noexcept
	{
		return this->gamepad_state.Gamepad.sThumbLY;
	}
	StickValue::Type WrappedGamepad::RightStickX() const noexcept
	{
		return this->gamepad_state.Gamepad.sThumbRX;
	}
	StickValue::Type WrappedGamepad::RightStickY() const noexcept
	{
		return this->gamepad_state.Gamepad.sThumbRY;
	}
	StickValue::Type WrappedGamepad::CalcLStickUp() const noexcept
	{
		return this->LeftStickY() > 0 ? this->LeftStickY() : 0;
	}
	StickValue::Type WrappedGamepad::CalcLStickDown() const noexcept
	{
		return this->LeftStickY() < -1 ? -(this->LeftStickY() + 1) : 0;
	}
	StickValue::Type WrappedGamepad::CalcLStickLeft() const noexcept
	{
		return this->LeftStickX() < -1 ? -(this->LeftStickX() + 1) : 0;
	}
	StickValue::Type WrappedGamepad::CalcLStickRight() const noexcept
	{
		return this->LeftStickX() > 0 ? this->LeftStickX() : 0;
	}
	StickValue::Type WrappedGamepad::CalcRStickUp() const noexcept
	{
		return this->RightStickY() > 0 ? this->RightStickY() : 0;
	}
	StickValue::Type WrappedGamepad::CalcRStickDown() const noexcept
	{
		return this->RightStickY() < -1 ? -(this->RightStickY() + 1) : 0;
	}
	StickValue::Type WrappedGamepad::CalcRStickLeft() const noexcept
	{
		return this->RightStickX() < -1 ? -(this->RightStickX() + 1) : 0;
	}
	StickValue::Type WrappedGamepad::CalcRStickRight() const noexcept
	{
		return this->RightStickX() > 0 ? this->RightStickX() : 0;
	}
	boolean WrappedGamepad::UpdateUserIndex(UserIndexValue::Type user_index) noexcept
	{
		if (user_index <= 3 && user_index >= 0)
		{
			this->user_index = user_index;
			this->gamepad_connected = false;
			this->state_changed = false;
			this->ClearState();
			return true;
		}
		return false;
	}
	win32dword WrappedGamepad::QueryState() noexcept
	{
		win32dword last_state = this->gamepad_state.dwPacketNumber;
		win32dword result = ::XInputGetState(this->user_index, &this->gamepad_state);
		if (result == ERROR_DEVICE_NOT_CONNECTED)
		{
			this->gamepad_connected = false;
			this->state_changed = false;
		}
		else
		{
			if ((this->gamepad_connected == true) && (this->gamepad_state.dwPacketNumber != last_state))
			{
				this->state_changed = true;
			}
			else
			{
				this->state_changed = false;
			}
			this->gamepad_connected = true;
		}
		return result;
	}
	win32dword WrappedGamepad::Vibration(VibrationValue::Type left_motor_speed, VibrationValue::Type right_motor_speed) const noexcept
	{
		::XINPUT_VIBRATION vibration{ left_motor_speed, right_motor_speed };
		return ::XInputSetState(this->user_index, &vibration);
	}
	win32dword WrappedGamepad::AudioDeviceIDs(ptr<WChar> render_device_id, win32uint& render_count, ptr<WChar> capture_device_id, win32uint& capture_count) const noexcept
	{
		return ::XInputGetAudioDeviceIds(this->user_index, render_device_id, &render_count, capture_device_id, &capture_count);
	}
	win32dword WrappedGamepad::GetBatteryInformation(BatteryInformation& battery_info) const noexcept
	{
		return ::XInputGetBatteryInformation(this->user_index, BATTERY_DEVTYPE_GAMEPAD, &battery_info);
	}
	win32dword WrappedGamepad::GetCapabilities(Capabilities& capabilities) const noexcept
	{
		return ::XInputGetCapabilities(this->user_index, XINPUT_FLAG_GAMEPAD, &capabilities);
	}
	win32dword WrappedGamepad::GetKeystroke(Keystroke& keystroke) const noexcept
	{
		return ::XInputGetKeystroke(this->user_index, 0, &keystroke);
	}
	Controller::Controller(UserIndexValue::Type user_index) :
		gamepad{ user_index },
		last_gamepad{ user_index }
	{

	}
	void Controller::ClearState() noexcept
	{
		this->gamepad.ClearState();
	}
	void Controller::ClearLastState() noexcept
	{
		this->last_gamepad.ClearState();
	}
	boolean Controller::UpdateUserIndex(UserIndexValue::Type user_index) noexcept
	{
		return this->gamepad.UpdateUserIndex(user_index);
	}
	const WrappedGamepad& Controller::GetWrappedGamepad() const noexcept
	{
		return this->gamepad;
	}
	const WrappedGamepad& Controller::GetLastWrappedGamepad() const noexcept
	{
		return this->last_gamepad;
	}
	win32dword Controller::QueryState() noexcept
	{
		this->last_gamepad = this->gamepad;
		return this->gamepad.QueryState();
	}
	void Controller::ExecuteEventProcess(Abstract::XInputEventProcess& proc, Resource& resource)
	{
		if (proc.Check(resource))
		{
			proc.Execution(resource);
		}
	}
}
