#pragma once

#include "Transform.hpp"
#include "UniqueTag.hpp"
#include "CommonTag.hpp"

namespace Libra
{
	class Actor : public Object
	{
	public:

		Actor();

		virtual ~Actor();

		/*-------------------------------- メッセージ関数 --------------------------------*/

		virtual void destruct()override;

		virtual void start()override {}

		virtual void update()override {}

		/*-------------------------------- 汎用関数 --------------------------------*/

		/// @brief Actorが所属するシーンを取得する
		ObjHandle<SceneBase> getScene()const noexcept;

		/// @brief Actorが参照するTransformを設定する
		void setupTransform(const WeakObj<Transform>& _transform);

		/// @brief Actorが参照するTransformを取得する
		/// @brief Transformの参照がない場合は空のObjHandleが返る
		ObjHandle<Transform> getTransform()const noexcept;

		/// @brief Actorの名前を設定する
		void setName(const String& _name);

		/// @brief Actorの名前を取得する
		const String& getName()const;

		/// @brief 指定した文字列がActorの名前と等しいか
		bool compareName(const String& _name)const noexcept;

		/// @brief Actorにタグを設定する 
		void addTag(const String& _tag);

		/// @brief Actorの持つタグを外す 
		void removeTag(const String& _tag);

		/// @brief Actorが持つすべてのタグを配列にして取得
		Array<String> getTagList()const;

		/// @brief 指定した文字列がActorの持つタグと等しいか
		bool compareTag(const String& _tag)const noexcept;

		/*-------------------------------- Componentの操作関数 --------------------------------*/

		/// @brief ComponentT型のComponentをActorにアタッチする
		template<class ComponentT>
		WeakObj<ComponentT> attachComponent();

		/// @brief Actorが所有するCmponentをTypeIDから取得
		WeakObj<Component> getComponent(const TypeID& _type)const;

		/// @brief Actorが所有するComponentを取得
		template<class ComponentT>
		WeakObj<ComponentT> getComponent()const;

		/// @brief Actorが所有するComponentTをすべて取得
		Array<WeakObj<Component>> getComponents(const TypeID& _type)const;

		/// @brief Actorが所有するComponentTをすべて取得
		template<class ComponentT>
		Array<WeakObj<ComponentT>> getComponents()const;

		/// @brief Actorが持つComponentをすべて取得
		Array<WeakObj<Component>> getComponentAll()const;

		/// @brief Actorが所有するComponentをTypeIDでディタッチ
		void detachComponent(const TypeID& _type);

		void detachComponent(const WeakObj<Component>& _componentRef);

		/// @brief Actorが所有するComponentをディタッチする
		template<class ComponentT>
		void detachComponent();

		/// @brief Actorが余裕するComponentを全てディタッチする
		void detachComponentAll();

		/*-------------------------------- static関数 --------------------------------*/

		/// @brief Actorの生成とSceneへのセットアップを行う
		/// @tparam ActorT 生成するActorを継承したクラス
		/// @param scene Actorが所属するシーン
		/// @param name 生成するActorの名前
		template<Internal::ModelActor ActorT>
		static SharedObj<ActorT> CreateActor(const WeakObj<SceneBase>& scene, const String& name);

		/// @brief Actorを弱参照から破棄
		static void Destroy(const WeakObj<Actor>& _actorRef)noexcept;

		/// @brief Actorをハンドルから削除
		static void Destroy(ObjHandle<Actor>&& _actor)noexcept;

	protected:

		/*-------------------------------- 派生クラスへのユーティリティ --------------------------------*/

		/// @brief Actorが3D描画を行えるようにする
		void applySpaceRendering()override final;

		/// @brief Actorが2D描画を行えるようにする
		void applyPlaneRendering()override final;

		/// @brief ActorがFixedUpdateを行えるようにする
		void applyFixedUpdate()override final;

	protected:

		// 自身の弱参照
		WeakObj<Actor> reference;

	private:

		// 所属するシーンの弱参照
		WeakObj<SceneBase> m_scene;

		// Actorが所持するTrasformの弱参照
		WeakObj<Transform> m_transform;

		// Actor固有の名前
		UniqueTag m_name;

		// Actorが持つタグの集合
		HashSet<CommonTag> m_tagSet;

		/// @brief ActorがアタッチしたComponentを管理する
		struct RefComponent
		{
			WeakObj<Component> component;
			TypeID tyep;
		};

		// Actorが参照するComponentのリスト
		Array<RefComponent> m_components;

	private:

		/*-------------------------------- 固有関数 --------------------------------*/

		Array<RefComponent>::const_iterator findComponent(const TypeID& _type)const;

		void addActorToScene(const TypeID& _type, SharedObj<Actor>&& _actor);

	};

	template<Internal::ModelActor ActorT>
	SharedObj<ActorT> Actor::CreateActor(const WeakObj<SceneBase>& scene, const String& name)
	{
		SharedObj<ActorT> actor{ MakeShared<ActorT>() };
		{
			actor->m_scene = scene;
			actor->addActorToScene(typeid(ActorT), actor.castTo<Actor>());
			actor->setName(name);
			actor->reference = actor.makeRef();
			actor->_internalConstruct();
		}
		return actor;
	}

	template<class ComponentT>
	WeakObj<ComponentT> Actor::attachComponent()
	{
		SharedObj<ComponentT> component{ Component::CreateComponent<ComponentT>(reference) };
		m_components << RefComponent{ .component = component.castTo<Component>().makeRef(), .tyep = typeid(ComponentT) };
		return component.makeRef();
	}

	template<class ComponentT>
	WeakObj<ComponentT> Actor::getComponent() const
	{
		if (auto found = getComponent(typeid(ComponentT)); found)
		{
			return found.castTo<ComponentT>();
		}
		return WeakObj<ComponentT>{};
	}

	template<class ComponentT>
	Array<WeakObj<ComponentT>> Actor::getComponents() const
	{
		Array<WeakObj<ComponentT>> result{};
		if (auto res = getComponents(typeid(ComponentT)); res)
		{
			result.reserve(res.size());
			for (auto&& ref : res)
			{
				result << ref.castTo<ComponentT>();
			}
		}
		return result;
	}

	template<class ComponentT>
	void Actor::detachComponent()
	{
		detachComponent(typeid(ComponentT));
	}
}
