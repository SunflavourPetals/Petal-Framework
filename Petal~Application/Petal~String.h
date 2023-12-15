#pragma once

#ifndef Petal_Header_String
#define Petal_Header_String

#include "Petal~BasicTypes.h"

#include <xstring>
#include <type_traits>

namespace Petal
{
	using Char = achar;
	using WChar = wchar;
	using CharU8 = u8char;
	using CharU16 = u16char;
	using CharU32 = u32char;
	using String = typename ::std::basic_string<Char>;
	using WString = typename ::std::basic_string<WChar>;
	using U8String = typename ::std::basic_string<CharU8>;
	using U16String = typename ::std::basic_string<CharU16>;
	using U32String = typename ::std::basic_string<CharU32>;
	using StringView = typename ::std::basic_string_view<Char>;
	using WStringView = typename ::std::basic_string_view<WChar>;
	using U8StringView = typename ::std::basic_string_view<CharU8>;
	using U16StringView = typename ::std::basic_string_view<CharU16>;
	using U32StringView = typename ::std::basic_string_view<CharU32>;

#if defined(Petal_Enable_Unicode)
	using TChar = WChar;
	using TString = WString;
	using TStringView = WStringView;
#else
	using TChar = Char;
	using TString = String;
	using TStringView = StringView;
#endif

	namespace EnumChar
	{
		inline constexpr Char cr{ '\r' };   // CR : Carriage Return
		inline constexpr Char lf{ '\n' };   // LF : Line Feed
		inline constexpr Char tab{ '\t' };  // HT : Horizontal Tab
		inline constexpr Char vtab{ '\v' }; // VT : Vertical Tab
		inline constexpr Char space{ ' ' }; // Space
		inline constexpr Char null{ '\0' }; // NUL : Null Character
		inline constexpr Char bell{ '\a' }; // BEL : Bell/Alert/Alarm
	}

	template <typename CharT, typename Traits = ::std::char_traits<CharT>, typename Alloc = ::std::allocator<CharT>>
	inline ::std::basic_string<CharT, Traits, Alloc> [[nodiscard]] StringToCStyleString(const ::std::basic_string<CharT, Traits, Alloc>& in_str)
	{
		using InnerChar = CharT;
		using InnerString = ::std::basic_string<CharT, Traits, Alloc>;
		constexpr InnerChar null_char{ static_cast<InnerChar>(EnumChar::null) };
		InnerString c_str;
		c_str.resize(in_str.length(), null_char);
		tsize index{ 0 };
		for (const auto& e : in_str)
		{
			if (e != null_char)
			{
				c_str[index] = e;
				++index;
			}
		}
		return c_str;
	}

#if defined(Petal_Enable_ForceDbgANSI)
	using DbgChar = Char;
	using DbgString = String;
#else
	using DbgChar = TChar;
	using DbgString = TString;
#endif
}

namespace Petal::TypeTraits
{
	template <typename Ty>
	constexpr bool is_tchar{ ::std::is_same_v<::std::remove_cv_t<Ty>, TChar> };

	template <typename Ty>
	constexpr bool is_dbg_char{ ::std::is_same_v<::std::remove_cv_t<Ty>, DbgChar> };
}

#ifdef Petal_TStr
#undef Petal_TStr
#endif
#ifdef Petal_DbgStr
#undef Petal_DbgStr
#endif

#if defined(Petal_Enable_Unicode)
#define Petal_TStr(quote) L##quote
#else
#define Petal_TStr(quote) quote
#endif

#if defined(Petal_Enable_ForceDbgANSI)
#define Petal_DbgStr(quote) quote
#else
#define Petal_DbgStr(quote) Petal_TStr(quote)
#endif

#endif // !Petal_Header_String
