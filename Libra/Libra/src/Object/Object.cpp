#include "../../include/Object.hpp"

namespace Libra
{
	Object::~Object()
	{

	}

	void Object::setRenderPriority(const uint64 _priority) noexcept
	{
		m_renderPriority = _priority;
	}

	uint64 Object::getRenderPriority() const noexcept
	{
		return m_renderPriority;
	}

	void Object::setActive(bool _active) noexcept
	{
		if (isActive() == _active)
			return;

		if (_active)
		{
			onActive();
			m_state.on(0);
		}
		else
		{
			onInactive();
			m_state.off(0);
		}
	}

	bool Object::isActive() const noexcept
	{
		return m_state.test(0);
	}

	bool Object::isStarted() const noexcept
	{
		return m_state.test(1);
	}

	bool Object::isPendingDestroy() const noexcept
	{
		return m_state.test(2);
	}

	void Object::setEnableUpdate(bool _flag) noexcept
	{
		m_state[3] = _flag;
	}

	bool Object::isEnabledUpdate() const noexcept
	{
		return m_state.test(3);
	}

	void Object::setEnableFixedUpdate(bool _flag) noexcept
	{
		m_state[4] = _flag;
	}

	bool Object::isEnabledFixedUpdate() const noexcept
	{
		return m_state.test(4);
	}

	void Object::setEnableDraw(bool _flag) noexcept
	{
		m_state[5] = _flag;
	}

	bool Object::isEnabledDraw() const noexcept
	{
		return m_state.test(5);
	}

	void Object::_internalConstruct()
	{
		construct();
	}

	void Object::_internalDestruct()
	{
		destruct();
	}

	void Object::_internalUpdate()
	{
		// Objectが有効じゃない
		if (not isActive())
			return;

		// update関数が有効じゃない
		if (not isEnabledUpdate())
			return;

		// 最初の更新がまだ行われていない
		if (not isStarted())
		{
			start();
			m_state.on(1);
		}

		update();
	}

	void Object::_internalFixedUpdate()
	{
		// Objectが有効じゃない
		if (not isActive())
			return;

		// update関数が有効じゃない
		if (not isEnabledFixedUpdate())
			return;

		fixedUpdate();
	}

	void Object::_internalDraw()const
	{
		// Objectが有効じゃない
		if (not isActive())
			return;

		// draw関数が有効じゃない
		if (not isEnabledDraw())
			return;

		draw();
	}

	void Object::_internalNotifyDestroy() noexcept
	{
		//既にキルされていなければ処理を行う
		if (not isPendingDestroy())
		{
			m_state.off(0); // Activeを無効にする
			m_state.on(2); // PendingDestroyを有効にする
			_internalDestruct();
		}
	}

	void Object::Destroy(Object* _object) noexcept
	{
		_object->_internalNotifyDestroy();
	}

	bool Object::stateSet(const uint64 _index, const bool _flag) noexcept
	{
		if (InRange(_index, 9ui64, 63ui64))
		{
			m_state.set(_index, _flag);
			return true;
		}

		return false;
	}

	bool Object::stateOn(const uint64 _index) noexcept
	{
		if (InRange(_index, 9ui64, 63ui64))
		{
			m_state.on(_index);
			return true;
		}

		return false;
	}

	bool Object::stateOff(const uint64 _index) noexcept
	{
		if (InRange(_index, 9ui64, 63ui64))
		{
			m_state.off(_index);
			return true;
		}

		return false;
	}

	bool Object::stateTest(const uint64 _index) const noexcept
	{
		return m_state.test(_index);
	}

	bool Object::stateNone(const uint64 _index) const noexcept
	{
		return m_state.none(_index);
	}

	bool Object::stateAll(const uint64 _mask) const noexcept
	{
		return m_state.all(_mask);
	}

	bool Object::stateAny(const uint64 _mask) const noexcept
	{
		return m_state.any(_mask);
	}

	void Object::applySpaceRendering()
	{
		m_state.on(7);
		setEnableDraw(true);
	}

	void Object::applyPlaneRendering()
	{
		m_state.on(6);
		setEnableDraw(true);
	}

	void Object::applyFixedUpdate()
	{
		m_state.on(8);
		setEnableFixedUpdate(true);
	}
}
