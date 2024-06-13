#include "Petal~RawInputWindow.h"
#include "Petal~VSDebugOutput.h"

namespace Petal
{
	void RawInputWindow::RawInputEvent(RawInputMessage& e)
	{
		win32uint size{};
		::GetRawInputData(e.HRawInput(), RID_INPUT, nullptr, &size, sizeof(Win32RawInputHeader));

		auto buffer = raw_input_buffer.WriteOnlyBuffer(size);

		if (::GetRawInputData(e.HRawInput(), RID_INPUT, buffer.data(), &size, sizeof(Win32RawInputHeader)) != buffer.size())
		{
			Petal_VSDbgT("[Petal] ::GetRawInputData(...) does not return correct size\r\n");
		}
		else
		{
			switch (raw_input_buffer.AsWin32RawInput().header.dwType)
			{
			case RIM_TYPEMOUSE:
				RawMouseEvent(e, raw_input_buffer.AsWin32RawInput(), raw_input_buffer.RawInputSize());
				break;
			case RIM_TYPEKEYBOARD:
				RawKeyboardEvent(e, raw_input_buffer.AsWin32RawInput(), raw_input_buffer.RawInputSize());
				break;
			case RIM_TYPEHID:
				RawHidEvent(e, raw_input_buffer.AsWin32RawInput(), raw_input_buffer.RawInputSize());
				break;
			default:
				break;
			}
		}

		if (e.IsSink())
		{
			e.DefaultProcess(WindowHandle());
		}
	}

	win32lres RawInputWindow::Process(win32msg msg, win32wprm w, win32lprm l)
	{
		switch (msg)
		{
		case WM_INPUT:
		{
			RawInputMessage message{ msg, w, l };
			RawInputEvent(message);
		}
			break;
		case WM_INPUT_DEVICE_CHANGE:
		{
			RawInputDeviceChangeMessage message{ msg, w, l };
			RawInputDeviceChangeEvent(message);
		}
			break;
		default:
			return Window::Process(msg, w, l);
			break;
		}
		return 0;
	}

}
