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
	class OutputLineBreakMode
	{
	public:
		LineBreakMode line_break_mode{ LineBreakMode::Default };
	};

	template <typename CharT, typename Traits = ::std::char_traits<CharT>, typename Alloc = ::std::allocator<CharT>>
	class BasicOutput : virtual public OutputLineBreakMode
	{
	public:
		using InnerChar = CharT;
		using InnerString = BasicString<InnerChar, Traits, Alloc>;
		using InnerStringView = BasicStringView<InnerChar, Traits>;
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
		inline virtual ~BasicOutput() = default;
	};
	using OutputA = BasicOutput<Char>;
	using OutputW = BasicOutput<WChar>;
	using OutputU8 = BasicOutput<U8Char>;
	using OutputU16 = BasicOutput<U16Char>;
	using OutputU32 = BasicOutput<U32Char>;

	template <typename CharT>
	class BasicCOutput : virtual public OutputLineBreakMode
	{
	public:
		using InnerChar = CharT;
	public:
		virtual void OutputCStr(ptrc<InnerChar> c_str) = 0;
		inline virtual BasicCOutput& operator-(ptrc<InnerChar> c_str)
		{
			this->OutputCStr(c_str);
			return *this;
		}
		inline virtual BasicCOutput& operator-(const fptr<void, BasicCOutput&> pfn)
		{
			pfn(*this);
			return *this;
		}
	};
	using COutputA = BasicCOutput<Char>;
	using COutputW = BasicCOutput<WChar>;
	using COutputU8 = BasicCOutput<U8Char>;
	using COutputU16 = BasicCOutput<U16Char>;
	using COutputU32 = BasicCOutput<U32Char>;
}

namespace Petal
{
	template <typename CharT>
	inline constexpr BasicCStringRef<CharT> GetLn(LineBreakMode mode) noexcept
	{
		using InnerChar = CharT;
		constexpr InnerChar cr{ static_cast<InnerChar>(EnumChar::cr) };
		constexpr InnerChar lf{ static_cast<InnerChar>(EnumChar::lf) };
		constexpr InnerChar nul{ static_cast<InnerChar>(EnumChar::null) };
		static constexpr const InnerChar CRLF[4]{ cr, lf, nul, nul };
		static constexpr const InnerChar LF[2]{ lf, nul };
		static constexpr const InnerChar CR[2]{ cr, nul };
		static constexpr tsize LEN_CRLF{ 2 };
		static constexpr tsize LEN_LF{ 1 };
		static constexpr tsize LEN_CR{ 1 };

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
		return { CRLF + LEN_CRLF, 0 };
	}
	template <typename CharT, typename Traits = ::std::char_traits<CharT>, typename Alloc = ::std::allocator<CharT>>
	inline void ln(Abstract::BasicOutput<CharT, Traits, Alloc>& output) noexcept(noexcept(GetLn<CharT>({})) && noexcept(output.Output({})))
	{
		output.Output(static_cast<BasicStringView<CharT, Traits>>(GetLn<CharT>(output.line_break_mode)));
	};
	template <typename CharT>
	inline void ln(Abstract::BasicCOutput<CharT>& output) noexcept(noexcept(GetLn<CharT>({})) && noexcept(output.OutputCStr({})))
	{
		output.OutputCStr(GetLn<CharT>(output.line_break_mode).c_str());
	};
}

#endif // !Petal_Header_Output
