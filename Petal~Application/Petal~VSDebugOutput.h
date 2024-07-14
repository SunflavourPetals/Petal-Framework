#pragma once

#ifndef Petal_Header_VSDebugOutput
#define Petal_Header_VSDebugOutput

#include "Petal~BasicTypes.h"
#include "Petal~WinTypes.h"
#include "Petal~String.h"
#include "Petal~Output.h"

#include <format>

namespace Petal::Abstract
{
	template <typename CharT, typename Traits = ::std::char_traits<CharT>>
	class VSDebugOutput : public Output<CharT, Traits>
	{
	private:
		using Base = Output<CharT, Traits>;
	public:
		using typename Base::CharType;
		using typename Base::TraitsType;
		using CStringType = Petal::ptrc<CharType>;
	public:
		virtual void Write(BasicStringView<CharType, TraitsType> str) = 0;
		virtual void WriteCStr(CStringType c_str) noexcept = 0;
		virtual LineBreakMode LnMode() noexcept override { return this->line_break_mode; }
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
		using typename Base::CStringType;
	public:
		void Write(BasicStringView<CharType, TraitsType> str) override;
		void WriteCStr(CStringType c_str) noexcept override;
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
		using typename Base::CStringType;
	public:
		void Write(BasicStringView<CharType, TraitsType> str) override;
		void WriteCStr(CStringType c_str) noexcept override;
	public:
		constexpr VSDebugOutputW() = default;
	};
}

namespace Petal
{
	inline constinit Debug::VSDebugOutputA dout{};
	inline constinit Debug::VSDebugOutputW dowt{};
}

namespace Petal::Debug::V
{
	template <typename... Args>
	inline void print(StringView fmt, Args&&... args)
	{
		dout << ::std::vformat(fmt, ::std::make_format_args(args...));
	}
	template <typename... Args>
	inline void print(WStringView fmt, Args&&... args)
	{
		dowt << ::std::vformat(fmt, ::std::make_wformat_args(args...));
	}
	inline void println()
	{
		dout << ln;
	}
	template <typename... Args>
	inline void println(StringView fmt, Args&&... args)
	{
		auto fmt_ln = ::std::format("{}{}", fmt, GetLn<decltype(dout)::CharType>(dout.LnMode()));
		print(fmt_ln, ::std::forward<Args>(args)...);
	}
	template <typename... Args>
	inline void println(WStringView fmt, Args&&... args)
	{
		auto fmt_ln = ::std::format(L"{}{}", fmt, GetLn<decltype(dowt)::CharType>(dowt.LnMode()));
		print(fmt_ln, ::std::forward<Args>(args)...);
	}
}

namespace Petal::Debug
{
	template <typename... Args>
	inline void print(const ::std::format_string<Args...> fmt, Args&&... args)
	{
		dout << ::std::vformat(fmt.get(), ::std::make_format_args(args...));
	}
	template <typename... Args>
	inline void print(const ::std::wformat_string<Args...> fmt, Args&&... args)
	{
		dowt << ::std::vformat(fmt.get(), ::std::make_wformat_args(args...));
	}
	inline void println()
	{
		dout << ln;
	}
	template <typename... Args>
	inline void println(const ::std::format_string<Args...> fmt, Args&&... args)
	{
		auto str = ::std::vformat(fmt.get(), ::std::make_format_args(args...));
		print("{}{}", str, GetLn<decltype(dout)::CharType>(dout.LnMode()));
	}
	template <typename... Args>
	inline void println(const ::std::wformat_string<Args...> fmt, Args&&... args)
	{
		auto str = ::std::vformat(fmt.get(), ::std::make_wformat_args(args...));
		print(L"{}{}", str, GetLn<decltype(dowt)::CharType>(dowt.LnMode()));
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
#define Petal_VSDebugOutputA(x) ::Petal::dout.WriteCStr( x )
#define Petal_VSDebugOutputW(x) ::Petal::dowt.WriteCStr( x )
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

// Implementation

namespace Petal::Debug
{
	inline void VSDebugOutputA::Write(BasicStringView<CharType, TraitsType> str)
	{
		BasicString<CharType, TraitsType> c_str{ str.data(), str.size() };
#ifdef Petal_Enable_ForceDbgRemoveNUL
		WriteCStr(StringToCStyleString(c_str).c_str());
#else
		WriteCStr(c_str.c_str());
#endif
	}
	inline void VSDebugOutputA::WriteCStr(CStringType c_str) noexcept
	{
		::OutputDebugStringA(c_str);
	}

	inline void VSDebugOutputW::Write(BasicStringView<CharType, TraitsType> str)
	{
		BasicString<CharType, TraitsType> c_str{ str.data(), str.size() };
#ifdef Petal_Enable_ForceDbgRemoveNUL
		WriteCStr(StringToCStyleString(c_str).c_str());
#else
		WriteCStr(c_str.c_str());
#endif
	}
	inline void VSDebugOutputW::WriteCStr(CStringType c_str) noexcept
	{
		::OutputDebugStringW(c_str);
	}
}

#endif // !Petal_Header_VSDebugOutput
