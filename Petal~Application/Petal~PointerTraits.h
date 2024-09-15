#pragma once

#ifndef Petal_Header_PointerTraits
#define Petal_Header_PointerTraits

#include <type_traits>
#include <memory>

namespace Petal::TypeTraits::Impl
{
	template <typename Ty, typename Dx = void>
	struct IsUniquePtr : std::false_type {};
	template <typename Ty, typename Dx>
	struct IsUniquePtr<::std::unique_ptr<Ty, Dx>> : std::true_type { };
	template <typename Ty, typename Dx>
	struct IsUniquePtr<const ::std::unique_ptr<Ty, Dx>> : std::true_type { };
	template <typename Ty, typename Dx>
	struct IsUniquePtr<volatile ::std::unique_ptr<Ty, Dx>> : std::true_type { };
	template <typename Ty, typename Dx>
	struct IsUniquePtr<const volatile ::std::unique_ptr<Ty, Dx>> : std::true_type { };
}

namespace Petal::TypeTraits
{
	// raw pointer

	template <typename Ty>
	struct IsRawPointer : ::std::bool_constant<::std::is_pointer_v<Ty>> {};

	template <typename Ty>
	constexpr bool is_raw_pointer{ IsRawPointer<Ty>::value };

	// unique pointer

	template <typename Ty>
	struct IsUniquePointer : std::bool_constant<Impl::IsUniquePtr<Ty>::value> {};

	template <typename Ty>
	constexpr bool is_unique_pointer{ IsUniquePointer<Ty>::value };
	
	// shared pointer

	template <typename>
	struct IsSharedPointer : ::std::false_type {};
	template <typename Ty>
	struct IsSharedPointer<::std::shared_ptr<Ty>> : ::std::true_type {};
	template <typename Ty>
	struct IsSharedPointer<const ::std::shared_ptr<Ty>> : ::std::true_type {};
	template <typename Ty>
	struct IsSharedPointer<volatile ::std::shared_ptr<Ty>> : ::std::true_type {};
	template <typename Ty>
	struct IsSharedPointer<const volatile ::std::shared_ptr<Ty>> : ::std::true_type {};

	template <typename Ty>
	constexpr bool is_shared_pointer{ IsSharedPointer<Ty>::value };

	// weak pointer

	template <typename>
	struct IsWeakPointer : ::std::false_type {};
	template <typename Ty>
	struct IsWeakPointer<::std::weak_ptr<Ty>> : ::std::true_type {};
	template <typename Ty>
	struct IsWeakPointer<const ::std::weak_ptr<Ty>> : ::std::true_type {};
	template <typename Ty>
	struct IsWeakPointer<volatile ::std::weak_ptr<Ty>> : ::std::true_type {};
	template <typename Ty>
	struct IsWeakPointer<const volatile ::std::weak_ptr<Ty>> : ::std::true_type {};

	template <typename Ty>
	constexpr bool is_weak_pointer{ IsWeakPointer<Ty>::value };

	// smart pointer

	template <typename Ty>
	constexpr bool is_smart_pointer{ is_unique_pointer<Ty> || is_shared_pointer<Ty> || is_weak_pointer<Ty> };

	template <typename Ty>
	struct IsSmartPointer : ::std::bool_constant<is_smart_pointer<Ty>> {};

	// any pointer

	template <typename Ty>
	constexpr bool is_any_pointer{ is_smart_pointer<Ty> || is_raw_pointer<Ty> };

	template <typename Ty>
	struct IsAnyPointer : ::std::bool_constant<is_any_pointer<Ty>> {};
}

namespace Petal::Concept
{
	template <typename Ty>
	concept RawPointer = TypeTraits::is_raw_pointer<Ty>;

	template <typename Ty>
	concept UniquePointer = TypeTraits::is_unique_pointer<Ty>;

	template <typename Ty>
	concept SharedPointer = TypeTraits::is_shared_pointer<Ty>;

	template <typename Ty>
	concept WeakPointer = TypeTraits::is_weak_pointer<Ty>;

	template <typename Ty>
	concept SmartPointer = TypeTraits::is_smart_pointer<Ty>;

	template <typename Ty>
	concept AnyPointer = TypeTraits::is_any_pointer<Ty>;
}

namespace Petal::TypeTraits
{
	// Element's type of pointer
	template <typename Ty>
	struct PointerElement {};
	template <Concept::RawPointer Ty>
	struct PointerElement<Ty> { using Type = ::std::remove_pointer_t<Ty>; };
	template <Concept::SmartPointer Ty>
	struct PointerElement<Ty> { using Type = typename Ty::element_type; };

	template <typename Ty>
	using PointerElementType = typename PointerElement<Ty>::Type;

	// Remove one raw pointer
	template <typename Ty> struct RemoveOneRawPointer { using Type = Ty; };
	template <Concept::RawPointer Ty> struct RemoveOneRawPointer<Ty> { using Type = PointerElementType<Ty>; };
	template <typename Ty> using RemoveOneRawPointerType = typename RemoveOneRawPointer<Ty>::Type;
	// Remove one unique pointer
	template <typename Ty> struct RemoveOneUniquePointer { using Type = Ty; };
	template <Concept::UniquePointer Ty> struct RemoveOneUniquePointer<Ty> { using Type = PointerElementType<Ty>; };
	template <typename Ty> using RemoveOneUniquePointerType = typename RemoveOneUniquePointer<Ty>::Type;
	// Remove one shared pointer
	template <typename Ty> struct RemoveOneSharedPointer { using Type = Ty; };
	template <Concept::SharedPointer Ty> struct RemoveOneSharedPointer<Ty> { using Type = PointerElementType<Ty>; };
	template <typename Ty> using RemoveOneSharedPointerType = typename RemoveOneSharedPointer<Ty>::Type;
	// Remove one weak pointer
	template <typename Ty> struct RemoveOneWeakPointer { using Type = Ty; };
	template <Concept::WeakPointer Ty> struct RemoveOneWeakPointer<Ty> { using Type = PointerElementType<Ty>; };
	template <typename Ty> using RemoveOneWeakPointerType = typename RemoveOneWeakPointer<Ty>::Type;
	// Remove one smart pointer
	template <typename Ty> struct RemoveOneSmartPointer { using Type = Ty; };
	template <Concept::SmartPointer Ty> struct RemoveOneSmartPointer<Ty> { using Type = PointerElementType<Ty>; };
	template <typename Ty> using RemoveOneSmartPointerType = typename RemoveOneSmartPointer<Ty>::Type;
	// Remove one any pointer
	template <typename Ty> struct RemoveOneAnyPointer { using Type = Ty; };
	template <Concept::AnyPointer Ty> struct RemoveOneAnyPointer<Ty> { using Type = PointerElementType<Ty>; };
	template <typename Ty> using RemoveOneAnyPointerType = typename RemoveOneAnyPointer<Ty>::Type;

	// Remove all raw pointer
	template <typename Ty> struct RemoveAllRawPointer { using Type = Ty; };
	template <Concept::RawPointer Ty> struct RemoveAllRawPointer<Ty> { using Type = typename RemoveAllRawPointer<RemoveOneRawPointerType<Ty>>::Type; };
	template <typename Ty> using RemoveAllRawPointerType = typename RemoveAllRawPointer<Ty>::Type;
	// Remove all unique pointer
	template <typename Ty> struct RemoveAllUniquePointer { using Type = Ty; };
	template <Concept::UniquePointer Ty> struct RemoveAllUniquePointer<Ty> { using Type = typename RemoveAllUniquePointer<RemoveOneUniquePointerType<Ty>>::Type; };
	template <typename Ty> using RemoveAllUniquePointerType = typename RemoveAllUniquePointer<Ty>::Type;
	// Remove all shared pointer
	template <typename Ty> struct RemoveAllSharedPointer { using Type = Ty; };
	template <Concept::SharedPointer Ty> struct RemoveAllSharedPointer<Ty> { using Type = typename RemoveAllSharedPointer<RemoveOneSharedPointerType<Ty>>::Type; };
	template <typename Ty> using RemoveAllSharedPointerType = typename RemoveAllSharedPointer<Ty>::Type;
	// Remove all weak pointer
	template <typename Ty> struct RemoveAllWeakPointer { using Type = Ty; };
	template <Concept::WeakPointer Ty> struct RemoveAllWeakPointer<Ty> { using Type = typename RemoveAllWeakPointer<RemoveOneWeakPointerType<Ty>>::Type; };
	template <typename Ty> using RemoveAllWeakPointerType = typename RemoveAllWeakPointer<Ty>::Type;
	// Remove all smart pointer
	template <typename Ty> struct RemoveAllSmartPointer { using Type = Ty; };
	template <Concept::SmartPointer Ty> struct RemoveAllSmartPointer<Ty> { using Type = typename RemoveAllSmartPointer<RemoveOneSmartPointerType<Ty>>::Type; };
	template <typename Ty> using RemoveAllSmartPointerType = typename RemoveAllSmartPointer<Ty>::Type;
	// Remove all any pointer
	template <typename Ty> struct RemoveAllAnyPointer { using Type = Ty; };
	template <Concept::AnyPointer Ty> struct RemoveAllAnyPointer<Ty> { using Type = typename RemoveAllAnyPointer<RemoveOneAnyPointerType<Ty>>::Type; };
	template <typename Ty> using RemoveAllAnyPointerType = typename RemoveAllAnyPointer<Ty>::Type;
}

#endif // !Petal_Header_PointerTraits
