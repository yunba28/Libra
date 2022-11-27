#pragma once

#include "Actor.hpp"

namespace Libra
{
	class DrawableComponent2D : public Component
	{
	public:

		DrawableComponent2D();

		~DrawableComponent2D();

		virtual void start()override {}

		virtual void update()override {}

		virtual void draw()const = 0;

		/// @brief [0,99999]の範囲で描画優先度を設定可能
		/// @param _priority 大きいほど後に描画される
		virtual void setRenderPriority(const uint64 _priority)noexcept override;

		void _internalConstruct()override final;

	};

	class DrawableComponent3D : public Component
	{
	public:

		DrawableComponent3D();

		~DrawableComponent3D();

		virtual void start()override {}

		virtual void update()override {}

		virtual void draw()const = 0;

		/// @brief 描画優先度を設定可能
		/// @param _priority 大きいほど後に描画される
		virtual void setRenderPriority(const uint64 _priority)noexcept override;

		void _internalConstruct()override final;

	};
}
