#include "../../include/ObjectPriority.hpp"

namespace Libra
{
	ObjectPriority& ObjectPriority::_Instance() noexcept
	{
		static ObjectPriority sObjectPriority{};
		return sObjectPriority;
	}

	int32 ObjectPriority::_Get(const TypeID& type)
	{
		auto& table = _Instance().m_priorities;
		if (table.contains(type))
		{
			return table[type];
		}
		return 1000;
	}

	void ObjectPriority::_Set(const TypeID& type, int32 priority)
	{
		auto& table = _Instance().m_priorities;
		table[type] = priority;
	}
}
