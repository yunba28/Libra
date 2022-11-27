#pragma once

#include "Actor.hpp"

namespace Libra::Internal
{
	class ActorArray final
	{
	public:

		ActorArray();

		~ActorArray();

		void add(const SharedObj<Actor>& _actor);

		void add(SharedObj<Actor>&& _actor);

		bool remove(const WeakObj<Actor>& _actorRef);

		bool removeByName(const String& _name);

		bool removeByTag(const String& _tag);

		[[nodiscard]]
		WeakObj<Actor> getActor(const String& _name)const;

		template<class Fty>
		[[nodiscard]]
		WeakObj<Actor> getActorBy(Fty _func)const requires(Concepts::ResultInvocable<Fty, bool, const SharedObj<Actor>&>);

		[[nodiscard]]
		WeakObj<Actor> getActorByTag(const String& _tag)const;

		[[nodiscard]]
		Array<WeakObj<Actor>> getActors()const;

		template<class Fty>
		[[nodiscard]]
		Array<WeakObj<Actor>> getActorsBy(Fty _func)const requires(Concepts::ResultInvocable<Fty, bool, const SharedObj<Actor>&>);

		[[nodiscard]]
		Array<WeakObj<Actor>> getActorsByTag(const String& _tag)const;

		void update();

	private:

		using PActor = SharedObj<Actor>;

		Array<PActor> m_actors;

		Array<PActor> m_pendingAddActor;

		bool m_needRemove;
	};

	template<class Fty>
	WeakObj<Actor> ActorArray::getActorBy(Fty _func) const requires(Concepts::ResultInvocable<Fty, bool, const SharedObj<Actor>&>)
	{
		for (auto& actor : m_actors)
		{
			if (_func(actor))
			{
				return actor.makeRef();
			}
		}
		return WeakObj<Actor>{};
	}

	template<class Fty>
	Array<WeakObj<Actor>> ActorArray::getActorsBy(Fty _func) const requires(Concepts::ResultInvocable<Fty, bool, const SharedObj<Actor>&>)
	{
		Array<WeakObj<Actor>> result;
		result.reserve(m_actors.size());
		for (auto& actor : m_actors)
		{
			if (_func(actor))
			{
				result << actor.makeRef();
			}
		}
		return result;
	}
}
