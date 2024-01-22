#include "Petal~RawInputWindow.h"
#include "Petal~VSDebugOutput.h"

namespace Petal
{
	RawInputDataBuffer::RawInputDataBuffer() :
		buffer_ptr{ new Petal::byte[this->min_size] },
		buffer_size{ this->min_size }
	{

	}
	RawInputDataBuffer::~RawInputDataBuffer()
	{
		this->raw_input_buffer = nullptr;
		this->raw_input_size = 0;
		if (this->buffer_ptr != nullptr)
		{
			delete[] this->buffer_ptr;
			this->buffer_ptr = nullptr;
		}
		this->buffer_size = 0;
	}
	::std::span<Petal::byte> RawInputDataBuffer::WriteOnlyBuffer(tsize size)
	{
		this->Alloc(size);
		this->raw_input_buffer = new(this->buffer_ptr) Petal::byte[size]{};
		this->raw_input_size = size;
		return { this->raw_input_buffer, this->raw_input_size };
	}
	::std::span<const Petal::byte> RawInputDataBuffer::ReadOnlyBuffer() const noexcept
	{
		return ::std::span<const Petal::byte>{ this->raw_input_buffer, this->raw_input_size };
	}
	tsize RawInputDataBuffer::BufferSize() const noexcept
	{
		return this->buffer_size;
	}
	tsize RawInputDataBuffer::RawInputSize() const noexcept
	{
		return this->raw_input_size;
	}
	RawInputRef RawInputDataBuffer::AsRawInput() const noexcept
	{
		return { (reinterpret_cast<ptr<Win32RawInput>>(this->raw_input_buffer)), this->raw_input_size };
	}
	Win32RawInput& RawInputDataBuffer::AsWin32RawInput() noexcept
	{
		return *(reinterpret_cast<ptr<Win32RawInput>>(this->raw_input_buffer));
	}
	const Win32RawInput& RawInputDataBuffer::AsWin32RawInput() const noexcept
	{
		return *(reinterpret_cast<ptr<Win32RawInput>>(this->raw_input_buffer));
	}
	void RawInputDataBuffer::Alloc(tsize size)
	{
		if (size > this->buffer_size)
		{
			if (this->buffer_ptr != nullptr)
			{
				delete[] this->buffer_ptr;
			}
			this->buffer_ptr = new Petal::byte[size]{};
			this->buffer_size = size;
			this->raw_input_buffer = nullptr;
			this->raw_input_size = 0;
		}
	}
}

namespace Petal
{
	RawInputMessage::RawInputMessage(win32msg msg, win32wprm w, win32lprm l) :
		BasicWindowMessage(msg, w, l),
		pt_raw_input_code{ GET_RAWINPUT_CODE_WPARAM(w) }
	{

	}
	win32wprm RawInputMessage::RawInputCode() const noexcept
	{
		return this->pt_raw_input_code;
	}
	boolean RawInputMessage::IsSink() const noexcept
	{
		return this->RawInputCode() == RIM_INPUTSINK;
	}
	Win32HRawInput RawInputMessage::HRawInput() const noexcept
	{
		return reinterpret_cast<Win32HRawInput>(this->Long());
	}
	win32lres RawInputMessage::DefaultProcess(win32hwnd window_handle) const noexcept
	{
		return IWin32::DefaultWindowProcess(window_handle, this->Message(), this->Word(), this->Long());
	}
}

namespace Petal
{
	RawInputDeviceChangeMessage::RawInputDeviceChangeMessage(win32msg msg, win32wprm w, win32lprm l) :
		BasicWindowMessage(msg, w, l)
	{

	}
	Win32HRawInput RawInputDeviceChangeMessage::RawInputHandle() const noexcept
	{
		return reinterpret_cast<Win32HRawInput>(this->Long());
	}
	win32wprm RawInputDeviceChangeMessage::GIDC() const noexcept
	{
		return this->Word();
	}
	boolean RawInputDeviceChangeMessage::Arrival() const noexcept
	{
		return this->GIDC() == GIDC_ARRIVAL;
	}
	boolean RawInputDeviceChangeMessage::Removal() const noexcept
	{
		return this->GIDC() == GIDC_REMOVAL;
	}
}

namespace Petal
{
	void RawInputWindow::RawInputEvent(RawInputMessage& e) noexcept
	{
		win32uint size{};
		::GetRawInputData(e.HRawInput(), RID_INPUT, nullptr, &size, sizeof(Win32RawInputHeader));

		auto buffer = this->pt_raw_input_buffer.WriteOnlyBuffer(size);

		if (::GetRawInputData(e.HRawInput(), RID_INPUT, buffer.data(), &size, sizeof(Win32RawInputHeader)) != buffer.size())
		{
			Petal_VSDbgT("[Petal] ::GetRawInputData(...) does not return correct size\r\n");
		}
		else
		{
			switch (this->pt_raw_input_buffer.AsWin32RawInput().header.dwType)
			{
			case RIM_TYPEMOUSE:
				this->RawMouseEvent(e, this->pt_raw_input_buffer.AsWin32RawInput(), this->pt_raw_input_buffer.RawInputSize());
				break;
			case RIM_TYPEKEYBOARD:
				this->RawKeyboardEvent(e, this->pt_raw_input_buffer.AsWin32RawInput(), this->pt_raw_input_buffer.RawInputSize());
				break;
			case RIM_TYPEHID:
				this->RawHidEvent(e, this->pt_raw_input_buffer.AsWin32RawInput(), this->pt_raw_input_buffer.RawInputSize());
				break;
			default:
				break;
			}
		}

		if (e.IsSink())
		{
			e.DefaultProcess(this->WindowHandle());
		}
	}
	void RawInputWindow::RawMouseEvent(RawMouseMessage& e, Win32RawInput& raw_input, tsize raw_input_size) noexcept {}
	void RawInputWindow::RawKeyboardEvent(RawKeyboardMessage& e, Win32RawInput& raw_input, tsize raw_input_size) noexcept {}
	void RawInputWindow::RawHidEvent(RawHidMessage& e, Win32RawInput& raw_input, tsize raw_input_size) noexcept {}
	void RawInputWindow::RawINputDeviceChangeEvent(RawInputDeviceChangeMessage& e) noexcept {}
	Petal::win32lres RawInputWindow::Process(Petal::win32msg msg, Petal::win32wprm w, Petal::win32lprm l) noexcept
	{
		switch (msg)
		{
		case WM_INPUT:
		{
			RawInputMessage message{ msg, w, l };
			this->RawInputEvent(message);
		}
		break;
		case WM_INPUT_DEVICE_CHANGE:
		{
			RawInputDeviceChangeMessage message{ msg, w, l };
			this->RawINputDeviceChangeEvent(message);
		}
		break;
		default:
			return this->Window::Process(msg, w, l);
			break;
		}
		return 0;
	}
	win32bool RawInputWindow::RegisterRawInputDevices(::std::span<const Win32RawInputDevice> devices)
	{
		return ::RegisterRawInputDevices(devices.data(), static_cast<win32uint>(devices.size()), sizeof(Win32RawInputDevice));
	}
}