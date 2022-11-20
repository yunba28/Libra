#pragma once

#include <tuple>
#include <initializer_list>
#include <functional>
#include <typeindex>
#include <memory>

namespace Libra
{
	template<class First, class Second>
	using Pair = std::pair<First, Second>;

	template<class ...Types>
	using Tuple = std::tuple<Types...>;

	template<class Type>
	using InitializerList = std::initializer_list<Type>;

	template<class Result, class ...Args>
	using Function = std::function<Result(Args...)>;

	using TypeID = std::type_index;

	template<class Type>
	using UniquePtr = std::unique_ptr<Type>;

	template<class Type>
	using SharedPtr = std::shared_ptr<Type>;

	template<class Type>
	using WeakPtr = std::weak_ptr<Type>;

	template<class First, class Second>
	[[nodiscard]]
	constexpr auto MakePair(First&& first, Second&& second)
	{
		return std::make_pair(std::forward<First>(first), std::forward<Second>(second));
	}

	template<class ...Types>
	[[nodiscard]]
	constexpr auto MakeTuple(Types&& ...args)
	{
		return std::make_tuple(std::forward<Types>(args)...);
	}

	template<class Type,class ...Args>
	[[nodiscard]]
	constexpr auto MakeUnique(Args&& ...args)
	{
		return std::make_unique<Type>(std::forward<Args>(args)...);
	}

	template<class Type, class ...Args>
	[[nodiscard]]
	constexpr auto MakeShared(Args&& ...args)
	{
		return std::make_shared<Type>(std::forward<Args>(args)...);
	}
}
