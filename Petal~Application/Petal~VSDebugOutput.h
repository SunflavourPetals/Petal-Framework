#pragma once

#ifndef Petal_Header_VSDebugOutput
#define Petal_Header_VSDebugOutput

#include "Petal~BasicTypes.h"
#include "Petal~String.h"
#include "Petal~Output.h"

#include <format>

namespace Petal::Debug
{
	class VSDebugOutputA : public Abstract::COutputA
	{
	public:
		virtual void Output(InnerStringView str);
		virtual void OutputCStr(ptrc<InnerChar> c_str) final;
	};

	class VSDebugOutputW : public Abstract::COutputW
	{
	public:
		virtual void Output(InnerStringView str);
		virtual void OutputCStr(ptrc<InnerChar> c_str) final;
	};
}

namespace Petal
{
	extern Debug::VSDebugOutputA dout;
	extern Debug::VSDebugOutputW dowt;
}

namespace Petal::Debug
{
	template <typename... Args>
	inline void print(StringView fmt, Args&&... args)
	{
		dout + ::std::vformat(fmt, ::std::make_format_args(::std::forward<Args>(args)...));
	}
	inline void println()
	{
		dout + ln;
	}
	template <typename... Args>
	inline void println(StringView fmt, Args&&... args)
	{
		print(fmt, ::std::forward<Args>(args)...);
		println();
	}
	template <typename... Args>
	inline void wprint(WStringView fmt, Args&&... args)
	{
		dowt + ::std::vformat(fmt, ::std::make_wformat_args(::std::forward<Args>(args)...));
	}
	inline void wprintln()
	{
		dowt + ln;
	}
	template <typename... Args>
	inline void wprintln(WStringView fmt, Args&&... args)
	{
		wprint(fmt, ::std::forward<Args>(args)...);
		wprintln();
	}
}

#ifdef Petal_VSDebugOutputA
#undef Petal_VSDebugOutputA
#endif
#ifdef Petal_VSDebugOutputW
#undef Petal_VSDebugOutputW
#endif
#ifdef Petal_VSDebugOutput
#undef Petal_VSDebugOutput
#endif
#ifdef Petal_VSDebugOutputT
#undef Petal_VSDebugOutputT
#endif
#ifdef Petal_VSDebugOutput_S
#undef Petal_VSDebugOutput_S
#endif
#ifdef Petal_VSDebugOutput_ST
#undef Petal_VSDebugOutput_ST
#endif
#ifdef Petal_VSDbgA
#undef Petal_VSDbgA
#endif
#ifdef Petal_VSDbgW
#undef Petal_VSDbgW
#endif
#ifdef Petal_VSDbg
#undef Petal_VSDbg
#endif
#ifdef Petal_VSDbgT
#undef Petal_VSDbgT
#endif
#ifdef Petal_VSDbgExcep
#undef Petal_VSDbgExcep
#endif

#if defined(Petal_Enable_VSDebugOutput)
#define Petal_VSDebugOutputA(x) ::Petal::dout.OutputCStr( x )
#define Petal_VSDebugOutputW(x) ::Petal::dowt.OutputCStr( x )
#else
#define Petal_VSDebugOutputA(x)
#define Petal_VSDebugOutputW(x)
#endif

#if defined(Petal_Enable_Unicode)
#define Petal_VSDebugOutput(x) Petal_VSDebugOutputW(x)
#define Petal_VSDebugOutputT(x) Petal_VSDebugOutputW(L##x)
#else
#define Petal_VSDebugOutput(x) Petal_VSDebugOutputA(x)
#define Petal_VSDebugOutputT(x) Petal_VSDebugOutputA(x)
#endif

#if defined(UNICODE) || defined(_UNICODE)
#define Petal_VSDebugOutput_S(x) Petal_VSDebugOutputW(x)
#define Petal_VSDebugOutput_ST(x) Petal_VSDebugOutputW(L##x)
#else
#define Petal_VSDebugOutput_S(x) Petal_VSDebugOutputA(x)
#define Petal_VSDebugOutput_ST(x) Petal_VSDebugOutputA(x)
#endif

#define Petal_VSDbgA(x) Petal_VSDebugOutputA(x)
#define Petal_VSDbgW(x) Petal_VSDebugOutputW(x)

#if defined(Petal_Enable_ForceDbgANSI)
#define Petal_VSDbg(x) Petal_VSDebugOutputA(x)
#define Petal_VSDbgT(x) Petal_VSDebugOutputA(x)
#else
#define Petal_VSDbg(x) Petal_VSDebugOutput(x)
#define Petal_VSDbgT(x) Petal_VSDebugOutputT(x)
#endif

#if defined(Petal_Enable_ForceExcepDescANSI)
#define Petal_VSDbgExcep(x) Petal_VSDebugOutputA(x)
#else
#define Petal_VSDbgExcep(x) Petal_VSDebugOutput(x)
#endif

#endif // !Petal_Header_VSDebugOutput
