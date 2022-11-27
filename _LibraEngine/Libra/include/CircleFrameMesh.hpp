#pragma once

#include "CircleMesh.hpp"

namespace Libra
{
	class CircleFrameMesh final : public CircleMesh
	{
	public:

		CircleFrameMesh();

		void draw()const override final;

		void setFrameColor(const Color& _color);

		void setFrameColor(const Color& _innerColor, const Color& _outerColor);

		const Pair<Color, Color>& getFrameColor()const noexcept;

		void setThickness(double _thickness);

		void setThickness(double _innerThickeness, double _outerThickeness);

		const Pair<double, double>& getThickness()const noexcept;

	private:

		Pair<Color, Color> m_frameColors;

		Pair<double,double> m_thickness;

	};
}
