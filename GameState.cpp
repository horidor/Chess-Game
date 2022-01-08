#include <sstream>
#include <iostream>
#include "GameState.hpp"
#include "PauseState.hpp"
#include "GameOverState.hpp"
#include "DEFINITIONS.hpp"


namespace Sonar
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
			this->_playerColor = BLACK_PIECE;
			this->_anotherColor = WHITE_PIECE;
		}
		else if (this->_data->firsttoPlay == "player")
		{
			this->_gameState = STATE_PLAYER_CHOOSING;
			this->_playerColor = WHITE_PIECE;
			this->_anotherColor = BLACK_PIECE;
		}


		this->ai = std::make_unique<AI>(_data);
		this->_data->assets.LoadTexture("Pause Button", PAUSE_BUTTON);
		this->_data->assets.LoadTexture("Grid Sprite", GRID_SPRITE_FILEPATH);
		this->_data->assets.LoadTexture("Black Tile", BLACK_TILE);
		this->_data->assets.LoadTexture("White Tile", WHITE_TILE);
		this->_data->assets.LoadTexture("Block Piece", BLOCK_PIECE);
		this->_data->assets.LoadTexture("Chess Pieces", CHESS_PIECE_ARRAY);

		_ChessPieces = this->_data->assets.GetTexture("Chess Pieces");
		_ChessPieces.setSmooth(1);

		_background.setTexture(this->_data->assets.GetTexture("Background"));
		_pauseButton.setTexture(this->_data->assets.GetTexture("Pause Button"));
		_gridSprite.setTexture(this->_data->assets.GetTexture("Grid Sprite"));
		_pauseButton.setPosition(SCREEN_WIDTH - _pauseButton.getGlobalBounds().width + 5, 13);
		
		sf::Vector2i tileMeas(this->_data->assets.GetTexture("Black Tile").getSize().x,
			this->_data->assets.GetTexture("Black Tile").getSize().y);

		_gridStart.x = SCREEN_WIDTH / 2 - (tileMeas.x * 8) / 2 - 50; _gridStart.y = SCREEN_HEIGHT / 2 - (tileMeas.y * 8) / 2;
		_gridPos.left = SCREEN_WIDTH / 2 - (tileMeas.x * 8) / 2 - 50; _gridPos.top = SCREEN_HEIGHT / 2 - (tileMeas.y * 8) / 2;
		_gridPos.width = tileMeas.x * 8;
		_gridPos.height = tileMeas.y * 8;
		 
		
		InitGridPiece();
		
		_inputState = true;
	}

	void GameState::HandleInput()
	{
		sf::Event event;
		while (this->_data->window.pollEvent(event))
		{
			switch (event.type) {
				case (sf::Event::Closed):
					this->_data->window.close();
					break;
				case (sf::Event::MouseButtonPressed):

					if (this->_data->input.isAreaClicked(this->_pauseButton, sf::Mouse::Left, this->_data->window))
						this->_data->machine.AddState(StateRef(new PauseState(_data)), false);

					else if (this->_data->input.isAreaClicked(this->_gridPos, sf::Mouse::Left, this->_data->window))
						this->CheckAndPlacePieces();

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
		//this->_data->window.draw(this->_gridSprite);

		for (int x = 0; x < 8; x++)
		{
			for (int y = 0; y < 8; y++)
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
		sf::Vector2u tempSpriteSize(60, 60);
		char piecesArrangment[8]{ 'R','k','B','Q','K','B','k','R' };
		bool WhiteOrBlackFlag = true;
		for (int x = 0; x < 8; x++)
		{
			WhiteOrBlackFlag = !WhiteOrBlackFlag;
			for (int y = 0; y < 8; y++)
			{
				if (WhiteOrBlackFlag) _gridSelection[x][y].setTexture(this->_data->assets.GetTexture("White Tile"));
				else  _gridSelection[x][y].setTexture(this->_data->assets.GetTexture("Black Tile"));

				_gridSelection[x][y].setPosition(_gridStart.x + _gridSelection[x][y].getGlobalBounds().width * y, _gridStart.y + _gridSelection[x][y].getGlobalBounds().height * x);
				WhiteOrBlackFlag = !WhiteOrBlackFlag;

				if (x < 6) _gridArray[x][y] = EMPTY_PIECE;

				if (x < 2) {
					int anotherX = 7 - x;
					int anotherY = 7 - y;

					_gridPieces[x][y].setTexture(_ChessPieces);
					
					_gridArray[x][y] = BLACK_PIECE;

					_gridPieces[anotherX][anotherY].setTexture(_ChessPieces);
					_gridArray[anotherX][anotherY] = WHITE_PIECE;

					sf::IntRect piecePosition;
					piecePosition.height = piecePosition.width = tempSpriteSize.x;
					piecePosition.top = 0;

					if (x == 1) {
						piecePosition.left = 300;
					}
					else if (x == 0)
					{
						switch (piecesArrangment[y])
						{
						case 'Q':
							piecePosition.left = 0;
							break;
						case 'K':
							piecePosition.left = 60;
							break;
						case 'R':
							piecePosition.left = 120;
							break;
						case 'k':
							piecePosition.left = 180;
							break;
						case 'B':
							piecePosition.left = 240;
							break;
						}
					}

					_gridPieces[x][y].setTextureRect(piecePosition);
					piecePosition.top = 60;
					_gridPieces[anotherX][anotherY].setTextureRect(piecePosition);
				}

				_gridPieces[x][y].setPosition(_gridStart.x + 10 + tempSpriteSize.y * y + 20 * y,
					_gridStart.y + 10 + tempSpriteSize.x * x + 20 * x);
				
				
			}
		}
	}

	void GameState::CheckAndPlacePieces()
	{
		this->_inputState = false;
		sf::Vector2i touchPoint = this->_data->input.GetMousePosition(this->_data->window);
		sf::FloatRect gridSize = _gridSprite.getGlobalBounds();
		sf::Vector2f gapOutsideOfGrid = sf::Vector2f((SCREEN_WIDTH - gridSize.width) / 2 - 50, (SCREEN_HEIGHT - gridSize.height) / 2);
		sf::Vector2f gridLocalTouchPos = sf::Vector2f(touchPoint.x - gapOutsideOfGrid.x, touchPoint.y - gapOutsideOfGrid.y);
		sf::Vector2f gridSectionSize = sf::Vector2f(gridSize.width / 8, gridSize.height / 8);
		int column = 0, row = 0;

		column = gridLocalTouchPos.x / gridSectionSize.x ;

		row = gridLocalTouchPos.y / gridSectionSize.y;
		
		switch (_gameState) {
		case STATE_PLAYER_CHOOSING:
			if (this->_gridArray[row][column] == _playerColor) {
				for (int x = 0; x < 8; x++) {
					for (int y = 0; y < 8; y++) {
						if (_gridArray[x][y] != _playerColor) _gridSelection[x][y].setColor(sf::Color(150, 255, 150, 255));
					}
				}
				_gameState = STATE_PLAYER_MOVING;
				_chosenPiece.x = row;
				_chosenPiece.y = column;
			}
			break;
		
		case STATE_PLAYER_MOVING:
			if (this->_gridArray[row][column] != _playerColor) {
				_gridPieces[row][column].setTexture(*_gridPieces[_chosenPiece.x][_chosenPiece.y].getTexture());
				_gridPieces[row][column].setTextureRect(_gridPieces[_chosenPiece.x][_chosenPiece.y].getTextureRect());
				_gridPieces[row][column].setColor(sf::Color(255, 255, 255, 255));
				_gridPieces[_chosenPiece.x][_chosenPiece.y].setColor(sf::Color(255, 255, 255, 0));
				for (int x = 0; x < 8; x++) {
					for (int y = 0; y < 8; y++)
						_gridSelection[x][y].setColor(sf::Color(255, 255, 255, 255));
				}

				_gameState = STATE_ANOTHER_CHOOSING;
				_gridArray[row][column] = _playerColor;
				_gridArray[_chosenPiece.x][_chosenPiece.y] = EMPTY_PIECE;
			}
			break;
		
		case STATE_ANOTHER_CHOOSING:
			if (this->_gridArray[row][column] == _anotherColor) {
				for (int x = 0; x < 8; x++) {
					for (int y = 0; y < 8; y++) {
						if (_gridArray[x][y] != _anotherColor) _gridSelection[x][y].setColor(sf::Color(150, 255, 150, 255));
					}
				}
				_gameState = STATE_ANOTHER_MOVING;
				_chosenPiece.x = row;
				_chosenPiece.y = column;
			}
			break;

		case STATE_ANOTHER_MOVING:
			if (this->_gridArray[row][column] != _anotherColor) {
				_gridPieces[row][column].setTexture(*_gridPieces[_chosenPiece.x][_chosenPiece.y].getTexture());
				_gridPieces[row][column].setTextureRect(_gridPieces[_chosenPiece.x][_chosenPiece.y].getTextureRect());
				_gridPieces[row][column].setColor(sf::Color(255, 255, 255, 255));
				_gridPieces[_chosenPiece.x][_chosenPiece.y].setColor(sf::Color(255, 255, 255, 0));
				for (int x = 0; x < 8; x++) {
					for (int y = 0; y < 8; y++)
						_gridSelection[x][y].setColor(sf::Color(255, 255, 255, 255));
				}

				_gameState = STATE_PLAYER_CHOOSING;
				_gridArray[row][column] = _anotherColor;
				_gridArray[_chosenPiece.x][_chosenPiece.y] = EMPTY_PIECE;
			}
			break;
		}
	}

	void GameState::CheckPlayerWon()
	{
		CheckGameOver();
		
		if (_gameState == STATE_ANOTHER_CHOOSING)
		{
			int col = -1, row = -1;

			int tempGrid[8][8];
			for (int i = 0; i < 8; i++)
			{
				for (int j = 0; j < 8; j++)
				{
					tempGrid[i][j] = this->_gridArray[i][j];
				}
			}
			
			ai->PlacePiece(tempGrid, col, row);

			_gameState = STATE_PLAYING;

			this->_gridArray[col][row] = CANT_MOVE;
			this->ColorAround(col, row);
			this->_gridPieces[col][row].setTexture(this->_data->assets.GetTexture("AI Piece"));
			CheckGameOver();
		}
	}

	void GameState::CheckGameOver()
	{
		if (!this->CheckEmptyPieces())
		{
			if (this->_gameState == STATE_PLAYING)
			{
				_gameState = STATE_LOSE;
			}
			else
			{
				_gameState = STATE_WON;
			}

			this->_clock.restart();
		}
	}

	bool GameState::CheckEmptyPieces()
	{
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				if (this->_gridArray[i][j] != CANT_MOVE)
				{
					return true;
				}
			}
		}
		return false;
	}

	void GameState::ColorAround(int c, int r)
	{
		for (int i = c - 1; i < c + 2; i++)
		{
			for (int j = r - 1; j < r + 2; j++)
			{
				if (i >= 0 && i <= 7 && j >= 0 && j <= 7)
				{
					this->_gridPieces[i][j].setColor(sf::Color(255, 255, 255, 255));
					this->_gridArray[i][j] = CANT_MOVE;
					if (i == c && j == r)
					{
						continue;
					}
					this->_gridPieces[i][j].setTexture(this->_data->assets.GetTexture("Block Piece"));

				}
			}
		}
	}

}

