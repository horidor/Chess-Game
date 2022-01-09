#pragma once

#include <memory>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include "StateMachine.hpp"
#include "AssetManager.hpp"
#include "InputManager.hpp"

namespace ChessGUI
{
	struct GameData
	{
		StateMachine machine;
		sf::RenderWindow window;
		AssetManager assets;
		InputManager input;
		std::string firsttoPlay;
	};

	typedef std::shared_ptr<GameData> GameDataRef;

	typedef std::array<std::array<int8_t, 8>, 8> cppTable;

	class Game
	{
	public:
		Game(int width, int height, std::string title);
	private:
		const float dt = 1.0f / 60.0f;
		sf::Clock _clock;
		GameDataRef _data = std::make_shared<GameData>();
		void Run();
	};
}
