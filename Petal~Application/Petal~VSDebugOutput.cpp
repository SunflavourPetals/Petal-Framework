#include "Petal~VSDebugOutput.h"
#include <Windows.h>

namespace Petal::Debug
{
	void VSDebugOutputA::Output(InnerStringView str)
	{
		InnerString c_str{};
		c_str.assign(str.data(), str.size());
		this->OutputCStr(c_str.c_str());
	}
	void VSDebugOutputA::OutputCStr(ptrc<InnerChar> c_str) const noexcept
	{
		::OutputDebugStringA(c_str);
	}
	void VSDebugOutputW::Output(InnerStringView str)
	{
		InnerString c_str{};
		c_str.assign(str.data(), str.size());
		this->OutputCStr(c_str.c_str());
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