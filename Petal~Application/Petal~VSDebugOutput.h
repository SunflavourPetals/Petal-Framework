#pragma once

#ifndef Petal_Header_VSDebugOutput
#define Petal_Header_VSDebugOutput

#include "Petal~BasicTypes.h"
#include "Petal~String.h"
#include "Petal~Output.h"

#include <format>

namespace Petal::Debug
{
	class VSDebugOutputA : public Abstract::OutputA, public Abstract::COutputA
	{
	public:
		using InnerChar = typename Abstract::OutputA::InnerChar;
		using InnerString = typename Abstract::OutputA::InnerString;
		using InnerStringView = typename Abstract::OutputA::InnerStringView;
		using InnerCStringRef = typename Abstract::OutputA::InnerCStringRef;
	public:
		virtual void Output(InnerStringView str);
		virtual void OutputCStr(ptrc<InnerChar> c_str);
	};

	class VSDebugOutputW : public Abstract::OutputW, public Abstract::COutputW
	{
	public:
		using InnerChar = typename Abstract::OutputW::InnerChar;
		using InnerString = typename Abstract::OutputW::InnerString;
		using InnerStringView = typename Abstract::OutputW::InnerStringView;
		using InnerCStringRef = typename Abstract::OutputW::InnerCStringRef;
	public:
		virtual void Output(InnerStringView str);
		virtual void OutputCStr(ptrc<InnerChar> c_str);
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
	template <tsize char_arr_size, typename... Args>
	inline void print(const Char (&fmt)[char_arr_size], Args&&... args)
	{
		print(StringView{ fmt, char_arr_size - 1 }, ::std::forward<Args>(args)...);
	}
	template <tsize char_arr_size, typename... Args>
	inline void print(Char (&fmt)[char_arr_size], Args&&... args)
	{
		print(StringView{ fmt, char_arr_size - 1 }, ::std::forward<Args>(args)...);
	}
	inline void println()
	{
		dout + ln;
	}
	template <typename... Args>
	inline void println(StringView fmt, Args&&... args)
	{
		StringView fmt_fmt{ "{}{}" };
		auto ln = GetLn<Char>(dout.line_break_mode);
		auto fmt_ln = ::std::vformat(fmt_fmt, ::std::make_format_args(fmt, ln));
		print(fmt_ln, ::std::forward<Args>(args)...);
	}
	template <tsize char_arr_size, typename... Args>
	inline void println(const Char (&fmt)[char_arr_size], Args&&... args)
	{
		println(StringView{ fmt, char_arr_size - 1 }, ::std::forward<Args>(args)...);
	}
	template <tsize char_arr_size, typename... Args>
	inline void println(Char (&fmt)[char_arr_size], Args&&... args)
	{
		println(StringView{ fmt, char_arr_size - 1 }, ::std::forward<Args>(args)...);
	}

	template <typename... Args>
	inline void wprint(WStringView fmt, Args&&... args)
	{
		dowt + ::std::vformat(fmt, ::std::make_wformat_args(::std::forward<Args>(args)...));
	}
	template <tsize char_arr_size, typename... Args>
	inline void wprint(const WChar (&fmt)[char_arr_size], Args&&... args)
	{
		wprint(WStringView{ fmt, char_arr_size - 1 }, ::std::forward<Args>(args)...);
	}
	template <tsize char_arr_size, typename... Args>
	inline void wprint(WChar (&fmt)[char_arr_size], Args&&... args)
	{
		wprint(WStringView{ fmt, char_arr_size - 1 }, ::std::forward<Args>(args)...);
	}
	inline void wprintln()
	{
		dowt + ln;
	}
	template <typename... Args>
	inline void wprintln(WStringView fmt, Args&&... args)
	{
		WStringView fmt_fmt{ L"{}{}" };
		auto ln = GetLn<WChar>(dowt.line_break_mode);
		auto fmt_ln = ::std::vformat(fmt_fmt, ::std::make_wformat_args(fmt, ln));
		wprint(fmt_ln, ::std::forward<Args>(args)...);
	}
	template <tsize char_arr_size, typename... Args>
	inline void wprintln(const WChar (&fmt)[char_arr_size], Args&&... args)
	{
		wprintln(WStringView{ fmt, char_arr_size - 1 }, ::std::forward<Args>(args)...);
	}
	template <tsize char_arr_size, typename... Args>
	inline void wprintln(WChar (&fmt)[char_arr_size], Args&&... args)
	{
		wprintln(WStringView{ fmt, char_arr_size - 1 }, ::std::forward<Args>(args)...);
	}
}

namespace Petal::Debug
{
	class VSLiteralFmtDbgOutA
	{
	public:
		StringView fmt{};
	public:
		constexpr VSLiteralFmtDbgOutA(StringView format) : fmt{ format } {}
		template <typename... Args>
		constexpr void operator()(Args&&... args)
		{
			print(fmt, ::std::forward<Args>(args)...);
		}
	};
	class VSLiteralFmtDbgOutLnA
	{
	public:
		StringView fmt{};
	public:
		constexpr VSLiteralFmtDbgOutLnA(StringView format) : fmt{ format } {}
		template <typename... Args>
		constexpr void operator()(Args&&... args)
		{
			println(fmt, ::std::forward<Args>(args)...);
		}
	};
	class VSLiteralFmtDbgOutW
	{
	public:
		WStringView fmt{};
	public:
		constexpr VSLiteralFmtDbgOutW(WStringView format) : fmt{ format } {}
		template <typename... Args>
		constexpr void operator()(Args&&... args)
		{
			wprint(fmt, ::std::forward<Args>(args)...);
		}
	};
	class VSLiteralFmtDbgOutLnW
	{
	public:
		WStringView fmt{};
	public:
		constexpr VSLiteralFmtDbgOutLnW(WStringView format) : fmt{ format } {}
		template <typename... Args>
		constexpr void operator()(Args&&... args)
		{
			wprintln(fmt, ::std::forward<Args>(args)...);
		}
	};
}

namespace Petal
{
	inline constexpr Debug::VSLiteralFmtDbgOutA operator""_dout(const Char * str, ::std::size_t length)
	{
		return { { str, length } };
	}
	inline constexpr Debug::VSLiteralFmtDbgOutLnA operator""_doutln(const Char * str, ::std::size_t length)
	{
		return { { str, length } };
	}
	inline constexpr Debug::VSLiteralFmtDbgOutW operator""_dout(const WChar * str, ::std::size_t length)
	{
		return { { str, length } };
	}
	inline constexpr Debug::VSLiteralFmtDbgOutLnW operator""_doutln(const WChar * str, ::std::size_t length)
	{
		return { { str, length } };
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

#endif // !Petal_Header_VSDebugOutput
