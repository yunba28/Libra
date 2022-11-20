#include "../../include/CircleFrameMesh.hpp"

namespace Libra
{
	CircleFrameMesh::CircleFrameMesh()
		: m_frameColors()
		, m_thickness(0.5, 0.5)
	{
	}

	void CircleFrameMesh::draw() const
	{
		CircleMesh::draw();

		Circle circle = getWorldCircle();
		const auto& [innerColor, outerColor] = getFrameColor();
		const auto& [innerThickness, outerThickness] = getThickness();

		circle.drawFrame(
			innerThickness,
			outerThickness,
			innerColor,
			outerColor);
	}

	void CircleFrameMesh::setFrameColor(const Color& _color)
	{
		m_frameColors = { _color,_color };
	}

	void CircleFrameMesh::setFrameColor(const Color& _innerColor, const Color& _outerColor)
	{
		m_frameColors = { _innerColor,_outerColor };
	}

	const Pair<Color, Color>& CircleFrameMesh::getFrameColor() const noexcept
	{
		return m_frameColors;
	}

	void CircleFrameMesh::setThickness(double _thickness)
	{
		double half = _thickness * 0.5;
		m_thickness = { half,half };
	}

	void CircleFrameMesh::setThickness(double _innerThickeness, double _outerThickeness)
	{
		m_thickness = { _innerThickeness,_outerThickeness };
	}

	const Pair<double, double>& CircleFrameMesh::getThickness() const noexcept
	{
		return m_thickness;
	}
}
