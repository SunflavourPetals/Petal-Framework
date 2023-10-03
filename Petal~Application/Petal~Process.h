#pragma once

#ifndef Petal_Header_Process
#define Petal_Header_Process

#include "Petal~BasicTypes.h"

namespace Petal::Abstract
{
	class ProcessNR
	{
	public:
		virtual void Execution() = 0;
	public:
		inline virtual ~ProcessNR() = default;
	};

	template <typename ResourceT>
	class Process : public ProcessNR
	{
	public:
		using ResourceType = ResourceT;
	public:
		inline virtual const ResourceType& Resource() const noexcept final { return *(this->resource_ref); }
		inline virtual void AssignResource(const ResourceType& res) noexcept final { this->resource_ref = &res; }
		inline virtual void EmptyResource() noexcept final { this->resource_ref = nullptr; }
		inline virtual bool ValidResource() const noexcept final { return (this->resource_ref != nullptr); }
	public:
		virtual void Execution() = 0;
	public:
		inline virtual ~Process() = default;
	private:
		ptrc<ResourceType> resource_ref{ nullptr };
	};
}

#endif // !Petal_Header_Process
