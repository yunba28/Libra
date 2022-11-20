#pragma once

#include "Object.hpp"
#include "SharedObj.hpp"

namespace Libra::Internal
{
	class PhysicsSystem final
	{
	public:

		PhysicsSystem();

		~PhysicsSystem();

		void add2D(const WeakObj<Object>& _physicsWeakObj);

		void add2D(WeakObj<Object>&& _physicsWeakObj);

		void joinCollider2D(P2BodyID _id, WeakObj<Object>&& _ref);

		void withdrawCollider2D(P2BodyID _id);

		void update();

		P2World getP2World()const noexcept;

		void setPhysicsTimestep(double _timestep)noexcept;

		double getPhysicsTimestep()const noexcept;

	private:

		Array<WeakObj<Object>> m_p2Object;
		Array<WeakObj<Object>> m_pendingP2Object;

		HashTable<P2BodyID, WeakObj<Object>> m_colliders;

		P2World m_p2World;

		double m_accumulationTime;
		double m_physicsTimestep;

		bool m_needRemoveByP2Object;

		void _updateP2World();

		static bool _internalRemoveConditions(const WeakObj<Object>& ref);
	};
}
