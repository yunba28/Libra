#include "../../include/SceneBase.hpp"
#include "../../include/ObjectPriority.hpp"
#include "../../include/World.hpp"
#include "../../include/ActorSystem.hpp"
#include "../../include/ComponentSystem.hpp"
#include "../../include/RenderingSystem.hpp"
#include "../../include/PhysicsSystem.hpp"

namespace Libra
{

	SceneBase::SceneBase()
		: reference()
		, m_world(nullptr)
		, m_actorSystem(MakeShared<Internal::ActorSystem>())
		, m_componentSystem(MakeShared<Internal::ComponentSystem>())
		, m_renderingSystem(MakeShared<Internal::RenderingSystem>())
		, m_physicsSystem(MakeShared<Internal::PhysicsSystem>())
	{
		setEnableDraw(true);
	}

	void SceneBase::updateFadeIn(double)
	{
		_internalUpdate();
	}

	void SceneBase::updateFadeOut(double)
	{
		_internalUpdate();
	}

	void SceneBase::drawFadeIn(double t)const
	{
		_internalDraw();

		Transformer2D transformer{ Mat3x2::Identity(), Transformer2D::Target::SetLocal };

		s3d::Scene::Rect().draw(ColorF{ m_world.lock()->getFadeColor(), (1.0 - t)});
	}

	void SceneBase::drawFadeOut(double t)const
	{
		_internalDraw();

		Transformer2D transformer{ Mat3x2::Identity(), Transformer2D::Target::SetLocal };

		Scene::Rect().draw(ColorF{ m_world.lock()->getFadeColor(), t });
	}

	ObjHandle<World> SceneBase::getWorld() const noexcept
	{
		return m_world.lock();
	}

	void SceneBase::notifyError()
	{
		if (auto world = m_world.lock(); world)
		{
			world->notifyError();
		}
	}

	bool SceneBase::changeScene(const String& _name, const Duration& _toransitionTime, CrossFade _crossFade)
	{
		if (auto world = m_world.lock(); world)
		{
			return world->changeScene(_name, _toransitionTime, _crossFade);
		}
		return false;
	}

	bool SceneBase::changeScene(const String& _name, const int32 _toransitionTimeMilliSec, CrossFade _crossFade)
	{
		if (auto world = m_world.lock(); world)
		{
			return world->changeScene(_name, _toransitionTimeMilliSec, _crossFade);
		}
		return false;
	}

	WeakObj<BasicCamera3D> SceneBase::getCamera3D() const noexcept
	{
		return m_renderingSystem->getCamera3D();
	}

	WeakObj<BasicCamera2D> SceneBase::getCamera2D() const noexcept
	{
		return m_renderingSystem->getCamera2D();
	}

	P2World SceneBase::getP2World() const noexcept
	{
		return m_physicsSystem->getP2World();
	}

	void SceneBase::setBackgroundColor(const Color& _color)
	{
		m_renderingSystem->setBackgroundColor(_color);
	}

	const Color& SceneBase::getBackgroundColor() const noexcept
	{
		return m_renderingSystem->getBackgroundColor();
	}

	void SceneBase::setPhysicsTimestep(double _timestep) noexcept
	{
		m_physicsSystem->setPhysicsTimestep(_timestep);
	}

	double SceneBase::getPhysicsTimestep() const noexcept
	{
		return m_physicsSystem->getPhysicsTimestep();;
	}

	WeakObj<Actor> SceneBase::getActor(const TypeID& _type, const String& _name) const
	{
		return m_actorSystem->getActor(_type, _name);
	}

	WeakObj<Actor> SceneBase::getActorByTag(const TypeID& _type, const String& _tag) const
	{
		return m_actorSystem->getActorByTag(_type, _tag);
	}

	Array<WeakObj<Actor>> SceneBase::getActorAll() const
	{
		return m_actorSystem->getActorAll();
	}

	void SceneBase::destroyActor(const WeakObj<Actor>& _actor)
	{
		Actor::Destroy(_actor);
	}

	void SceneBase::destroyActor(const String& _name)
	{
		m_actorSystem->removeByName(_name);
	}

	void SceneBase::destroyActorsByTag(const String& _tag)
	{
		m_actorSystem->removeByTag(_tag);
	}

	void SceneBase::_internalUpdate()
	{
		m_componentSystem->update();
		m_actorSystem->update();
		m_physicsSystem->update();
		m_renderingSystem->update();

		Object::_internalUpdate();
	}

	void SceneBase::_internalDraw() const
	{
		m_renderingSystem->draw();

		Object::_internalDraw();
	}

	WeakObj<Internal::ActorSystem> SceneBase::_getActorSystem() const noexcept
	{
		return m_actorSystem.makeRef();
	}

	WeakObj<Internal::ComponentSystem> SceneBase::_getComponentSystem() const noexcept
	{
		return m_componentSystem.makeRef();;
	}

	WeakObj<Internal::RenderingSystem> SceneBase::_getRenderingSystem() const noexcept
	{
		return m_renderingSystem.makeRef();
	}

	WeakObj<Internal::PhysicsSystem> SceneBase::_getPhysicsSystem() const noexcept
	{
		return m_physicsSystem.makeRef();
	}
}
