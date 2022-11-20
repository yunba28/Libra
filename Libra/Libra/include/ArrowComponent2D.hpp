#pragma once

#include "DrawableComponent.hpp"

namespace Libra
{
	class ArrowComponent2D final : public DrawableComponent2D
	{
	public:

		ArrowComponent2D();

		void draw()const override final;

		void setColor(const Color& _color);

		const Color& getColor()const noexcept;

		void setHeadSize(SizeF _size)noexcept;

		SizeF getHeadSize()const noexcept;

		void setLength(double _length)noexcept;

		double getLength()const noexcept;

		void setThickness(double _thickness)noexcept;

		double getThickness()const noexcept;

		void setLineStyle(LineStyle _style)noexcept;

		LineStyle getLineStyle()const noexcept;

		void setDoubleHead(bool _flag)noexcept;

		bool isDoubleHead()const noexcept;

	private:

		Color m_color;

		SizeF m_headSize;

		double m_length;

		double m_thickness;

		LineStyle m_style;

		bool m_doubleHead;

	};
}
