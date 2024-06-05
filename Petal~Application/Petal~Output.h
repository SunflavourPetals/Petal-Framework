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
	class Output
	{
	public:
		using CharType = CharT;
		using TraitsType = Traits;
		using StringType = BasicString<CharType, TraitsType>;
		using StringViewType = BasicStringView<CharType, TraitsType>;
		using CStringRefType = BasicCStringRef<CharType>;
	public:
		virtual void Write(StringViewType str) = 0;
		virtual LineBreakMode LnMode() noexcept = 0;
		friend Output& operator<<(Output& out, typename StringViewType str)
		{
			out.Write(str);
			return out;
		}
		friend Output& operator<<(Output& out, const fptr<void, Output&> pfn)
		{
			pfn(out);
			return out;
		}
	public:
		constexpr Output() = default;
		virtual ~Output() = default;
	};

	using IOutputA = Output<Char>;
	using IOutputW = Output<WChar>;
	using IOutputU8 = Output<U8Char>;
	using IOutputU16 = Output<U16Char>;
	using IOutputU32 = Output<U32Char>;
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
	inline void ln(Abstract::Output<CharT, Traits>& output)
	{
		output.Write(GetLn<CharT>(output.LnMode()).view());
	};
}

#endif // !Petal_Header_Output
