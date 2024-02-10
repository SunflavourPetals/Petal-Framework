#pragma once

#ifndef Petal_Header_String
#define Petal_Header_String

#include "Petal~BasicTypes.h"

#include <iosfwd>
#include <format>
#include <string>
#include <compare>
#include <type_traits>

namespace Petal::EnumChar
{
	inline constexpr char cr{ '\r' };   // CR : Carriage Return
	inline constexpr char lf{ '\n' };   // LF : Line Feed
	inline constexpr char tab{ '\t' };  // HT : Horizontal Tab
	inline constexpr char vtab{ '\v' }; // VT : Vertical Tab
	inline constexpr char space{ ' ' }; // Space
	inline constexpr char null{ '\0' }; // NUL : Null Character
	inline constexpr char bell{ '\a' }; // BEL : Bell/Alert/Alarm
}
#include <Windows.h>
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

	template <typename CharT, typename Traits = ::std::char_traits<CharT>, typename Alloc = ::std::allocator<CharT>>
	[[nodiscard]] inline BasicString<CharT, Traits, Alloc> StringToCStyleString(BasicStringView<CharT, Traits> in_str)
	{
		using InnerChar = CharT;
		using InnerString = BasicString<CharT, Traits, Alloc>;
		constexpr InnerChar null_char = InnerChar(EnumChar::null);
		InnerString c_str;
		c_str.resize(in_str.size(), null_char);
		auto it_c = c_str.begin();
		for (const auto& e : in_str)
		{
			if (e != null_char)
			{
				(*it_c) = e;
				++it_c;
			}
		}
		c_str.erase(it_c, c_str.end());
		return c_str;
	}
	template <typename CharT, typename Traits = ::std::char_traits<CharT>, typename Alloc = ::std::allocator<CharT>>
	[[nodiscard]] inline auto StringToCStyleString(const BasicString<CharT, Traits, Alloc>& in_str)
		-> BasicString<CharT, Traits, Alloc>
	{
		return StringToCStyleString<CharT, Traits, Alloc>(BasicStringView<CharT, Traits>{ in_str });
	}
	template <tsize char_arr_size, typename InCharT,
		typename CharT = ::std::remove_const_t<InCharT>, typename Traits = ::std::char_traits<CharT>, typename Alloc = ::std::allocator<CharT>>
		requires ::std::is_same_v<CharT, ::std::remove_cv_t<CharT>>
	[[nodiscard]] inline auto StringToCStyleString(InCharT (&in_str)[char_arr_size])
		-> BasicString<CharT, Traits, Alloc>
	{
		return StringToCStyleString<CharT, Traits, Alloc>(BasicStringView<CharT, Traits>{ &in_str[0], char_arr_size });
	}

#if _ITERATOR_DEBUG_LEVEL >= 1
#ifndef Petal_Debug_CStringRefIterator
#define Petal_Debug_CStringRefIterator
#endif
#endif

	template <typename CharT>
	class BasicCStringRef;
	template <typename CharT>
	class CStringRefHash;
	template <class CharT>
	class CStringRefIterator;

	template <class CharT>
	class CStringRefIterator {
	public:
#ifdef __cpp_lib_concepts
		using iterator_concept = typename ::std::contiguous_iterator_tag;
#endif
		using iterator_category = typename ::std::random_access_iterator_tag;
		using value_type = CharT;
		using difference_type = typename ::std::ptrdiff_t;
		using pointer = const value_type*;
		using reference = const value_type&;

		constexpr CStringRefIterator() noexcept = default;
		constexpr CStringRefIterator(const CStringRefIterator&) noexcept = default;
		constexpr CStringRefIterator& operator= (const CStringRefIterator&)noexcept = default;
		constexpr ~CStringRefIterator() noexcept = default;

	private:
		friend BasicCStringRef<value_type>;

#ifdef Petal_Debug_CStringRefIterator
		constexpr CStringRefIterator(pointer ref_data, tsize ref_size, tsize ref_off) noexcept :
			data{ ref_data },
			size{ ref_size },
			offset{ ref_off } {}
#else
		constexpr explicit CStringRefIterator(pointer elem_ptr) noexcept :
			ptr{ elem_ptr } {}
#endif

	public:
		[[nodiscard]] constexpr reference operator*() const noexcept
		{
#ifdef Petal_Debug_CStringRefIterator
			_STL_VERIFY(this->data,
				"[Petal] Cannot dereference value-initialized BasicCStringRef iterator");
			_STL_VERIFY(this->offset < this->size,
				"[Petal] Cannot dereference end BasicCStringRef iterator");
			return this->data[this->offset];
#else
			return *(this->ptr);
#endif
		}

		[[nodiscard]] constexpr pointer operator->() const noexcept
		{
#ifdef Petal_Debug_CStringRefIterator
			_STL_VERIFY(this->data,
				"[Petal] Cannot dereference value-initialized BasicCStringRef iterator");
			_STL_VERIFY(this->offset < this->size,
				"[Petal] Cannot dereference end BasicCStringRef iterator");
			return this->data + this->offset;
#else
			return this->ptr;
#endif
		}

		constexpr CStringRefIterator& operator++() noexcept
		{
#ifdef Petal_Debug_CStringRefIterator
			_STL_VERIFY(this->data,
				"[Petal] Cannot increment value-initialized BasicCStringRef iterator");
			_STL_VERIFY(this->offset < this->size,
				"[Petal] Cannot increment BasicCStringRef iterator past end");
			++(this->offset);
#else
			++(this->ptr);
#endif
			return *this;
		}

		constexpr CStringRefIterator operator++(int) noexcept
		{
			auto temp = *this;
			++(*this);
			return temp;
		}

		constexpr CStringRefIterator& operator--() noexcept
		{
#ifdef Petal_Debug_CStringRefIterator
			_STL_VERIFY(this->data,
				"[Petal] Cannot decrement value-initialized BasicCStringRef iterator");
			_STL_VERIFY(this->offset != 0,
				"[Petal] Cannot decrement BasicCStringRef iterator before begin");
			--(this->offset);
#else
			--(this->ptr);
#endif
			return *this;
		}

		constexpr CStringRefIterator operator--(int) noexcept
		{
			auto temp = *this;
			--(*this);
			return temp;
		}

		constexpr CStringRefIterator& operator+=(difference_type off) noexcept
		{
#ifdef Petal_Debug_CStringRefIterator
			if (off != 0)
			{
				_STL_VERIFY(this->data,
					"[Petal] Cannot seek value-initialized BasicCStringRef iterator");
			}
			if (off < 0)
			{
				_STL_VERIFY(this->offset >= tsize{ 0 } - static_cast<tsize>(off),
					"[Petal] Cannot seek BasicCStringRef iterator before begin");
			}
			if (off > 0)
			{
				_STL_VERIFY(this->size - this->offset >= static_cast<tsize>(off),
					"[Petal] cannot seek BasicCStringRef iterator after end");
			}
			this->offset += static_cast<tsize>(off);
#else
			this->ptr += off;
#endif
			return *this;
		}

		[[nodiscard]] friend constexpr CStringRefIterator operator+
			(CStringRefIterator left, difference_type off) noexcept
		{
			left += off;
			return left;
		}

		[[nodiscard]] friend constexpr CStringRefIterator operator+
			(difference_type off, CStringRefIterator right) noexcept
		{
			right += off;
			return right;
		}

		constexpr CStringRefIterator& operator-=(difference_type off) noexcept
		{
#ifdef Petal_Debug_CStringRefIterator
			if (off != 0)
			{
				_STL_VERIFY(this->data,
					"[Petal] Cannot seek value-initialized BasicCStringRef iterator");
			}
			if (off > 0)
			{
				_STL_VERIFY(this->offset >= static_cast<size_t>(off),
					"[Petal] Cannot seek BasicCStringRef iterator before begin");
			}
			if (off < 0)
			{
				_STL_VERIFY(this->size - this->offset >= size_t{ 0 } - static_cast<size_t>(off),
					"[Petal] Cannot seek BasicCStringRef iterator after end");
			}
			this->offset -= static_cast<size_t>(off);
#else
			this->ptr -= off;
#endif
			return *this;
		}

		[[nodiscard]] constexpr CStringRefIterator operator-(difference_type off) const noexcept
		{
			auto temp = *this;
			temp -= off;
			return temp;
		}

		[[nodiscard]] constexpr difference_type operator-(const CStringRefIterator& right) const noexcept
		{
#ifdef Petal_Debug_CStringRefIterator
			_STL_VERIFY(this->data == right.data && this->size == right.size,
				"[Petal] Cannot subtract incompatible BasicCStringRef iterators");
			return static_cast<difference_type>(this->offset - right.offset);
#else
			return this->ptr - right.ptr;
#endif
		}

		[[nodiscard]] constexpr reference operator[](difference_type off) const noexcept
		{
			return *((*this) + off);
		}

		[[nodiscard]] friend constexpr bool operator==
			(const CStringRefIterator& left, const CStringRefIterator& right) noexcept
		{
#ifdef Petal_Debug_CStringRefIterator
			_STL_VERIFY(left.data == right.data && left.size == right.size,
				"[Petal] Cannot compare incompatible BasicCStringRef iterators for equality");
			return left.offset == right.offset;
#else
			return left.ptr == right.ptr;
#endif
		}

		[[nodiscard]] friend constexpr ::std::strong_ordering operator<=>
			(const CStringRefIterator& left, const CStringRefIterator& right) noexcept
		{
#ifdef Petal_Debug_CStringRefIterator
			if (left.data && right.data)
			{
				_STL_VERIFY(left.data == right.data && left.size == right.size,
					"[Petal] Cannot compare incompatible BasicCStringRef iterators for equality");
			}
			return left.offset <=> right.offset;
#else
			return left.ptr <=> right.ptr;
#endif
		}

	private:
#ifdef Petal_Debug_CStringRefIterator
		pointer data{};
		tsize size{};
		tsize offset{};
#else
		pointer ptr{};
#endif
	};
	template <typename CharT>
	class BasicCStringRef
	{
	public:
		using Hash = typename CStringRefHash<CharT>;
	public:
		using value_type = CharT;
		using pointer = typename ptr<CharT>;
		using const_pointer = typename ptrc<CharT>;
		using reference = CharT&;
		using const_reference = const CharT&;
		using const_iterator = CStringRefIterator<value_type>;
		using iterator = const_iterator;
		using const_reverse_iterator = typename ::std::reverse_iterator<const_iterator>;
		using reverse_iterator = const_reverse_iterator;
		using size_type = typename ::std::size_t;
		using difference_type = typename ::std::ptrdiff_t;
		using hasher = Hash;
	public:
		[[nodiscard]] constexpr size_type size() const noexcept
		{
			return this->str_length;
		}
		[[nodiscard]] constexpr size_type length() const noexcept
		{
			return this->str_length;
		}
		[[nodiscard]] constexpr const_pointer data() const noexcept
		{
			return this->str_ptr;
		}
		[[nodiscard]] constexpr const_pointer c_str() const noexcept
		{
			return this->str_ptr;
		}
		[[nodiscard]] constexpr bool empty() const noexcept
		{
			return this->size() == 0;
		}
		template <typename Traits = ::std::char_traits<CharT>>
		[[nodiscard]] constexpr BasicStringView<CharT, Traits> view() const noexcept
		{
			return BasicStringView<CharT, Traits>{ this->data(), this->size() };
		}
		template <typename Traits = ::std::char_traits<CharT>, typename Alloc = ::std::allocator<CharT>>
		[[nodiscard]] constexpr BasicString<CharT, Traits, Alloc> to_string() const
			noexcept(noexcept(BasicString<CharT, Traits, Alloc>(::std::declval<BasicCStringRef>().view())))
		{
			return BasicString<CharT, Traits, Alloc>{ this->view<Traits>() };
		}
		[[nodiscard]] constexpr const_iterator begin() const noexcept
		{
#ifdef Petal_Debug_CStringRefIterator
			return { this->data(), this->size(), 0 };
#else
			return const_iterator{ this->data() };
#endif
		}
		[[nodiscard]] constexpr const_iterator end() const noexcept
		{
#ifdef Petal_Debug_CStringRefIterator
			return { this->data(), this->size(), this->size() };
#else
			return const_iterator{ this->data() + this->size() };
#endif
		}
		[[nodiscard]] constexpr const_iterator cbegin() const noexcept
		{
			return begin();
		}
		[[nodiscard]] constexpr const_iterator cend() const noexcept
		{
			return end();
		}
		[[nodiscard]] constexpr const_reverse_iterator rbegin() const noexcept
		{
			return const_reverse_iterator{ end() };
		}
		[[nodiscard]] constexpr const_reverse_iterator rend() const noexcept
		{
			return const_reverse_iterator{ begin() };
		}
		[[nodiscard]] constexpr const_reverse_iterator crbegin() const noexcept
		{
			return rbegin();
		}
		[[nodiscard]] constexpr const_reverse_iterator crend() const noexcept
		{
			return rend();
		}
		[[nodiscard]] constexpr const CharT& at(size_type pos) const
		{
			if (pos >= this->size())
			{
				throw ::std::out_of_range{ "[Petal] exception: invalid CStringRef position" };
			}
			return this->c_str()[pos];
		}
	public:
		constexpr BasicCStringRef() = default;
		template <typename Traits, typename Alloc>
		constexpr BasicCStringRef(const BasicString<CharT, Traits, Alloc>& ref_str) :
			str_ptr{ ref_str.c_str() },
			str_length{ ref_str.length() }
		{

		}
		constexpr BasicCStringRef(const_pointer str, size_type length)
		{
			if (str[length] != 0)
			{
				throw ::std::exception{ "[Petal] exception: str is not c style string in Petal::BasicCStringRef<CharT>(const_pointer, size_type)" };
			}
			this->str_ptr = str;
			this->str_length = length;
		}
		constexpr BasicCStringRef(const BasicCStringRef&) = default;
		constexpr ~BasicCStringRef() = default;
		constexpr const CharT& operator[](size_type pos) const noexcept
		{
			return this->c_str()[pos];
		}
		constexpr BasicCStringRef& operator= (const BasicCStringRef str) noexcept
		{
			this->str_ptr = str.c_str();
			this->str_length = str.length();
			return *this;
		}
		template <typename Traits, typename Alloc>
		constexpr BasicCStringRef& operator= (const BasicString<CharT, Traits, Alloc>& ref_str) noexcept
		{
			this->str_ptr = ref_str.c_str();
			this->str_length = ref_str.length();
			return *this;
		}
		constexpr BasicCStringRef& operator= (::std::nullptr_t) noexcept
		{
			this->str_ptr = nullptr;
			this->str_length = 0;
			return *this;
		}
		constexpr operator bool() const noexcept
		{
			return this->str_ptr;
		}
		constexpr operator const CharT*() const noexcept
		{
			return this->str_ptr;
		}
		constexpr operator BasicStringView<CharT>() const
			noexcept(noexcept(::std::declval<BasicCStringRef>().view()))
		{
			return this->view();
		}
		constexpr operator BasicString<CharT>() const
			noexcept(noexcept(::std::declval<BasicCStringRef>().to_string()))
		{
			return this->to_string();
		}
	private:
		const_pointer str_ptr{ nullptr };
		size_type str_length{};
	};

	template <typename CharT>
	[[nodiscard]] constexpr bool operator== (const BasicCStringRef<CharT> lhs, const BasicCStringRef<CharT> rhs)
		noexcept(noexcept(lhs.view() == rhs.view()))
	{
		return lhs.view() == rhs.view();
	}

	template <typename CharT>
	[[nodiscard]] constexpr auto operator<=> (const BasicCStringRef<CharT> lhs, const BasicCStringRef<CharT> rhs)
		noexcept(noexcept(lhs.view() <=> rhs.view()))
	{
		return lhs.view() <=> rhs.view();
	}

	using CStringRef = BasicCStringRef<Char>;
	using WCStringRef = BasicCStringRef<WChar>;
	using U8CStringRef = BasicCStringRef<U8Char>;
	using U16CStringRef = BasicCStringRef<U16Char>;
	using U32CStringRef = BasicCStringRef<U32Char>;

	template <typename CharT>
	class CStringRefHash
	{
	public:
		using KeyTy = BasicCStringRef<CharT>;
		using ResultTy = ::std::size_t;
		[[nodiscard]] ResultTy operator()(const KeyTy& key_val) const
			noexcept(noexcept(HashValue({})))
		{
			return HashValue(key_val);
		}
		[[nodiscard]] static constexpr ResultTy HashValue(const KeyTy& key_val)
			noexcept(noexcept(::std::declval<::std::hash<::std::basic_string_view<CharT>>>()({})))
		{
			return ::std::hash<::std::basic_string_view<CharT>>{}(key_val.view());
		}
	};

	template <typename CharT, typename Traits = ::std::char_traits<CharT>>
	::std::basic_ostream<CharT, Traits>& operator<<(::std::basic_ostream<CharT, Traits>& out, const BasicCStringRef<CharT>& csr)
	{
		return out << csr.view();
	}

#if defined(Petal_Enable_Unicode)
	using TChar = WChar;
	using TString = WString;
	using TStringView = WStringView;
	using TCStringRef = WCStringRef;
#else
	using TChar = Char;
	using TString = String;
	using TStringView = StringView;
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

template <typename CharT>
struct ::std::formatter<::Petal::BasicCStringRef<CharT>, CharT> :
	::std::formatter<::std::basic_string_view<CharT>, CharT> { };

namespace Petal::TypeTraits
{
	template <typename Ty>
	constexpr bool is_tchar{ ::std::is_same_v<::std::remove_cv_t<Ty>, TChar> };

	template <typename Ty>
	constexpr bool is_dbg_char{ ::std::is_same_v<::std::remove_cv_t<Ty>, DbgChar> };
}

namespace Petal::StringLiterals
{
	inline constexpr String operator""_s(const Char * str, ::std::size_t length)
	{
		using namespace std::string_literals;
		return { str, length };
	}
	inline constexpr WString operator""_s(const WChar * str, ::std::size_t length)
	{
		return { str, length };
	}
	inline constexpr U8String operator""_s(const U8Char * str, ::std::size_t length)
	{
		return { str, length };
	}
	inline constexpr U16String operator""_s(const U16Char * str, ::std::size_t length)
	{
		return { str, length };
	}
	inline constexpr U32String operator""_s(const U32Char * str, ::std::size_t length)
	{
		return { str, length };
	}

	inline constexpr StringView operator""_sv(const Char * str, ::std::size_t length)
	{
		return { str, length };
	}
	inline constexpr WStringView operator""_sv(const WChar * str, ::std::size_t length)
	{
		return { str, length };
	}
	inline constexpr U8StringView operator""_sv(const U8Char * str, ::std::size_t length)
	{
		return { str, length };
	}
	inline constexpr U16StringView operator""_sv(const U16Char * str, ::std::size_t length)
	{
		return { str, length };
	}
	inline constexpr U32StringView operator""_sv(const U32Char * str, ::std::size_t length)
	{
		return { str, length };
	}

	inline constexpr CStringRef operator""_csr(const Char * str, ::std::size_t length)
	{
		return { str, length };
	}
	inline constexpr WCStringRef operator""_csr(const WChar * str, ::std::size_t length)
	{
		return { str, length };
	}
	inline constexpr U8CStringRef operator""_csr(const U8Char * str, ::std::size_t length)
	{
		return { str, length };
	}
	inline constexpr U16CStringRef operator""_csr(const U16Char * str, ::std::size_t length)
	{
		return { str, length };
	}
	inline constexpr U32CStringRef operator""_csr(const U32Char * str, ::std::size_t length)
	{
		return { str, length };
	}
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
