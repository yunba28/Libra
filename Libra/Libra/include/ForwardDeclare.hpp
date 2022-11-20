#pragma once

#include "Concepts.hpp"

#define DECLARE_MODEL( Base )\
template<class Derived>\
concept Model##Base = std::derived_from<Derived,Base>;

#define DECLARE_INTERNAL_MODEL( Base )\
namespace Internal{\
template<class Derived>\
concept Model##Base = std::derived_from<Derived,Base>;}

namespace Libra
{
	class Object;
	class Actor;
	class Component;
	class SceneBase;
	class World;
	class Transform;
	struct Collision;

	namespace Internal
	{
		class ActorArray;
		class ActorSystem;
		class ComponentArray;
		class ComponentSystem;
		class RenderingSystem;
		class PhysicsSystem;
	}

	DECLARE_INTERNAL_MODEL(Object);
	DECLARE_INTERNAL_MODEL(Actor);
	DECLARE_INTERNAL_MODEL(Component);
	DECLARE_INTERNAL_MODEL(SceneBase);
}
