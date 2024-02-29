#include "Petal~Bom.h"

namespace Petal
{
	namespace BOM
	{
		const Bom no_bom{ 0, 0x00u, 0x00u, 0x00u, 0x00u };
		const Bom utf_8{ 3, 0xEFu, 0xBBu, 0xBFu, 0x00u };
		const Bom utf_16_le{ 2, 0xFFu, 0xFEu, 0x00u, 0x00u };
		const Bom utf_16_be{ 2, 0xFEu, 0xFFu, 0x00u, 0x00u };
		const Bom utf_32_le{ 4, 0xFFu, 0xFEu, 0x00u, 0x00u };
		const Bom utf_32_be{ 4, 0x00u, 0x00u, 0xFEu, 0xFFu };
		Bom::Bom(tsize size, byte b1, byte b2, byte b3, byte b4) noexcept :
			data{ b1, b2, b3, b4 },
			size{ (size > 4) ? 4 : ((size < 0) ? 0 : size) } {}
		ptrc<byte> Bom::Data() const noexcept
		{
			return this->data;
		}
		tsize Bom::Size() const noexcept
		{
			return this->size;
		}
	}
}
