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
		ptrc<byte> Data() const noexcept;
		tsize Size() const noexcept;
	public:
		Bom(tsize size, byte b1 = 0, byte b2 = 0, byte b3 = 0, byte b4 = 0) noexcept;
		Bom(const Bom&) noexcept = default;
		Bom(Bom&&) noexcept = default;
		~Bom() = default;
	private:
		tsize size{ 0 };
		byte data[4]{};
	public:
	};

	extern const Bom no_bom;
	extern const Bom utf_8;
	extern const Bom utf_16_le;
	extern const Bom utf_16_be;
	extern const Bom utf_32_le;
	extern const Bom utf_32_be;

	template <typename CharT, boolean force_utf_8_with_bom = false>
	[[nodiscard]] constexpr const Bom& RecommendBom() noexcept
	{
		constexpr boolean endian_le{ ::std::endian::native == ::std::endian::little };
		constexpr u32 utf_8_code{ 0x01u };
		constexpr u32 utf_16_code{ 0x02u };
		constexpr u32 utf_32_code{ 0x04u };
		u32 bom_code{ 0u };

		bom_code |= ::std::is_same_v<char8_t, CharT>* utf_8_code;
		bom_code |= ::std::is_same_v<char16_t, CharT>* utf_16_code;
		bom_code |= ::std::is_same_v<char32_t, CharT>* utf_32_code;
		bom_code |= ::std::is_same_v<wchar_t, CharT>* (0x01u << static_cast<u32>(sizeof(wchar_t) / 2));

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
