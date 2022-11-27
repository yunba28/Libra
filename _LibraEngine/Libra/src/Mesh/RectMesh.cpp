#include "../../include/RectMesh.hpp"

namespace Libra
{
	RectMesh::RectMesh()
		: m_mesh()
		, m_colors{ Palette::White }
		, m_diffuse(Palette::White)
		, m_local(-50, -50, 100, 100)
		, m_radius(0.0)
	{
	}

	void RectMesh::draw() const
	{
		drawRegion();
	}

	void RectMesh::setTexture(const TextureRegion& _textureRegion)
	{
		m_mesh = _textureRegion;
	}

	const Texture& RectMesh::getTexture() const
	{
		return m_mesh.texture;
	}

	const TextureRegion& RectMesh::getTextureRegion() const
	{
		return m_mesh;
	}

	void RectMesh::setColor(const Color& _color)
	{
		m_colors[0] = _color;
		m_colors[1] = _color;
		m_colors[2] = _color;
		m_colors[3] = _color;
	}

	void RectMesh::setColor(const ColorF(&_colors)[4])
	{
		m_colors[0] = _colors[0];
		m_colors[1] = _colors[1];
		m_colors[2] = _colors[2];
		m_colors[3] = _colors[3];
	}

	void RectMesh::setColor(Arg::left_<Color> _leftColor, Arg::right_<Color> _rightColor)
	{
		m_colors[0] = *_leftColor;
		m_colors[1] = *_rightColor;
		m_colors[2] = *_rightColor;
		m_colors[3] = *_leftColor;
	}

	void RectMesh::setColor(Arg::top_<Color> _topColor, Arg::bottom_<Color> _bottomColor)
	{
		m_colors[0] = *_topColor;
		m_colors[1] = *_topColor;
		m_colors[2] = *_bottomColor;
		m_colors[3] = *_bottomColor;
	}

	const ColorF& RectMesh::getColor(size_t _index) const
	{
		if (_index < 4)
		{
			return m_colors[_index];
		}

		Throwing<Exceptions::OutOfRange>("out of range");
	}

	void RectMesh::setDiffuse(const Color& _diffuse)
	{
		m_diffuse = _diffuse;
	}

	const Color& RectMesh::getDiffuse() const noexcept
	{
		return m_diffuse;
	}

	void RectMesh::setRadius(double _radius)
	{
		m_radius = _radius;
	}

	double RectMesh::getRadius() const noexcept
	{
		return m_radius;
	}

	void RectMesh::setLocalRect(RectF _rect)
	{
		m_local = _rect;
	}

	RectF RectMesh::getLocalRect() const noexcept
	{
		return m_local;
	}

	RectF RectMesh::getWorldRect() const
	{
		auto tf = getActor()->getTransform();
		RectF rect = m_local;
		rect.moveBy(tf->getWorldPos().xy());
		return rect.scaled(tf->getWorldScale().xy());
	}

	void RectMesh::drawRect(const RectF& _rect) const
	{
		_rect.draw(m_colors);

		if (m_mesh.texture)
		{
			_rect(m_mesh).draw(m_diffuse);
		}
	}

	void RectMesh::drawRect(const RectF& _rect, double _angle) const
	{
		const Transformer2D transformer{ Mat3x2::Rotate(_angle, _rect.center()) };

		_rect.draw(m_colors);

		if (m_mesh.texture)
		{
			_rect(m_mesh).draw(m_diffuse);
		}
	}

	void RectMesh::drawRoundRect(const RectF& _rect) const
	{
		RoundRect rrect{ _rect,getRadius() };

		rrect.draw(m_colors[0]);

		if (m_mesh.texture)
		{
			rrect(m_mesh).draw(getDiffuse());
		}
	}

	void RectMesh::drawRoundRect(const RectF& _rect, double _angle) const
	{
		const Transformer2D transformer{ Mat3x2::Rotate(_angle, _rect.center()) };

		RoundRect rrect{ _rect,getRadius() };

		rrect.draw(m_colors[0]);

		if (m_mesh.texture)
		{
			rrect(m_mesh).draw(getDiffuse());
		}
	}

	Pair<RectF, double> RectMesh::drawRegion() const
	{
		RectF rect = getWorldRect();

		auto tf = getActor()->getTransform();
		double angle = tf->eulerAngles().z;

		// 通常のRectを描画
		if (getRadius() == 0.0)
		{
			//回転させるか
			if (angle == 0)
			{
				drawRect(rect);
			}
			else
			{
				drawRect(rect, angle);
			}
		}
		// 角の丸いRectを描画
		else
		{
			if (angle == 0)
			{
				drawRoundRect(rect);
			}
			else
			{
				drawRoundRect(rect, angle);
			}
		}

		return { rect,angle };
	}
}
