#pragma once

#ifndef Petal_Header_Process
#define Petal_Header_Process

#include "Petal~BasicTypes.h"

namespace Petal::Abstract
{
	template <typename... Resources>
	class Process
	{
	public:
		virtual void Execution(const ::std::remove_cvref_t<Resources>&... resource) = 0;
	public:
		virtual ~Process() = default;
	};
}

#endif // !Petal_Header_Process
