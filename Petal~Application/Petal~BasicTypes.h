#pragma once

#ifndef Petal_Header_BasicTypes
#define Petal_Header_BasicTypes

#include <cstdint>
#include <cstddef>

namespace Petal
{
	using i8 = typename ::std::int8_t;
	using i16 = typename ::std::int16_t;
	using i32 = typename ::std::int32_t;
	using i64 = typename ::std::int64_t;
	using u8 = typename ::std::uint8_t;
	using u16 = typename ::std::uint16_t;
	using u32 = typename ::std::uint32_t;
	using u64 = typename ::std::uint64_t;
	using f32 = float;
	using f64 = double;
	using boolean = bool;
	using achar = char;
	using wchar = wchar_t;
	using u8char = char8_t;
	using u16char = char16_t;
	using u32char = char32_t;

	using tsize = typename ::std::size_t;

	using byte = u8;

	// pointer to object
	template <typename Ty> using ptr = Ty*;

	// pointer to const object
	template <typename Ty> using ptrc = const Ty*;

	// pointer to function
	template <typename ReturnT, typename... Args> using fptr = ReturnT(*)(Args...);
}

#endif // !Petal_Header_BasicTypes
