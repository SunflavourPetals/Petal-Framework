#include "Petal~VSDebugOutput.h"
#include "Petal~WinTypes.h"

namespace Petal::Debug
{
	void VSDebugOutputA::Write(StringViewType str)
	{
		StringType c_str{ str.data(), str.size() };
#ifdef Petal_Enable_ForceDbgRemoveNUL
		this->WriteCStr(StringToCStyleString(c_str).c_str());
#else
		this->WriteCStr(c_str.c_str());
#endif
	}
	void VSDebugOutputA::WriteCStr(CStringType c_str) noexcept
	{
		::OutputDebugStringA(c_str);
	}

	void VSDebugOutputW::Write(StringViewType str)
	{
		StringType c_str{ str.data(), str.size() };
#ifdef Petal_Enable_ForceDbgRemoveNUL
		this->WriteCStr(StringToCStyleString(c_str).c_str());
#else
		this->WriteCStr(c_str.c_str());
#endif
	}
	void VSDebugOutputW::WriteCStr(CStringType c_str) noexcept
	{
		::OutputDebugStringW(c_str);
	}
}

namespace Petal
{
	constinit Debug::VSDebugOutputA dout{};
	constinit Debug::VSDebugOutputW dowt{};
}
