#pragma once

#ifndef Petal_Header_String
#define Petal_Header_String

#include "Petal~BasicTypes.h"

#include <format>
#include <xstring>
#include <type_traits>

namespace Petal
{
	using Char = achar;
	using WChar = wchar;
	using U8Char = u8char;
	using U16Char = u16char;
	using U32Char = u32char;
	template <typename CharT, typename Traits = ::std::char_traits<CharT>, typename Alloc = ::std::allocator<CharT>>
	using BasicString = ::std::basic_string<CharT, Traits, Alloc>;
	using String = BasicString<Char>;
	using WString = BasicString<WChar>;
	using U8String = BasicString<U8Char>;
	using U16String = BasicString<U16Char>;
	using U32String = BasicString<U32Char>;
	template <typename CharT, typename Traits = ::std::char_traits<CharT>>
	using BasicStringView = ::std::basic_string_view<CharT, Traits>;
	using StringView = BasicStringView<Char>;
	using WStringView = BasicStringView<WChar>;
	using U8StringView = BasicStringView<U8Char>;
	using U16StringView = BasicStringView<U16Char>;
	using U32StringView = BasicStringView<U32Char>;

	inline constexpr String operator""_s(const Char* str, ::std::size_t length)
	{
		return { str, length };
	}
	inline constexpr WString operator""_s(const WChar* str, ::std::size_t length)
	{
		return { str, length };
	}
	inline constexpr U8String operator""_s(const U8Char* str, ::std::size_t length)
	{
		return { str, length };
	}
	inline constexpr U16String operator""_s(const U16Char* str, ::std::size_t length)
	{
		return { str, length };
	}
	inline constexpr U32String operator""_s(const U32Char* str, ::std::size_t length)
	{
		return { str, length };
	}

	inline constexpr StringView operator""_sv(const Char* str, ::std::size_t length)
	{
		return { str, length };
	}
	inline constexpr WStringView operator""_sv(const WChar* str, ::std::size_t length)
	{
		return { str, length };
	}
	inline constexpr U8StringView operator""_sv(const U8Char* str, ::std::size_t length)
	{
		return { str, length };
	}
	inline constexpr U16StringView operator""_sv(const U16Char* str, ::std::size_t length)
	{
		return { str, length };
	}
	inline constexpr U32StringView operator""_sv(const U32Char* str, ::std::size_t length)
	{
		return { str, length };
	}

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
	inline BasicString<CharT, Traits, Alloc> [[nodiscard]] StringToCStyleString(BasicStringView<CharT, Traits> in_str)
	{
		using InnerChar = CharT;
		using InnerString = BasicString<CharT, Traits, Alloc>;
		constexpr InnerChar null_char = InnerChar(EnumChar::null);
		InnerString c_str;
		c_str.resize(in_str.size(), null_char);
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
	template <typename CharT, typename Traits = ::std::char_traits<CharT>, typename Alloc = ::std::allocator<CharT>>
	inline BasicString<CharT, Traits, Alloc> [[nodiscard]] StringToCStyleString(const BasicString<CharT, Traits, Alloc>& in_str)
	{
		return StringToCStyleString(BasicStringView<CharT, Traits>{ in_str });
	}
	template <typename CharT, tsize char_arr_size, typename Traits = ::std::char_traits<CharT>, typename Alloc = ::std::allocator<CharT>>
	inline BasicString<CharT, Traits, Alloc> [[nodiscard]] StringToCStyleString(const CharT (&in_str)[char_arr_size])
	{
		return StringToCStyleString(BasicStringView<CharT, Traits>{ in_str, char_arr_size - 1 });
	}
	template <typename CharT, tsize char_arr_size, typename Traits = ::std::char_traits<CharT>, typename Alloc = ::std::allocator<CharT>>
	inline BasicString<CharT, Traits, Alloc> [[nodiscard]] StringToCStyleString(CharT (&in_str)[char_arr_size])
	{
		return StringToCStyleString(BasicStringView<CharT, Traits>{ in_str, char_arr_size - 1 });
	}

	template <typename Ty>
	class BasicCStringRef
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
	public:
		constexpr BasicCStringRef() = default;
		constexpr BasicCStringRef(ptrc<Ty> str, tsize length)
		{
			if (str[length] != 0)
			{
				throw ::std::exception{ "[Petal] expection: str is not c style string in Petal::BasicCStringRef<Ty>(ptrc<Ty>, tsize)" };
			}
			this->str_ptr = str;
			this->str_length = length;
		}
		constexpr BasicCStringRef(const BasicString<Ty>& ref_str)
		{
			this->str_ptr = ref_str.c_str();
			this->str_length = ref_str.length();
		}
		constexpr BasicCStringRef(const BasicCStringRef&) = default;
		constexpr ~BasicCStringRef() = default;
		constexpr BasicCStringRef& operator= (const BasicCStringRef str)
		{
			this->str_ptr = str.c_str();
			this->str_length = str.length();
			return *this;
		}
		template <typename Traits, typename Alloc>
		constexpr BasicCStringRef& operator= (const BasicString<Ty, Traits, Alloc>& ref_str)
		{
			this->str_ptr = ref_str.c_str();
			this->str_length = ref_str.length();
			return *this;
		}
		constexpr BasicCStringRef& operator= (::std::nullptr_t)
		{
			this->str_ptr = nullptr;
			this->str_length = 0;
			return *this;
		}
		operator bool()
		{
			return this->str_ptr;
		}
		operator const Ty*()
		{
			return this->str_ptr;
		}
		operator BasicStringView<Ty>()
		{
			return { this->c_str(), this->length() };
		}
	private:
		ptrc<Ty> str_ptr{ nullptr };
		tsize str_length{};
	};

	template <typename CharT, typename Traits = ::std::char_traits<CharT>, typename Alloc = ::std::allocator<CharT>>
	inline constexpr [[nodiscard]] BasicCStringRef<CharT>
		MakeCStringRef(const BasicString<CharT, Traits, Alloc>& ref_str)
	{
		return BasicCStringRef<CharT>{ ref_str.c_str(), ref_str.length() };
	}
	template <typename CharT, tsize char_arr_size>
	inline constexpr [[nodiscard]] BasicCStringRef<CharT>
		MakeCStringRef(const CharT (&ref_str)[char_arr_size])
	{
		return BasicCStringRef<CharT>{ ref_str, char_arr_size - 1 };
	}
	template <typename CharT, tsize char_arr_size>
	inline constexpr [[nodiscard]] BasicCStringRef<CharT>
		MakeCStringRef(CharT (&ref_str)[char_arr_size])
	{
		return BasicCStringRef<CharT>{ ref_str, char_arr_size - 1 };
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

#if defined(Petal_Enable_Unicode)
	using TCStringRef = WCStringRef;
#else
	using TCStringRef = CStringRef;
#endif

#if defined(Petal_Enable_ForceDbgANSI)
	using DbgChar = Char;
	using DbgString = String;
	using DbgStringView = StringView;
	using DbgCStringRef = CStringRef;
#else
	using DbgChar = TChar;
	using DbgString = TString;
	using DbgStringView = TStringView;
	using DbgCStringRef = TCStringRef;
#endif
}

template<typename CharT>
struct std::formatter<::Petal::BasicCStringRef<CharT>, CharT> :
	std::formatter<::std::basic_string_view<CharT>, CharT> { };

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
