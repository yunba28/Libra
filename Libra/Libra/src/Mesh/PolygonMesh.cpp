#include "../../include/PolygonMesh.hpp"

namespace Libra
{
	PolygonMesh::PolygonMesh()
		: m_mesh()
		, m_color(Palette::White)
		, m_diffuse(Palette::White)
		, m_local(Shape2D::Hexagon(50))
	{
	}

	void PolygonMesh::draw() const
	{
		drawRegion();
	}

	void PolygonMesh::setTexture(const Texture& _texture)
	{
		m_mesh = _texture;
	}

	const Texture& PolygonMesh::getTexture() const
	{
		return m_mesh;
	}

	void PolygonMesh::setColor(const Color& _color)
	{
		m_color = _color;
	}

	const Color& PolygonMesh::getColor() const noexcept
	{
		return m_color;
	}

	void PolygonMesh::setDiffuse(const Color& _color)
	{
		m_diffuse = _color;
	}

	const Color& PolygonMesh::getDiffuse() const noexcept
	{
		return m_diffuse;
	}

	void PolygonMesh::setLocalPolygon(const Polygon& _localPolygon)
	{
		m_local = _localPolygon;
	}

	const Polygon& PolygonMesh::getLocalPolygon() const noexcept
	{
		return m_local;
	}

	Polygon PolygonMesh::getWorldPolygon() const
	{
		auto tf = getActor()->getTransform();
		const Vec2 pos = tf->getWorldPos().xy();
		const double rot = tf->eulerAngles().z;
		const Vec2 scale = tf->getWorldScale().xy();

		return m_local.movedBy(pos).scale(scale).rotatedAt(pos, rot);
	}

	Polygon PolygonMesh::drawRegion() const
	{
		Polygon polygon = getWorldPolygon();

		polygon.draw(m_color);

		if (m_mesh)
		{
			polygon
				.toBuffer2D(Arg::center = polygon.centroid(), m_mesh.size())
				.draw(m_mesh);
		}

		return polygon;
	}
}
