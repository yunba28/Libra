#pragma once

#include "DrawableComponent.hpp"

namespace Libra
{
	class RectMesh : public DrawableComponent2D
	{
	public:

		RectMesh();

		virtual void draw()const override;

		/// @brief Circleに描画するテクスチャを設定する
		void setTexture(const TextureRegion& _textureRegion);

		/// @brief 設定されたTextureRegionのTextureを取得する
		const Texture& getTexture()const;

		/// @brief 設定されたTextureRegionを取得する
		const TextureRegion& getTextureRegion()const;

		/// @brief Rectのカラーを設定する
		void setColor(const Color& _color);

		void setColor(const ColorF(&_colors)[4]);

		void setColor(Arg::left_<Color> _leftColor, Arg::right_<Color> _rightColor);

		void setColor(Arg::top_<Color> _topColor, Arg::bottom_<Color> _bottomColor);

		const ColorF& getColor(size_t _index)const;

		void setDiffuse(const Color& _diffuse);

		const Color& getDiffuse()const noexcept;

		void setRadius(double _radius);

		double getRadius()const noexcept;

		void setLocalRect(RectF _rect);

		RectF getLocalRect()const noexcept;

		RectF getWorldRect()const;

	private:

		/// @brief Rectに貼るテクスチャ
		TextureRegion m_mesh;

		/// @brief 各頂点のカラー
		ColorF m_colors[4];

		/// @brief テクスチャに適応されるディフーズカラー
		Color m_diffuse;

		RectF m_local;

		double m_radius;

		void drawRect(const RectF& _rect)const;

		void drawRect(const RectF& _rect, double _angle)const;

		void drawRoundRect(const RectF& _rect)const;

		void drawRoundRect(const RectF& _rect, double _angle)const;

	protected:

		Pair<RectF, double> drawRegion()const;

	};
}
