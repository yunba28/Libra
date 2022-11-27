#include "../../include/SceneBase.hpp" // included "Actor.hpp"
#include "../../include/ActorSystem.hpp"
#include "../../include/ComponentSystem.hpp"
#include "../../include/RenderingSystem.hpp"
#include "../../include/PhysicsSystem.hpp"

namespace Libra
{
	Actor::Actor()
		: reference()
		, m_scene(nullptr)
		, m_transform(nullptr)
		, m_name(U"Actor"_s)
		, m_tagSet()
		, m_components()
	{
	}

	Actor::~Actor()
	{
	}

	void Actor::destruct()
	{
		detachComponentAll();
	}

	ObjHandle<SceneBase> Actor::getScene() const noexcept
	{
		return m_scene.lock();
	}

	void Actor::setupTransform(const WeakObj<Transform>& _transform)
	{
		m_transform = _transform;
	}

	ObjHandle<Transform> Actor::getTransform() const noexcept
	{
		return m_transform.lock();
	}

	void Actor::setName(const String& _name)
	{
		m_name.set(_name);
	}

	const String& Actor::getName() const
	{
		return m_name.get();
	}

	bool Actor::compareName(const String& _name) const noexcept
	{
		return m_name == _name;
	}

	void Actor::addTag(const String& _tag)
	{
		CommonTag tag{ _tag };
		if (not m_tagSet.contains(tag))
		{
			m_tagSet.emplace(std::move(tag));
		}
	}

	void Actor::removeTag(const String& _tag)
	{
		CommonTag tag{ _tag };
		if (m_tagSet.contains(tag))
		{
			m_tagSet.erase(tag);
		}
	}

	Array<String> Actor::getTagList() const
	{
		Array<String> result{};
		result.reserve(m_tagSet.size());
		for (auto& tag : m_tagSet)
		{
			result << tag.get();
		}
		return result;
	}

	bool Actor::compareTag(const String& _tag) const noexcept
	{
		CommonTag tag{ _tag };
		return m_tagSet.contains(tag);
	}

	bool Actor::compareAnyTag(const Array<String>& _tag) const noexcept
	{
		for (auto& str : _tag)
		{
			CommonTag tag{ str };
			if (m_tagSet.contains(tag))
			{
				return true;
			}
		}
		return false;
	}

	bool Actor::compareAllTag(const Array<String>& _tag) const noexcept
	{
		for (auto& str : _tag)
		{
			CommonTag tag{ str };
			if (not m_tagSet.contains(tag))
			{
				return false;
			}
		}
		return true;
	}

	WeakObj<Component> Actor::getComponent(const TypeID& _type) const
	{
		if (auto found = findComponent(_type); found != m_components.end())
		{
			return (*found).component;
		}
		return WeakObj<Component>{};
	}

	Array<WeakObj<Component>> Actor::getComponents(const TypeID& _type) const
	{
		Array<WeakObj<Component>> result{};
		result.reserve(m_components.size());
		for (auto& ref : m_components)
		{
			if (ref.tyep == _type)
				result << ref.component;
		}
		return result;
	}

	Array<WeakObj<Component>> Actor::getComponentAll() const
	{
		Array<WeakObj<Component>> result;
		result.reserve(m_components.size());
		for (auto& ref: m_components)
		{
			result << ref.component;
		}
		return result;
	}

	void Actor::detachComponent(const TypeID& _type)
	{
		if (auto found = findComponent(_type); found != m_components.end())
		{
			found->component.lock()->_internalNotifyDestroy();
			m_components.erase(found);
		}
	}

	void Actor::detachComponent(const WeakObj<Component>& _componentRef)
	{
		for (auto itr = m_components.begin();itr != m_components.end();++itr)
		{
			if ((*itr).component == _componentRef)
			{
				(*itr).component.lock()->_internalNotifyDestroy();
				m_components.erase(itr);
				return;
			}
		}
	}

	void Actor::detachComponentAll()
	{
		for (auto& ref :m_components)
		{
			Component::Destroy(ref.component);
		}
		m_components.clear();
	}

	void Actor::Destroy(const WeakObj<Actor>& _actorRef) noexcept
	{
		if (auto actor = _actorRef.lock(); actor)
		{
			actor->_internalNotifyDestroy();
		}
	}

	void Actor::Destroy(ObjHandle<Actor>&& _actor) noexcept
	{
		if (_actor)
		{
			_actor->_internalNotifyDestroy();
		}
	}

	void Actor::applySpaceRendering()
	{
		// まだ3Dレンダリング可能ではない
		if (stateNone(7))
		{
			auto rs = getScene()->_getRenderingSystem().lock();
			rs->add3D(reference.castTo<Object>());
			// Object側で3D描画可能のフラグを立ててもらう
			Object::applySpaceRendering();
		}
	}

	void Actor::applyPlaneRendering()
	{
		// まだ2Dレンダリング可能ではない
		if (stateNone(6))
		{
			auto rs = getScene()->_getRenderingSystem().lock();
			rs->add2D(reference.castTo<Object>());
			// Object側で2D描画可能のフラグを立ててもらう
			Object::applyPlaneRendering();
		}
	}

	void Actor::applyFixedUpdate()
	{
		// まだfixedUpdateが使えない
		if (stateNone(8))
		{
			auto ps = getScene()->_getPhysicsSystem().lock();
			ps->add2D(reference.castTo<Object>());
			// Object側でfixedUpdateが使えるようになったフラグを立ててもらう
			Object::applyFixedUpdate();
		}
	}

	Array<Actor::RefComponent>::const_iterator Actor::findComponent(const TypeID& _type) const
	{
		return std::ranges::find_if(m_components.getContainer(), [&_type](const RefComponent& ref) {return ref.tyep == _type; });
	}

	void Actor::addActorToScene(const TypeID& _type, SharedObj<Actor>&& _actor)
	{
		auto as = getScene()->_getActorSystem().lock();
		as->add(_type, std::forward<SharedObj<Actor>>(_actor));
	}
}
