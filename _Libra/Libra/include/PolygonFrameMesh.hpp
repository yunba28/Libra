#pragma once

#include "PolygonMesh.hpp"

namespace Libra
{
	class PolygonFrameMesh final : public PolygonMesh
	{
	public:

		PolygonFrameMesh();

		void draw()const override final;

		void setFrameColor(const Color& _color);

		const Color& getFrameColor()const noexcept;

		void setThickness(double _thickness);

		double getThickness()const noexcept;

	private:

		Color m_frameColor;

		double m_thickness;

	};
}
