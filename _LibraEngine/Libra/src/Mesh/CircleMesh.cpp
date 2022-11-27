#include "../../include/CircleMesh.hpp"

namespace Libra
{
	CircleMesh::CircleMesh()
		: m_mesh()
		, m_colors(Palette::White, Palette::White)
		, m_diffuse(Palette::White)
		, m_local(Circle{ 50 })
	{
	}

	void CircleMesh::draw() const
	{
		Circle circle = getWorldCircle();
		const auto& [inner, outer] = getColor();

		circle.draw(inner, outer);

		if (m_mesh.texture)
		{
			circle(m_mesh).draw(getDiffuse());
		}
	}

	void CircleMesh::setTexture(const TextureRegion& _textureRegion)
	{
		m_mesh = _textureRegion;
	}

	const Texture& CircleMesh::getTexture() const
	{
		return m_mesh.texture;
	}

	const TextureRegion& CircleMesh::getTextureRegion() const
	{
		return m_mesh;
	}

	void CircleMesh::setColor(const Color& _color)
	{
		m_colors = {_color,_color};
	}

	void CircleMesh::setColor(const Color& _innerColor, const Color& _outerColor)
	{
		m_colors = { _innerColor,_outerColor };
	}

	const Pair<Color, Color>& CircleMesh::getColor() const noexcept
	{
		return m_colors;
	}

	void CircleMesh::setDiffuse(const Color& _color)
	{
		m_diffuse = _color;
	}

	const Color& CircleMesh::getDiffuse() const noexcept
	{
		return m_diffuse;
	}

	void CircleMesh::setLocalCircle(Circle _local) noexcept
	{
		m_local = _local;
	}

	Circle CircleMesh::getLocalCircle() const noexcept
	{
		return m_local;
	}

	Circle CircleMesh::getWorldCircle() const
	{
		auto tf = getActor()->getTransform();
		Circle circle = m_local;
		circle.moveBy(tf->getWorldPos().xy());
		return circle.scaled(tf->getWorldScale().maxComponent());
	}
}
