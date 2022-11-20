#include "../../include/DrawableComponent.hpp"
#include "../../include/SceneBase.hpp"
#include "../../include/RenderingSystem.hpp"

namespace Libra
{
	DrawableComponent2D::DrawableComponent2D()
	{
		setEnableDraw(true);
	}

	DrawableComponent2D::~DrawableComponent2D()
	{
	}

	void DrawableComponent2D::setRenderPriority(const uint64 _priority) noexcept
	{
		Object::setRenderPriority(Min(_priority, 99999ui64));
		auto rs = getActor()->getScene()->_getRenderingSystem().lock();
		rs->notifySort2D();
	}

	void DrawableComponent2D::_internalConstruct()
	{
		Object::_internalConstruct();

		applyPlaneRendering();
	}

	DrawableComponent3D::DrawableComponent3D()
	{
		setEnableDraw(true);
	}

	DrawableComponent3D::~DrawableComponent3D()
	{
	}

	void DrawableComponent3D::setRenderPriority(const uint64 _priority) noexcept
	{
		Object::setRenderPriority(_priority);
		auto rs = getActor()->getScene()->_getRenderingSystem().lock();
		rs->notifySort3D();
	}

	void DrawableComponent3D::_internalConstruct()
	{

		Object::_internalConstruct();

		applySpaceRendering();
	}
}
