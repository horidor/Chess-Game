#pragma once

#include <SFML/Graphics.hpp>
#include "State.hpp"
#include "Game.hpp"
#include "AI.hpp"
#include "ChessLogic.hpp"
#include <array>

namespace ChessGUI
{
	class GameState : public State
	{
	public:
		GameState(GameDataRef data);
		void Init();
		void HandleInput();
		void Update(float dt);
		void Draw(float dt);
	private:
		void InitGridPiece();
		void CheckAndPlacePieces(sf::Vector2i touchPoint);

		void ChoosingPhase(int choosingColor);
		void MovingPhase(int movingColor);

		GameDataRef _data;
		sf::Sprite _background;
		sf::Sprite _pauseButton;
		sf::Sprite _gridSprite;
		sf::Sprite _gridPieces[8][8];
		sf::Sprite _gridSelection[8][8];
		sf::Texture _ChessPieces;
		sf::Clock _clock;
		cppTable _gridArray;

		int _gameState;
		bool _inputState;
		int _playerColor, _anotherColor;
		sf::IntRect _gridPos;
		
		std::unique_ptr<AI> ai;
		std::unique_ptr<ChessEngine::ChessLogic> chessLogic;
	};
}
