#pragma once

#include "Component.hpp"
#include "Collision.hpp"

namespace Libra
{
	class P2Collider : public Component
	{
	public:

		P2Collider();

		void addCircle(
			Circle _localCircle,
			const P2Material& _material = P2Material{},
			const P2Filter& _filter = P2Filter{});

		void addCircleSensor(
			Circle _localCircle,
			const P2Filter& _filter = P2Filter{});

		void addClosedLineString(
			const LineString& _localLineString,
			OneSided _oneSided = OneSided::No,
			const P2Material& _material = P2Material{},
			const P2Filter& _filter = P2Filter{});

		void addLine(
			const Line& _localLine,
			OneSided _oneSided = OneSided::No,
			const P2Material& _material = P2Material{},
			const P2Filter& _filter = P2Filter{});


		void addLineString(
			const LineString& _localLineString,
			OneSided _oneSided = OneSided::No,
			const P2Material& _material = P2Material{},
			const P2Filter& _filter = P2Filter{});

		void addPolygon(
			const Polygon& _localPolygon,
			const P2Material& _material = P2Material{},
			const P2Filter& _filter = P2Filter{});

		void addMultiPolygon(
			const MultiPolygon& _localPolygon,
			const P2Material& _material = P2Material{},
			const P2Filter& _filter = P2Filter{});

		void addQuad(
			const Quad& _localQuad,
			const P2Material& _material = P2Material{},
			const P2Filter& _filter = P2Filter{});

		void addRect(
			const RectF& _localRect,
			const P2Material& _material = P2Material{},
			const P2Filter& _filter = P2Filter{});

		void addTriangle(
			const Triangle& _localTriangle,
			const P2Material& _material = P2Material{},
			const P2Filter& _filter = P2Filter{});

		void setDistanceJoint(
			const WeakObj<P2Collider>& _other,
			Vec2 _jointPos,
			double _length,
			EnableCollision _enableCollision = EnableCollision::Yes);

		/// @brief MultiColliderに設定されたすべての部品を解除し新たなプレースホルダを生成します
		void resetShapes();

		void setPos(Vec2 _worldPos);

		void setPos(double _worldX, double _worldY);

		Vec2 getPos()const noexcept;

		void setVelocity(Vec2 _velocity);

		void setVelocity(double _velocityX, double _velocityY);

		Vec2 getVelocity()const;

		void setAngle(double _angle);

		double getAngle()const noexcept;

		void setAngularVelocity(double _angularVelocity);

		double getAngularVelocity()const;

		void setAngularDamping(double _damping);

		void setGravityScale(double _scale);

		void setFixedRotation(bool _flag);

		void applyAngularImpulse(double _torque);

		void applyForce(Vec2 _force);

		void applyForce(Vec2 _force, Vec2 _offset);

		void applyForceAt(Vec2 _force, Vec2 _pos);

		void applyLinearImpulse(Vec2 _force);

		void applyLinearImpulse(Vec2 _force, Vec2 _offset);

		void applyLinearImpulseAt(Vec2 _force, Vec2 _pos);

		void applyTorque(double _torque);

		void setBodyType(P2BodyType _body);

		P2BodyType getBodyType()const;

		void setDensity(double _density, size_t _index);

		void setDensityAllShapes(double _density);

		void setFilter(P2Filter _filter, size_t _index);

		void setFilterAllShapes(P2Filter _filter);

		void setFriction(double _friction, size_t _index);

		void setFrictionAllShapes(double _friction);

		void setRestitution(double _restitution, size_t _index);

		void setRestitutionAllShapes(double _restitution);

		void setRestitutionThreshold(double _restitutionThreshold, size_t _index);

		void setRestitutionThresholdAllShapes(double _restitutionThreshold);

		size_t getShapeCount()const;

		void setAwake(bool _flag);

		void setSleep(bool _flag);

		void setViewWireframe(bool _flag);

		void start()override {}

		void update()override {}

		void draw()const override;

		void fixedUpdate()override final;

		void _internalConstruct()override final;

		void _internalDestruct()override final;

		void _internalOnCollision(const Collision& _collision)override final;

	protected:

		P2Body m_p2Body;

		Array<Collision> m_collisions{};

		/// @brief 最後に当たった衝突
		Collision m_lastCollision{};

		bool m_prevCollided;

		void callCollisionEvent(const Array<WeakObj<Component>>& _referents, void(Object::* _event)(const Collision&));
	};
}
