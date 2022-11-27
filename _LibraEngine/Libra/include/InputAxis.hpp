#pragma once

#include <Siv3D.hpp>

namespace Libra
{
	class InputAxis_impl final
	{
	public:

		struct State
		{
			Input input;
			double scale;

			bool operator==(const State& _other)const noexcept;
		};

		InputAxis_impl& add(const Input& _input, double _scale);

		void remove(const Input& _input, double _scale);

		void clear();

		Array<State> getStates()const noexcept;

		double value()const;

	private:

		Array<State> m_states;

	};

	inline HashTable<String, InputAxis_impl> InputAxis{};
}
