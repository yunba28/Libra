#include "../../include/ObjectExecutionOrder.hpp"

namespace Libra
{
	ObjectExecutionOrder& ObjectExecutionOrder::_Instance() noexcept
	{
		static ObjectExecutionOrder sObjectPriority{};
		return sObjectPriority;
	}

	int32 ObjectExecutionOrder::_Get(const TypeID& type)
	{
		auto& table = _Instance().m_priorities;
		if (table.contains(type))
		{
			return table[type];
		}
		return 1000;
	}

	void ObjectExecutionOrder::_Set(const TypeID& type, int32 priority)
	{
		auto& table = _Instance().m_priorities;
		table[type] = priority;
	}

	bool ObjectExecutionOrder::Compare(const TypeID& a, const TypeID& b)
	{
		return ObjectExecutionOrder::_Get(a) < ObjectExecutionOrder::_Get(b);
	}
}
