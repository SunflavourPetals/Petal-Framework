#include "Petal~VSDebugOutput.h"
#include <Windows.h>

namespace Petal::Debug
{
	void VSDebugOutputA::Output(InnerStringView str)
	{
		InnerString c_str{ str.data(), str.size() };
#ifdef Petal_Enable_ForceDbgRemoveNUL
		this->OutputCStr(StringToCStyleString(c_str).c_str());
#else
		this->OutputCStr(c_str.c_str());
#endif
	}
	void VSDebugOutputA::OutputCStr(ptrc<InnerChar> c_str)
	{
		::OutputDebugStringA(c_str);
	}
	void VSDebugOutputW::Output(InnerStringView str)
	{
		InnerString c_str{ str.data(), str.size() };
#ifdef Petal_Enable_ForceDbgRemoveNUL
		this->OutputCStr(StringToCStyleString(c_str).c_str());
#else
		this->OutputCStr(c_str.c_str());
#endif
	}
	void VSDebugOutputW::OutputCStr(ptrc<InnerChar> c_str)
	{
		::OutputDebugStringW(c_str);
	}
}

namespace Petal
{
	Debug::VSDebugOutputA dout{};
	Debug::VSDebugOutputW dowt{};
}