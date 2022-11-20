#pragma once

#include "DrawableComponent.hpp"

namespace Libra
{
	class CircleMesh : public DrawableComponent2D
	{
	public:

		CircleMesh();

		virtual void draw()const override;

		/// @brief Circleに描画するテクスチャを設定する
		void setTexture(const TextureRegion& _textureRegion);

		/// @brief 設定されたTextureRegionのTextureを取得する
		const Texture& getTexture()const;

		/// @brief 設定されたTextureRegionを取得する
		const TextureRegion& getTextureRegion()const;

		/// @brief Circleのカラーを設定する
		void setColor(const Color& _color);

		/// @brief Circleのカラーを設定する
		void setColor(const Color& _innerColor, const Color& _outerColor);

		/// @brief Circleのインナーカラーとアウターカラーを取得する
		/// @return [inner, outer]
		const Pair<Color,Color>& getColor()const noexcept;

		void setDiffuse(const Color& _color);

		const Color& getDiffuse()const noexcept;

		void setLocalCircle(Circle _local)noexcept;

		Circle getLocalCircle()const noexcept;

		Circle getWorldCircle()const;

	private:

		/// @brief Circleに貼るテクスチャ
		TextureRegion m_mesh;

		/// @brief Circleのカラー
		/// @brief [inner, outer]
		Pair<Color, Color> m_colors;

		/// @brief テクスチャに適応されるディフーズカラー
		Color m_diffuse;

		/// @brief Circleを描画するローカル座標
		Circle m_local;
	};
}
