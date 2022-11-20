#include "../../include/RectFrameMesh.hpp"

namespace Libra
{
	RectFrameMesh::RectFrameMesh()
		: m_frameColors{ Palette::White }
		, m_thickness(1, 1)
	{
	}

	void RectFrameMesh::draw() const
	{
		auto [rect, angle] = drawRegion();

		if (getRadius() == 0.0)
		{
			if (angle == 0)
			{
				drawFrameRect(rect);
			}
			else
			{
				drawFrameRect(rect, angle);
			}
		}
		else
		{
			if (angle == 0)
			{
				drawFrameRoundRect(rect);
			}
			else
			{
				drawFrameRoundRect(rect, angle);
			}
		}
	}

	void RectFrameMesh::setFrameColor(const Color& _color)
	{
		m_frameColors[0] = _color;
		m_frameColors[1] = _color;
		m_frameColors[2] = _color;
		m_frameColors[3] = _color;
	}

	void RectFrameMesh::setFrameColor(const Color& _innerColor, const Color& _outerColor)
	{
		m_frameColors[0] = _innerColor;
		m_frameColors[1] = _outerColor;
	}

	const ColorF& RectFrameMesh::getFrameColor(size_t _index) const noexcept
	{
		if (_index < 4)
		{
			return m_frameColors[_index];
		}

		Throwing<Exceptions::OutOfRange>("out of range");
	}

	void RectFrameMesh::setThickness(double _thickness)
	{
		const double half = _thickness * 0.5;
		m_thickness = { half,half };
	}

	void RectFrameMesh::setThickness(double _innerThickeness, double _outerThickeness)
	{
		m_thickness = { _innerThickeness,_outerThickeness };
	}

	const Pair<double, double>& RectFrameMesh::getThickness() const noexcept
	{
		return m_thickness;
	}

	void RectFrameMesh::drawFrameRect(const RectF& _rect) const
	{
		const ColorF& innerColor = m_frameColors[0];
		const ColorF& outerColor = m_frameColors[1];
		const auto& [innerThickness, outerThickness] = m_thickness;

		_rect.drawFrame(
			innerThickness,
			outerThickness,
			innerColor,
			outerColor);
	}

	void RectFrameMesh::drawFrameRect(const RectF& _rect, double _angle) const
	{
		const Transformer2D transformer{ Mat3x2::Rotate(_angle, _rect.center()) };

		const ColorF& innerColor = m_frameColors[0];
		const ColorF& outerColor = m_frameColors[1];
		const auto& [innerThickness, outerThickness] = m_thickness;

		_rect.drawFrame(
			innerThickness,
			outerThickness,
			innerColor,
			outerColor);
	}

	void RectFrameMesh::drawFrameRoundRect(const RectF& _rect) const
	{
		const RoundRect rrect{ _rect,getRadius() };
		const auto& [innerThickness, outerThickness] = m_thickness;

		rrect.drawFrame(
			innerThickness,
			outerThickness,
			m_frameColors[0]);
	}

	void RectFrameMesh::drawFrameRoundRect(const RectF& _rect, double _angle) const
	{
		const Transformer2D transformer{ Mat3x2::Rotate(_angle, _rect.center()) };

		const RoundRect rrect{ _rect,getRadius() };
		const auto& [innerThickness, outerThickness] = m_thickness;

		rrect.drawFrame(
			innerThickness,
			outerThickness,
			m_frameColors[0]);
	}
}
