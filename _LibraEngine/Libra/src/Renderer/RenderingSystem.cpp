#include "../../include/RenderingSystem.hpp"

namespace Libra::Internal
{
	RenderingSystem::RenderingSystem()
		: m_3dRenderer()
		, m_pending3dRenderer()
		, m_2dRenderer()
		, m_pending2dRenderer()
		, m_camera3d(MakeShared<BasicCamera3D>(Scene::Size(), BasicCamera3D::DefaultVerticalFOV, Vec3{ 0,0,-10 }))
		, m_camera2d(MakeShared<BasicCamera2D>(Scene::CenterF()))
		, m_renderTexture(Scene::Size(), TextureFormat::R8G8B8A8_Unorm_SRGB, HasDepth::Yes)
		, m_backgroundColor()
		, m_needSortBy3dRender(false)
		, m_needSortBy2dRender(false)
		, m_needRemoveBy3dRender(false)
		, m_needRemoveBy2dRender(false)
	{
		setBackgroundColor(Palette::DefaultBackground);
	}

	RenderingSystem::~RenderingSystem()
	{
	}

	void RenderingSystem::add2D(const WeakObj<Object>& _renderWeakObj)
	{
		m_pending2dRenderer << _renderWeakObj;
	}

	void RenderingSystem::add2D(WeakObj<Object>&& _renderWeakObj)
	{
		m_pending2dRenderer << std::forward<WeakObj<Object>>(_renderWeakObj);
	}

	void RenderingSystem::add3D(const WeakObj<Object>& _renderWeakObj)
	{
		m_pending3dRenderer << _renderWeakObj;
	}

	void RenderingSystem::add3D(WeakObj<Object>&& _renderWeakObj)
	{
		m_pending3dRenderer << std::forward<WeakObj<Object>>(_renderWeakObj);
	}

	bool _CompareByRenderPriority(const WeakObj<Object>& a, const WeakObj<Object>& b)
	{
		return a.lock()->getRenderPriority() < b.lock()->getRenderPriority();
	}

	void RenderingSystem::update()
	{
		//3D描画オブジェクトの削除要請
		if (m_needRemoveBy3dRender)
		{
			m_3dRenderer.remove_if(_internalRemoveConditions);
			m_needRemoveBy2dRender = false;
		}

		//2D描画オブジェクトの削除要請
		if (m_needRemoveBy2dRender)
		{
			m_2dRenderer.remove_if(_internalRemoveConditions);
			m_needRemoveBy2dRender = false;
		}

		//3D描画オブジェクトの追加要請
		if (m_pending3dRenderer)
		{
			m_3dRenderer.append(m_pending3dRenderer);
			m_pending3dRenderer.clear();
			m_needSortBy3dRender = true;
		}

		//2D描画オブジェクトの追加要請
		if (m_pending2dRenderer)
		{
			m_2dRenderer.append(m_pending2dRenderer);
			m_pending2dRenderer.clear();
			m_needSortBy2dRender = true;
		}

		//3D描画オブジェクトの並び替え要請
		if (m_needSortBy3dRender)
		{
			m_3dRenderer.stable_sort_by(_CompareByRenderPriority);
			m_needSortBy3dRender = false;
		}

		//2D描画オブジェクトの並び替え要請
		if (m_needSortBy2dRender)
		{
			m_2dRenderer.stable_sort_by(_CompareByRenderPriority);
			m_needSortBy2dRender = false;
		}
	}

	void RenderingSystem::draw() const
	{
		//3D描画
		if (m_3dRenderer)
			_draw3D();

		//2D描画
		if (m_2dRenderer)
			_draw2D();
	}

	WeakObj<BasicCamera3D> RenderingSystem::getCamera3D() const noexcept
	{
		return m_camera3d.makeRef();
	}

	WeakObj<BasicCamera2D> RenderingSystem::getCamera2D() const noexcept
	{
		return m_camera2d.makeRef();
	}

	void RenderingSystem::setBackgroundColor(const Color& _color)
	{
		m_backgroundColor = _color;
		Scene::SetBackground(_color);
	}

	const Color& RenderingSystem::getBackgroundColor() const noexcept
	{
		return m_backgroundColor;
	}

	void RenderingSystem::notifySort3D() noexcept
	{
		m_needSortBy3dRender = true;
	}

	void RenderingSystem::notifySort2D() noexcept
	{
		m_needSortBy2dRender = true;
	}

	void RenderingSystem::_draw3D() const
	{
		Graphics3D::SetCameraTransform(*m_camera3d);

		//描画処理
		{
			// renderTexture を背景色で塗りつぶし、
			// renderTexture を 3D 描画のレンダーターゲットに
			const ScopedRenderTarget3D target{ m_renderTexture.clear(m_backgroundColor.removeSRGBCurve()) };

			for (auto& ref : m_3dRenderer)
			{
				auto obj = ref.lock();
				if ((not obj) or obj->isPendingDestroy())
				{
					m_needRemoveBy3dRender = true;
					continue;
				}
				obj->_internalDraw();
			}
		}

		//3D描画を2D描画に転写
		{
			// renderTexture を resolve する前に 3D 描画を実行する
			Graphics3D::Flush();

			// マルチサンプル・テクスチャのリゾルブ
			m_renderTexture.resolve();

			// リニアレンダリングされた renderTexture をシーンに転送
			Shader::LinearToScreen(m_renderTexture);
		}
	}

	void RenderingSystem::_draw2D() const
	{
		const auto transformer = m_camera2d->createTransformer();

		for (auto& ref : m_2dRenderer)
		{
			auto obj = ref.lock();
			if ((not obj) or obj->isPendingDestroy())
			{
				m_needRemoveBy2dRender = true;
				continue;
			}
			obj->_internalDraw();
		}
	}

	bool RenderingSystem::_internalRemoveConditions(const WeakObj<Object>& ref)
	{
		auto obj = ref.lock();
		return (not obj) or obj->isPendingDestroy();
	}
}
