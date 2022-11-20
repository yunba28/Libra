#include "../../include/LineComponent2D.hpp"

namespace Libra
{
	LineComponent2D::LineComponent2D()
		: m_colors{ Palette::White,Palette::White }
		, m_thickness(1)
		, m_style(LineStyle::Default)
		, m_local(Vec2{ -50,0 }, Vec2{ 50,0 })
	{
	}

	void LineComponent2D::draw() const
	{
		drawRegion();
	}

	void LineComponent2D::setColor(const Color& _color)
	{
		m_colors = { _color,_color };
	}

	void LineComponent2D::setColor(const Color& _beginColor, const Color& _endColor)
	{
		m_colors = { _beginColor,_endColor };
	}

	const Pair<Color, Color>& LineComponent2D::getColors() const noexcept
	{
		return m_colors;
	}

	const Color& LineComponent2D::getBeginColor() const noexcept
	{
		return m_colors.first;
	}

	const Color& LineComponent2D::getEndColor() const noexcept
	{
		return m_colors.second;
	}

	void LineComponent2D::setThickness(double _thickness) noexcept
	{
		m_thickness = _thickness;
	}

	double LineComponent2D::getThickness() const noexcept
	{
		return m_thickness;
	}

	void LineComponent2D::setLineStyle(LineStyle _style) noexcept
	{
		m_style = _style;
	}

	LineStyle LineComponent2D::getLineStyle() const noexcept
	{
		return m_style;
	}

	void LineComponent2D::setLocalLine(const Line& _localLine) noexcept
	{
		m_local = _localLine;
	}

	const Line& LineComponent2D::getLocalLine() const noexcept
	{
		return m_local;
	}

	Line LineComponent2D::getWorldLine() const
	{
		auto tf = getActor()->getTransform();
		Vec2 pos = tf->getWorldPos().xy();
		double angle = tf->eulerAngles().z;

		Line line = m_local;

		if (angle != 0)
		{
			Vec2 direction = m_local.vector().normalized();
			direction.rotate(angle);
			double half = m_local.length() * 0.5;
			line.begin = pos.movedBy(-direction * half);
			line.end = pos.movedBy(direction * half);
		}
		else
		{
			line.moveBy(pos);
		}
		
		return line;
	}

	Line LineComponent2D::drawRegion() const
	{
		Line line = getWorldLine();
		const auto& [beginColor, endColor] = getColors();

		line.draw(m_style, m_thickness, beginColor, endColor);

		return line;
	}
}
