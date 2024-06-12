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

#define Petal_Generate_RemoveOneXPtr(X) \
		template <typename Ty> \
		struct RemoveOne##X##Pointer { using Type = Ty; }; \
		template <Concept::##X##Pointer Ty> \
		struct RemoveOne##X##Pointer<Ty> { using Type = PointerElementType<Ty>; }; \
		template <typename Ty> \
		using RemoveOne##X##PointerType = typename RemoveOne##X##Pointer<Ty>::Type

	Petal_Generate_RemoveOneXPtr(Raw);      // Remove one raw pointer
	Petal_Generate_RemoveOneXPtr(Unique);   // Remove one unique pointer
	Petal_Generate_RemoveOneXPtr(Shared);   // Remove one shared pointer
	Petal_Generate_RemoveOneXPtr(Weak);     // Remove one weak pointer
	Petal_Generate_RemoveOneXPtr(Smart);    // Remove one smart pointer
	Petal_Generate_RemoveOneXPtr(Any);      // Remove one any pointer
#undef Petal_Generate_RemoveOneXPtr

#define Petal_Generate_RemoveAllXPtr(X) \
		template <typename Ty> \
		struct RemoveAll##X##Pointer { using Type = Ty; }; \
		template <Concept::##X##Pointer Ty> \
		struct RemoveAll##X##Pointer<Ty> { using Type = typename RemoveAll##X##Pointer<RemoveOne##X##PointerType<Ty>>::Type; }; \
		template <typename Ty> \
		using RemoveAll##X##PointerType = typename RemoveAll##X##Pointer<Ty>::Type

	Petal_Generate_RemoveAllXPtr(Raw);      // Remove all raw pointer
	Petal_Generate_RemoveAllXPtr(Unique);   // Remove all unique pointer
	Petal_Generate_RemoveAllXPtr(Shared);   // Remove all shared pointer
	Petal_Generate_RemoveAllXPtr(Weak);     // Remove all weak pointer
	Petal_Generate_RemoveAllXPtr(Smart);    // Remove all smart pointer
	Petal_Generate_RemoveAllXPtr(Any);      // Remove all any pointer
#undef Petal_Generate_RemoveAllXPtr
}

#endif // !Petal_Header_PointerTraits
