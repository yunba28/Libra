#include "../../include/PhysicsSystem.hpp"
#include "../../include/ObjectPriority.hpp"
#include "../../include/Collision.hpp"

namespace Libra::Internal
{

	PhysicsSystem::PhysicsSystem()
		: m_p2Object()
		, m_pendingP2Object()
		, m_colliders()
		, m_p2World(980)
		, m_accumulationTime(0)
		, m_physicsTimestep(1.0 / 60.0)
		, m_needRemoveByP2Object(false)
	{

	}

	PhysicsSystem::~PhysicsSystem()
	{
	}

	void PhysicsSystem::add2D(const WeakObj<Object>& _physicsWeakObj)
	{
		m_pendingP2Object << _physicsWeakObj;
	}

	void PhysicsSystem::add2D(WeakObj<Object>&& _physicsWeakObj)
	{
		m_pendingP2Object << std::forward<WeakObj<Object>>(_physicsWeakObj);
	}

	void PhysicsSystem::joinCollider2D(P2BodyID _id, WeakObj<Object>&& _ref)
	{
		if (not m_colliders.contains(_id))
		{
			m_colliders.emplace(_id,std::forward<WeakObj<Object>>(_ref));
		}
	}

	void PhysicsSystem::withdrawCollider2D(P2BodyID _id)
	{
		if (m_colliders.contains(_id))
		{
			m_colliders.erase(_id);
		}
	}

	void PhysicsSystem::update()
	{
		for (m_accumulationTime += Scene::DeltaTime(); m_physicsTimestep <= m_accumulationTime;m_accumulationTime -= m_physicsTimestep)
		{
			if (m_needRemoveByP2Object)
			{
				m_p2Object.remove_if(_internalRemoveConditions);
				m_needRemoveByP2Object = false;
			}

			if (m_pendingP2Object)
			{
				m_p2Object.append(m_pendingP2Object);
				m_pendingP2Object.clear();
			}

			// 2D 物理演算のワールドを更新
			_updateP2World();
		}
	}

	P2World PhysicsSystem::getP2World() const noexcept
	{
		return m_p2World;
	}

	void PhysicsSystem::setPhysicsTimestep(double _timestep) noexcept
	{
		m_physicsTimestep = _timestep;
	}

	double PhysicsSystem::getPhysicsTimestep() const noexcept
	{
		return m_physicsTimestep;
	}

	void PhysicsSystem::_updateP2World()
	{
		m_p2World.update(m_physicsTimestep);

		for (const auto& [pair, collision] : m_p2World.getCollisions())
		{
			const auto& refA = m_colliders.at(pair.a);
			const auto& refB = m_colliders.at(pair.b);

			Collision collA{};
			{
				collA.other = refB;
				collA.info2d.normal = collision.normal();
			}

			Collision collB{};
			{
				collB.other = refA;
				collB.info2d.normal = -collision.normal();
			}

			if (collision.hasContacts())
			{
				const auto& contact = collision.contact(0);

				collA.info2d.point = contact.point;
				collA.info2d.normalImpulse = contact.normalImpulse;
				collA.info2d.tangentImpulse = contact.tangentImpulse;

				collB.info2d.point = contact.point;
				collB.info2d.normalImpulse = contact.normalImpulse;
				collB.info2d.tangentImpulse = contact.tangentImpulse;
			}

			refA.lock()->_internalOnCollision(collA);
			refB.lock()->_internalOnCollision(collB);
		}

		for (auto& ref : m_p2Object)
		{
			auto obj = ref.lock();
			if ((not obj) or obj->isPendingDestroy())
			{
				m_needRemoveByP2Object = true;
				continue;
			}
			obj->_internalFixedUpdate();
		}
	}

	bool PhysicsSystem::_internalRemoveConditions(const WeakObj<Object>& ref)
	{
		auto obj = ref.lock();
		return (not obj) or obj->isPendingDestroy();
	}
}
