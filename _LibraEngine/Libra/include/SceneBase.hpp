#pragma once

#include "Actor.hpp"

namespace Libra
{
	class SceneBase : public Object
	{
	public:

		SceneBase();

		virtual ~SceneBase() = default;

		virtual void start()override {}

		virtual void update()override {}

		virtual void updateFadeIn(double);

		virtual void updateFadeOut(double);

		virtual void draw()const override {}

		virtual void drawFadeIn(double t)const;

		virtual void drawFadeOut(double t)const;

		ObjHandle<World> getWorld()const noexcept;

		void notifyError();

		bool changeScene(const String& _name, const Duration& _toransitionTime = 2.0s, CrossFade _crossFade = CrossFade::No);

		bool changeScene(const String& _name, const int32 _toransitionTimeMilliSec, CrossFade _crossFade);

		WeakObj<BasicCamera3D> getCamera3D()const noexcept;

		WeakObj<BasicCamera2D> getCamera2D()const noexcept;

		/// @brief sceneの背景色を設定
		void setBackgroundColor(const Color& _color);

		/// @brief Sceneの背景色を取得
		const Color& getBackgroundColor()const noexcept;

		/// @brief 物理演算とfixedUpdateを行う間隔を設定
		void setPhysicsTimestep(double _timestep)noexcept;

		/// @brief 物理演算とfixedUpdateを行う間隔を取得
		double getPhysicsTimestep()const noexcept;

		/// @brief ActorをTransformをアタッチした状態で作成する
		/// @tparam ActorT 作成するActorの型
		/// @param _name 作成するActorの名前
		/// @return Actorの弱参照
		template<class ActorT = Actor>
		WeakObj<ActorT> createActor(const String& _name);

		/// @brief ActorをTransformをアタッチした状態で作成する
		/// @tparam ActorT 作成するActorの型
		/// @param _name 作成するActorの名前
		/// @param _pos 作成するActorの初期座標
		/// @param _rot 作成するActorの初期回転
		/// @return Actorの弱参照
		template<class ActorT = Actor>
		WeakObj<ActorT> createActor(const String& _name, Vec3 _pos, Quaternion _rot = Quaternion::Identity());

		/// @brief なにもアタッチしていない状態のActorを作成する
		/// @brief このActorは初期状態では更新も描画もすべて無効
		/// @tparam ActorT 作成するActorの型
		/// @param _name 作成するActorの名前
		/// @return Actorの弱参照
		template<class ActorT = Actor>
		WeakObj<ActorT> createEmptyActor(const String& _name);

		WeakObj<Actor> getActor(const TypeID& _type, const String& _name)const;

		template<class ActorT = Actor>
		WeakObj<ActorT> getActor(const String& _name)const;

		WeakObj<Actor> getActorByTag(const TypeID& _type, const String& _tag)const;

		template<class ActorT = Actor>
		WeakObj<ActorT> getActorByTag(const String& _tag)const;

		Array<WeakObj<Actor>> getActorAll()const;

		void destroyActor(const WeakObj<Actor>& _actor);

		void destroyActor(const String& _name);

		void destroyActorsByTag(const String& _tag);

		void _internalUpdate()override final;

		void _internalDraw()const override final;

		WeakObj<Internal::ActorSystem> _getActorSystem()const noexcept;

		WeakObj<Internal::ComponentSystem> _getComponentSystem()const noexcept;

		WeakObj<Internal::RenderingSystem> _getRenderingSystem()const noexcept;

		WeakObj<Internal::PhysicsSystem> _getPhysicsSystem()const noexcept;

	protected:

		WeakObj<SceneBase> reference;

	private:

		WeakObj<World> m_world;

		SharedObj<Internal::ActorSystem> m_actorSystem;

		SharedObj<Internal::ComponentSystem> m_componentSystem;

		SharedObj<Internal::RenderingSystem> m_renderingSystem;

		SharedObj<Internal::PhysicsSystem> m_physicsSystem;

		friend class World;

	private:

		void fixedUpdate()override final {}

		void onActive()override final {}

		void onInactive()override final {}

	};

	template<class ActorT>
	WeakObj<ActorT> SceneBase::createActor(const String& _name)
	{
		SharedObj<ActorT> actor{ Actor::CreateActor<ActorT>(reference,_name) };
		{
			auto tf = actor->attachComponent<Transform>();
			actor->setupTransform(tf);
		}
		return actor.makeRef();
	}

	template<class ActorT>
	WeakObj<ActorT> SceneBase::createActor(const String& _name, Vec3 _pos, Quaternion _rot)
	{
		SharedObj<ActorT> actor{ Actor::CreateActor<ActorT>(reference,_name) };
		{
			auto tf = actor->attachComponent<Transform>();
			tf.lock()->set(_pos, _rot, Vec3{ 1,1,1 });
			actor->setupTransform(tf);
		}
		return actor.makeRef();
	}

	template<class ActorT>
	WeakObj<ActorT> SceneBase::createEmptyActor(const String& _name)
	{
		SharedObj<ActorT> actor{ Actor::CreateActor<ActorT>(reference,_name) };
		actor->setEnableUpdate(false);
		actor->setEnableFixedUpdate(false);
		actor->setEnableDraw(false);
		return actor.makeRef();
	}

	template<class ActorT>
	WeakObj<ActorT> SceneBase::getActor(const String& _name) const
	{
		if (auto actor = getActor(typeid(ActorT), _name))
		{
			return actor.castTo<ActorT>();
		}
		return WeakObj<ActorT>{};
	}

	template<class ActorT>
	WeakObj<ActorT> SceneBase::getActorByTag(const String& _tag) const
	{
		return getActorByTag(typeid(ActorT),_tag);
	}
}
