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
	using StringU8 = typename ::std::basic_string<CharU8>;
	using StringU16 = typename ::std::basic_string<CharU16>;
	using StringU32 = typename ::std::basic_string<CharU32>;

#if defined(Petal_Enable_Unicode)
	using TChar = WChar;
	using TString = WString;
#else
	using TChar = Char;
	using TString = String;
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

#if defined(Petal_Enable_ForceExcepDescANSI)
	using ExcepChar = Char;
#else
	using ExcepChar = TChar;
#endif
}

namespace Petal::TypeTraits
{
	template <typename Ty>
	struct IsString : ::std::false_type {};

	template <typename Ty, typename Traits, typename Alloc>
	struct IsString<::std::basic_string<Ty, Traits, Alloc>> : ::std::true_type { using ElemType = Ty; };

	template <typename Ty, typename Traits, typename Alloc>
	struct IsString<const ::std::basic_string<Ty, Traits, Alloc>> : ::std::true_type { using ElemType = Ty; };

	template <typename Ty, typename Traits, typename Alloc>
	struct IsString<volatile ::std::basic_string<Ty, Traits, Alloc>> : ::std::true_type { using ElemType = Ty; };

	template <typename Ty, typename Traits, typename Alloc>
	struct IsString<const volatile ::std::basic_string<Ty, Traits, Alloc>> : ::std::true_type { using ElemType = Ty; };

	template <typename Ty>
	constexpr bool is_char_type{ ::std::is_same_v<::std::remove_cv_t<Ty>, Char> || ::std::is_same_v<::std::remove_cv_t<Ty>, WChar> || ::std::is_same_v<::std::remove_cv_t<Ty>, CharU8> || ::std::is_same_v<::std::remove_cv_t<Ty>, CharU16> || ::std::is_same_v<::std::remove_cv_t<Ty>, CharU32> };

	template <typename Ty>
	constexpr bool is_string{ IsString<Ty>::value };

	template <typename Ty>
	constexpr bool is_string_of_char_type{ is_string<Ty> && is_char_type<typename IsString<Ty>::ElemType> };
	
	template <typename Ty>
	constexpr bool is_tchar{ ::std::is_same_v<::std::remove_cv_t<Ty>, TChar> };

	template <typename Ty>
	constexpr bool is_dbg_char{ ::std::is_same_v<::std::remove_cv_t<Ty>, DbgChar> };

	template <typename Ty>
	constexpr bool is_excep_char{ ::std::is_same_v<::std::remove_cv_t<Ty>, ExcepChar> };
}

namespace Petal::Concept
{
	template <typename Ty>
	concept CharType = TypeTraits::is_char_type<Ty>;

	template <typename Ty>
	concept String = TypeTraits::is_string<Ty>;

	template <typename Ty>
	concept StringOfCharType = TypeTraits::is_string_of_char_type<Ty>;
}

#ifdef Petal_TStr
#undef Petal_TStr
#endif
#ifdef Petal_DbgStr
#undef Petal_DbgStr
#endif
#ifdef Petal_ExcepCStr
#undef Petal_ExcepCStr
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

#if defined(Petal_Enable_ForceExcepDescANSI)
#define Petal_ExcepStr(quote) quote
#else
#define Petal_ExcepStr(quote) Petal_TStr(quote)
#endif

#endif // !Petal_Header_String
