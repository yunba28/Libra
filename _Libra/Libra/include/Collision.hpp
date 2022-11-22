#pragma once

#include <Siv3D.hpp>
#include "Object.hpp"
#include "SharedObj.hpp"

namespace Libra
{
	struct Collision
	{
		WeakObj<Object> object{};
		WeakObj<Object> other{};

		struct P2Collision
		{
			P2BodyID a{};
			P2BodyID b{};
			Vec2 point{0,0};
			Vec2 normal{ 0,0 };
			double normalImpulse{};
			double tangentImpulse{};
		};
		P2Collision info2d{};

	private:

		struct P3Collision
		{
			Vec3 point{0,0,0};
		};
		P3Collision info3d{};

	public:

		bool isCollided()const noexcept { return other.lock().operator bool(); }

	};
}
