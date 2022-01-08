#pragma once
#include <SFML/Graphics.hpp>
#include "State.hpp"
#include "Game.hpp"

namespace Sonar
{
	class GameChooseState : public State
	{
	public:
		GameChooseState(GameDataRef data);
		void Init();
		void HandleInput();
		void Update(float dt);
		void Draw(float dt);
	private:
		GameDataRef _data;
		sf::Sprite _XButton;
		sf::Sprite _YButton;
		int _gameState;
	};
}