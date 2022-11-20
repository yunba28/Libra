#pragma once

#include "Object.hpp"
#include "SharedObj.hpp"

namespace Libra::Internal
{
	class RenderingSystem final
	{
	public:

		RenderingSystem();

		~RenderingSystem();

		void add2D(const WeakObj<Object>& _renderWeakObj);

		void add2D(WeakObj<Object>&& _renderWeakObj);

		void add3D(const WeakObj<Object>& _renderWeakObj);

		void add3D(WeakObj<Object>&& _renderWeakObj);

		void update();

		void draw()const;

		WeakObj<BasicCamera3D> getCamera3D()const noexcept;

		WeakObj<BasicCamera2D> getCamera2D()const noexcept;

		void setBackgroundColor(const Color& _color);

		const Color& getBackgroundColor()const noexcept;

		void notifySort3D()noexcept;

		void notifySort2D()noexcept;

	private:

		Array<WeakObj<Object>> m_3dRenderer;
		Array<WeakObj<Object>> m_pending3dRenderer;

		Array<WeakObj<Object>> m_2dRenderer;
		Array<WeakObj<Object>> m_pending2dRenderer;

		SharedObj<BasicCamera3D> m_camera3d;
		SharedObj<BasicCamera2D> m_camera2d;

		MSRenderTexture m_renderTexture;

		Color m_backgroundColor;

		bool m_needSortBy3dRender;
		bool m_needSortBy2dRender;
		
		mutable bool m_needRemoveBy3dRender;
		mutable bool m_needRemoveBy2dRender;

		void _draw3D()const;

		void _draw2D()const;

		static bool _internalRemoveConditions(const WeakObj<Object>& ref);
	};
}
