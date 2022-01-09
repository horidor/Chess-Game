#include <sstream>
#include <iostream>
#include "MainMenuState.hpp"
#include "GameState.hpp"
#include "GameChooseState.hpp"
#include "DEFINITIONS.hpp"

namespace ChessGUI
{
	GameChooseState::GameChooseState(GameDataRef data)
	{
		this->_data = data;
	}

	void GameChooseState::Init()
	{
		this->_data->assets.LoadTexture("Pieces", CHESS_PIECE_ARRAY);
		_mainTexture = this->_data->assets.GetTexture("Pieces");
		_mainTexture.setSmooth(1);

		this->_XButton.setTexture(_mainTexture);
		this->_YButton.setTexture(_mainTexture);

		this->_XButton.setPosition(SCREEN_WIDTH / 2 - 130,
			this->_data->window.getSize().y / 2 + this->_XButton.getLocalBounds().height / 2);
		this->_YButton.setPosition(SCREEN_WIDTH / 2 + 40,
			this->_data->window.getSize().y / 2 + this->_YButton.getLocalBounds().height / 2);
		
		this->_XButton.setScale(1.5f, 1.5f);
		this->_YButton.setScale(1.5f, 1.5f);

		this->_XButton.setTextureRect(sf::IntRect(60, 60, 60, 60));
		this->_YButton.setTextureRect(sf::IntRect(60, 0, 60, 60));
	}

	void GameChooseState::HandleInput()
	{
		sf::Event event;
		while (this->_data->window.pollEvent(event))
		{
			switch (event.type) {
			case sf::Event::Closed:
				this->_data->window.close();
				break;

			case sf::Event::MouseButtonPressed:
				if (this->_data->input.isAreaClicked(this->_XButton, sf::Mouse::Left, this->_data->window)) {
					this->_data->firsttoPlay = "player";
					this->_data->machine.AddState(StateRef(new GameState(_data)), true);
				}
				if (this->_data->input.isAreaClicked(this->_YButton, sf::Mouse::Left, this->_data->window)) {
					this->_data->firsttoPlay = "another";
					this->_data->machine.AddState(StateRef(new GameState(_data)), true);
				}
				break;

			default:
				break;
			}
		}
	}

	void GameChooseState::Update(float dt)
	{

	}

	void GameChooseState::Draw(float dt)
	{
		this->_data->window.clear(sf::Color(255, 36, 70, 1));
		this->_data->window.draw(this->_XButton);
		this->_data->window.draw(this->_YButton);
		this->_data->window.display();
	}
}