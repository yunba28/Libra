#include "../../include/ComponentArray.hpp"

namespace Libra::Internal
{
	ComponentArray::ComponentArray()
		: m_components()
		, m_pendingAddComponent()
		, m_needRemove(false)
	{
	}

	ComponentArray::~ComponentArray()
	{
	}

	void ComponentArray::add(const SharedObj<Component>& _component)
	{
		m_pendingAddComponent << _component;
	}

	void ComponentArray::add(SharedObj<Component>&& _component)
	{
		m_pendingAddComponent << std::forward<SharedObj<Component>>(_component);
	}

	bool ComponentArray::remove(const WeakObj<Component>& _componentRef)
	{
		for (auto& component : m_components)
		{
			if (component == _componentRef)
			{
				component->_internalNotifyDestroy();
				return true;
			}
		}
		return false;
	}

	bool ComponentArray::removeByActorRef(const WeakObj<Actor>& _actorRef)
	{
		for (auto& component : m_components)
		{
			if (component->getActor() == _actorRef)
			{
				component->_internalNotifyDestroy();
				return true;
			}
		}
		return false;
	}

	bool ComponentArray::removeByActorName(const String& _name)
	{
		for (auto& component : m_components)
		{
			if (component->compareName(_name))
			{
				component->_internalNotifyDestroy();
				return true;
			}
		}
		return false;
	}

	bool ComponentArray::removeByActorTag(const String& _tag)
	{
		for (auto& component : m_components)
		{
			if (component->compareTag(_tag))
			{
				component->_internalNotifyDestroy();
				return true;
			}
		}
		return false;
	}

	WeakObj<Component> ComponentArray::getComponent(const WeakObj<Actor>& _actor) const
	{
		for (auto& component : m_components)
		{
			if (component->getActor() == _actor)
			{
				return component.makeRef();
			}
		}
		return WeakObj<Component>{};
	}

	WeakObj<Component> ComponentArray::getComponentByActorName(const String& _name) const
	{
		for (auto& component : m_components)
		{
			if (component->compareName(_name))
			{
				return component.makeRef();
			}
		}
		return WeakObj<Component>{};
	}

	WeakObj<Component> ComponentArray::getComponentByActorTag(const String& _tag) const
	{
		for (auto& component : m_components)
		{
			if (component->compareTag(_tag))
			{
				return component.makeRef();
			}
		}
		return WeakObj<Component>{};
	}

	Array<WeakObj<Component>> ComponentArray::getComponents() const
	{
		Array<WeakObj<Component>> result;
		result.reserve(m_components.size());
		for (auto& component : m_components)
		{
			result << component.makeRef();
		}
		return result;
	}

	Array<WeakObj<Component>> ComponentArray::getComponents(const WeakObj<Actor>& _actorRef) const
	{
		Array<WeakObj<Component>> result;
		result.reserve(m_components.size());
		for (auto& component : m_components)
		{
			if (component->getActor() == _actorRef)
			{
				result << component.makeRef();
			}
		}
		return result;
	}

	Array<WeakObj<Component>> ComponentArray::getComponentsByActorTag(const String& _tag)const
	{
		Array<WeakObj<Component>> result;
		result.reserve(m_components.size());
		for (auto& component : m_components)
		{
			if (component->compareTag(_tag))
			{
				result << component.makeRef();
			}
		}
		return result;
	}

	void ComponentArray::update()
	{
		// Actorの削除が必要
		if (m_needRemove)
		{
			m_components.remove(nullptr);
			m_needRemove = false;
		}

		// 追加待ちのActorを追加する
		if (not m_pendingAddComponent.empty())
		{
			m_components.append(m_pendingAddComponent);
			m_pendingAddComponent.clear();
		}

		for (auto& component : m_components)
		{
			// Actorが破棄待ちだったらnullptrを挿入して処理をスキップ
			if ((not component) or component->isPendingDestroy())
			{
				component = nullptr;
				m_needRemove = true;
				continue;
			}

			component->_internalUpdate();
		}
	}
}
