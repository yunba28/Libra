#pragma once

#include <Siv3D.hpp>

namespace Libra
{
	class InputAction_impl final
	{
	public:

		using InputEvent = bool(InputGroup::*)()const;

		InputAction_impl& add(const InputGroup& _input);

		void remove(size_t _index);

		void clear();

		void setEvent(InputEvent _event);

		Array<InputGroup> getActions()const noexcept;

		bool value()const;

		double pressedDuration()const;

	private:

		Array<InputGroup> m_actions{};

		InputEvent m_event{ &InputGroup::down };

	};

	inline HashTable<String, InputAction_impl> InputAction{};
}
