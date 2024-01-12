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
	using U8Char = u8char;
	using U16Char = u16char;
	using U32Char = u32char;
	using String = typename ::std::basic_string<Char>;
	using WString = typename ::std::basic_string<WChar>;
	using U8String = typename ::std::basic_string<U8Char>;
	using U16String = typename ::std::basic_string<U16Char>;
	using U32String = typename ::std::basic_string<U32Char>;
	using StringView = typename ::std::basic_string_view<Char>;
	using WStringView = typename ::std::basic_string_view<WChar>;
	using U8StringView = typename ::std::basic_string_view<U8Char>;
	using U16StringView = typename ::std::basic_string_view<U16Char>;
	using U32StringView = typename ::std::basic_string_view<U32Char>;

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
		c_str.erase(c_str.begin() + index, c_str.end());
		return c_str;
	}

#if defined(Petal_Enable_ForceDbgANSI)
	using DbgChar = Char;
	using DbgString = String;
	using DbgStringView = StringView;
#else
	using DbgChar = TChar;
	using DbgString = TString;
	using DbgStringView = TStringView;
#endif

	template <typename Ty>
	class BasicCStringRef final
	{
	public:
		constexpr tsize size() const noexcept
		{
			return this->str_length;
		}
		constexpr tsize length() const noexcept
		{
			return this->str_length;
		}
		constexpr ptrc<Ty> data() const noexcept
		{
			return this->str_ptr;
		}
		constexpr ptrc<Ty> c_str() const noexcept
		{
			return this->str_ptr;
		}

		constexpr BasicCStringRef() = default;
		constexpr BasicCStringRef(ptrc<Ty> str, tsize length)
		{
			if (str[length] != 0)
			{
				throw ::std::exception{ "[Petal] std::expection: str is not c style string in Petal::BasicCStringRef<Ty>(ptrc<Ty>, tsize)" };
			}
			this->str_ptr = str;
			this->str_length = length;
		}
		constexpr BasicCStringRef(const ::std::basic_string<Ty>& ref_str)
		{
			this->str_ptr = ref_str.c_str();
			this->str_length = ref_str.length();
		}

		constexpr const BasicCStringRef& operator= (const BasicCStringRef str)
		{
			this->str_ptr = str.c_str();
			this->str_length = str.length();
			return *this;
		}
		template <typename Traits, typename Alloc>
		constexpr const BasicCStringRef& operator= (const ::std::basic_string<Ty, Traits, Alloc>& ref_str)
		{
			this->str_ptr = ref_str.c_str();
			this->str_length = ref_str.length();
			return *this;
		}

		constexpr const BasicCStringRef& operator= (::std::nullptr_t)
		{
			this->str_ptr = nullptr;
			this->str_length = 0;
			return *this;
		}

		operator bool()
		{
			return this->str_ptr;
		}

		operator ::std::basic_string_view<Ty>()
		{
			return { c_str(), length() };
		}
	private:
		ptrc<Ty> str_ptr{ nullptr };
		tsize str_length{};
	};

	template <typename CharT, typename Traits = ::std::char_traits<CharT>, typename Alloc = ::std::allocator<CharT>>
	inline constexpr [[nodiscard]] BasicCStringRef<CharT>
		MakeCStringRef(const ::std::basic_string<CharT, Traits, Alloc>& ref_str)
	{
		return BasicCStringRef<CharT>{ ref_str.c_str(), ref_str.length() };
	}

	using CStringRef = BasicCStringRef<Char>;
	using WCStringRef = BasicCStringRef<WChar>;
	using U8CStringRef = BasicCStringRef<U8Char>;
	using U16CStringRef = BasicCStringRef<U16Char>;
	using U32CStringRef = BasicCStringRef<U32Char>;

	inline constexpr CStringRef operator""_csr(const Char* str, ::std::size_t length)
	{
		return { str, length };
	}
	inline constexpr WCStringRef operator""_csr(const WChar* str, ::std::size_t length)
	{
		return { str, length };
	}
	inline constexpr U8CStringRef operator""_csr(const U8Char* str, ::std::size_t length)
	{
		return { str, length };
	}
	inline constexpr U16CStringRef operator""_csr(const U16Char* str, ::std::size_t length)
	{
		return { str, length };
	}
	inline constexpr U32CStringRef operator""_csr(const U32Char* str, ::std::size_t length)
	{
		return { str, length };
	}
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
