#pragma once

#include "Actor.hpp"

namespace Libra
{
	class DrawableActor2D : public Actor
	{
	public:

		DrawableActor2D();

		~DrawableActor2D();

		virtual void draw()const = 0;

		/// @brief [0,99999]の範囲で描画優先度を設定可能
		/// @param _priority 大きいほど後に描画される
		virtual void setRenderPriority(const uint64 _priority)noexcept override;

		void _internalConstruct()override final;

	};

	class DrawableActor3D : public Actor
	{
	public:

		DrawableActor3D();

		~DrawableActor3D();

		virtual void draw()const = 0;

		/// @brief 描画優先度を設定可能
		/// @param _priority 大きいほど後に描画される
		virtual void setRenderPriority(const uint64 _priority)noexcept override;

		void _internalConstruct()override final;

	};
}
