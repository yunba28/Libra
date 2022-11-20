#pragma once

#include "Object.hpp"
#include "SharedObj.hpp"

namespace Libra
{
	class Component : public Object
	{
	public:

		Component();

		virtual ~Component() = 0;

		/*-------------------------------- メッセージ関数 --------------------------------*/

		virtual void start()override {}

		virtual void update()override {}

		/*-------------------------------- 汎用関数 --------------------------------*/

		/// @brief Componentが所属するActorを取得する
		ObjHandle<Actor> getActor()const noexcept;

		/// @brief Actorが持つTransformを取得する
		ObjHandle<Transform> getTransform()const noexcept;

		/// @brief Componentが所属するActorの名前を比較する
		/// @param _name 比較する文字列
		/// @return 比較の結果が等しければtrue
		bool compareName(const String& _name)const noexcept;

		/// @brief Componentが所属するActorのタグを比較する
		/// @param _name 比較する文字列
		/// @return 比較の結果が等しければtrue
		bool compareTag(const String& _tag)const noexcept;

		/*-------------------------------- 内部関数 --------------------------------*/

		virtual void _internalUpdate()override;

		/*-------------------------------- static関数 --------------------------------*/

		template<Internal::ModelComponent ComponentT>
		static SharedObj<ComponentT> CreateComponent(const WeakObj<Actor>& actor);

		static void Destroy(const WeakObj<Component>& _componentRef)noexcept;

		static void Destroy(ObjHandle<Component>&& _componentRef)noexcept;

	protected:

		/*-------------------------------- 派生クラスへのユーティリティ --------------------------------*/

		/// @brief Componentが3D描画を行えるようにする
		void applySpaceRendering()override final;

		/// @brief Componentが2D描画を行えるようにする
		void applyPlaneRendering()override final;

		/// @brief ComponentがFixedUpdateを行えるようにする
		void applyFixedUpdate()override final;

	protected:

		/// @brief 自身の弱参照
		WeakObj<Component> reference;

	private:

		WeakObj<Actor> m_actor;

		/*-------------------------------- 固有関数 --------------------------------*/

		void addComponentToScene(const TypeID& _type, SharedObj<Component>&& _component);

	};

	template<Internal::ModelComponent ComponentT>
	SharedObj<ComponentT> Component::CreateComponent(const WeakObj<Actor>& actor)
	{
		SharedObj<ComponentT> component{ MakeShared<ComponentT>() };
		component->m_actor = actor;
		component->addComponentToScene(typeid(ComponentT), component.castTo<Component>());
		component->reference = component.makeRef();
		component->_internalConstruct();
		return component;
	}
}
