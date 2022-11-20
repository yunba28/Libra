#pragma once

#include "DrawableComponent.hpp"

namespace Libra
{
	class PolygonMesh : public DrawableComponent2D
	{
	public:

		PolygonMesh();

		virtual void draw()const override;

		/// @brief Polygonに描画するテクスチャを設定する
		void setTexture(const Texture& _texture);

		/// @brief 設定されたTextureRegionのTextureを取得する
		const Texture& getTexture()const;

		/// @brief Polygonのカラーを設定する
		void setColor(const Color& _color);

		const Color& getColor()const noexcept;

		void setDiffuse(const Color& _color);

		const Color& getDiffuse()const noexcept;

		void setLocalPolygon(const Polygon& _localPolygon);

		const Polygon& getLocalPolygon()const noexcept;

		Polygon getWorldPolygon()const;

	private:

		Texture m_mesh;

		Color m_color;

		Color m_diffuse;

		Polygon m_local;

	protected:

		Polygon drawRegion()const;

	};
}
