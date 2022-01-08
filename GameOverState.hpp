#pragma once

#include <SFML/Graphics.hpp>
#include "State.hpp"
#include "Game.hpp"

namespace Sonar
{
	class GameOverState : public State
	{
	public:
		GameOverState(GameDataRef data, int gameState);
		void Init();
		void HandleInput();
		void Update(float dt);
		void Draw(float dt);
	private:
		GameDataRef _data;
		sf::Sprite _retryButton;
		sf::Sprite _homeButton;
		sf::Sprite _result;
		int _gameState;
	};
}