#pragma once

#ifndef Petal_Header_RawInputWindow
#define Petal_Header_RawInputWindow

#include "Petal~Window.h"

#include <span>
#include <hidusage.h>

namespace Petal
{
	using Win32RawInputDevice = ::RAWINPUTDEVICE;
	using Win32RawInput = ::RAWINPUT;
	using Win32HRawInput = ::HRAWINPUT;
	using Win32RawInputHeader = ::RAWINPUTHEADER;

	class RawInputWindow;

	struct RawInputRef final
	{
		ptr<Win32RawInput> raw_input{ nullptr };
		tsize valid_size{};
	};

	class RawInputDataBuffer final
	{
	public:
		// Call to get buffer for write only, and the buffer is uninitialized when size > buffer_size, invoke for ::GetRawInputData.
		::std::span<byte> WriteOnlyBuffer(tsize size);
		// meaningful only after ::GetRawInputData writes data to buffer, and before next calling WriteOnlyBuffer!
		::std::span<const byte> ReadOnlyBuffer() const noexcept;
		tsize BufferSize() const noexcept;
		RawInputRef AsRawInput() const noexcept;
		Win32RawInput& AsWin32RawInput() noexcept;
		const Win32RawInput& AsWin32RawInput() const noexcept;
		tsize RawInputSize() const noexcept;
	private:
		RawInputDataBuffer();
	public:
		RawInputDataBuffer(const RawInputDataBuffer&) = delete;
		RawInputDataBuffer& operator=(const RawInputDataBuffer&) = delete;
		~RawInputDataBuffer();
	public:
		static constexpr tsize min_size{ sizeof(Win32RawInput) };
	private:
		ptr<byte> buffer_ptr{ nullptr }; // pointer to raw memory
		tsize buffer_size{}; // size of raw memory
		tsize raw_input_size{}; // raw input size
		friend class RawInputWindow;
	};

	class RawInputMessage : public BasicWindowMessage
	{
	public:
		win32wprm RawInputCode() const noexcept;
		boolean IsSink() const noexcept;
		Win32HRawInput HRawInput() const noexcept;
		win32lres DefaultProcess(win32hwnd window_handle) const noexcept;
	public:
		RawInputMessage(win32msg msg, win32wprm w, win32lprm l);
		RawInputMessage(const RawInputMessage&) = default;
		RawInputMessage(RawInputMessage&&) noexcept = default;
		virtual ~RawInputMessage() = default;
	private:
		win32wprm raw_input_code{};
	};

	class RawInputDeviceChangeMessage : public BasicWindowMessage
	{
	public:
		Win32HRawInput RawInputHandle() const noexcept;
		win32wprm GIDC() const noexcept;
		boolean Arrival() const noexcept;
		boolean Removal() const noexcept;
	public:
		RawInputDeviceChangeMessage(win32msg msg, win32wprm w, win32lprm l);
		RawInputDeviceChangeMessage(const RawInputDeviceChangeMessage&) = default;
		RawInputDeviceChangeMessage(RawInputDeviceChangeMessage&&) noexcept = default;
		virtual ~RawInputDeviceChangeMessage() = default;
	};

	using RawMouseMessage = RawInputMessage;
	using RawKeyboardMessage = RawInputMessage;
	using RawHidMessage = RawInputMessage;

	class RawInputWindow : public Window
	{
	protected:
		virtual win32lres Process(win32msg msg, win32wprm w, win32lprm l) override;
		virtual void RawInputEvent(RawInputMessage& e);
		virtual void RawInputDeviceChangeEvent(RawInputDeviceChangeMessage& e);
		virtual void RawMouseEvent(RawMouseMessage& e, Win32RawInput& raw_input, tsize raw_input_size);
		virtual void RawKeyboardEvent(RawKeyboardMessage& e, Win32RawInput& raw_input, tsize raw_input_size);
		virtual void RawHidEvent(RawHidMessage& e, Win32RawInput& raw_input, tsize raw_input_size);
	public:
		static win32bool RegisterRawInputDevices(::std::span<const Win32RawInputDevice> devices);
	public:
		RawInputWindow() = default;
		RawInputWindow(const RawInputWindow&) = delete;
		RawInputWindow(RawInputWindow&&) noexcept = delete;
		virtual ~RawInputWindow() = default;
		RawInputWindow& operator= (const RawInputWindow&) = delete;
		RawInputWindow& operator= (RawInputWindow&&) = delete;
	private:
		RawInputDataBuffer raw_input_buffer{};
	};
}

#endif // !Petal_Header_RawInputWindow
