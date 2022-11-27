#pragma once

#include "ActorArray.hpp"

namespace Libra::Internal
{
	class ActorSystem final
	{
	public:

		ActorSystem();

		~ActorSystem();

		void add(const TypeID& _type, const SharedObj<Actor>& _actor);

		void add(const TypeID& _type, SharedObj<Actor>&& _actor);

		bool remove(const WeakObj<Actor>& _actorRef);

		bool remove(const TypeID& _type, const WeakObj<Actor>& _actorRef);

		bool removeByName(const String& _name);

		bool removeByTag(const String& _tag);

		[[nodiscard]]
		WeakObj<Actor> getActor(const TypeID& _type, const String& _name)const;

		template<ModelActor ActorT = Actor>
		[[nodiscard]]
		WeakObj<ActorT> getActor(const String& _name)const;

		template<class Fty>
		[[nodiscard]]
		WeakObj<Actor> getActorBy(const TypeID& _type, Fty _func)const;

		template<ModelActor ActorT, class Fty>
		[[nodiscard]]
		WeakObj<ActorT> getActorBy(Fty _func)const;

		[[nodiscard]]
		WeakObj<Actor> getActorByTag(const TypeID& _type, const String& _tag)const;

		template<ModelActor ActorT = Actor>
		[[nodiscard]]
		WeakObj<ActorT> getActorByTag(const String& _tag)const;

		[[nodiscard]]
		Array<WeakObj<Actor>> getActors(const TypeID& _type)const;

		[[nodiscard]]
		Array<WeakObj<Actor>> getActorAll()const;

		template<class Fty>
		[[nodiscard]]
		Array<WeakObj<Actor>> getActorsBy(Fty _func)const;

		template<class Fty>
		[[nodiscard]]
		Array<WeakObj<Actor>> getActorsBy(const TypeID& _type, Fty _func)const;

		[[nodiscard]]
		Array<WeakObj<Actor>> getActorsByTag(const String& _tag)const;

		[[nodiscard]]
		Array<WeakObj<Actor>> getActorsByTag(const TypeID& _type, const String& _tag)const;

		void update();

	private:

		HashTable<TypeID, ActorArray> m_actorArrays;

		Array<TypeID> m_sortedTypeList;

		Array<TypeID> m_pendingAddType;

	};

	template<ModelActor ActorT>
	WeakObj<ActorT> ActorSystem::getActor(const String& _name) const
	{
		if (auto found = getActor(typeid(ActorT), _name); found)
		{
			return found.castTo<ActorT>();
		}
		return WeakObj<ActorT>{};
	}

	template<class Fty>
	WeakObj<Actor> ActorSystem::getActorBy(const TypeID& _type, Fty _func) const
	{
		if (not m_actorArrays.contains(_type))
			return WeakObj<Actor>{};

		return m_actorArrays.at(_type).getActorBy(_type, _func);
	}

	template<ModelActor ActorT, class Fty>
	WeakObj<ActorT> ActorSystem::getActorBy(Fty _func) const
	{
		if (auto found = getActorBy(typeid(ActorT), _func);found)
		{
			return found.castTo<ActorT>();
		}
		return WeakObj<ActorT>{};
	}

	template<ModelActor ActorT>
	WeakObj<ActorT> ActorSystem::getActorByTag(const String& _tag) const
	{
		if (auto found = getActorByTag(typeid(ActorT), _tag); found)
		{
			return found.castTo<ActorT>();
		}
		return WeakObj<ActorT>{};
	}

	template<class Fty>
	Array<WeakObj<Actor>> ActorSystem::getActorsBy(Fty _func) const
	{
		Array<WeakObj<Actor>> result;

		for (auto& type : m_sortedTypeList)
		{
			if (Array<WeakObj<Actor>> res = m_actorArrays.at(type).getActorsBy(_func); not res.empty())
			{
				result.append(res);
			}
		}

		return result;
	}

	template<class Fty>
	Array<WeakObj<Actor>> ActorSystem::getActorsBy(const TypeID& _type, Fty _func) const
	{
		if (not m_actorArrays.contains(_type))
			return Array<WeakObj<Actor>>{};

		return m_actorArrays.at(_type).getActorsBy(_func);
	}
}
