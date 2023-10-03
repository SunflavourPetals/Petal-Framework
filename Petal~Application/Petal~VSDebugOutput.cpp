#include "Petal~VSDebugOutput.h"
#include <Windows.h>

namespace Petal::Debug
{
	void VSDebugOutputA::Output(ptrc<InnerChar> data, tsize count)
	{
		if (data == nullptr) return;
		InnerString str{};
		str.assign(data, count);
		this->Output(str);
	}
	void VSDebugOutputA::Output(const InnerString& str)
	{
		InnerString c_str{ StringToCStyleString(str) };
		::OutputDebugStringA(c_str.c_str());
	}
	void VSDebugOutputA::OutputCStr(ptrc<InnerChar> c_str) const noexcept
	{
		::OutputDebugStringA(c_str);
	}
	void VSDebugOutputW::Output(ptrc<InnerChar> data, tsize count)
	{
		if (data == nullptr) return;
		InnerString str{};
		str.assign(data, count);
		this->Output(str);
	}
	void VSDebugOutputW::Output(const InnerString& str)
	{
		{
			InnerString cstr{ StringToCStyleString(str) };
			::OutputDebugStringW(cstr.c_str());
		}
	}
	void VSDebugOutputW::OutputCStr(ptrc<InnerChar> c_str) const noexcept
	{
		::OutputDebugStringW(c_str);
	}
}

namespace Petal
{
	Debug::VSDebugOutputA dout{};
	Debug::VSDebugOutputW dowt{};
}