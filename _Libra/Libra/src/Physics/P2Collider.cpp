#include "../../include/P2Collider.hpp"
#include "../../include/SceneBase.hpp"
#include "../../include/RenderingSystem.hpp"
#include "../../include/PhysicsSystem.hpp"

namespace Libra
{
	P2Collider::P2Collider()
		: m_p2Body()
	{
	}

	void P2Collider::addCircle(Circle _localCircle, const P2Material& _material, const P2Filter& _filter)
	{
		m_p2Body.addCircle(_localCircle, _material, _filter);
	}

	void P2Collider::addCircleSensor(Circle _localCircle, const P2Filter& _filter)
	{
		m_p2Body.addCircleSensor(_localCircle,  _filter);
	}

	void P2Collider::addClosedLineString(const LineString& _localLineString, OneSided _oneSided, const P2Material& _material, const P2Filter& _filter)
	{
		m_p2Body.addClosedLineString(_localLineString, _oneSided, _material, _filter);
	}

	void P2Collider::addLine(const Line& _localLine, OneSided _oneSided, const P2Material& _material, const P2Filter& _filter)
	{
		m_p2Body.addLine(_localLine, _oneSided, _material, _filter);
	}

	void P2Collider::addLineString(const LineString& _localLineString, OneSided _oneSided, const P2Material& _material, const P2Filter& _filter)
	{
		m_p2Body.addLineString(_localLineString, _oneSided, _material, _filter);
	}

	void P2Collider::addPolygon(const Polygon& _localPolygon, const P2Material& _material, const P2Filter& _filter)
	{
		m_p2Body.addPolygon(_localPolygon, _material, _filter);
	}

	void P2Collider::addMultiPolygon(const MultiPolygon& _localMultiPolygon, const P2Material& _material, const P2Filter& _filter)
	{
		m_p2Body.addPolygons(_localMultiPolygon, _material, _filter);
	}

	void P2Collider::addQuad(const Quad& _localQuad, const P2Material& _material, const P2Filter& _filter)
	{
		m_p2Body.addQuad(_localQuad, _material, _filter);
	}

	void P2Collider::addRect(const RectF& _localRect, const P2Material& _material, const P2Filter& _filter)
	{
		m_p2Body.addRect(_localRect, _material, _filter);
	}

	void P2Collider::addTriangle(const Triangle& _localTriangle, const P2Material& _material, const P2Filter& _filter)
	{
		m_p2Body.addTriangle(_localTriangle, _material, _filter);
	}

	void P2Collider::setBodyType(P2BodyType _body)
	{
		m_p2Body.setBodyType(_body);
	}

	P2BodyType P2Collider::getBodyType() const
	{
		return m_p2Body.getBodyType();
	}

	void P2Collider::setDensity(double _density, size_t _index)
	{
		if (_index < getShapeCount())
		{
			m_p2Body.shape(_index).setDensity(_density);
			return;
		}

		Throwing<Exceptions::OutOfRange>("out of range");
	}

	void P2Collider::setDensityAllShapes(double _density)
	{
		for (auto i : step(getShapeCount()))
		{
			m_p2Body.shape(i).setDensity(_density);
		}
	}

	void P2Collider::setFilter(P2Filter _filter, size_t _index)
	{
		if (_index < getShapeCount())
		{
			m_p2Body.shape(_index).setFilter(_filter);
			return;
		}

		Throwing<Exceptions::OutOfRange>("out of range");
	}

	void P2Collider::setFilterAllShapes(P2Filter _filter)
	{
		for (auto i : step(getShapeCount()))
		{
			m_p2Body.shape(i).setFilter(_filter);
		}
	}

	void P2Collider::setFriction(double _friction, size_t _index)
	{
		if (_index < getShapeCount())
		{
			m_p2Body.shape(_index).setFriction(_friction);
			return;
		}

		Throwing<Exceptions::OutOfRange>("out of range");
	}

	void P2Collider::setFrictionAllShapes(double _friction)
	{
		for (auto i : step(getShapeCount()))
		{
			m_p2Body.shape(i).setFriction(_friction);
		}
	}

	void P2Collider::setRestitution(double _restitution, size_t _index)
	{
		if (_index < getShapeCount())
		{
			m_p2Body.shape(_index).setRestitution(_restitution);
			return;
		}

		Throwing<Exceptions::OutOfRange>("out of range");
	}

	void P2Collider::setRestitutionAllShapes(double _restitution)
	{
		for (auto i : step(getShapeCount()))
		{
			m_p2Body.shape(i).setRestitution(_restitution);
		}
	}

	void P2Collider::setRestitutionThreshold(double _restitutionThreshold, size_t _index)
	{
		if (_index < getShapeCount())
		{
			m_p2Body.shape(_index).setRestitutionThreshold(_restitutionThreshold);
		}

		Throwing<Exceptions::OutOfRange>("out of range");
	}

	void P2Collider::setRestitutionThresholdAllShapes(double _restitutionThreshold)
	{
		for (auto i : step(getShapeCount()))
		{
			m_p2Body.shape(i).setRestitutionThreshold(_restitutionThreshold);
		}
	}

	size_t P2Collider::getShapeCount() const
	{
		return m_p2Body.num_shapes();
	}

	void P2Collider::resetShapes()
	{
		auto ps = getActor()->getScene()->_getPhysicsSystem().lock();
		ps->withdrawCollider2D(m_p2Body.id());
		m_p2Body = ps->getP2World().createPlaceholder(m_p2Body.getBodyType(), m_p2Body.getPos());
		ps->joinCollider2D(m_p2Body.id(), reference.castTo<Object>());
	}

	void P2Collider::setPos(Vec2 _worldPos)
	{
		m_p2Body.setPos(_worldPos);
	}

	void P2Collider::setPos(double _worldX, double _worldY)
	{
		m_p2Body.setPos(_worldX, _worldY);
	}

	Vec2 P2Collider::getPos() const noexcept
	{
		return m_p2Body.getPos();
	}

	void P2Collider::setVelocity(Vec2 _velocity)
	{
		m_p2Body.setVelocity(_velocity);
	}

	void P2Collider::setVelocity(double _velocityX, double _velocityY)
	{
		m_p2Body.setVelocity(Vec2{ _velocityX,_velocityY });
	}

	Vec2 P2Collider::getVelocity() const
	{
		return m_p2Body.getVelocity();
	}

	void P2Collider::setAngle(double _angle)
	{
		m_p2Body.setAngle(_angle);
	}

	double P2Collider::getAngle() const noexcept
	{
		return m_p2Body.getAngle();
	}

	void P2Collider::setAngularVelocity(double _angularVelocity)
	{
		m_p2Body.setAngularVelocity(_angularVelocity);
	}

	double P2Collider::getAngularVelocity() const
	{
		return m_p2Body.getAngularVelocity();
	}

	void P2Collider::setAngularDamping(double _damping)
	{
		m_p2Body.setAngularDamping(_damping);
	}

	void P2Collider::setGravityScale(double _scale)
	{
		m_p2Body.setGravityScale(_scale);
	}

	void P2Collider::setFixedRotation(bool _flag)
	{
		m_p2Body.setFixedRotation(_flag);
	}

	void P2Collider::applyAngularImpulse(double _torque)
	{
		m_p2Body.applyAngularImpulse(_torque);
	}

	void P2Collider::applyForce(Vec2 _force)
	{
		m_p2Body.applyForce(_force);
	}

	void P2Collider::applyForce(Vec2 _force, Vec2 _offset)
	{
		m_p2Body.applyForce(_force, _offset);
	}

	void P2Collider::applyForceAt(Vec2 _force, Vec2 _pos)
	{
		m_p2Body.applyForceAt(_force, _pos);
	}

	void P2Collider::applyLinearImpulse(Vec2 _force)
	{
		m_p2Body.applyLinearImpulse(_force);
	}

	void P2Collider::applyLinearImpulse(Vec2 _force, Vec2 _offset)
	{
		m_p2Body.applyLinearImpulse(_force, _offset);
	}

	void P2Collider::applyLinearImpulseAt(Vec2 _force, Vec2 _pos)
	{
		m_p2Body.applyLinearImpulseAt(_force, _pos);
	}

	void P2Collider::applyTorque(double _torque)
	{
		m_p2Body.applyTorque(_torque);
	}

	void P2Collider::setDensity(double _density)
	{
		m_p2Body.shape(0).setDensity(_density);
	}

	void P2Collider::setFilter(P2Filter _filter)
	{
		m_p2Body.shape(0).setFilter(_filter);
	}

	void P2Collider::setFriction(double _friction)
	{
		m_p2Body.shape(0).setFriction(_friction);
	}

	void P2Collider::setRestitution(double _restitution)
	{
		m_p2Body.shape(0).setRestitution(_restitution);
	}

	void P2Collider::setRestitutionThreshold(double _restitutionThreshold)
	{
		m_p2Body.shape(0).setRestitutionThreshold(_restitutionThreshold);
	}

	void P2Collider::setViewWireframe(bool _flag)
	{
		setEnableDraw(_flag);
	}

	void P2Collider::draw() const
	{
		m_p2Body.drawWireframe(1.0, ColorF{0.0,1.0,0.7});
	}

	void P2Collider::fixedUpdate()
	{
		auto tf = getTransform();
		{
			tf->setPos(m_p2Body.getPos().xy0(), Space::World);
			tf->setRot(Mat4x4::RotateZ(m_p2Body.getAngle()));
		}

		// 衝突が行われたか
		bool isCollided = m_currentCollision.isCollided();

		// 前回衝突があったか
		bool isPrevCollided = m_prevCollision.isCollided();

		// イベント発生の条件が整っていない
		if ((not isPrevCollided) and (not isCollided))
		{
			m_prevCollision = Collision{};
			return;
		}

		auto refs = getActor()->getComponentAll();

		// 当たった瞬間
		if ((not isPrevCollided) and isCollided)
		{
			//最後の衝突を記憶
			m_lastCollision = m_currentCollision;
			callCollisionEvent(refs, &Object::onCollisionEnter);
			return;
		}

		// 当たってる間
		if (isPrevCollided and isCollided)
		{
			m_lastCollision = m_currentCollision;
			callCollisionEvent(refs, &Object::onCollisionStay);
			return;
		}

		// 離れた瞬間
		if (isPrevCollided and (not isCollided))
		{
			m_currentCollision = m_lastCollision;
			callCollisionEvent(refs, &Object::onCollisionExit);
			m_prevCollision = Collision{};
			return;
		}
	}

	void P2Collider::_internalConstruct()
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

	void P2Collider::_internalDestruct()
	{
		Object::_internalDestruct();
		auto ps = getActor()->getScene()->_getPhysicsSystem().lock();
		ps->withdrawCollider2D(m_p2Body.id());
	}

	void P2Collider::_internalOnCollision(const Collision& _collision)
	{
		m_currentCollision = _collision;
	}

	void P2Collider::callCollisionEvent(const Array<WeakObj<Component>>& _referents, void(Object::* _event)(const Collision&))
	{
		for (auto& ref : _referents)
		{
			if (not ref.isLinked())
				continue;

			((*ref.lock()).*_event)(m_currentCollision);
		}
		m_prevCollision = m_currentCollision;
		m_currentCollision = Collision{};
	}
	
}
