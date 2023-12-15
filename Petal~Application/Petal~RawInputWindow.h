#pragma once

#ifndef Petal_Header_RawInputWindow
#define Petal_Header_RawInputWindow

#include "Petal~Window.h"
#include "Petal~VSDebugOutput.h"

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
		ptr<Win32RawInput> raw_input;
		tsize valid_size;
	};

	class RawInputDataBuffer final
	{
	private:
		void Alloc(tsize size);
	public:
		::std::span<Petal::byte> WriteOnlyBuffer(tsize size);
		// 对此函数的调用只能在调用 WriteOnlyBuffer 函数之后！
		::std::span<const Petal::byte> ReadOnlyBuffer() const noexcept;
		tsize BufferSize() const noexcept;
		// 对此函数的调用只能在调用 WriteOnlyBuffer 函数之后！
		RawInputRef AsRawInput() const noexcept;
		// 对此函数的调用只能在调用 WriteOnlyBuffer 函数之后！
		Win32RawInput& AsWin32RawInput() noexcept;
		// 对此函数的调用只能在调用 WriteOnlyBuffer 函数之后！
		const Win32RawInput& AsWin32RawInput() const noexcept;
		// 对此函数的调用只能在调用 WriteOnlyBuffer 函数之后！
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
		ptr<Petal::byte> buffer_ptr{};
		tsize buffer_size{};
		ptr<Petal::byte> raw_input_buffer{};
		tsize raw_input_size{};
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
		win32wprm pt_raw_input_code{};
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
		virtual void RawINputDeviceChangeEvent(RawInputDeviceChangeMessage& e) noexcept;
		virtual void RawMouseEvent(RawMouseMessage& e, Win32RawInput& raw_input, tsize raw_input_size) noexcept;
		virtual void RawKeyboardEvent(RawKeyboardMessage& e, Win32RawInput& raw_input, tsize raw_input_size) noexcept;
		virtual void RawHidEvent(RawHidMessage& e, Win32RawInput& raw_input, tsize raw_input_size) noexcept;
	public:
		static win32bool RegisterRawInputDevices(::std::span<Win32RawInputDevice> devices);
	public:
		RawInputWindow() = default;
		RawInputWindow(const RawInputWindow&) = delete;
		RawInputWindow(RawInputWindow&&) noexcept = delete;
		virtual ~RawInputWindow() = default;
		RawInputWindow& operator= (const RawInputWindow&) = delete;
		RawInputWindow& operator= (RawInputWindow&&) = delete;
	private:
		RawInputDataBuffer pt_raw_input_buffer;
	};
}

#endif // !Petal_Header_RawInputWindow
