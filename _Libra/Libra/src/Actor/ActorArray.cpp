#include "../../include/ActorArray.hpp"

namespace Libra::Internal
{
	ActorArray::ActorArray()
		: m_actors()
		, m_pendingAddActor()
		, m_needRemove(false)
	{
	}

	ActorArray::~ActorArray()
	{
	}

	void ActorArray::add(const SharedObj<Actor>& _actor)
	{
		m_pendingAddActor << _actor;
	}

	void ActorArray::add(SharedObj<Actor>&& _actor)
	{
		m_pendingAddActor << std::forward<SharedObj<Actor>>(_actor);
	}

	bool ActorArray::remove(const WeakObj<Actor>& _actorRef)
	{
		for (auto& actor : m_actors)
		{
			if (actor == _actorRef)
			{
				actor->_internalNotifyDestroy();
				return true;
			}
		}
		return false;
	}

	bool ActorArray::removeByName(const String& _name)
	{
		for (auto& actor : m_actors)
		{
			if (actor->compareName(_name))
			{
				actor->_internalNotifyDestroy();
				return true;
			}
		}
		return false;
	}

	bool ActorArray::removeByTag(const String& _tag)
	{
		for (auto& actor : m_actors)
		{
			if (actor->compareTag(_tag))
			{
				actor->_internalNotifyDestroy();
			}
		}
		return true;
	}

	WeakObj<Actor> ActorArray::getActor(const String& _name) const
	{
		// Actorを名前で検索するためのlambda式
		auto func = [&_name](const PActor& actor)
		{
			return actor->compareName(_name);
		};

		// funcを使用してActorを検索
		auto found = std::ranges::find_if(m_actors, func);

		// Actorが見つかれば弱参照を返す
		if (found != m_actors.end())
		{
			return (*found).makeRef();
		}

		return WeakObj<Actor>{};
	}

	WeakObj<Actor> ActorArray::getActorByTag(const String& _tag) const
	{
		// Actorをタグで検索するためのlambda式
		auto func = [&_tag](const PActor& actor)
		{
			return actor->compareTag(_tag);
		};

		// funcを使用してActorを検索
		auto found = std::ranges::find_if(m_actors, func);

		// Actorが見つかれば弱参照を返す
		if (found != m_actors.end())
		{
			return (*found).makeRef();
		}

		return WeakObj<Actor>{};
	}

	Array<WeakObj<Actor>> ActorArray::getActors() const
	{
		Array<WeakObj<Actor>> result;
		result.reserve(m_actors.size());
		for (auto& actor : m_actors)
		{
			result << actor.makeRef();
		}
		return result;
	}

	Array<WeakObj<Actor>> ActorArray::getActorsByTag(const String& _tag) const
	{
		Array<WeakObj<Actor>> result;
		result.reserve(m_actors.size());
		for (auto& actor : m_actors)
		{
			if (actor->compareTag(_tag))
			{
				result << actor.makeRef();
			}
		}
		return result;
	}

	void ActorArray::update()
	{
		// Actorの削除が必要
		if (m_needRemove)
		{
			m_actors.remove(nullptr);
			m_needRemove = false;
		}

		// 追加待ちのActorを追加する
		if (not m_pendingAddActor.empty())
		{
			m_actors.append(m_pendingAddActor);
			m_pendingAddActor.clear();
		}

		for (auto& actor : m_actors)
		{
			// Actorが破棄待ちだったらnullptrを挿入して処理をスキップ
			if ((not actor) or actor->isPendingDestroy())
			{
				actor = nullptr;
				m_needRemove = true;
				continue;
			}

			actor->_internalUpdate();
		}
	}
}
