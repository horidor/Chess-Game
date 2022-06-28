#pragma once

#include <SFML/Graphics.hpp>
#include "State.hpp"
#include "Game/Game.hpp"
#include "ChessEngine/AI.hpp"
#include "ChessEngine/ChessLogic.hpp"
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
		enum PieceColour {
			BLACK = -1,
			NONE = 0,
			WHITE = 1
		};

		void InitGridPiece();
		void CheckAndPlacePieces(sf::Vector2i touchPoint);

		bool givenPlayersPiece(int piece, int pColor);

		std::vector<std::pair<int, int>>& ChoosingPhase(int row, int column, int choosingColor);
		void MovingPhase(int row, int column, int movingColor, std::vector<std::pair<int, int>>& legalMoves);

		void endCheck(int movingColour);

		void GUIPromotion(sf::Vector2i _touch);

		GameDataRef _data;
		sf::Sprite _background;
		sf::Sprite _pauseButton;
		sf::Sprite _gridSprite;
		sf::Sprite _gridPieces[8][8];
		sf::Sprite _gridSelection[8][8];
		sf::Sprite _promotionSelection[8];
		 
		sf::Texture _ChessPieces;
		sf::Clock _clock;
		cppTable _gridArray;

		int _gameState;
		int _playerColor, _anotherColor;
		int _whoseTurn = WHITE;
		int _movingColour;
		bool blockBoardInput = false;
		std::pair<int, int> promotionPair;

		sf::IntRect _gridPos;
		sf::IntRect _promotePos;
		sf::Vector2i _chosenPiece;
		
		std::unique_ptr<AI> ai;
		std::unique_ptr<ChessEngine::ChessLogic> chessLogic;
	};
}
