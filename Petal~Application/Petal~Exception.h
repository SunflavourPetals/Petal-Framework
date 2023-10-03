#pragma once

#ifndef Petal_Header_Exception
#define Petal_Header_Exception

#include "Petal~BasicTypes.h"
#include "Petal~String.h"

#ifdef Petal_ExcepArgs
#undef Petal_ExcepArgs
#endif
#ifdef Petal_MakeExcepArgs
#undef Petal_MakeExcepArgs
#endif

#if defined(Petal_Enable_ExceptionDescription)
#define Petal_ExcepArgs(code, desc) Petal::ExceptionCode code, Petal::ptrc<Petal::ExcepChar> desc
#define Petal_MakeExcepArgs(code, desc) code, Petal_ExcepStr(desc)
#else
#define Petal_ExcepArgs(code, desc) Petal::ExceptionCode code
#define Petal_MakeExcepArgs(code, desc) code
#endif

namespace Petal
{
	enum class ExceptionCode : i32
	{
		// Reason
		// Reason_In_C_XXX_M_XXX // ---- // C means Class,Obj; M means Method; XXX means name of class or method.
		// Reason_In_F_XXX // ---------- // F means Function.
		Unknown = 0,
		// WindowManager
		InvalidMutex_In_C_WindowSet_M_Create = 0x1001,
		FailedInRecordWindow_In_C_WindowSet_M_Create = 0x1002,
		InvalidMutex_In_C_WindowSet_M_Destroy = 0x1008,
		FailedInEraseWindow_In_C_WindowSet_M_Destroy = 0x1009,
	};
	class Exception
	{
	public:
		ExceptionCode Code() const noexcept;
		ptrc<ExcepChar> Desc() const noexcept;
	public:
		Exception(Petal_ExcepArgs(code, desc)) noexcept;
		Exception(const Exception&) noexcept = default;
		Exception(Exception&&) noexcept = default;
		virtual ~Exception() noexcept = default;
	private:
		static constexpr ptrc<ExcepChar> pt_prompt{ Petal_ExcepStr("[Petal] Exception") };
	private:
		ExceptionCode pt_code{ ExceptionCode::Unknown };
#if defined(Petal_Enable_ExceptionDescription)
	private:
		ptrc<ExcepChar> pt_desc{ pt_prompt };
#endif
	};
}

#endif // !Petal_Header_Exception
