#pragma once
#include <SFML/Graphics.hpp>
#include "State.hpp"
#include "Game/Game.hpp"

namespace ChessGUI
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
		sf::Texture _mainTexture;
		int _gameState;
	};
}