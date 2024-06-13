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
		win32wprm RawInputCode() const noexcept { return raw_input_code; }
		boolean IsSink() const noexcept { return RawInputCode() == RIM_INPUTSINK; }
		Win32HRawInput HRawInput() const noexcept { return reinterpret_cast<Win32HRawInput>(Long()); }
		win32lres DefaultProcess(win32hwnd window_handle) const noexcept
		{
			return IWindow::SystemDefWndProc(window_handle, Message(), Word(), Long());
		}
	public:
		RawInputMessage(win32msg msg, win32wprm w, win32lprm l) :
			BasicWindowMessage(msg, w, l),
			raw_input_code{ GET_RAWINPUT_CODE_WPARAM(w) } {}
		RawInputMessage(const RawInputMessage&) = default;
		RawInputMessage(RawInputMessage&&) noexcept = default;
		virtual ~RawInputMessage() = default;
	private:
		win32wprm raw_input_code{};
	};

	class RawInputDeviceChangeMessage : public BasicWindowMessage
	{
	public:
		Win32HRawInput RawInputHandle() const noexcept { return reinterpret_cast<Win32HRawInput>(Long()); }
		win32wprm GIDC() const noexcept { return Word(); }
		boolean Arrival() const noexcept { return GIDC() == GIDC_ARRIVAL; }
		boolean Removal() const noexcept { return GIDC() == GIDC_REMOVAL; }
	public:
		RawInputDeviceChangeMessage(win32msg msg, win32wprm w, win32lprm l) :
			BasicWindowMessage(msg, w, l) {}
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
		virtual void RawInputDeviceChangeEvent(RawInputDeviceChangeMessage& e) {}
		virtual void RawMouseEvent(RawMouseMessage& e, Win32RawInput& raw_input, tsize raw_input_size) {}
		virtual void RawKeyboardEvent(RawKeyboardMessage& e, Win32RawInput& raw_input, tsize raw_input_size) {}
		virtual void RawHidEvent(RawHidMessage& e, Win32RawInput& raw_input, tsize raw_input_size) {}
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

// Implementation

namespace Petal
{
	inline win32bool RawInputWindow::RegisterRawInputDevices(::std::span<const Win32RawInputDevice> devices)
	{
		return ::RegisterRawInputDevices(devices.data(), static_cast<win32uint>(devices.size()), sizeof(Win32RawInputDevice));
	}
}

namespace Petal
{
	inline RawInputDataBuffer::RawInputDataBuffer() :
		buffer_ptr{ static_cast<ptr<byte>>(operator new[](min_size * sizeof(byte))) },
		buffer_size{ min_size },
		raw_input_size{} {}
	inline RawInputDataBuffer::~RawInputDataBuffer()
	{
		raw_input_size = 0;
		if (buffer_ptr != nullptr)
		{
			operator delete[](buffer_ptr);
			buffer_ptr = nullptr;
		}
		buffer_size = 0;
	}
	inline ::std::span<byte> RawInputDataBuffer::WriteOnlyBuffer(tsize size)
	{
		if (size > buffer_size) // calling WriteOnlyBuffer may change the buffer(for performance, we do not copy content to new buffer)
		{
			operator delete[](buffer_ptr); // delete decltype(buffer_ptr)(nullptr) is safe
			buffer_ptr = static_cast<ptr<byte>>(operator new[](size * sizeof(byte)));
			buffer_size = size;
		}
		raw_input_size = size;
		return { buffer_ptr, raw_input_size };
	}
	inline ::std::span<const byte> RawInputDataBuffer::ReadOnlyBuffer() const noexcept
	{
		return ::std::span<const byte>{ buffer_ptr, raw_input_size };
	}
	inline tsize RawInputDataBuffer::BufferSize() const noexcept
	{
		return buffer_size;
	}
	inline tsize RawInputDataBuffer::RawInputSize() const noexcept
	{
		return raw_input_size;
	}
	inline RawInputRef RawInputDataBuffer::AsRawInput() const noexcept
	{
		return { (reinterpret_cast<ptr<Win32RawInput>>(this->buffer_ptr)), this->raw_input_size };
	}
	inline Win32RawInput& RawInputDataBuffer::AsWin32RawInput() noexcept
	{
		return *(reinterpret_cast<ptr<Win32RawInput>>(buffer_ptr));
	}
	inline const Win32RawInput& RawInputDataBuffer::AsWin32RawInput() const noexcept
	{
		return *(reinterpret_cast<ptr<Win32RawInput>>(buffer_ptr));
	}
}

#endif // !Petal_Header_RawInputWindow
