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
		_inputState = false;

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
		_inputState = true;
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
		this->_inputState = false;
		if (_gameState == STATE_LOSE || _gameState == STATE_WON)
		{
			if (this->_clock.getElapsedTime().asSeconds() > TIME_BEFORE_SHOWING_GAME_OVER)
			{
				this->_data->machine.AddState(StateRef(new GameOverState(_data, _gameState)), true);
			}
		}
		this->_inputState = true;
	}

	void GameState::Draw(float dt)
	{
		this->_inputState = false;
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
		this->_inputState = true;
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
		static sf::Vector2i _chosenPiece;
		static int _oldPiece;

		sf::FloatRect gridSize(_gridPos);
		sf::Vector2f gapOutsideOfGrid = sf::Vector2f((SCREEN_WIDTH - gridSize.width) / 2 - 50, (SCREEN_HEIGHT - gridSize.height) / 2);
		sf::Vector2f gridLocalTouchPos = sf::Vector2f(touchPoint.x - gapOutsideOfGrid.x, touchPoint.y - gapOutsideOfGrid.y);
		sf::Vector2f gridSectionSize = sf::Vector2f(gridSize.width / 8, gridSize.height / 8);
		

		int column = 0, row = 0;

		column = gridLocalTouchPos.x / gridSectionSize.x ;

		row = gridLocalTouchPos.y / gridSectionSize.y;

		std::vector<std::pair<int, int>> legalMoves = chessLogic->GUILegalMoves(row, column);
		
		switch (_gameState) {

		case STATE_PLAYER_CHOOSING:
			if (this->_gridArray[row][column] <= _playerColor && this->_gridArray[row][column] >= _playerColor - 5) {
				this->_gridSelection[row][column].setColor(sf::Color(255, 255, 50, 255));

				for (int x = 0; x < GRID_SIZE; x++) {
					for (int y = 0; y < GRID_SIZE; y++) {
						if ((this->_gridArray[x][y] > _playerColor) || (this->_gridArray[x][y] < _playerColor - 5)) 
							_gridSelection[x][y].setColor(sf::Color(150, 255, 150, 255));
					}
				}
				_gameState = STATE_PLAYER_MOVING;
				_oldPiece = _gridArray[row][column];
				_chosenPiece.x = row;
				_chosenPiece.y = column;
			}
			break;
		
		case STATE_PLAYER_MOVING:
			if (this->_gridArray[row][column] > _playerColor || this->_gridArray[row][column] < _playerColor - 5) {
				_gridPieces[row][column].setTexture(*_gridPieces[_chosenPiece.x][_chosenPiece.y].getTexture());
				_gridPieces[row][column].setTextureRect(_gridPieces[_chosenPiece.x][_chosenPiece.y].getTextureRect());
				_gridPieces[row][column].setColor(sf::Color(255, 255, 255, 255));
				_gridPieces[_chosenPiece.x][_chosenPiece.y].setColor(sf::Color(255, 255, 255, 0));
				for (int x = 0; x < GRID_SIZE; x++) {
					for (int y = 0; y < GRID_SIZE; y++)
						_gridSelection[x][y].setColor(sf::Color(255, 255, 255, 255));
				}

				_gameState = STATE_ANOTHER_CHOOSING;
				_gridArray[row][column] = _oldPiece;
				_gridArray[_chosenPiece.x][_chosenPiece.y] = EMPTY_PIECE;
			}
			break;
		
		case STATE_ANOTHER_CHOOSING:
			if (this->_gridArray[row][column] <= _anotherColor && this->_gridArray[row][column] >= _anotherColor - 5) {
				this->_gridSelection[row][column].setColor(sf::Color(255, 255, 50, 255));
				for (int x = 0; x < GRID_SIZE; x++) {
					for (int y = 0; y < GRID_SIZE; y++) {
						if (this->_gridArray[x][y] > _anotherColor || this->_gridArray[x][y] < _anotherColor - 5) 
							_gridSelection[x][y].setColor(sf::Color(150, 255, 150, 255));
					}
				}
				_gameState = STATE_ANOTHER_MOVING;
				_oldPiece = _gridArray[row][column];
				_chosenPiece.x = row;
				_chosenPiece.y = column;
			}
			break;

		case STATE_ANOTHER_MOVING:
			if (this->_gridArray[row][column] < _anotherColor || this->_gridArray[row][column] > _anotherColor - 5) {
				_gridPieces[row][column].setTexture(*_gridPieces[_chosenPiece.x][_chosenPiece.y].getTexture());
				_gridPieces[row][column].setTextureRect(_gridPieces[_chosenPiece.x][_chosenPiece.y].getTextureRect());
				_gridPieces[row][column].setColor(sf::Color(255, 255, 255, 255));
				_gridPieces[_chosenPiece.x][_chosenPiece.y].setColor(sf::Color(255, 255, 255, 0));
				for (int x = 0; x < GRID_SIZE; x++) {
					for (int y = 0; y < GRID_SIZE; y++)
						_gridSelection[x][y].setColor(sf::Color(255, 255, 255, 255));
				}

				_gameState = STATE_PLAYER_CHOOSING;
				_gridArray[row][column] = _oldPiece;
				_gridArray[_chosenPiece.x][_chosenPiece.y] = EMPTY_PIECE;
			}
			break;
		}
	}


}

