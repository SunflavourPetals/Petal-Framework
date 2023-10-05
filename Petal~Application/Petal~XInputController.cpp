#include "Petal~XInputController.h"

#pragma comment(lib, "Xinput.lib")

namespace Petal::XInput
{
	using Petal::boolean;
	WrappedGamepad::WrappedGamepad(UserIndexValue::Type user_index) :
		WrappedGamepad()
	{
		this->UpdateUserIndex(user_index);
	}
	boolean WrappedGamepad::Connected() const noexcept
	{
		return this->pt_gamepad_connected;
	}
	boolean WrappedGamepad::CheckConnection() noexcept
	{
		this->pt_gamepad_connected = const_cast<ptrc<WrappedGamepad>>(this)->CheckConnection();
		return this->Connected();
	}
	boolean WrappedGamepad::CheckConnection() const noexcept
	{
		XINPUT_STATE state{};
		win32dword result{ ::XInputGetState(this->pt_user_index, &state) };
		return result != ERROR_DEVICE_NOT_CONNECTED;
	}
	UserIndexValue::Type WrappedGamepad::UserIndex() const noexcept
	{
		return this->pt_user_index;
	}
	const State& WrappedGamepad::GetState() const noexcept
	{
		return this->pt_gamepad_state;
	}
	const Gamepad& WrappedGamepad::GetGamepad() const noexcept
	{
		return this->pt_gamepad_state.Gamepad;
	}
	boolean WrappedGamepad::StateChanged() const noexcept
	{
		return this->pt_state_changed;
	}
	void WrappedGamepad::ClearState() noexcept
	{
		this->pt_gamepad_state = {};
	}
	boolean WrappedGamepad::Pushed(Button::Type buttons) const noexcept
	{
		return (this->pt_gamepad_state.Gamepad.wButtons & buttons) == buttons;
	}
	TriggerValue::Type WrappedGamepad::LeftTrigger() const noexcept
	{
		return this->pt_gamepad_state.Gamepad.bLeftTrigger;
	}
	TriggerValue::Type WrappedGamepad::RightTrigger() const noexcept
	{
		return this->pt_gamepad_state.Gamepad.bRightTrigger;
	}
	StickValue::Type WrappedGamepad::LeftStickX() const noexcept
	{
		return this->pt_gamepad_state.Gamepad.sThumbLX;
	}
	StickValue::Type WrappedGamepad::LeftStickY() const noexcept
	{
		return this->pt_gamepad_state.Gamepad.sThumbLY;
	}
	StickValue::Type WrappedGamepad::RightStickX() const noexcept
	{
		return this->pt_gamepad_state.Gamepad.sThumbRX;
	}
	StickValue::Type WrappedGamepad::RightStickY() const noexcept
	{
		return this->pt_gamepad_state.Gamepad.sThumbRY;
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
			this->pt_user_index = user_index;
			this->pt_gamepad_connected = false;
			this->pt_state_changed = false;
			this->ClearState();
			return true;
		}
		return false;
	}
	win32dword WrappedGamepad::QueryState() noexcept
	{
		win32dword last_state = this->pt_gamepad_state.dwPacketNumber;
		win32dword result = ::XInputGetState(this->pt_user_index, &this->pt_gamepad_state);
		if (result == ERROR_DEVICE_NOT_CONNECTED)
		{
			pt_gamepad_connected = false;
			pt_state_changed = false;
		}
		else
		{
			if ((this->pt_gamepad_connected == true) && (this->pt_gamepad_state.dwPacketNumber != last_state))
			{
				this->pt_state_changed = true;
			}
			else
			{
				this->pt_state_changed = false;
			}
			this->pt_gamepad_connected = true;
		}
		return result;
	}
	win32dword WrappedGamepad::Vibration(VibrationValue::Type left_motor_speed, VibrationValue::Type right_motor_speed) const noexcept
	{
		::XINPUT_VIBRATION vibration{ left_motor_speed, right_motor_speed };
		return ::XInputSetState(this->pt_user_index, &vibration);
	}
	win32dword WrappedGamepad::AudioDeviceIDs(ptr<WChar> render_device_id, win32uint& render_count, ptr<WChar> capture_device_id, win32uint& capture_count) const noexcept
	{
		return ::XInputGetAudioDeviceIds(this->pt_user_index, render_device_id, &render_count, capture_device_id, &capture_count);
	}
	win32dword WrappedGamepad::GetBatteryInformation(BatteryInformation& battery_info) const noexcept
	{
		return ::XInputGetBatteryInformation(this->pt_user_index, BATTERY_DEVTYPE_GAMEPAD, &battery_info);
	}
	win32dword WrappedGamepad::GetCapabilities(Capabilities& capabilities) const noexcept
	{
		return ::XInputGetCapabilities(this->pt_user_index, XINPUT_FLAG_GAMEPAD, &capabilities);
	}
	win32dword WrappedGamepad::GetKeystroke(Keystroke& keystroke) const noexcept
	{
		return ::XInputGetKeystroke(this->pt_user_index, 0, &keystroke);
	}
	Controller::Controller(UserIndexValue::Type user_index) :
		pt_gamepad(user_index),
		pt_last_gamepad()
	{

	}
	void Controller::ClearState() noexcept
	{
		this->pt_gamepad.ClearState();
	}
	void Controller::ClearLastState() noexcept
	{
		this->pt_last_gamepad.ClearState();
	}
	boolean Controller::UpdateUserIndex(UserIndexValue::Type user_index) noexcept // 指定新的用户索引
	{
		return this->pt_gamepad.UpdateUserIndex(user_index);
	}
	const WrappedGamepad& Controller::GetWrappedGamepad() const noexcept
	{
		return this->pt_gamepad;
	}
	const WrappedGamepad& Controller::GetLastWrappedGamepad() const noexcept
	{
		return this->pt_last_gamepad;
	}
	const PerformanceCounter& Controller::GetCounter() const noexcept
	{
		return this->pt_counter;
	}
	win32dword Controller::QueryState() noexcept // 查询控制器输入状态
	{
		this->pt_last_gamepad = this->pt_gamepad;
		return this->pt_gamepad.QueryState();
	}
}
