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
		using StringType = BasicString<CharType, TraitsType>;
		using StringViewType = BasicStringView<CharType, TraitsType>;
		using CStringRefType = BasicCStringRef<CharType>;
	public:
		virtual void Output(StringViewType str) = 0;
		virtual LineBreakMode LnMode() noexcept = 0;
	public:
		constexpr IOutput() = default;
		virtual ~IOutput() = default;
	};

	template <typename CharT, typename Traits>
	inline IOutput<CharT, Traits>& operator+(
		IOutput<CharT, Traits>& out,
		typename IOutput<CharT, Traits>::StringViewType str)
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
		using CStringType = ptrc<CharType>;
		using CStringRefType = BasicCStringRef<CharType>;
	public:
		virtual void OutputCStr(CStringType c_str) = 0;
		virtual LineBreakMode LnModeCStr() noexcept = 0;
	public:
		constexpr ICOutput() = default;
		virtual ~ICOutput() = default;
	};

	template <typename CharT>
	inline ICOutput<CharT>& operator-(
		ICOutput<CharT>& out,
		typename ICOutput<CharT>::CStringType c_str)
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

	template <typename CharT, typename Traits = ::std::char_traits<CharT>>
	class IBOutput : public IOutput<CharT, Traits>, public ICOutput<CharT>
	{
	private:
		using Base = IOutput<CharT, Traits>;
		using BaseC = ICOutput<CharT>;
		static_assert(
			::std::is_same_v<typename Base::CharType, typename BaseC::CharType>,
			"[Petal] Different CharType in Abstract::IBOutput");
		static_assert(
			::std::is_same_v<typename Base::CStringRefType, typename BaseC::CStringRefType>,
			"[Petal] Different CStringRefType in Abstract::IBOutput");
	public:
		using typename Base::CharType;
		using typename Base::TraitsType;
		using typename Base::StringType;
		using typename Base::StringViewType;
		using typename Base::CStringRefType;
		using typename BaseC::CStringType;
	public:
		virtual void Output(StringViewType str) = 0;
		virtual void OutputCStr(CStringType c_str) = 0;
		virtual LineBreakMode LnMode() noexcept = 0;
		virtual LineBreakMode LnModeCStr() noexcept = 0;
	};

	template <typename CharT, typename Traits>
	inline IBOutput<CharT, Traits>& operator+(
		IBOutput<CharT, Traits>& out,
		typename IBOutput<CharT, Traits>::StringViewType str)
	{
		out.Output(str);
		return out;
	}
	template <typename CharT, typename Traits>
	inline IBOutput<CharT, Traits>& operator+(
		IBOutput<CharT, Traits>& out,
		const fptr<void, IOutput<CharT, Traits>&> pfn)
	{
		pfn(out);
		return out;
	}
	template <typename CharT>
	inline IBOutput<CharT>& operator-(
		IBOutput<CharT>& out,
		typename IBOutput<CharT>::CStringType c_str)
	{
		out.OutputCStr(c_str);
		return out;
	}
	template <typename CharT>
	inline IBOutput<CharT>& operator-(
		IBOutput<CharT>& out,
		const fptr<void, ICOutput<CharT>&> pfn)
	{
		pfn(out);
		return out;
	}

	using IBOutputA = IBOutput<Char>;
	using IBOutputW = IBOutput<WChar>;
	using IBOutputU8 = IBOutput<U8Char>;
	using IBOutputU16 = IBOutput<U16Char>;
	using IBOutputU32 = IBOutput<U32Char>;
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
