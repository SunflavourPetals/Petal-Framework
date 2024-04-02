#include "Petal~VSDebugOutput.h"
#include "Petal~WinTypes.h"

namespace Petal::Debug
{
	void VSDebugOutputA::Output(StringViewType str)
	{
		StringType c_str{ str.data(), str.size() };
#ifdef Petal_Enable_ForceDbgRemoveNUL
		this->OutputCStr(StringToCStyleString(c_str).c_str());
#else
		this->OutputCStr(c_str.c_str());
#endif
	}
	void VSDebugOutputA::OutputCStr(CStringType c_str) noexcept
	{
		::OutputDebugStringA(c_str);
	}

	void VSDebugOutputW::Output(StringViewType str)
	{
		StringType c_str{ str.data(), str.size() };
#ifdef Petal_Enable_ForceDbgRemoveNUL
		this->OutputCStr(StringToCStyleString(c_str).c_str());
#else
		this->OutputCStr(c_str.c_str());
#endif
	}
	void VSDebugOutputW::OutputCStr(CStringType c_str) noexcept
	{
		::OutputDebugStringW(c_str);
	}
}

namespace Petal
{
	constinit Debug::VSDebugOutputA dout{};
	constinit Debug::VSDebugOutputW dowt{};
}
