#pragma once

#ifndef Petal_Header_SmartPointer
#define Petal_Header_SmartPointer

#include <memory>

namespace Petal::TypeTraits
{
	// raw pointer

	template <typename Ty>
	struct IsRawPointer : ::std::false_type {};

	template <typename Ty>
	struct IsRawPointer<Ty*> : ::std::true_type {};

	template <typename Ty>
	struct IsRawPointer<Ty* const> : ::std::true_type {};

	template <typename Ty>
	struct IsRawPointer<Ty* volatile> : ::std::true_type {};

	template <typename Ty>
	struct IsRawPointer<Ty* const volatile> : ::std::true_type {};

	template <typename Ty>
	constexpr bool is_raw_pointer{ IsRawPointer<Ty>::value };

	// smart pointer

	template <typename Ty>
	struct IsSmartPointer : ::std::false_type {};

	template <typename Ty>
	struct IsSmartPointer<::std::unique_ptr<Ty>> : ::std::true_type {};
	template <typename Ty>
	struct IsSmartPointer<const ::std::unique_ptr<Ty>> : ::std::true_type {};
	template <typename Ty>
	struct IsSmartPointer<volatile ::std::unique_ptr<Ty>> : ::std::true_type {};
	template <typename Ty>
	struct IsSmartPointer<const volatile ::std::unique_ptr<Ty>> : ::std::true_type {};

	template <typename Ty>
	struct IsSmartPointer<::std::shared_ptr<Ty>> : ::std::true_type {};
	template <typename Ty>
	struct IsSmartPointer<const ::std::shared_ptr<Ty>> : ::std::true_type {};
	template <typename Ty>
	struct IsSmartPointer<volatile ::std::shared_ptr<Ty>> : ::std::true_type {};
	template <typename Ty>
	struct IsSmartPointer<const volatile ::std::shared_ptr<Ty>> : ::std::true_type {};

	template <typename Ty>
	struct IsSmartPointer<::std::weak_ptr<Ty>> : ::std::true_type {};
	template <typename Ty>
	struct IsSmartPointer<const ::std::weak_ptr<Ty>> : ::std::true_type {};
	template <typename Ty>
	struct IsSmartPointer<volatile ::std::weak_ptr<Ty>> : ::std::true_type {};
	template <typename Ty>
	struct IsSmartPointer<const volatile ::std::weak_ptr<Ty>> : ::std::true_type {};

	template <typename Ty>
	constexpr bool is_smart_pointer{ IsSmartPointer<Ty>::value };

	// generic pointer

	template <typename Ty>
	constexpr bool is_generic_pointer{ is_smart_pointer<Ty> || is_raw_pointer<Ty> };
}

namespace Petal::Concept
{
	template <typename Ty>
	concept RawPointer = TypeTraits::is_raw_pointer<Ty>;

	template <typename Ty>
	concept SmartPointer = TypeTraits::is_smart_pointer<Ty>;

	template <typename Ty>
	concept GenericPointer = TypeTraits::is_generic_pointer<Ty>;
}

namespace Petal::TypeTraits
{
	// Remove one raw pointer
	
	template <typename Ty>
	struct RemoveOneRawPointer
	{
		using Type = Ty;
	};

	template <typename Ty>
	struct RemoveOneRawPointer<Ty*>
	{
		using Type = Ty;
	};

	template <typename Ty>
	struct RemoveOneRawPointer<Ty* const>
	{
		using Type = Ty;
	};

	template <typename Ty>
	struct RemoveOneRawPointer<Ty* volatile>
	{
		using Type = Ty;
	};

	template <typename Ty>
	struct RemoveOneRawPointer<Ty* const volatile>
	{
		using Type = Ty;
	};

	template <typename Ty>
	using RemoveOneRawPointerType = typename RemoveOneRawPointer<Ty>::Type;

	// Remove all raw pointer

	template <typename Ty>
	struct RemoveAllRawPointer
	{
		using Type = Ty;
	};

	template <typename Ty>
	struct RemoveAllRawPointer<Ty*>
	{
		using Type = typename RemoveAllRawPointer<Ty>::Type;
	};

	template <typename Ty>
	struct RemoveAllRawPointer<Ty* const>
	{
		using Type = typename RemoveAllRawPointer<Ty>::Type;
	};

	template <typename Ty>
	struct RemoveAllRawPointer<Ty* volatile>
	{
		using Type = typename RemoveAllRawPointer<Ty>::Type;
	};

	template <typename Ty>
	struct RemoveAllRawPointer<Ty* const volatile>
	{
		using Type = typename RemoveAllRawPointer<Ty>::Type;
	};

	template <typename Ty>
	using RemoveAllRawPointerType = typename RemoveAllRawPointer<Ty>::Type;

	// Remove one smart pointer
	
	template <typename Ty>
	struct RemoveOneSmartPointer
	{
		using Type = Ty;
	};

	template <typename Ty>
	struct RemoveOneSmartPointer<::std::unique_ptr<Ty>>
	{
		using Type = Ty;
	};
	template <typename Ty>
	struct RemoveOneSmartPointer<::std::shared_ptr<Ty>>
	{
		using Type = Ty;
	};
	template <typename Ty>
	struct RemoveOneSmartPointer<::std::weak_ptr<Ty>>
	{
		using Type = Ty;
	};

	template <typename Ty>
	struct RemoveOneSmartPointer<const ::std::unique_ptr<Ty>>
	{
		using Type = Ty;
	};
	template <typename Ty>
	struct RemoveOneSmartPointer<const ::std::shared_ptr<Ty>>
	{
		using Type = Ty;
	};
	template <typename Ty>
	struct RemoveOneSmartPointer<const ::std::weak_ptr<Ty>>
	{
		using Type = Ty;
	};

	template <typename Ty>
	struct RemoveOneSmartPointer<volatile ::std::unique_ptr<Ty>>
	{
		using Type = Ty;
	};
	template <typename Ty>
	struct RemoveOneSmartPointer<volatile ::std::shared_ptr<Ty>>
	{
		using Type = Ty;
	};
	template <typename Ty>
	struct RemoveOneSmartPointer<volatile ::std::weak_ptr<Ty>>
	{
		using Type = Ty;
	};

	template <typename Ty>
	struct RemoveOneSmartPointer<const volatile ::std::unique_ptr<Ty>>
	{
		using Type = Ty;
	};
	template <typename Ty>
	struct RemoveOneSmartPointer<const volatile ::std::shared_ptr<Ty>>
	{
		using Type = Ty;
	};
	template <typename Ty>
	struct RemoveOneSmartPointer<const volatile ::std::weak_ptr<Ty>>
	{
		using Type = Ty;
	};

	template <typename Ty>
	using RemoveOneSmartPointerType = typename RemoveOneSmartPointer<Ty>::Type;

	// Remove all smart pointer

	template <typename Ty>
	struct RemoveAllSmartPointer
	{
		using Type = Ty;
	};

	template <typename Ty>
	struct RemoveAllSmartPointer<::std::unique_ptr<Ty>>
	{
		using Type = typename RemoveAllSmartPointer<Ty>::Type;
	};
	template <typename Ty>
	struct RemoveAllSmartPointer<::std::shared_ptr<Ty>>
	{
		using Type = typename RemoveAllSmartPointer<Ty>::Type;
	};
	template <typename Ty>
	struct RemoveAllSmartPointer<::std::weak_ptr<Ty>>
	{
		using Type = typename RemoveAllSmartPointer<Ty>::Type;
	};

	template <typename Ty>
	struct RemoveAllSmartPointer<const ::std::unique_ptr<Ty>>
	{
		using Type = typename RemoveAllSmartPointer<Ty>::Type;
	};
	template <typename Ty>
	struct RemoveAllSmartPointer<const ::std::shared_ptr<Ty>>
	{
		using Type = typename RemoveAllSmartPointer<Ty>::Type;
	};
	template <typename Ty>
	struct RemoveAllSmartPointer<const ::std::weak_ptr<Ty>>
	{
		using Type = typename RemoveAllSmartPointer<Ty>::Type;
	};

	template <typename Ty>
	struct RemoveAllSmartPointer<volatile ::std::unique_ptr<Ty>>
	{
		using Type = typename RemoveAllSmartPointer<Ty>::Type;
	};
	template <typename Ty>
	struct RemoveAllSmartPointer<volatile ::std::shared_ptr<Ty>>
	{
		using Type = typename RemoveAllSmartPointer<Ty>::Type;
	};
	template <typename Ty>
	struct RemoveAllSmartPointer<volatile ::std::weak_ptr<Ty>>
	{
		using Type = typename RemoveAllSmartPointer<Ty>::Type;
	};

	template <typename Ty>
	struct RemoveAllSmartPointer<const volatile ::std::unique_ptr<Ty>>
	{
		using Type = typename RemoveAllSmartPointer<Ty>::Type;
	};
	template <typename Ty>
	struct RemoveAllSmartPointer<const volatile ::std::shared_ptr<Ty>>
	{
		using Type = typename RemoveAllSmartPointer<Ty>::Type;
	};
	template <typename Ty>
	struct RemoveAllSmartPointer<const volatile ::std::weak_ptr<Ty>>
	{
		using Type = typename RemoveAllSmartPointer<Ty>::Type;
	};

	template <typename Ty>
	using RemoveAllSmartPointerType = typename RemoveAllSmartPointer<Ty>::Type;

	// Remove one generic pointer type

	template <typename Ty>
	struct RemoveOneGenericPointer
	{
		using Type = Ty;
	};

	template <typename Ty>
	struct RemoveOneGenericPointer<Ty*>
	{
		using Type = Ty;
	};
	template <typename Ty>
	struct RemoveOneGenericPointer<::std::unique_ptr<Ty>>
	{
		using Type = Ty;
	};
	template <typename Ty>
	struct RemoveOneGenericPointer<::std::shared_ptr<Ty>>
	{
		using Type = Ty;
	};
	template <typename Ty>
	struct RemoveOneGenericPointer<::std::weak_ptr<Ty>>
	{
		using Type = Ty;
	};

	template <typename Ty>
	struct RemoveOneGenericPointer<Ty* const>
	{
		using Type = Ty;
	};
	template <typename Ty>
	struct RemoveOneGenericPointer<const ::std::unique_ptr<Ty>>
	{
		using Type = Ty;
	};
	template <typename Ty>
	struct RemoveOneGenericPointer<const ::std::shared_ptr<Ty>>
	{
		using Type = Ty;
	};
	template <typename Ty>
	struct RemoveOneGenericPointer<const ::std::weak_ptr<Ty>>
	{
		using Type = Ty;
	};

	template <typename Ty>
	struct RemoveOneGenericPointer<Ty* volatile>
	{
		using Type = Ty;
	};
	template <typename Ty>
	struct RemoveOneGenericPointer<volatile ::std::unique_ptr<Ty>>
	{
		using Type = Ty;
	};
	template <typename Ty>
	struct RemoveOneGenericPointer<volatile ::std::shared_ptr<Ty>>
	{
		using Type = Ty;
	};
	template <typename Ty>
	struct RemoveOneGenericPointer<volatile ::std::weak_ptr<Ty>>
	{
		using Type = Ty;
	};

	template <typename Ty>
	struct RemoveOneGenericPointer<Ty* const volatile>
	{
		using Type = Ty;
	};
	template <typename Ty>
	struct RemoveOneGenericPointer<const volatile ::std::unique_ptr<Ty>>
	{
		using Type = Ty;
	};
	template <typename Ty>
	struct RemoveOneGenericPointer<const volatile ::std::shared_ptr<Ty>>
	{
		using Type = Ty;
	};
	template <typename Ty>
	struct RemoveOneGenericPointer<const volatile ::std::weak_ptr<Ty>>
	{
		using Type = Ty;
	};

	template <typename Ty>
	using RemoveOneGenericPointerType = typename RemoveOneGenericPointer<Ty>::Type;

	// Remove all generic pointer

	template <typename Ty>
	struct RemoveAllGenericPointer
	{
		using Type = Ty;
	};

	template <typename Ty>
	struct RemoveAllGenericPointer<Ty*>
	{
		using Type = typename RemoveAllGenericPointer<Ty>::Type;
	};
	template <typename Ty>
	struct RemoveAllGenericPointer<::std::unique_ptr<Ty>>
	{
		using Type = typename RemoveAllGenericPointer<Ty>::Type;
	};
	template <typename Ty>
	struct RemoveAllGenericPointer<::std::shared_ptr<Ty>>
	{
		using Type = typename RemoveAllGenericPointer<Ty>::Type;
	};
	template <typename Ty>
	struct RemoveAllGenericPointer<::std::weak_ptr<Ty>>
	{
		using Type = typename RemoveAllGenericPointer<Ty>::Type;
	};

	template <typename Ty>
	struct RemoveAllGenericPointer<Ty* const>
	{
		using Type = typename RemoveAllGenericPointer<Ty>::Type;
	};
	template <typename Ty>
	struct RemoveAllGenericPointer<const ::std::unique_ptr<Ty>>
	{
		using Type = typename RemoveAllGenericPointer<Ty>::Type;
	};
	template <typename Ty>
	struct RemoveAllGenericPointer<const ::std::shared_ptr<Ty>>
	{
		using Type = typename RemoveAllGenericPointer<Ty>::Type;
	};
	template <typename Ty>
	struct RemoveAllGenericPointer<const ::std::weak_ptr<Ty>>
	{
		using Type = typename RemoveAllGenericPointer<Ty>::Type;
	};

	template <typename Ty>
	struct RemoveAllGenericPointer<Ty* volatile>
	{
		using Type = typename RemoveAllGenericPointer<Ty>::Type;
	};
	template <typename Ty>
	struct RemoveAllGenericPointer<volatile ::std::unique_ptr<Ty>>
	{
		using Type = typename RemoveAllGenericPointer<Ty>::Type;
	};
	template <typename Ty>
	struct RemoveAllGenericPointer<volatile ::std::shared_ptr<Ty>>
	{
		using Type = typename RemoveAllGenericPointer<Ty>::Type;
	};
	template <typename Ty>
	struct RemoveAllGenericPointer<volatile ::std::weak_ptr<Ty>>
	{
		using Type = typename RemoveAllGenericPointer<Ty>::Type;
	};

	template <typename Ty>
	struct RemoveAllGenericPointer<Ty* const volatile>
	{
		using Type = typename RemoveAllGenericPointer<Ty>::Type;
	};
	template <typename Ty>
	struct RemoveAllGenericPointer<const volatile ::std::unique_ptr<Ty>>
	{
		using Type = typename RemoveAllGenericPointer<Ty>::Type;
	};
	template <typename Ty>
	struct RemoveAllGenericPointer<const volatile ::std::shared_ptr<Ty>>
	{
		using Type = typename RemoveAllGenericPointer<Ty>::Type;
	};
	template <typename Ty>
	struct RemoveAllGenericPointer<const volatile ::std::weak_ptr<Ty>>
	{
		using Type = typename RemoveAllGenericPointer<Ty>::Type;
	};

	template <typename Ty>
	using RemoveAllGenericPointerType = typename RemoveAllGenericPointer<Ty>::Type;
}

#endif // !Petal_Header_SmartPointer
