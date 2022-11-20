#pragma once

#include "Component.hpp"

namespace Libra::Internal
{
	class ComponentArray final
	{
	public:

		ComponentArray();

		~ComponentArray();

		void add(const SharedObj<Component>& _component);

		void add(SharedObj<Component>&& _component);

		bool remove(const WeakObj<Component>& _componentRef);

		bool removeByActorRef(const WeakObj<Actor>& _actorRef);

		bool removeByActorName(const String& _name);

		bool removeByActorTag(const String& _tag);

		[[nodiscard]]
		WeakObj<Component> getComponent(const WeakObj<Actor>& _actor)const;

		template<class Fty>
		[[nodiscard]]
		WeakObj<Component> getComponentBy(Fty _func)const requires(Concepts::ResultInvocable<Fty, bool, const SharedObj<Component>&>);

		[[nodiscard]]
		WeakObj<Component> getComponentByActorName(const String& _name)const;

		[[nodiscard]]
		WeakObj<Component> getComponentByActorTag(const String& _tag)const;

		[[nodiscard]]
		Array<WeakObj<Component>> getComponents()const;

		[[nodiscard]]
		Array<WeakObj<Component>> getComponents(const WeakObj<Actor>& _actorRef)const;

		template<class Fty>
		[[nodiscard]]
		Array<WeakObj<Component>> getComponentsBy(Fty _func)const requires(Concepts::ResultInvocable<Fty, bool, const SharedObj<Component>&>);

		[[nodiscard]]
		Array<WeakObj<Component>> getComponentsByActorTag(const String& _tag)const;

		void update();

	private:

		using PComponent = SharedObj<Component>;

		Array<PComponent> m_components;

		Array<PComponent> m_pendingAddComponent;

		bool m_needRemove;

	};

	template<class Fty>
	WeakObj<Component> ComponentArray::getComponentBy(Fty _func) const requires(Concepts::ResultInvocable<Fty, bool, const SharedObj<Component>&>)
	{
		for (auto& component : m_components)
		{
			if (_func(component))
			{
				return component.makeRef();
			}
		}
		return WeakObj<Component>{};
	}

	template<class Fty>
	Array<WeakObj<Component>> ComponentArray::getComponentsBy(Fty _func) const requires(Concepts::ResultInvocable<Fty, bool, const SharedObj<Component>&>)
	{
		Array<WeakObj<Component>> result;
		result.reserve(m_components.size());
		for (auto& component : m_components)
		{
			if (_func(component))
			{
				result << component.makeRef();
			}
		}
		return result;
	}
}
