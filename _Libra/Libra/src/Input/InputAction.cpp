#include "../../include//InputAction.hpp"

namespace Libra
{
	InputAction_impl& InputAction_impl::add(const InputGroup& _input)
	{
		m_actions << _input;
		return *this;
	}

	void InputAction_impl::remove(size_t _index)
	{
		if (_index < m_actions.size())
		{
			m_actions.remove_at(_index);
		}
	}

	void InputAction_impl::clear()
	{
		m_actions.clear();
	}

	void InputAction_impl::setEvent(InputEvent _event)
	{
		m_event = _event;
	}

	Array<InputGroup> InputAction_impl::getActions() const noexcept
	{
		return m_actions;
	}

	bool InputAction_impl::value() const
	{
		for (auto& action : m_actions)
		{
			if ((action.*m_event)())
			{
				return true;
			}
		}
		return false;
	}

	double InputAction_impl::pressedDuration() const
	{
		double elapsed{ 0 };
		for (auto& action : m_actions)
		{
			if ((action.*m_event)())
			{
				elapsed = Max(elapsed, action.pressedDuration().count());
			}
		}
		return elapsed;
	}
}
