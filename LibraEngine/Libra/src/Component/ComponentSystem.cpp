#include "../../include/ComponentSystem.hpp"
#include "../../include/ObjectExecutionOrder.hpp"

namespace Libra::Internal
{
	ComponentSystem::ComponentSystem()
	{
	}

	ComponentSystem::~ComponentSystem()
	{
	}

	void ComponentSystem::add(const TypeID& _type, const SharedObj<Component>& _component)
	{
		if (not m_componentArrays.contains(_type))
		{
			m_pendingAddType << _type;
			m_componentArrays.emplace(_type, ComponentArray{});
		}
		m_componentArrays[_type].add(_component);
	}

	void ComponentSystem::add(const TypeID& _type, SharedObj<Component>&& _component)
	{
		if (not m_componentArrays.contains(_type))
		{
			m_pendingAddType << _type;
			m_componentArrays.emplace(_type, ComponentArray{});
		}
		m_componentArrays[_type].add(std::forward<SharedObj<Component>>(_component));
	}

	bool ComponentSystem::remove(const WeakObj<Component>& _componentRef)
	{
		for (auto& type : m_sortedTypeList)
		{
			if (m_componentArrays[type].remove(_componentRef))
				return true;
		}
		return false;
	}

	bool ComponentSystem::remove(const TypeID& _type, const WeakObj<Component>& _componentRef)
	{
		if (m_componentArrays.contains(_type))
		{
			return m_componentArrays[_type].remove(_componentRef);
		}
		return false;
	}

	bool ComponentSystem::removeByActorRef(const TypeID& _type, const WeakObj<Actor>& _actorRef)
	{
		if (m_componentArrays.contains(_type))
		{
			return m_componentArrays[_type].removeByActorRef(_actorRef);
		}
		return false;
	}

	bool ComponentSystem::removeAllByActorRef(const WeakObj<Actor>& _actorRef)
	{
		bool result = false;
		for (auto& type : m_sortedTypeList)
		{
			result |= m_componentArrays[type].removeByActorRef(_actorRef);
		}
		return result;
	}

	bool ComponentSystem::removeAllByActorName(const String& _name)
	{
		bool result = false;
		for (auto& type : m_sortedTypeList)
		{
			result |= m_componentArrays[type].removeByActorName(_name);
		}
		return result;
	}

	bool ComponentSystem::removeAllByActorTag(const String& _tag)
	{
		bool result = false;
		for (auto& type : m_sortedTypeList)
		{
			result |= m_componentArrays[type].removeByActorTag(_tag);
		}
		return result;
	}

	WeakObj<Component> ComponentSystem::getComponent(const TypeID& _type, const WeakObj<Actor>& _actorRef) const
	{
		if (not m_componentArrays.contains(_type))
			return WeakObj<Component>{};

		return m_componentArrays.at(_type).getComponent(_actorRef);
	}

	WeakObj<Component> ComponentSystem::getComponentByActorName(const TypeID& _type, const String& _name) const
	{
		if (not m_componentArrays.contains(_type))
			return WeakObj<Component>{};

		return m_componentArrays.at(_type).getComponentByActorName(_name);
	}

	WeakObj<Component> ComponentSystem::getComponentByActorTag(const TypeID& _type, const String& _tag) const
	{
		if (not m_componentArrays.contains(_type))
			return WeakObj<Component>{};

		return m_componentArrays.at(_type).getComponentByActorTag(_tag);
	}

	Array<WeakObj<Component>> ComponentSystem::getComponents(const TypeID& _type) const
	{
		if (not m_componentArrays.contains(_type))
			return Array<WeakObj<Component>>{};

		return m_componentArrays.at(_type).getComponents();
	}

	Array<WeakObj<Component>> ComponentSystem::getComponentsByActorRef(const WeakObj<Actor>& _actorRef) const
	{
		Array<WeakObj<Component>> result;
		for (auto& type : m_sortedTypeList)
		{
			if (Array<WeakObj<Component>> res = m_componentArrays.at(type).getComponents(_actorRef); not res.empty())
			{
				result.append(res);
			}
		}
		return result;
	}

	Array<WeakObj<Component>> ComponentSystem::getComponentsByActorTag(const String& _tag) const
	{
		Array<WeakObj<Component>> result;
		for (auto& type : m_sortedTypeList)
		{
			if (Array<WeakObj<Component>> res = m_componentArrays.at(type).getComponentsByActorTag(_tag); not res.empty())
			{
				result.append(res);
			}
		}
		return result;
	}

	Array<WeakObj<Component>> ComponentSystem::getComponentsByActorTag(const TypeID& _type, const String& _tag) const
	{
		if(not m_componentArrays.contains(_type))
			return Array<WeakObj<Component>>{};

		return m_componentArrays.at(_type).getComponentsByActorTag(_tag);
	}

	void ComponentSystem::update()
	{
		// 型リストの追加・並び替え
		if (not m_pendingAddType.empty())
		{
			m_sortedTypeList.append(m_pendingAddType);
			m_sortedTypeList.stable_sort_by(ObjectExecutionOrder::Compare);
			m_pendingAddType.clear();
		}

		// 各Actorの更新処理
		for (const auto& type : m_sortedTypeList)
		{
			m_componentArrays[type].update();
		}
	}
}
