#pragma once

#ifndef Petal_Header_Output
#define Petal_Header_Output

#include "Petal~BasicTypes.h"
#include "Petal~String.h"

namespace Petal
{
	enum class LineBreakMode : i32
	{
		CRLF = 1,
		CR = 2,
		LF = 3,
		Default = CRLF,
	};
}

namespace Petal::Abstract
{
	template <typename CharT, typename Traits = ::std::char_traits<CharT>, typename Alloc = ::std::allocator<CharT>>
	class BasicOutput
	{
	public:
		using InnerChar = CharT;
		using InnerString = ::std::basic_string<InnerChar, Traits, Alloc>;
		using InnerStringView = ::std::basic_string_view<InnerChar>;
	public:
		virtual void Output(InnerStringView str) = 0;
	public:
		inline virtual BasicOutput& operator+(InnerStringView str)
		{
			this->Output(str);
			return *this;
		}
		inline virtual BasicOutput& operator+(const fptr<void, BasicOutput&> pfn)
		{
			pfn(*this);
			return *this;
		}
	public:
		LineBreakMode line_break_mode{ LineBreakMode::Default };
	public:
		inline virtual ~BasicOutput() = default;
	};
	using OutputA = BasicOutput<Char>;
	using OutputW = BasicOutput<WChar>;
	using OutputU8 = BasicOutput<CharU8>;
	using OutputU16 = BasicOutput<CharU16>;
	using OutputU32 = BasicOutput<CharU32>;

	template <typename CharT, typename Traits = ::std::char_traits<CharT>, typename Alloc = ::std::allocator<CharT>>
	class BasicCOutput : public BasicOutput<CharT, Traits, Alloc>
	{
	public:
		using InnerChar = CharT;
		using InnerString = ::std::basic_string<InnerChar, Traits, Alloc>;
		using InnerStringView = ::std::basic_string_view<InnerChar>;
	public:
		virtual void OutputCStr(ptrc<InnerChar> c_str) = 0;
	};
	using COutputA = BasicCOutput<Char>;
	using COutputW = BasicCOutput<WChar>;
	using COutputU8 = BasicCOutput<CharU8>;
	using COutputU16 = BasicCOutput<CharU16>;
	using COutputU32 = BasicCOutput<CharU32>;
}

namespace Petal
{
	template <typename CharT>
	inline ::std::basic_string_view<CharT> GetLn(LineBreakMode mode) noexcept
	{
		using InnerChar = CharT;
		static constexpr InnerChar cr{ static_cast<InnerChar>(EnumChar::cr) };
		static constexpr InnerChar lf{ static_cast<InnerChar>(EnumChar::lf) };
		static constexpr InnerChar nul{ static_cast<InnerChar>(EnumChar::null) };
		static constexpr InnerChar CRLF[4]{ cr, lf, nul, nul };
		static constexpr InnerChar LF[2]{ lf, nul };
		static constexpr InnerChar CR[2]{ cr, nul };
		static constexpr tsize C_CRLF{ 2 };
		static constexpr tsize C_LF{ 1 };
		static constexpr tsize C_CR{ 1 };

		switch (mode)
		{
		case LineBreakMode::CRLF:
			return { CRLF, C_CRLF };
			break;
		case LineBreakMode::CR:
			return { CR, C_CR };
			break;
		case LineBreakMode::LF:
			return { LF, C_LF };
			break;
		default:
			break;
		}
		return { &CRLF[2], 0 };
	}
	template <typename CharT, typename Traits = ::std::char_traits<CharT>, typename Alloc = ::std::allocator<CharT>>
	inline void ln(Abstract::BasicOutput<CharT, Traits, Alloc>& output) noexcept(noexcept(GetLn<CharT>({})) && noexcept(output.Output({})))
	{
		output.Output(GetLn<CharT>(output.line_break_mode));
	};
}

#endif // !Petal_Header_Output
