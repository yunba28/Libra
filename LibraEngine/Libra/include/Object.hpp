#pragma once

#include <Siv3D.hpp>
#include "Common.hpp"

namespace Libra
{
	/// @brief Libraで管理されるActorやComponentの基礎クラス
	class Object
	{
	public:

		Object() = default;

		virtual ~Object() = 0;

		/*-------------------------------- メッセージ関数 --------------------------------*/

		/// @brief Objectの構築時に呼ばれる
		virtual void construct() {}

		/// @brief Objectに破棄の通知が来た際に呼ばれる
		virtual void destruct() {}

		/// @brief Objectの最初の更新で呼ばれる
		virtual void start() {}

		/// @brief 毎フレーム呼ばれる
		virtual void update() {}

		/// @brief １秒間に決まった回数呼ばれる（Default：1/60s）
		virtual void fixedUpdate() {}

		/// @brief 毎フレーム呼ばれる描画関数
		virtual void draw()const {}

		/// @brief Objectが有効になった際に呼ばれる
		virtual void onActive() {}

		/// @brief Objectが無効になった際に呼ばれる
		virtual void onInactive() {}

		/// @brief Objectが衝突した瞬間に呼ばれる
		virtual void onCollisionEnter(const Collision&) {};

		/// @brief Objectが衝突し続けている間呼ばれる 
		virtual void onCollisionStay(const Collision&) {};

		/// @brief Objectの衝突が終わった瞬間に呼ばれる
		virtual void onCollisionExit(const Collision&) {};

		/*-------------------------------- 汎用関数 --------------------------------*/

		/// @brief Objectの描画優先度を設定する
		/// @brief この関数はかならずnoexcept（例外を起こさない）でなければならない
		/// @param _priority 値が大きいほど後に描画される
		virtual void setRenderPriority(const uint64 _priority)noexcept;

		/// @brief Objectの描画優先度を取得
		/// @brief この関数はかならずnoexcept（例外を起こさない）でなければならない
		virtual uint64 getRenderPriority()const noexcept;

		/// @brief Objectの有効・無効を設定
		void setActive(bool _active)noexcept;

		/// @brief Objectが有効かどうか
		bool isActive()const noexcept;

		/// @brief Objectが最初の更新を行ったかどうか
		bool isStarted()const noexcept;

		/// @brief Objectが破棄待ちかどうか
		bool isPendingDestroy()const noexcept;

		/// @brief update関数の有効・無効を設定する
		void setEnableUpdate(bool _flag)noexcept;

		/// @brief update関数が有効かどうか
		bool isEnabledUpdate()const noexcept;

		/// @brief fixedUpdate関数の有効・無効を設定する
		void setEnableFixedUpdate(bool _flag)noexcept;

		/// @brief fixedUpdate関数が有効かどうか
		bool isEnabledFixedUpdate()const noexcept;

		/// @brief draw関数の有効・無効を設定する
		void setEnableDraw(bool _flag)noexcept;

		/// @brief draw関数が有効かどうか
		bool isEnabledDraw()const noexcept;

		/*-------------------------------- 内部関数 --------------------------------*/

		/// @brief Objectの初期化関数
		/// @brief 注：ユーザーからの使用はしないでください
		virtual void _internalConstruct();

		/// @brief Objectの終了関数
		/// @brief 注：ユーザーからの使用はしないでください
		virtual void _internalDestruct();

		/// @brief Objectの更新関数
		/// @brief 注：ユーザーからの使用はしないでください
		virtual void _internalUpdate();

		/// @brief Objectの固定更新関数
		/// @brief 注：ユーザーからの使用はしないでください
		virtual void _internalFixedUpdate();

		/// @brief Objectの描画関数
		/// @brief 注：ユーザーからの使用はしないでください
		virtual void _internalDraw()const;

		/// @brief Objectの衝突イベント
		/// @brief 注：ユーザーからの使用はしないでください
		virtual void _internalOnCollision(const Collision&) {}

		/// @brief Objectに破棄することを伝える
		/// @brief 注：ユーザーからの使用はしないでください
		void _internalNotifyDestroy()noexcept;

		/*-------------------------------- static関数 --------------------------------*/

		/// @brief Objectを破壊する
		static void Destroy(Object* _object)noexcept;

	protected:

		/*-------------------------------- 派生クラスへのユーティリティ --------------------------------*/

		/// @brief index番目のStateBitを設定する
		/// @param _index [9,63]の範囲のみ設定可能
		/// @param _flag [true]:on [false]:false
		/// @return indexの範囲外を指定した場合はfalse
		bool stateSet(const uint64 _index, const bool _flag)noexcept;

		/// @brief index番目のStateBitを有効にする
		/// @param _index [9,63]の範囲のみ設定可能
		/// @return indexの範囲外を指定した場合はfalse
		bool stateOn(const uint64 _index)noexcept;

		/// @brief index番目のStateBitを無効にする
		/// @param _index [9,63]の範囲のみ設定可能
		/// @return indexの範囲外を指定した場合はfalse
		bool stateOff(const uint64 _index)noexcept;

		/// @brief index番目のStateBitが有効かどうか
		/// @param _index [0,63]の範囲のみ判定可能
		bool stateTest(const uint64 _index)const noexcept;

		/// @brief index番目のStateBitが無効かどうか
		/// @param _index [0,63]の範囲のみ判定可能
		bool stateNone(const uint64 _index)const noexcept;

		/// @brief 指定したマスクビットがすべて有効かどうか
		bool stateAll(const uint64 _mask)const noexcept;

		/// @brief 指定したマスクビットのいずれかが有効かどうか
		bool stateAny(const uint64 _mask)const noexcept;

		/// @brief Objectが3D描画を行えるようにする
		virtual void applySpaceRendering();

		/// @brief Objectが2D描画を行えるようにする
		virtual void applyPlaneRendering();

		/// @brief ObjectがFixedUpdateを行えるようにする
		virtual void applyFixedUpdate();

	private:

		/// @brief 描画を行う際の優先度
		/// @brief 値が大きいほど後に描画が行われる
		uint64 m_renderPriority{ 0 };

		/// @brief Objectの状態を管理するビット列
		/// @brief [0] isActive
		/// @brief [1] isStarted
		/// @brief [2] isPendingDestroy
		/// @brief [3] isEnabledUpdate
		/// @brief [4] isEnabledFixedUpdate
		/// @brief [5] isEnabledDraw
		/// @brief [6] isApplyPlaneRendering
		/// @brief [7] isApplySpaceRendering
		/// @brief [8] isApplyPhysics
		BitSet64 m_state{ 0b00001001 };
	};
}
