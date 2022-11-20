#include "../../include/SceneBase.hpp"
#include "../../include/ComponentSystem.hpp"
#include "../../include/RenderingSystem.hpp"
#include "../../include/PhysicsSystem.hpp"

namespace Libra
{
	Component::Component()
		: m_actor(nullptr)
	{
	}

	Component::~Component()
	{
	}

	ObjHandle<Actor> Component::getActor()const  noexcept
	{
		return m_actor.lock();
	}

	ObjHandle<Transform> Component::getTransform() const noexcept
	{
		if (auto actor = getActor(); actor)
		{
			return actor->getTransform();
		}

		//空のハンドルを生成
		return WeakObj<Transform>{}.lock();
	}

	bool Component::compareName(const String& _name) const noexcept
	{
		if (auto actor = getActor(); actor)
		{
			return actor->compareName(_name);
		}
		return false;
	}

	bool Component::compareTag(const String& _tag) const noexcept
	{
		if (auto actor = getActor(); actor)
		{
			return actor->compareTag(_tag);
		}
		return false;
	}

	void Component::_internalUpdate()
	{
		auto actor = getActor();

		// Actorが死んでいる
		if (not actor)
		{
			_internalNotifyDestroy();
			return;
		}

		// Actorが有効じゃない
		if (not actor->isActive())
			return;

		Object::_internalUpdate();
	}

	void Component::Destroy(const WeakObj<Component>& _componentRef) noexcept
	{
		auto component = _componentRef.lock();

		// 空のComponent
		if (not component)
			return;

		auto actor = component->getActor();

		// Actorが死んでいる
		if (not actor)
		{
			component->_internalNotifyDestroy();
			return;
		}

		actor->detachComponent(_componentRef);
	}

	void Component::Destroy(ObjHandle<Component>&& _componentRef) noexcept
	{
		// 空のComponent
		if (not _componentRef)
			return;

		auto actor = _componentRef->getActor();

		// Actorが死んでいる
		if (not actor)
		{
			_componentRef->_internalNotifyDestroy();
		}

		actor->detachComponent(_componentRef);
	}

	void Component::applySpaceRendering()
	{
		// まだ3Dレンダリング可能ではない
		if (stateNone(7))
		{
			auto rs = getActor()->getScene()->_getRenderingSystem().lock();
			rs->add3D(reference.castTo<Object>());
			// Object側で3D描画可能のフラグを立ててもらう
			Object::applySpaceRendering();
		}
	}

	void Component::applyPlaneRendering()
	{
		// まだ2Dレンダリング可能ではない
		if (stateNone(6))
		{
			auto rs = getActor()->getScene()->_getRenderingSystem().lock();
			rs->add2D(reference.castTo<Object>());
			// Object側で2D描画可能のフラグを立ててもらう
			Object::applyPlaneRendering();
		}
	}

	void Component::applyFixedUpdate()
	{
		// まだfixedUpdateが使えない
		if (stateNone(8))
		{
			auto ps = getActor()->getScene()->_getPhysicsSystem().lock();
			ps->add2D(reference.castTo<Object>());
			// Object側でfixedUpdateが使えるようになったフラグを立ててもらう
			Object::applyFixedUpdate();
		}
	}

	void Component::addComponentToScene(const TypeID& _type, SharedObj<Component>&& _component)
	{
		auto cs = getActor()->getScene()->_getComponentSystem().lock();
		cs->add(_type, std::forward<SharedObj<Component>>(_component));
	}
}
