#pragma once

#include "RectMesh.hpp"

namespace Libra
{
	class RectFrameMesh final : public RectMesh
	{
	public:

		RectFrameMesh();

		void draw()const override final;

		void setFrameColor(const Color& _color);

		void setFrameColor(const Color& _innerColor, const Color& _outerColor);

		const ColorF& getFrameColor(size_t _index)const noexcept;

		void setThickness(double _thickness);

		void setThickness(double _innerThickeness, double _outerThickeness);

		const Pair<double, double>& getThickness()const noexcept;

	private:

		/// @brief 各頂点の色
		/// @brief Siv3D v6.6になるのに合わせて改良予定のため各頂点分
		ColorF m_frameColors[4];

		Pair<double, double> m_thickness;

		void drawFrameRect(const RectF& _rect)const;

		void drawFrameRect(const RectF& _rect, double _angle)const;

		void drawFrameRoundRect(const RectF& _rect)const;

		void drawFrameRoundRect(const RectF& _rect, double _angle)const;
	};
}
