#include <sstream>
#include <iostream>
#include "GameState.hpp"
#include "PauseState.hpp"
#include "GameOverState.hpp"
#include "DEFINITIONS.hpp"


namespace ChessGUI
{
	GameState::GameState(GameDataRef data)
	{
		this->_data = data;
	}

	void GameState::Init()
	{

		if (this->_data->firsttoPlay == "another")
		{
			this->_gameState = STATE_ANOTHER_CHOOSING;
			this->_playerColor = BLACK_QUEEN;
			this->_anotherColor = WHITE_QUEEN;
		}
		else if (this->_data->firsttoPlay == "player")
		{
			this->_gameState = STATE_PLAYER_CHOOSING;
			this->_playerColor = WHITE_QUEEN;
			this->_anotherColor = BLACK_QUEEN;
		}


		this->ai = std::make_unique<AI>(_data);
		
		this->_data->assets.LoadTexture("Pause Button", PAUSE_BUTTON);
		this->_data->assets.LoadTexture("Grid Sprite", GRID_SPRITE_FILEPATH);
		this->_data->assets.LoadTexture("Black Tile", BLACK_TILE);
		this->_data->assets.LoadTexture("White Tile", WHITE_TILE);
		this->_data->assets.LoadTexture("Block Piece", BLOCK_PIECE);
		this->_data->assets.LoadTexture("Chess Pieces", CHESS_PIECE_ARRAY);

		_ChessPieces = this->_data->assets.GetTexture("Chess Pieces");
		_ChessPieces.setSmooth(true);

		_background.setTexture(this->_data->assets.GetTexture("Background"));
		_pauseButton.setTexture(this->_data->assets.GetTexture("Pause Button"));
		_gridSprite.setTexture(this->_data->assets.GetTexture("Grid Sprite"));
		_pauseButton.setPosition(SCREEN_WIDTH - _pauseButton.getGlobalBounds().width + 5, 13);
		
		sf::Vector2i tileMeas(this->_data->assets.GetTexture("Black Tile").getSize().x,
			this->_data->assets.GetTexture("Black Tile").getSize().y);

		_gridPos.left = SCREEN_WIDTH / 2 - (tileMeas.x * GRID_SIZE) / 2 - 50; _gridPos.top = SCREEN_HEIGHT / 2 - (tileMeas.y * GRID_SIZE) / 2;
		_gridPos.width = tileMeas.x * GRID_SIZE;
		_gridPos.height = tileMeas.y * GRID_SIZE;
		 
		
		InitGridPiece();

		this->chessLogic = std::make_unique<ChessEngine::ChessLogic>(_gridArray);
	}

	void GameState::HandleInput()
	{
		sf::Event event;
		sf::Vector2i touchPoint;
		while (this->_data->window.pollEvent(event))
		{
			switch (event.type) {
				case (sf::Event::Closed):
					this->_data->window.close();
					break;
				case (sf::Event::MouseButtonPressed):
					touchPoint = this->_data->input.GetMousePosition(this->_data->window);

					if (this->_data->input.isAreaClicked(this->_pauseButton, sf::Mouse::Left, this->_data->window))
						this->_data->machine.AddState(StateRef(new PauseState(_data)), false);

					else if (this->_data->input.isAreaClicked(this->_gridPos, sf::Mouse::Left, this->_data->window))
						this->CheckAndPlacePieces(touchPoint);

					break;
				default:
					break;
			}

		}
	}

	void GameState::Update(float dt)
	{
		if (_gameState == STATE_LOSE || _gameState == STATE_WON)
		{
			if (this->_clock.getElapsedTime().asSeconds() > TIME_BEFORE_SHOWING_GAME_OVER)
			{
				this->_data->machine.AddState(StateRef(new GameOverState(_data, _gameState)), true);
			}
		}
	}

	void GameState::Draw(float dt)
	{
		this->_data->window.clear();
		this->_data->window.draw(this->_background);
		this->_data->window.draw(this->_pauseButton);

		for (int x = 0; x < GRID_SIZE; x++)
		{
			for (int y = 0; y < GRID_SIZE; y++)
			{
				this->_data->window.draw(this->_gridSelection[x][y]);
				this->_data->window.draw(this->_gridPieces[x][y]);
			}
		}

		this->_data->window.display();
	}

	void GameState::InitGridPiece()
	{
		sf::Vector2u tempChessPieceSize(60, 60);
		char piecesArrangment[8]{ 'R','k','B','Q','K','B','k','R' };
		bool WhiteOrBlackFlag = true;
		for (int x = 0; x < GRID_SIZE; x++)
		{
			WhiteOrBlackFlag = !WhiteOrBlackFlag;
			for (int y = 0; y < GRID_SIZE; y++)
			{
				if (!WhiteOrBlackFlag) _gridSelection[x][y].setTexture(this->_data->assets.GetTexture("White Tile"));
				else  _gridSelection[x][y].setTexture(this->_data->assets.GetTexture("Black Tile"));

				_gridSelection[x][y].setPosition(_gridPos.left + _gridSelection[x][y].getGlobalBounds().width * y, _gridPos.top + _gridSelection[x][y].getGlobalBounds().height * x);
				WhiteOrBlackFlag = !WhiteOrBlackFlag;

				if (x < 6) _gridArray[x][y] = EMPTY_PIECE;

				if (x < 2) {
					int anotherX = 7 - x;
					int anotherY = 7 - y;

					_gridPieces[x][y].setTexture(_ChessPieces);

					_gridPieces[anotherX][y].setTexture(_ChessPieces);

					sf::IntRect piecePosition;
					piecePosition.height = piecePosition.width = tempChessPieceSize.x;
					piecePosition.top = 0;

					if (x == 1) {
						piecePosition.left = 300;
						_gridArray[x][y] = BLACK_PAWN;
						_gridArray[anotherX][y] = WHITE_PAWN;
					}
					else if (x == 0)
					{
						switch (piecesArrangment[y])
						{
						case 'Q':
							piecePosition.left = 0;
							_gridArray[x][y] = BLACK_QUEEN;
							_gridArray[anotherX][y] = WHITE_QUEEN;
							break;
						case 'K':
							piecePosition.left = 60;
							_gridArray[x][y] = BLACK_KING;
							_gridArray[anotherX][y] = WHITE_KING;
							break;
						case 'R':
							piecePosition.left = 120;
							_gridArray[x][y] = BLACK_ROOK;
							_gridArray[anotherX][y] = WHITE_ROOK;
							break;
						case 'k':
							piecePosition.left = 180;
							_gridArray[x][y] = BLACK_KNIGHT;
							_gridArray[anotherX][y] = WHITE_KNIGHT;
							break;
						case 'B':
							piecePosition.left = 240;
							_gridArray[x][y] = BLACK_BISHOP;
							_gridArray[anotherX][y] = WHITE_BISHOP;
							break;
						}
					}

					_gridPieces[x][y].setTextureRect(piecePosition);
					piecePosition.top = 60;
					_gridPieces[anotherX][y].setTextureRect(piecePosition);
				}

				_gridPieces[x][y].setPosition(_gridPos.left + 10 + tempChessPieceSize.y * y + 20 * y,
					_gridPos.top + 10 + tempChessPieceSize.x * x + 20 * x);
				
				
			}
		}
	}

	void GameState::CheckAndPlacePieces(sf::Vector2i touchPoint)
	{
		sf::FloatRect gridSize(_gridPos);
		sf::Vector2f gapOutsideOfGrid = sf::Vector2f((SCREEN_WIDTH - gridSize.width) / 2 - 50, (SCREEN_HEIGHT - gridSize.height) / 2);
		sf::Vector2f gridLocalTouchPos = sf::Vector2f(touchPoint.x - gapOutsideOfGrid.x, touchPoint.y - gapOutsideOfGrid.y);
		sf::Vector2f gridSectionSize = sf::Vector2f(gridSize.width / 8, gridSize.height / 8);

		int column = 0, row = 0;

		column = gridLocalTouchPos.x / gridSectionSize.x ;

		row = gridLocalTouchPos.y / gridSectionSize.y;

		static std::vector<std::pair<int, int>> legalMoves;
		
		switch (_gameState) {

		case STATE_PLAYER_CHOOSING:
			legalMoves = ChoosingPhase(row, column, _playerColor);
			break;
		
		case STATE_PLAYER_MOVING:
			if (_chosenPiece.x == row && _chosenPiece.y == column) {
				for (int i = 0; i < 8; i++)
					for (int j = 0; j < 8; j++)
						_gridSelection[i][j].setColor(sf::Color(255, 255, 255, 255));
				_chosenPiece.x = -1; _chosenPiece.y = -1;
				_gameState = STATE_PLAYER_CHOOSING;
			}
			MovingPhase(row, column, _playerColor, legalMoves);
			endCheck(_anotherColor);
			break;
		
		case STATE_ANOTHER_CHOOSING:
			legalMoves = ChoosingPhase(row, column, _anotherColor);
			break;

		case STATE_ANOTHER_MOVING:
			if (_chosenPiece.x == row && _chosenPiece.y == column) {
				for (int i = 0; i < 8; i++)
					for (int j = 0; j < 8; j++)
						_gridSelection[i][j].setColor(sf::Color(255, 255, 255, 255));
				_chosenPiece.x = -1; _chosenPiece.y = -1;
				_gameState = STATE_ANOTHER_CHOOSING;
			}
			MovingPhase(row, column, _anotherColor, legalMoves);
			endCheck(_playerColor);
			break;
		}
	}

	std::vector<std::pair<int, int>>& GameState::ChoosingPhase(int row, int column, int choosingColor) {
		static std::vector<std::pair <int, int>> legalMoves;
		if (givenPlayersPiece(_gridArray[row][column], choosingColor)) {
			_gridSelection[row][column].setColor(sf::Color(235, 235, 50, 255));
			legalMoves = chessLogic->GUILegalMoves(row, column);
			for (auto& it : legalMoves)
			{
				_gridSelection[it.first][it.second].setColor(sf::Color(50, 255, 50, 255));
			}

			_chosenPiece.x = row;
			_chosenPiece.y = column;
			
			if (choosingColor == _playerColor) {
				_gameState = STATE_PLAYER_MOVING;
			}
			else {
				_gameState = STATE_ANOTHER_MOVING;
			}

		}

		return legalMoves;
	}

	void GameState::MovingPhase(int row, int column, int movingColor, std::vector<std::pair<int, int>>& legalMoves) {
		/// checks if selected row and column is indeed in legal moves 
		if ([](auto& legalMoves, auto row, auto column) {
			for (auto& it : legalMoves) {
				if (std::make_pair(row, column) == it)
					return true;
			}
			return false; }(legalMoves, row, column)) 
		{
			////////////////////

			chessLogic->makeMove(_chosenPiece.x, _chosenPiece.y, row, column);
			std::pair<int, int> enPassant = chessLogic->enPassantOuter();
			if (enPassant.first != -1) _gridPieces[enPassant.first][enPassant.second].setColor(sf::Color(255, 255, 255, 0));

			sf::IntRect pieceRect = _gridPieces[_chosenPiece.x][_chosenPiece.y].getTextureRect();
			_gridPieces[row][column].setTexture(_ChessPieces);
			_gridPieces[row][column].setTextureRect(pieceRect);

			for (int i = 0; i < 8; i++)
				for (int j = 0; j < 8; j++)
					_gridSelection[i][j].setColor(sf::Color(255, 255, 255, 255));

			_gridPieces[_chosenPiece.x][_chosenPiece.y].setColor(sf::Color(255, 255, 255, 0));
			_gridPieces[row][column].setColor(sf::Color(255, 255, 255, 255));

			_gridArray[row][column] = _gridArray[_chosenPiece.x][_chosenPiece.y];
			_gridArray[_chosenPiece.x][_chosenPiece.y] = EMPTY_PIECE;

			if (movingColor == _playerColor) {
				_gameState = STATE_ANOTHER_CHOOSING;
			}
			else {
				_gameState = STATE_PLAYER_CHOOSING;
			}

			legalMoves.clear();
			_chosenPiece.x = -1; _chosenPiece.y = -1;

		}
	}

	bool GameState::givenPlayersPiece(int piece, int pColor) {
		if ((piece > 0) == (pColor > 0) and (piece != EMPTY_PIECE))
			return true;

		return false;
	}

	void GameState::endCheck(int movingColour) {
		if (chessLogic->getEnd()) {
			if (movingColour == _playerColor) _gameState = STATE_LOSE;
			else if (movingColour == _anotherColor) _gameState = STATE_WON;
		}
	}

}

