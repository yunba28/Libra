#pragma once

#include "ComponentArray.hpp"

namespace Libra::Internal
{
	class ComponentSystem final
	{
	public:

		ComponentSystem();

		~ComponentSystem();

		void add(const TypeID& _type, const SharedObj<Component>& _component);

		void add(const TypeID& _type, SharedObj<Component>&& _component);

		bool remove(const WeakObj<Component>& _componentRef);

		bool remove(const TypeID& _type, const WeakObj<Component>& _componentRef);

		bool removeByActorRef(const TypeID& _type, const WeakObj<Actor>& _actorRef);

		bool removeAllByActorRef(const WeakObj<Actor>& _actorRef);

		bool removeAllByActorName(const String& _name);

		bool removeAllByActorTag(const String& _tag);

		[[nodiscard]]
		WeakObj<Component> getComponent(const TypeID& _type, const WeakObj<Actor>& _actorRef)const;

		template<ModelComponent ComponentT>
		[[nodiscard]]
		WeakObj<Component> getComponent(const WeakObj<Actor>& _actorRef)const;

		template<class Fty>
		[[nodiscard]]
		WeakObj<Component> getComponentBy(const TypeID& _type, Fty _func)const;

		template<ModelComponent ComponentT, class Fty>
		[[nodiscard]]
		WeakObj<ComponentT> getComponentBy(Fty _func)const;

		[[nodiscard]]
		WeakObj<Component> getComponentByActorName(const TypeID& _type, const String& _name)const;

		template<ModelComponent ComponentT>
		[[nodiscard]]
		WeakObj<ComponentT> getComponentByActorName(const String& _name)const;

		[[nodiscard]]
		WeakObj<Component> getComponentByActorTag(const TypeID& _type, const String& _tag)const;

		template<ModelComponent ComponentT>
		[[nodiscard]]
		WeakObj<ComponentT> getComponentByActorTag(const String& _tag)const;

		[[nodiscard]]
		Array<WeakObj<Component>> getComponents(const TypeID& _type)const;

		[[nodiscard]]
		Array<WeakObj<Component>> getComponentsByActorRef(const WeakObj<Actor>& _actorRef)const;

		template<class Fty>
		[[nodiscard]]
		Array<WeakObj<Component>> getComponentsBy(Fty _func)const;

		template<class Fty>
		[[nodiscard]]
		Array<WeakObj<Component>> getComponentsBy(const TypeID& _type, Fty _func)const;

		[[nodiscard]]
		Array<WeakObj<Component>> getComponentsByActorTag(const String& _tag)const;

		[[nodiscard]]
		Array<WeakObj<Component>> getComponentsByActorTag(const TypeID& _type, const String& _tag)const;

		void update();

	private:

		HashTable<TypeID, ComponentArray> m_componentArrays;

		Array<TypeID> m_sortedTypeList;

		Array<TypeID> m_pendingAddType;
	};

	template<ModelComponent ComponentT>
	WeakObj<Component> ComponentSystem::getComponent(const WeakObj<Actor>& _actorRef) const
	{
		if (auto found = getComponent(typeid(ComponentT), _actorRef); found)
		{
			return found.castTo<ComponentT>();
		}
		return WeakObj<Component>{};
	}

	template<class Fty>
	WeakObj<Component> ComponentSystem::getComponentBy(const TypeID& _type, Fty _func) const
	{
		if (not m_componentArrays.contains(_type))
			return WeakObj<Component>{};

		return m_componentArrays.at(_type).getComponentBy(_func);;
	}

	template<ModelComponent ComponentT, class Fty>
	WeakObj<ComponentT> ComponentSystem::getComponentBy(Fty _func) const
	{
		if (auto found = getComponentBy(typeid(ComponentT), _func); found)
		{
			return found.castTo<ComponentT>();
		}
		return WeakObj<ComponentT>{};
	}

	template<ModelComponent ComponentT>
	WeakObj<ComponentT> ComponentSystem::getComponentByActorName(const String& _name) const
	{
		if (auto found = getComponentByActorName(typeid(ComponentT), _name); found)
		{
			return found.castTo<ComponentT>();
		}
		return WeakObj<ComponentT>{};
	}

	template<ModelComponent ComponentT>
	WeakObj<ComponentT> ComponentSystem::getComponentByActorTag(const String& _tag) const
	{
		if (auto found = getComponentByActorTag(typeid(ComponentT), _tag); found)
		{
			return found.castTo<ComponentT>();
		}
		return WeakObj<ComponentT>{};
	}

	template<class Fty>
	Array<WeakObj<Component>> ComponentSystem::getComponentsBy(Fty _func) const
	{
		Array<WeakObj<Component>> result;

		for (auto& type : m_sortedTypeList)
		{
			if (Array<WeakObj<Component>> res = m_componentArrays.at(type).getComponentsBy(_func); not res.empty())
			{
				result.append(res);
			}
		}

		return result;
	}

	template<class Fty>
	Array<WeakObj<Component>> ComponentSystem::getComponentsBy(const TypeID& _type, Fty _func) const
	{
		if(not m_componentArrays.contains(_type))
			return Array<WeakObj<Component>>{};

		return m_componentArrays.at(_type).getComponentsBy(_func);
	}
}
