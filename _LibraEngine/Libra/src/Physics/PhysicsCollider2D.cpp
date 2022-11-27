#include "../../include/PhysicsCollider2D.hpp"
#include "../../include/SceneBase.hpp"
#include "../../include/RenderingSystem.hpp"
#include "../../include/PhysicsSystem.hpp"

namespace Libra
{
	PhysicsCollider2D::PhysicsCollider2D()
		: m_p2Body()
		, m_collisions()
		, m_lastCollision()
		, m_prevCollided(false)
	{
	}

	void PhysicsCollider2D::addCircle(Circle _localCircle, const P2Material& _material, const P2Filter& _filter)
	{
		m_p2Body.addCircle(_localCircle, _material, _filter);
	}

	void PhysicsCollider2D::addCircleSensor(Circle _localCircle, const P2Filter& _filter)
	{
		m_p2Body.addCircleSensor(_localCircle,  _filter);
	}

	void PhysicsCollider2D::addClosedLineString(const LineString& _localLineString, OneSided _oneSided, const P2Material& _material, const P2Filter& _filter)
	{
		m_p2Body.addClosedLineString(_localLineString, _oneSided, _material, _filter);
	}

	void PhysicsCollider2D::addLine(const Line& _localLine, OneSided _oneSided, const P2Material& _material, const P2Filter& _filter)
	{
		m_p2Body.addLine(_localLine, _oneSided, _material, _filter);
	}

	void PhysicsCollider2D::addLineString(const LineString& _localLineString, OneSided _oneSided, const P2Material& _material, const P2Filter& _filter)
	{
		m_p2Body.addLineString(_localLineString, _oneSided, _material, _filter);
	}

	void PhysicsCollider2D::addPolygon(const Polygon& _localPolygon, const P2Material& _material, const P2Filter& _filter)
	{
		m_p2Body.addPolygon(_localPolygon, _material, _filter);
	}

	void PhysicsCollider2D::addMultiPolygon(const MultiPolygon& _localMultiPolygon, const P2Material& _material, const P2Filter& _filter)
	{
		m_p2Body.addPolygons(_localMultiPolygon, _material, _filter);
	}

	void PhysicsCollider2D::addQuad(const Quad& _localQuad, const P2Material& _material, const P2Filter& _filter)
	{
		m_p2Body.addQuad(_localQuad, _material, _filter);
	}

	void PhysicsCollider2D::addRect(const RectF& _localRect, const P2Material& _material, const P2Filter& _filter)
	{
		m_p2Body.addRect(_localRect, _material, _filter);
	}

	void PhysicsCollider2D::addTriangle(const Triangle& _localTriangle, const P2Material& _material, const P2Filter& _filter)
	{
		m_p2Body.addTriangle(_localTriangle, _material, _filter);
	}

	void PhysicsCollider2D::createPivotJoint(const WeakObj<PhysicsCollider2D>& _parent, Vec2 _localPivot, EnableCollision _enableCollision)
	{
		const P2Body& parentBody = _parent.lock()->m_p2Body;
		auto ps = getActor()->getScene()->_getPhysicsSystem().lock();
		m_pivotJoint = ps->getP2World().createPivotJoint(parentBody, m_p2Body, parentBody.getPos() + _localPivot, _enableCollision);
	}

	void PhysicsCollider2D::setBodyType(P2BodyType _body)
	{
		m_p2Body.setBodyType(_body);
	}

	P2BodyType PhysicsCollider2D::getBodyType() const
	{
		return m_p2Body.getBodyType();
	}

	void PhysicsCollider2D::setDensity(double _density, size_t _index)
	{
		if (_index < getShapeCount())
		{
			m_p2Body.shape(_index).setDensity(_density);
			return;
		}

		Throwing<Exceptions::OutOfRange>("out of range");
	}

	void PhysicsCollider2D::setDensityAllShapes(double _density)
	{
		for (auto i : step(getShapeCount()))
		{
			m_p2Body.shape(i).setDensity(_density);
		}
	}

	void PhysicsCollider2D::setFilter(P2Filter _filter, size_t _index)
	{
		if (_index < getShapeCount())
		{
			m_p2Body.shape(_index).setFilter(_filter);
			return;
		}

		Throwing<Exceptions::OutOfRange>("out of range");
	}

	void PhysicsCollider2D::setFilterAllShapes(P2Filter _filter)
	{
		for (auto i : step(getShapeCount()))
		{
			m_p2Body.shape(i).setFilter(_filter);
		}
	}

	void PhysicsCollider2D::setFriction(double _friction, size_t _index)
	{
		if (_index < getShapeCount())
		{
			m_p2Body.shape(_index).setFriction(_friction);
			return;
		}

		Throwing<Exceptions::OutOfRange>("out of range");
	}

	void PhysicsCollider2D::setFrictionAllShapes(double _friction)
	{
		for (auto i : step(getShapeCount()))
		{
			m_p2Body.shape(i).setFriction(_friction);
		}
	}

	void PhysicsCollider2D::setRestitution(double _restitution, size_t _index)
	{
		if (_index < getShapeCount())
		{
			m_p2Body.shape(_index).setRestitution(_restitution);
			return;
		}

		Throwing<Exceptions::OutOfRange>("out of range");
	}

	void PhysicsCollider2D::setRestitutionAllShapes(double _restitution)
	{
		for (auto i : step(getShapeCount()))
		{
			m_p2Body.shape(i).setRestitution(_restitution);
		}
	}

	void PhysicsCollider2D::setRestitutionThreshold(double _restitutionThreshold, size_t _index)
	{
		if (_index < getShapeCount())
		{
			m_p2Body.shape(_index).setRestitutionThreshold(_restitutionThreshold);
		}

		Throwing<Exceptions::OutOfRange>("out of range");
	}

	void PhysicsCollider2D::setRestitutionThresholdAllShapes(double _restitutionThreshold)
	{
		for (auto i : step(getShapeCount()))
		{
			m_p2Body.shape(i).setRestitutionThreshold(_restitutionThreshold);
		}
	}

	size_t PhysicsCollider2D::getShapeCount() const
	{
		return m_p2Body.num_shapes();
	}

	void PhysicsCollider2D::setAwake(bool _flag)
	{
		m_p2Body.setAwake(_flag);
	}

	void PhysicsCollider2D::setSleep(bool _flag)
	{
		m_p2Body.setSleepEnabled(_flag);
	}

	void PhysicsCollider2D::resetShapes()
	{
		auto ps = getActor()->getScene()->_getPhysicsSystem().lock();
		ps->withdrawCollider2D(m_p2Body.id());
		m_p2Body = ps->getP2World().createPlaceholder(m_p2Body.getBodyType(), m_p2Body.getPos());
		ps->joinCollider2D(m_p2Body.id(), reference.castTo<Object>());
	}

	void PhysicsCollider2D::setPos(Vec2 _worldPos)
	{
		m_p2Body.setPos(_worldPos);
	}

	void PhysicsCollider2D::setPos(double _worldX, double _worldY)
	{
		m_p2Body.setPos(_worldX, _worldY);
	}

	Vec2 PhysicsCollider2D::getPos() const noexcept
	{
		return m_p2Body.getPos();
	}

	void PhysicsCollider2D::setVelocity(Vec2 _velocity)
	{
		m_p2Body.setVelocity(_velocity);
	}

	void PhysicsCollider2D::setVelocity(double _velocityX, double _velocityY)
	{
		m_p2Body.setVelocity(Vec2{ _velocityX,_velocityY });
	}

	Vec2 PhysicsCollider2D::getVelocity() const
	{
		return m_p2Body.getVelocity();
	}

	void PhysicsCollider2D::setAngle(double _angle)
	{
		m_p2Body.setAngle(_angle);
	}

	double PhysicsCollider2D::getAngle() const noexcept
	{
		return m_p2Body.getAngle();
	}

	void PhysicsCollider2D::setAngularVelocity(double _angularVelocity)
	{
		m_p2Body.setAngularVelocity(_angularVelocity);
	}

	double PhysicsCollider2D::getAngularVelocity() const
	{
		return m_p2Body.getAngularVelocity();
	}

	void PhysicsCollider2D::setAngularDamping(double _damping)
	{
		m_p2Body.setAngularDamping(_damping);
	}

	void PhysicsCollider2D::setGravityScale(double _scale)
	{
		m_p2Body.setGravityScale(_scale);
	}

	void PhysicsCollider2D::setFixedRotation(bool _flag)
	{
		m_p2Body.setFixedRotation(_flag);
	}

	void PhysicsCollider2D::applyAngularImpulse(double _torque)
	{
		m_p2Body.applyAngularImpulse(_torque);
	}

	void PhysicsCollider2D::applyForce(Vec2 _force)
	{
		m_p2Body.applyForce(_force);
	}

	void PhysicsCollider2D::applyForce(Vec2 _force, Vec2 _offset)
	{
		m_p2Body.applyForce(_force, _offset);
	}

	void PhysicsCollider2D::applyForceAt(Vec2 _force, Vec2 _pos)
	{
		m_p2Body.applyForceAt(_force, _pos);
	}

	void PhysicsCollider2D::applyLinearImpulse(Vec2 _force)
	{
		m_p2Body.applyLinearImpulse(_force);
	}

	void PhysicsCollider2D::applyLinearImpulse(Vec2 _force, Vec2 _offset)
	{
		m_p2Body.applyLinearImpulse(_force, _offset);
	}

	void PhysicsCollider2D::applyLinearImpulseAt(Vec2 _force, Vec2 _pos)
	{
		m_p2Body.applyLinearImpulseAt(_force, _pos);
	}

	void PhysicsCollider2D::applyTorque(double _torque)
	{
		m_p2Body.applyTorque(_torque);
	}

	void PhysicsCollider2D::setViewWireframe(bool _flag)
	{
		setEnableDraw(_flag);
	}

	void PhysicsCollider2D::draw() const
	{
		m_p2Body.drawWireframe(1.0, ColorF{0.0,1.0,0.7});
	}

	void PhysicsCollider2D::fixedUpdate()
	{
		auto tf = getTransform();
		{
			tf->setPos(m_p2Body.getPos().xy0(), Space::World);
			tf->setRot(Mat4x4::RotateZ(m_p2Body.getAngle()));
		}

		// 衝突が行われたか
		bool isCollided = not m_collisions.empty();

		// イベント発生の条件が整っていない
		if ((not m_prevCollided) and (not isCollided))
			return;

		auto refs = getActor()->getComponentAll();

		// 当たった瞬間
		if ((not m_prevCollided) and isCollided)
		{
			//最後の衝突を記憶
			m_lastCollision = m_collisions.front();
			callCollisionEvent(refs, &Object::onCollisionEnter);
			return;
		}

		// 当たってる間
		if (m_prevCollided and isCollided)
		{
			m_lastCollision = m_collisions.front();
			callCollisionEvent(refs, &Object::onCollisionStay);
			return;
		}

		// 離れた瞬間
		if (m_prevCollided and (not isCollided))
		{
			m_collisions << m_lastCollision;
			callCollisionEvent(refs, &Object::onCollisionExit);
			m_prevCollided = false;
			return;
		}
	}

	void PhysicsCollider2D::_internalConstruct()
	{
		Object::_internalConstruct();

		//PhysicsSystemへの設定
		{
			auto ps = getActor()->getScene()->_getPhysicsSystem().lock();
			m_p2Body = ps->getP2World().createPlaceholder(P2Dynamic, Vec2{ 0,0 });
			ps->joinCollider2D(m_p2Body.id(), reference.castTo<Object>());
			setEnableFixedUpdate(true);
			applyFixedUpdate();
		}

		//RenderingSystemへの設定
		applyPlaneRendering();
	}

	void PhysicsCollider2D::_internalDestruct()
	{
		Object::_internalDestruct();
		auto ps = getActor()->getScene()->_getPhysicsSystem().lock();
		ps->withdrawCollider2D(m_p2Body.id());
	}

	void PhysicsCollider2D::_internalOnCollision(const Collision& _collision)
	{
		m_collisions << _collision;
	}

	void PhysicsCollider2D::callCollisionEvent(const Array<WeakObj<Component>>& _referents, void(Object::* _event)(const Collision&))
	{
		for (auto& ref : _referents)
		{
			if (not ref.isLinked())
				continue;

			for (auto& collision : m_collisions)
			{
				((*ref.lock()).*_event)(collision);
			}
		}
		m_prevCollided = true;
		m_collisions.clear();
	}
	
}
