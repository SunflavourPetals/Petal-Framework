#pragma once

#ifndef Petal_Header_VSDebugOutput
#define Petal_Header_VSDebugOutput

#include "Petal~BasicTypes.h"
#include "Petal~String.h"
#include "Petal~Output.h"

#include <format>

namespace Petal::Abstract
{
	template <typename CharT, typename Traits = ::std::char_traits<CharT>>
	class VSDebugOutput : public IBOutput<CharT, Traits>
	{
	private:
		using Base = IBOutput<CharT, Traits>;
	public:
		using typename Base::CharType;
		using typename Base::TraitsType;
		using typename Base::StringType;
		using typename Base::StringViewType;
		using typename Base::CStringRefType;
		using typename Base::CStringType;
	public:
		virtual void Output(StringViewType str) = 0;
		virtual void OutputCStr(CStringType c_str) = 0;
		virtual LineBreakMode LnMode() noexcept override { return this->line_break_mode; }
		virtual LineBreakMode LnModeCStr() noexcept override { return this->LnMode(); }
	public:
		LineBreakMode line_break_mode{};
	};
}

namespace Petal::Debug
{
	class VSDebugOutputA : public Abstract::VSDebugOutput<Char>
	{
	private:
		using Base = Abstract::VSDebugOutput<Char>;
	public:
		using typename Base::CharType;
		using typename Base::TraitsType;
		using typename Base::StringType;
		using typename Base::StringViewType;
		using typename Base::CStringRefType;
		using typename Base::CStringType;
	public:
		virtual void Output(StringViewType str) override;
		virtual void OutputCStr(CStringType c_str) noexcept override;
	public:
		constexpr VSDebugOutputA() = default;
	};

	class VSDebugOutputW : public Abstract::VSDebugOutput<WChar>
	{
	private:
		using Base = Abstract::VSDebugOutput<WChar>;
	public:
		using typename Base::CharType;
		using typename Base::TraitsType;
		using typename Base::StringType;
		using typename Base::StringViewType;
		using typename Base::CStringRefType;
		using typename Base::CStringType;
	public:
		virtual void Output(StringViewType str) override;
		virtual void OutputCStr(CStringType c_str) noexcept override;
	public:
		constexpr VSDebugOutputW() = default;
	};
}

namespace Petal
{
	extern constinit Debug::VSDebugOutputA dout;
	extern constinit Debug::VSDebugOutputW dowt;
}

namespace Petal::Debug::V
{
	template <typename... Args>
	inline void print(StringView fmt, Args&&... args)
	{
		dout + ::std::vformat(fmt, ::std::make_format_args(args...));
	}
	template <typename CharT, tsize char_arr_size, typename... Args>
		requires std::is_same_v<Char, std::remove_const_t<CharT>>
	inline void print(CharT (&fmt)[char_arr_size], Args&&... args)
	{
		print(StringView{ fmt, char_arr_size }, ::std::forward<Args>(args)...);
	}
	inline void println()
	{
		dout + ln;
	}
	template <typename... Args>
	inline void println(StringView fmt, Args&&... args)
	{
		auto fmt_ln = ::std::format("{}{}", fmt, GetLn<decltype(dout)::CharType>(dout.LnMode()));
		print(fmt_ln, ::std::forward<Args>(args)...);
	}
	template <typename CharT, tsize char_arr_size, typename... Args>
		requires std::is_same_v<Char, std::remove_const_t<CharT>>
	inline void println(CharT (&fmt)[char_arr_size], Args&&... args)
	{
		println(StringView{ fmt, char_arr_size }, ::std::forward<Args>(args)...);
	}

	template <typename... Args>
	inline void wprint(WStringView fmt, Args&&... args)
	{
		dowt + ::std::vformat(fmt, ::std::make_wformat_args(args...));
	}
	template <typename CharT, tsize char_arr_size, typename... Args>
		requires std::is_same_v<WChar, std::remove_const_t<CharT>>
	inline void wprint(CharT (&fmt)[char_arr_size], Args&&... args)
	{
		wprint(WStringView{ fmt, char_arr_size }, ::std::forward<Args>(args)...);
	}
	inline void wprintln()
	{
		dowt + ln;
	}
	template <typename... Args>
	inline void wprintln(WStringView fmt, Args&&... args)
	{
		auto fmt_ln = ::std::format(L"{}{}", fmt, GetLn<decltype(dowt)::CharType>(dowt.LnMode()));
		wprint(fmt_ln, ::std::forward<Args>(args)...);
	}
	template <typename CharT, tsize char_arr_size, typename... Args>
		requires std::is_same_v<WChar, std::remove_const_t<CharT>>
	inline void wprintln(CharT (&fmt)[char_arr_size], Args&&... args)
	{
		wprintln(WStringView{ fmt, char_arr_size }, ::std::forward<Args>(args)...);
	}
}

namespace Petal::Debug
{
	template <typename... Args>
	inline void print(const ::std::format_string<Args...> fmt, Args&&... args)
	{
		dout + ::std::vformat(fmt.get(), ::std::make_format_args(args...));
	}
	inline void println()
	{
		dout + ln;
	}
	template <typename... Args>
	inline void println(const ::std::format_string<Args...> fmt, Args&&... args)
	{
		auto str = ::std::vformat(fmt.get(), ::std::make_format_args(args...));
		print("{}{}", str, GetLn<decltype(dout)::CharType>(dout.LnMode()));
	}

	template <typename... Args>
	inline void wprint(const ::std::wformat_string<Args...> fmt, Args&&... args)
	{
		dowt + ::std::vformat(fmt.get(), ::std::make_wformat_args(args...));
	}
	inline void wprintln()
	{
		dowt + ln;
	}
	template <typename... Args>
	inline void wprintln(const ::std::wformat_string<Args...> fmt, Args&&... args)
	{
		auto str = ::std::vformat(fmt.get(), ::std::make_wformat_args(args...));
		wprint(L"{}{}", str, GetLn<decltype(dowt)::CharType>(dowt.LnMode()));
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
