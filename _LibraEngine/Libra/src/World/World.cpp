#include "../../include/World.hpp"

namespace Libra
{
	World::World()
		: m_factories()
		, m_current(nullptr)
		, m_next(nullptr)
		, m_currentState(U"")
		, m_nextState(U"")
		, m_first(unspecified)
		, m_error(false)
	{
	}

	World::~World()
	{
	}

	bool World::build(const String& _name)
	{
		if (m_current)
		{
			return false;
		}

		auto it = m_factories.find(_name);

		if (it == m_factories.end())
		{
			return false;
		}

		m_currentState = _name;

		m_current = it->second();
		{
			_setupScene(m_current);
		}

		if (hasError())
		{
			return false;
		}

		m_transitionState = TransitionState::FadeIn;

		m_stopwatch.restart();

		return true;
	}

	bool World::update()
	{
		if (not _updateScene())
		{
			return false;
		}

		_drawScene();

		return true;
	}

	bool World::changeScene(const String& _name, const Duration& transitionTime, CrossFade crossFade)
	{
		return changeScene(_name, static_cast<int32>(transitionTime.count() * 1000), crossFade);
	}

	bool World::changeScene(const String& _name, const int32 transitionTimeMillisec, CrossFade crossFade)
	{
		if (_name == m_currentState)
		{
			crossFade = CrossFade::No;
		}

		if (not m_factories.contains(_name))
		{
			return false;
		}

		m_nextState = _name;

		m_crossFade = crossFade;

		if (crossFade)
		{
			m_transitionTimeMillisec = transitionTimeMillisec;

			m_transitionState = TransitionState::FadeInOut;

			m_next = m_factories[m_nextState]();
			{
				_setupScene(m_next);
			}

			if (hasError())
			{
				return false;
			}

			m_currentState = m_nextState;

			m_stopwatch.restart();
		}
		else
		{
			m_transitionTimeMillisec = (transitionTimeMillisec / 2);

			m_transitionState = TransitionState::FadeOut;

			m_stopwatch.restart();
		}

		return true;
	}

	void World::notifyError() noexcept
	{
		m_error = true;
	}

	bool World::hasError() const noexcept
	{
		return m_error;
	}

	void World::setFadeColor(const ColorF& _color)
	{
		m_fadeColor = _color;
	}

	const ColorF& World::getFadeColor() const
	{
		return m_fadeColor;
	}

	SharedObj<World> World::CreateWorld()
	{
		SharedObj<World> world{ MakeShared<World>() };
		return world;
	}

	void World::_setupScene(SharedObj<SceneBase>& _ptr)
	{
		_ptr->m_world = reference;
		_ptr->reference = _ptr.makeRef();
		_ptr->construct();
	}

	bool World::_updateScene()
	{
		if (hasError())
		{
			return false;
		}

		if (not m_current)
		{
			if (not m_first)
			{
				return true;
			}
			else if (not build(*m_first))
			{
				return false;
			}
		}

		if (m_crossFade)
		{
			return _updateCross();
		}
		else
		{
			return _updateSingle();
		}
	}

	bool World::_updateSingle()
	{
		double elapsed = m_stopwatch.msF();

		if ((m_transitionState == TransitionState::FadeOut)
			&& (m_transitionTimeMillisec <= elapsed))
		{
			m_current->destruct();
			m_current = nullptr;

			m_current = m_factories[m_nextState]();
			{
				_setupScene(m_current);
			}

			if (hasError())
			{
				return false;
			}

			m_currentState = m_nextState;

			m_transitionState = TransitionState::FadeIn;

			m_stopwatch.restart();

			elapsed = 0.0;
		}

		if ((m_transitionState == TransitionState::FadeIn)
			&& (m_transitionTimeMillisec <= elapsed))
		{
			m_stopwatch.reset();

			m_transitionState = TransitionState::Active;
		}

		const double t = (m_transitionTimeMillisec ? (elapsed / m_transitionTimeMillisec) : 1.0);

		switch (m_transitionState)
		{
		case TransitionState::FadeIn:
			m_current->updateFadeIn(t);
			break;
		case TransitionState::Active:
			m_current->_internalUpdate();
			break;
		case TransitionState::FadeOut:
			m_current->updateFadeOut(t);
			break;
		default:
			return false;
		}

		return (not hasError());
	}

	bool World::_updateCross()
	{
		const double elapsed = m_stopwatch.msF();

		if ((m_transitionState == TransitionState::FadeInOut)
			&& (m_transitionTimeMillisec <= elapsed))
		{
			m_current = m_next;

			m_next = nullptr;

			m_stopwatch.reset();

			m_transitionState = TransitionState::Active;
		}

		if (m_transitionState == TransitionState::Active)
		{
			m_current->_internalUpdate();
		}
		else
		{
			assert(m_transitionTimeMillisec);

			const double t = (m_transitionTimeMillisec ? (elapsed / m_transitionTimeMillisec) : 1.0);

			m_current->updateFadeOut(t);

			if (hasError())
			{
				return false;
			}

			m_next->updateFadeIn(t);
		}

		return (not hasError());
	}

	void World::_drawScene() const
	{
		if (not m_current)
		{
			return;
		}

		if ((m_transitionState == TransitionState::Active)
			|| (m_transitionTimeMillisec <= 0))
		{
			m_current->_internalDraw();
		}

		const double elapsed = m_stopwatch.msF();
		const double t = (m_transitionTimeMillisec ? (elapsed / m_transitionTimeMillisec) : 1.0);

		if (m_transitionState == TransitionState::FadeIn)
		{
			m_current->drawFadeIn(t);
		}
		else if (m_transitionState == TransitionState::FadeOut)
		{
			m_current->drawFadeOut(t);
		}
		else if (m_transitionState == TransitionState::FadeInOut)
		{
			m_current->drawFadeOut(t);

			if (m_next)
			{
				m_next->drawFadeIn(t);
			}
		}
	}
}
