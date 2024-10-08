#pragma once

#ifndef Petal_Header_Bom
#define Petal_Header_Bom

#include "Petal~BasicTypes.h"

#include <type_traits>
#include <bit>

namespace Petal::BOM
{
	class Bom final
	{
	public:
		ptrc<byte> Data() const noexcept
		{
			return this->data;
		}
		tsize Size() const noexcept
		{
			return this->size;
		}
	public:
		Bom(byte b1, byte b2, byte b3, byte b4) noexcept : data{ b1, b2, b3, b4 }, size{ 4 } {}
		Bom(byte b1, byte b2, byte b3) noexcept : data{ b1, b2, b3 }, size{ 3 } {}
		Bom(byte b1, byte b2) noexcept : data{ b1, b2 }, size{ 2 } {}
		Bom(byte b1) noexcept : data{ b1 }, size{ 1 } {}
		Bom() noexcept {}
		Bom(const Bom&) noexcept = default;
		Bom(Bom&&) noexcept = default;
		~Bom() = default;
	private:
		byte data[8]{};
		tsize size{ 0 };
	};

	inline const Bom no_bom{};
	inline const Bom utf_8{ 0xEFu, 0xBBu, 0xBFu };
	inline const Bom utf_16_le{ 0xFFu, 0xFEu };
	inline const Bom utf_16_be{ 0xFEu, 0xFFu };
	inline const Bom utf_32_le{ 0xFFu, 0xFEu, 0x00u, 0x00u };
	inline const Bom utf_32_be{ 0x00u, 0x00u, 0xFEu, 0xFFu };

	// Recommend BOM by template parameter CharT, std::endian::native and nttp force_utf_8_with_bom.
	// Never recommend BOM to char.
	// Assume that wchar_t uses unicode.
	template <typename CharT, boolean force_utf_8_with_bom = false>
	[[nodiscard]] constexpr const Bom& RecommendBom() noexcept
	{
		using char_type = typename ::std::remove_cvref_t<CharT>;
		constexpr boolean endian_le{ ::std::endian::native == ::std::endian::little };
		constexpr u32 utf_8_code{ 0x01u };
		constexpr u32 utf_16_code{ 0x02u };
		constexpr u32 utf_32_code{ 0x04u };
		u32 bom_code{ 0u };

		bom_code |= ::std::is_same_v<char8_t, char_type>* utf_8_code;
		bom_code |= ::std::is_same_v<char16_t, char_type>* utf_16_code;
		bom_code |= ::std::is_same_v<char32_t, char_type>* utf_32_code;
		bom_code |= ::std::is_same_v<wchar_t, char_type>* (0x01u << static_cast<u32>(sizeof(wchar_t) / 2));

		switch (bom_code)
		{
		case utf_8_code:
			return force_utf_8_with_bom ? utf_8 : no_bom;
			break;
		case utf_16_code:
			return endian_le ? utf_16_le : utf_16_be;
			break;
		case utf_32_code:
			return endian_le ? utf_32_le : utf_32_be;
			break;
		default:
			break;
		}

		return no_bom;
	}
}

#endif // !Petal_Header_Bom
