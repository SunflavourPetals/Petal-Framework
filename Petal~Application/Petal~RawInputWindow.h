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
	struct RawInputRef final
	{
		ptr<Win32RawInput> raw_input{ nullptr };
		tsize valid_size{};
	};

	class RawInputDataBuffer final
	{
	private:
		void Alloc(tsize size); // Alloc when bigger buffer is required
	public:
		// Call to get buffer for write only.
		::std::span<Petal::byte> WriteOnlyBuffer(tsize size);
		// Invocable only after function WriteOnlyBuffer called!
		::std::span<const Petal::byte> ReadOnlyBuffer() const noexcept;
		tsize BufferSize() const noexcept;
		// Invocable only after function WriteOnlyBuffer called!
		RawInputRef AsRawInput() const noexcept;
		// Invocable only after function WriteOnlyBuffer called!
		Win32RawInput& AsWin32RawInput() noexcept;
		// Invocable only after function WriteOnlyBuffer called!
		const Win32RawInput& AsWin32RawInput() const noexcept;
		// Invocable only after function WriteOnlyBuffer called!
		tsize RawInputSize() const noexcept;
	public:
		RawInputDataBuffer();
		RawInputDataBuffer(const RawInputDataBuffer&) = delete;
		RawInputDataBuffer(RawInputDataBuffer&&) noexcept = delete;
		RawInputDataBuffer& operator=(const RawInputDataBuffer&) = delete;
		~RawInputDataBuffer();
	public:
		static constexpr tsize min_size{ sizeof(Win32RawInput) };
	private:
		ptr<Petal::byte> buffer_ptr{ nullptr }; // pointer to raw memory
		tsize buffer_size{}; // size of raw memory
		ptr<Petal::byte> raw_input_buffer{ nullptr }; // raw input buffer
		tsize raw_input_size{}; // raw input size
		::std::allocator<Petal::byte> allocator{};
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
		virtual Petal::win32lres Process(Petal::win32msg msg, Petal::win32wprm w, Petal::win32lprm l) noexcept override;
		virtual void RawInputEvent(RawInputMessage& e) noexcept;
		virtual void RawInputDeviceChangeEvent(RawInputDeviceChangeMessage& e) noexcept;
		virtual void RawMouseEvent(RawMouseMessage& e, Win32RawInput& raw_input, tsize raw_input_size) noexcept;
		virtual void RawKeyboardEvent(RawKeyboardMessage& e, Win32RawInput& raw_input, tsize raw_input_size) noexcept;
		virtual void RawHidEvent(RawHidMessage& e, Win32RawInput& raw_input, tsize raw_input_size) noexcept;
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
