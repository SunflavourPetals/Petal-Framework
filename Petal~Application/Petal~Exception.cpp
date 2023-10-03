#include "Petal~Exception.h"

#include "Petal~VSDebugOutput.h"

namespace Petal
{
	Exception::Exception(Petal_ExcepArgs(code, desc)) noexcept
	{
		this->pt_code = code;
#if defined(Petal_Enable_ExceptionDescription)
		this->pt_desc = desc;
#endif
	}
	ExceptionCode Exception::Code() const noexcept
	{
		return this->pt_code;
	}
	ptrc<ExcepChar> Exception::Desc() const noexcept
	{
#if defined(Petal_Enable_ExceptionDescription)
		return ((this->pt_desc != nullptr) ? this->pt_desc : this->pt_prompt);
#else
		return this->pt_prompt;
#endif
	}
}
