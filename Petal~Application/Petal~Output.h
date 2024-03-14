#pragma once

#ifndef Petal_Header_Output
#define Petal_Header_Output

#include "Petal~BasicTypes.h"
#include "Petal~String.h"

namespace Petal
{
	enum class LineBreakMode
	{
		CRLF = 1,
		CR = 2,
		LF = 3,
		Default = CRLF,
	};
}

namespace Petal::Abstract
{
	template <typename CharT, typename Traits = ::std::char_traits<CharT>>
	class IOutput
	{
	public:
		using CharType = CharT;
		using TraitsType = Traits;
		using InnerChar = CharT;
		using InnerString = BasicString<InnerChar, Traits>;
		using InnerStringView = BasicStringView<InnerChar, Traits>;
		using InnerCStringRef = BasicCStringRef<InnerChar>;
	public:
		virtual void Output(InnerStringView str) = 0;
		virtual LineBreakMode LnMode() noexcept = 0;
	public:
		constexpr IOutput() = default;
		virtual ~IOutput() = default;
	};

	template <typename CharT, typename Traits>
	inline IOutput<CharT, Traits>& operator+(
		IOutput<CharT, Traits>& out,
		typename IOutput<CharT, Traits>::InnerStringView str)
	{
		out.Output(str);
		return out;
	}
	template <typename CharT, typename Traits>
	inline IOutput<CharT, Traits>& operator+(
		IOutput<CharT, Traits>& out,
		const fptr<void, IOutput<CharT, Traits>&> pfn)
	{
		pfn(out);
		return out;
	}

	using IOutputA = IOutput<Char>;
	using IOutputW = IOutput<WChar>;
	using IOutputU8 = IOutput<U8Char>;
	using IOutputU16 = IOutput<U16Char>;
	using IOutputU32 = IOutput<U32Char>;

	template <typename CharT>
	class ICOutput
	{
	public:
		using CharType = CharT;
		using InnerChar = CharT;
		using InnerCStringRef = BasicCStringRef<InnerChar>;
	public:
		virtual void OutputCStr(ptrc<InnerChar> c_str) = 0;
		virtual LineBreakMode LnModeCStr() noexcept = 0;
	public:
		constexpr ICOutput() = default;
		virtual ~ICOutput() = default;
	};

	template <typename CharT>
	inline ICOutput<CharT>& operator-(
		ICOutput<CharT>& out,
		ptrc<typename ICOutput<CharT>::InnerChar> c_str)
	{
		out.OutputCStr(c_str);
		return out;
	}
	template <typename CharT>
	inline ICOutput<CharT>& operator-(
		ICOutput<CharT>& out,
		const fptr<void, ICOutput<CharT>&> pfn)
	{
		pfn(out);
		return out;
	}

	using ICOutputA = ICOutput<Char>;
	using ICOutputW = ICOutput<WChar>;
	using ICOutputU8 = ICOutput<U8Char>;
	using ICOutputU16 = ICOutput<U16Char>;
	using ICOutputU32 = ICOutput<U32Char>;
}

namespace Petal
{
	template <typename CharT>
	inline constexpr BasicCStringRef<CharT> GetLn(LineBreakMode mode) noexcept
	{
		using InnerChar = CharT;
		constexpr InnerChar cr{ InnerChar{ EnumChar::cr } };
		constexpr InnerChar lf{ InnerChar{ EnumChar::lf } };
		constexpr InnerChar nul{ InnerChar{ EnumChar::null } };
		constexpr tsize LEN_CRLF{ 2 };
		constexpr tsize LEN_LF{ 1 };
		constexpr tsize LEN_CR{ 1 };

		static const InnerChar CRLF[4]{ cr, lf, nul, nul };
		static const InnerChar LF[2]{ lf, nul };
		static const InnerChar CR[2]{ cr, nul };

		switch (mode)
		{
		case LineBreakMode::CRLF:
			return { CRLF, LEN_CRLF };
			break;
		case LineBreakMode::CR:
			return { CR, LEN_CR };
			break;
		case LineBreakMode::LF:
			return { LF, LEN_LF };
			break;
		default:
			break;
		}
#ifdef WIN32
		return { CRLF, LEN_CRLF };
#else
		return { LF, LEN_LF };
#endif
	}
	template <typename CharT, typename Traits = ::std::char_traits<CharT>>
	inline void ln(Abstract::IOutput<CharT, Traits>& output) noexcept(noexcept(GetLn<CharT>({})) && noexcept(output.Output({})))
	{
		output.Output(GetLn<CharT>(output.LnMode()).view());
	};
	template <typename CharT>
	inline void ln(Abstract::ICOutput<CharT>& output) noexcept(noexcept(GetLn<CharT>({})) && noexcept(output.OutputCStr({})))
	{
		output.OutputCStr(GetLn<CharT>(output.LnModeCStr()).c_str());
	};
}

#endif // !Petal_Header_Output
