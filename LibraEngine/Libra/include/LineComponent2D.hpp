#pragma once

#include "DrawableComponent.hpp"

namespace Libra
{
	class LineComponent2D : public DrawableComponent2D
	{
	public:

		LineComponent2D();

		virtual void draw()const override;

		void setColor(const Color& _color);

		void setColor(const Color& _beginColor, const Color& _endColor);

		const Pair<Color, Color>& getColors()const noexcept;

		const Color& getBeginColor()const noexcept;

		const Color& getEndColor()const noexcept;

		void setThickness(double _thickness)noexcept;

		double getThickness()const noexcept;

		void setLineStyle(LineStyle _style)noexcept;

		LineStyle getLineStyle()const noexcept;

		void setLocalLine(const Line& _localLine)noexcept;

		const Line& getLocalLine()const noexcept;

		Line getWorldLine()const;

	private:

		Pair<Color, Color> m_colors;

		double m_thickness;

		LineStyle m_style;

		Line m_local;

		Line drawRegion()const;

	};
}
