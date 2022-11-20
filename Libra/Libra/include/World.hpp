#pragma once

#include "SceneBase.hpp"

namespace Libra
{
	class World
	{
	public:

		World();

		~World();

		/// @brief Worldにシーンを追加する
		/// @tparam SceneT 追加するシーンの型
		/// @param _name 追加するシーンの名前
		template<Internal::ModelSceneBase SceneT>
		World& add(const String& _name);
 
		bool build(const String& _name);

		bool update();

		bool changeScene(const String& _name, const Duration& transitionTime = 2.0s, CrossFade crossFade = CrossFade::No);

		bool changeScene(const String& _name, const int32 transitionTimeMillisec = 2000, CrossFade crossFade = CrossFade::No);

		void notifyError()noexcept;

		bool hasError()const noexcept;

		void setFadeColor(const ColorF& _color);

		const ColorF& getFadeColor()const;

		static SharedObj<World> CreateWorld();

	public:

		WeakObj<World> reference;

	private:

		using Scene_t = SharedObj<SceneBase>;

		using Factory_t = Function<Scene_t>;

		HashTable<String, Factory_t> m_factories;

		Scene_t m_current;

		Scene_t m_next;

		String m_currentState;

		String m_nextState;

		Optional<String> m_first;

		bool m_error;

		enum class TransitionState
		{
			None,

			FadeIn,

			Active,

			FadeOut,

			FadeInOut,

		} m_transitionState = TransitionState::None;

		Stopwatch m_stopwatch;

		int32 m_transitionTimeMillisec = 1000;

		ColorF m_fadeColor = Palette::Black;

		CrossFade m_crossFade = CrossFade::No;

		void _setupScene(SharedObj<SceneBase>& _ptr);

		bool _updateScene();

		bool _updateSingle();

		bool _updateCross();

		void _drawScene()const;
	};

	template<Internal::ModelSceneBase SceneT>
	World& World::add(const String& _name)
	{
		auto factory = [this]()
		{
			auto scene = SharedObj<SceneBase>{ MakeShared<SceneT>() };
			return scene;
		};

		auto it = m_factories.find(_name);

		if (it != m_factories.end())
		{
			it->second = factory;
		}
		else
		{
			m_factories.emplace(_name, factory);

			if (not m_first)
			{
				m_first = _name;
			}
		}

		return *this;
	}
}
