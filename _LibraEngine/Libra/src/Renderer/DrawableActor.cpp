#include "../../include/DrawableActor.hpp"
#include "../../include/SceneBase.hpp"
#include "../../include/RenderingSystem.hpp"

namespace Libra
{
	DrawableActor2D::DrawableActor2D()
	{
		setEnableDraw(true);
	}

	DrawableActor2D::~DrawableActor2D()
	{
	}

	void DrawableActor2D::setRenderPriority(const uint64 _priority) noexcept
	{
		Object::setRenderPriority(Min(_priority, 99999ui64));
		auto rs = getScene()->_getRenderingSystem().lock();
		rs->notifySort2D();
	}

	void DrawableActor2D::_internalConstruct()
	{
		Object::_internalConstruct();

		applyPlaneRendering();
	}

	DrawableActor3D::DrawableActor3D()
	{
		setEnableDraw(true);
	}

	DrawableActor3D::~DrawableActor3D()
	{
	}

	void DrawableActor3D::draw() const
	{
	}

	void DrawableActor3D::setRenderPriority(const uint64 _priority) noexcept
	{
		Object::setRenderPriority(_priority);
		auto rs = getScene()->_getRenderingSystem().lock();
		rs->notifySort3D();
	}

	void DrawableActor3D::_internalConstruct()
	{
		Object::_internalConstruct();

		applySpaceRendering();
	}
}
