#pragma once

#ifndef Petal_Header_Types
#define Petal_Header_Types

#include "Petal~BasicTypes.h"

namespace Petal
{
	template <typename Ty>
	struct Size2D
	{
		Ty width{};
		Ty height{};
		friend bool operator==(const Size2D&, const Size2D&) noexcept = default;
	};
	using Size2DT = Size2D<tsize>;
	using Size2DI32 = Size2D<i32>;
	using Size2DU32 = Size2D<u32>;
	using Size2DF32 = Size2D<f32>;

	template <typename Ty>
	struct Position2D
	{
		Ty x{};
		Ty y{};
		friend bool operator==(const Position2D&, const Position2D&) noexcept = default;
	};
	using Position2DI32 = Position2D<i32>;
	using Position2DU32 = Position2D<u32>;
	using Position2DF32 = Position2D<f32>;

	template <typename Ty>
	struct Position3D
	{
		Ty x{};
		Ty y{};
		Ty z{};
		friend bool operator==(const Position3D&, const Position3D&) noexcept = default;
	};
	using Position3DI32 = Position3D<i32>;
	using Position3DU32 = Position3D<u32>;
	using Position3DF32 = Position3D<f32>;

	template <typename Ty>
	struct Rectangle
	{
		Position2D<Ty> left_top;
		Position2D<Ty> right_bottom;
		friend bool operator==(const Rectangle&, const Rectangle&) noexcept = default;
	};
	using RectangleI32 = Rectangle<i32>;
	using RectangleU32 = Rectangle<u32>;
	using RectangleF32 = Rectangle<f32>;
}

#endif // !Petal_Header_Types
