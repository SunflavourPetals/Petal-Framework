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
		// 对 ReadOnlyBuffer 函数的调用只能在调用 WriteOnlyBuffer 函数之后！
		::std::span<const Petal::byte> ReadOnlyBuffer() const noexcept;
		// 对 AsRawInput 函数的调用只能在调用 WriteOnlyBuffer 函数之后！
		RawInputRef AsRawInput() const noexcept;
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
		Petal::boolean IsSink() const noexcept;
		Win32HRawInput HRawInput() const noexcept;
		const Win32RawInput& RawInput() const noexcept;
		tsize RawInputSize() const noexcept;
	public:
		RawInputMessage(win32msg msg, win32wprm w, win32lprm l, RawInputRef input);
		RawInputMessage(const RawInputMessage&) = default;
		RawInputMessage(RawInputMessage&&) noexcept = default;
		virtual ~RawInputMessage() = default;
	private:
		win32wprm pt_raw_input_code{};
		RawInputRef raw_input;
	};

	class RawInputDeviceChangeMessage : public BasicWindowMessage
	{
	public:
		Win32HRawInput RawInputHandle() const noexcept;
		win32wprm GIDC() const noexcept;
		Petal::boolean Arrival() const noexcept;
		Petal::boolean Removal() const noexcept;
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
		virtual void RawInputEvent(RawInputMessage& e) noexcept;
		virtual void RawMouseEvent(RawMouseMessage& e) noexcept;
		virtual void RawKeyboardEvent(RawKeyboardMessage& e) noexcept;
		virtual void RawHidEvent(RawHidMessage& e) noexcept;
		virtual void RawINputDeviceChangeEvent(RawInputDeviceChangeMessage& e) noexcept;
		virtual Petal::win32lres Process(Petal::win32msg msg, Petal::win32wprm w, Petal::win32lprm l) noexcept override;
	public:
		win32bool RegisterRawInputDevices(::std::span<Win32RawInputDevice> devices);
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
