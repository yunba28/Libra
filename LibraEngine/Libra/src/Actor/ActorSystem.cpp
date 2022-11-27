#include "../../include/ActorSystem.hpp"
#include "../../include/ObjectExecutionOrder.hpp"

namespace Libra::Internal
{
	ActorSystem::ActorSystem()
		: m_actorArrays()
		, m_sortedTypeList()
		, m_pendingAddType()
	{
	}

	ActorSystem::~ActorSystem()
	{
	}

	void ActorSystem::add(const TypeID& _type, const SharedObj<Actor>& _actor)
	{
		if (not m_actorArrays.contains(_type))
		{
			m_pendingAddType << _type;
			m_actorArrays.emplace(_type, ActorArray{});
		}
		m_actorArrays[_type].add(_actor);
	}

	void ActorSystem::add(const TypeID& _type, SharedObj<Actor>&& _actor)
	{
		if (not m_actorArrays.contains(_type))
		{
			m_pendingAddType << _type;
			m_actorArrays.emplace(_type, ActorArray{});
		}
		m_actorArrays[_type].add(std::forward<SharedObj<Actor>>(_actor));
	}

	bool ActorSystem::remove(const WeakObj<Actor>& _actorRef)
	{
		for (auto& type : m_sortedTypeList)
		{
			if (m_actorArrays[type].remove(_actorRef))
				return true;
		}
		return false;
	}

	bool ActorSystem::remove(const TypeID& _type, const WeakObj<Actor>& _actorRef)
	{
		if (m_actorArrays.contains(_type))
		{
			return m_actorArrays[_type].remove(_actorRef);
		}
		return false;
	}

	bool ActorSystem::removeByName(const String& _name)
	{
		for (auto& type : m_sortedTypeList)
		{
			if (m_actorArrays[type].removeByName(_name))
			{
				return true;
			}
		}
		return false;
	}

	bool ActorSystem::removeByTag(const String& _tag)
	{
		bool result = false;
		for (auto& type : m_sortedTypeList)
		{
			result |= m_actorArrays[type].removeByTag(_tag);
		}
		return result;
	}

	WeakObj<Actor> ActorSystem::getActor(const TypeID& _type, const String& _name) const
	{
		if (m_actorArrays.contains(_type))
		{
			return m_actorArrays.at(_type).getActor(_name);
		}
		return WeakObj<Actor>{};
	}

	WeakObj<Actor> ActorSystem::getActorByTag(const TypeID& _type, const String& _tag) const
	{
		if (m_actorArrays.contains(_type))
		{
			return m_actorArrays.at(_type).getActorByTag(_tag);
		}
		return WeakObj<Actor>{};
	}

	Array<WeakObj<Actor>> ActorSystem::getActors(const TypeID& _type) const
	{
		if (m_actorArrays.contains(_type))
		{
			return m_actorArrays.at(_type).getActors();
		}
		return Array<WeakObj<Actor>>{};
	}

	Array<WeakObj<Actor>> ActorSystem::getActorAll() const
	{
		Array<WeakObj<Actor>> result;
		for (auto& type : m_sortedTypeList)
		{
			if (auto res = m_actorArrays.at(type).getActors(); res)
			{
				result.append(res);
			}
		}
		return result;
	}

	Array<WeakObj<Actor>> ActorSystem::getActorsByTag(const String& _tag) const
	{
		Array<WeakObj<Actor>> result;
		for (auto& type : m_sortedTypeList)
		{
			if (Array<WeakObj<Actor>> res = m_actorArrays.at(type).getActorsByTag(_tag); not res.empty())
			{
				result.append(res);
			}
		}
		return result;
	}

	Array<WeakObj<Actor>> ActorSystem::getActorsByTag(const TypeID& _type, const String& _tag) const
	{
		if (m_actorArrays.contains(_type))
		{
			return m_actorArrays.at(_type).getActorsByTag(_tag);
		}
		return Array<WeakObj<Actor>>{};
	}

	void ActorSystem::update()
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
			m_actorArrays[type].update();
		}
	}
}
