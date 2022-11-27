#include "../../include/InputAxis.hpp"

namespace Libra
{
	bool InputAxis_impl::State::operator==(const State& _other)const noexcept
	{
		return (input == _other.input) and (scale == _other.scale);
	}

	InputAxis_impl& InputAxis_impl::add(const Input& _input, double _scale)
	{
		State state{ _input,_scale };

		if (auto found = std::ranges::find(m_states, state); found == m_states.end())
		{
			m_states << state;
		}

		return *this;
	}

	void InputAxis_impl::remove(const Input& _input, double _scale)
	{
		m_states.remove(State{ _input,_scale });
	}

	void InputAxis_impl::clear()
	{
		m_states.clear();
	}

	Array<InputAxis_impl::State> InputAxis_impl::getStates()const noexcept
	{
		return m_states;
	}

	double InputAxis_impl::value()const
	{
		double value{ 0 };
		for (auto& state : m_states)
		{
			if (state.input.pressed())
			{
				value += state.scale;
			}
		}
		return value;
	}
}
