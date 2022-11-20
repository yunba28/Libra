#include "../../include/PolygonFrameMesh.hpp"

namespace Libra
{
	PolygonFrameMesh::PolygonFrameMesh()
		: m_frameColor(Palette::White)
		, m_thickness(2.0)
	{
	}

	void PolygonFrameMesh::draw() const
	{
		Polygon polygon = drawRegion();

		polygon.drawFrame(m_thickness, m_frameColor);
	}

	void PolygonFrameMesh::setFrameColor(const Color& _color)
	{
		m_frameColor = _color;
	}

	const Color& PolygonFrameMesh::getFrameColor() const noexcept
	{
		return m_frameColor;
	}

	void PolygonFrameMesh::setThickness(double _thickness)
	{
		m_thickness = _thickness;
	}

	double PolygonFrameMesh::getThickness() const noexcept
	{
		return m_thickness;
	}
}
