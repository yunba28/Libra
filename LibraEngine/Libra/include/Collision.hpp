#pragma once

#include <Siv3D.hpp>
#include "Object.hpp"
#include "SharedObj.hpp"

namespace Libra
{
	class PhysicsCollider2D;

	struct Collision
	{
		struct P2
		{
			WeakObj<PhysicsCollider2D> opponent{}; // 衝突した相手コライダー
			P2BodyID a{}; // 自身のP2BodyID
			P2BodyID b{}; // 相手のP2BodyID
			Vec2 point{ 0,0 }; // 衝突座標
			Vec2 normal{ 0,0 }; // 衝突した部分からの法線
			double normalImpulse{};
			double tangentImpulse{};
			bool operator==(const P2& _other)const noexcept { return a == _other.a and b == _other.b; }
		}p2;

	private:

		struct P3
		{
			WeakObj<class PhysicsCollider3D> opponent{}; // 衝突した相手コライダー
			Vec3 point{ 0,0,0 }; // 衝突座標
			Vec3 normal{ 0,0,0 }; // 衝突した部分からの法線
			double normalImpulse{};
			double tangentImpulse{};
			bool operator==(const P3&)const noexcept { return false; }
		}p3;

	public:

		bool isCollided()const noexcept
		{
			return p2.opponent.lock().operator bool() or
				p3.opponent.lock().operator bool();
		}

		bool operator==(const Collision& _other)const
		{
			return (p2 == _other.p2) or (p3 == _other.p3);
		}

	};
}
