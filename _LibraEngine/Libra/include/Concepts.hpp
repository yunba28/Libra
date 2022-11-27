#pragma once

#include <concepts>

namespace Libra::Concepts
{
	template<class T>
	concept Integer = std::is_integral_v<T>;

	template<class T>
	concept Floating = std::is_floating_point_v<T>;

	template<class T>
	concept Arithmetic = std::is_arithmetic_v<T>;

	template<class Fty, class ...Args>
	concept Invocable = std::is_invocable_v<Fty, Args...>;

	template<class Fty, class Result, class ...Args>
	concept ResultInvocable = std::is_invocable_r_v<Result, Fty, Args...>;

	template<class Base, class Derived>
	concept BaseOf = std::derived_from<Derived, Base>;

	template<class Type, class Type2>
	concept Lineage = std::derived_from<Type, Type2> or std::derived_from<Type2, Type>;

	template<class T>
	concept PlusUnary = requires(T & x)
	{
		+x;
	};

	template<class T>
	concept MinusUnary = requires(T & x)
	{
		-x;
	};

	template<class T>
	concept Unary = requires(T & x)
	{
		+x;
		-x;
	};
}

#define DECLARE_MODEL( Base )\
template<class Derived>\
concept Model##Base = std::derived_from<Derived,Base>;

#define DECLARE_INTERNAL_MODEL( Base )\
namespace Internal{\
template<class Derived>\
concept Model##Base = std::derived_from<Derived,Base>;}
