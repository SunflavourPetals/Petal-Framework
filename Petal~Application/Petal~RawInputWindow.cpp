#include "Petal~RawInputWindow.h"

namespace Petal
{
	RawInputDataBuffer::RawInputDataBuffer()
	{
		this->buffer_ptr = new Petal::byte[this->min_size]{};
		this->buffer_size = this->min_size;
	}
	RawInputDataBuffer::~RawInputDataBuffer()
	{
		this->raw_input_buffer = nullptr;
		if (this->buffer_ptr != nullptr)
		{
			delete[] this->buffer_ptr;
			this->buffer_ptr = nullptr;
		}
		this->buffer_size = this->raw_input_size = 0;
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
	RawInputRef RawInputDataBuffer::AsRawInput() const noexcept
	{
		return { (reinterpret_cast<ptr<Win32RawInput>>(this->raw_input_buffer)), this->raw_input_size };
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
		}
	}
}

namespace Petal
{
	RawInputMessage::RawInputMessage(win32msg msg, win32wprm w, win32lprm l, RawInputRef input) :
		BasicWindowMessage(msg, w, l),
		pt_raw_input_code{ GET_RAWINPUT_CODE_WPARAM(w) },
		raw_input{ input }
	{

	}
	win32wprm RawInputMessage::RawInputCode() const noexcept
	{
		return this->pt_raw_input_code;
	}
	Petal::boolean RawInputMessage::IsSink() const noexcept
	{
		return this->RawInputCode() == RIM_INPUTSINK;
	}
	Win32HRawInput RawInputMessage::HRawInput() const noexcept
	{
		return reinterpret_cast<Win32HRawInput>(this->Long());
	}
	const Win32RawInput& RawInputMessage::RawInput() const noexcept
	{
		return *(this->raw_input.raw_input);
	}
	tsize RawInputMessage::RawInputSize() const noexcept
	{
		return this->raw_input.valid_size;
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
	Petal::boolean RawInputDeviceChangeMessage::Arrival() const noexcept
	{
		return this->GIDC() == GIDC_ARRIVAL;
	}
	Petal::boolean RawInputDeviceChangeMessage::Removal() const noexcept
	{
		return this->GIDC() == GIDC_REMOVAL;
	}
}

namespace Petal
{
	void RawInputWindow::RawInputEvent(RawInputMessage& e) noexcept
	{
		switch (e.RawInput().header.dwType)
		{
		case RIM_TYPEMOUSE:
			this->RawMouseEvent(e);
			break;
		case RIM_TYPEKEYBOARD:
			this->RawKeyboardEvent(e);
			break;
		case RIM_TYPEHID:
			this->RawHidEvent(e);
			break;
		default:
			break;
		}
	}
	void RawInputWindow::RawMouseEvent(RawMouseMessage& e) noexcept {}
	void RawInputWindow::RawKeyboardEvent(RawKeyboardMessage& e) noexcept {}
	void RawInputWindow::RawHidEvent(RawHidMessage& e) noexcept {}
	void RawInputWindow::RawINputDeviceChangeEvent(RawInputDeviceChangeMessage& e) noexcept {}
	Petal::win32lres RawInputWindow::Process(Petal::win32msg msg, Petal::win32wprm w, Petal::win32lprm l) noexcept
	{
		switch (msg)
		{
		case WM_INPUT:
		{
			win32uint size{};
			::GetRawInputData(reinterpret_cast<Win32HRawInput>(l), RID_INPUT, nullptr, &size, sizeof(Win32RawInputHeader));

			auto buffer = this->pt_raw_input_buffer.WriteOnlyBuffer(size);

			if (::GetRawInputData(reinterpret_cast<Win32HRawInput>(l), RID_INPUT, buffer.data(), &size, sizeof(::RAWINPUTHEADER)) != buffer.size())
			{
				Petal_VSDbgT("[Petal] ::GetRawInputData(...) does not return correct size\r\n");
				return 0;
			}

			RawMouseMessage message{ msg, w, l, this->pt_raw_input_buffer.AsRawInput() };

			this->RawInputEvent(message);

			if (message.IsSink()) IWin32::DefaultWindowProcess(this->WindowHandle(), msg, w, l);
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
	win32bool RawInputWindow::RegisterRawInputDevices(::std::span<Win32RawInputDevice> devices)
	{
		return ::RegisterRawInputDevices(devices.data(), static_cast<win32uint>(devices.size()), sizeof(Win32RawInputDevice));
	}
}