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
	void VSDebugOutputA::OutputCStr(ptrc<InnerChar> c_str) noexcept
	{
		::OutputDebugStringA(c_str);
	}
	LineBreakMode VSDebugOutputA::LnMode() noexcept
	{
		return this->line_break_mode;
	}
	LineBreakMode VSDebugOutputA::LnModeCStr() noexcept
	{
		return this->line_break_mode;
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
	void VSDebugOutputW::OutputCStr(ptrc<InnerChar> c_str) noexcept
	{
		::OutputDebugStringW(c_str);
	}
	LineBreakMode VSDebugOutputW::LnMode() noexcept
	{
		return this->line_break_mode;
	}
	LineBreakMode VSDebugOutputW::LnModeCStr() noexcept
	{
		return this->line_break_mode;
	}
}

namespace Petal
{
	constinit Debug::VSDebugOutputA dout{};
	constinit Debug::VSDebugOutputW dowt{};
}