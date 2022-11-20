#include "../../include/ArrowComponent2D.hpp"

namespace Libra
{
	ArrowComponent2D::ArrowComponent2D()
		: m_color(Palette::White)
		, m_headSize(25, 25)
		, m_length(100)
		, m_thickness(5)
		, m_style(LineStyle::Default)
		, m_doubleHead(false)
	{
	}

	void ArrowComponent2D::draw() const
	{
		auto tf = getTransform();
		Vec2 pos = tf->getWorldPos().xy();
		Vec2 fwd = -tf->up().xy().normalize(); //2D描画のためVec2{0,-1}を前方ベクトルの基準値にする
		double scale = tf->getWorldScale().maxComponent();

		Line arrow{ pos,pos + (fwd * getLength() * scale) };

		if (m_doubleHead)
		{
			arrow.drawDoubleHeadedArrow(m_thickness, m_headSize, m_color);
		}
		else
		{
			arrow.drawArrow(m_thickness, m_headSize, m_color);
		}
	}

	void ArrowComponent2D::setColor(const Color& _color)
	{
		m_color = _color;
	}

	const Color& ArrowComponent2D::getColor() const noexcept
	{
		return m_color;
	}

	void ArrowComponent2D::setHeadSize(SizeF _size) noexcept
	{
		m_headSize = _size;
	}

	SizeF ArrowComponent2D::getHeadSize() const noexcept
	{
		return m_headSize;
	}

	void ArrowComponent2D::setLength(double _length) noexcept
	{
		m_length = _length;
	}

	double ArrowComponent2D::getLength() const noexcept
	{
		return m_length;
	}

	void ArrowComponent2D::setThickness(double _thickness) noexcept
	{
		m_thickness = _thickness;
	}

	double ArrowComponent2D::getThickness() const noexcept
	{
		return m_thickness;
	}

	void ArrowComponent2D::setLineStyle(LineStyle _style) noexcept
	{
		m_style = _style;
	}

	LineStyle ArrowComponent2D::getLineStyle() const noexcept
	{
		return m_style;
	}

	void ArrowComponent2D::setDoubleHead(bool _flag) noexcept
	{
		m_doubleHead = _flag;
	}

	bool ArrowComponent2D::isDoubleHead() const noexcept
	{
		return m_doubleHead;
	}
}
