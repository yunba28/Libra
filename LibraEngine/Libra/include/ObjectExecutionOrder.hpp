#pragma once

#include "Object.hpp"

namespace Libra
{
	class ObjectExecutionOrder final
	{
	private:

		ObjectExecutionOrder() = default;

		~ObjectExecutionOrder() = default;

		static ObjectExecutionOrder& _Instance()noexcept;

	public:

		[[nodiscard]]
		static int32 _Get(const TypeID& type);

		static void _Set(const TypeID& type, int32 priority);

		/// @brief Object型ごとの優先度を取得する
		/// @return デフォルト値は1000
		template<Internal::ModelObject ObjectT>
		[[nodiscard]]
		static int32 Get() { return _Get(typeid(ObjectT)); }

		/// @brief Object型ごとの優先度を設定する
		template<Internal::ModelObject ObjectT>
		static void Set(int32 priority) { _Set(typeid(ObjectT), priority); }

		static bool Compare(const TypeID& a, const TypeID& b);

	private:

		/// @brief Objectの型優先度
		HashTable<TypeID, int32> m_priorities;

	};
}
