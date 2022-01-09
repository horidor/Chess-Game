#include <sstream>
#include <iostream>
#include "MainMenuState.hpp"
#include "GameOverState.hpp"
#include "DEFINITIONS.hpp"
#include "GameChooseState.hpp"

namespace ChessGUI
{
	GameOverState::GameOverState(GameDataRef data, int gameState)
	{
		this->_data = data;
		this->_gameState = gameState;
	}

	void GameOverState::Init()
	{
		this->_data->assets.LoadTexture("Retry Button", RETRY_BUTTON);
		this->_data->assets.LoadTexture("Home Button", HOME_BUTTON);

		this->_retryButton.setTexture(this->_data->assets.GetTexture("Retry Button"));
		this->_homeButton.setTexture(this->_data->assets.GetTexture("Home Button"));

		if (this->_gameState == STATE_WON)
		{
			this->_data->assets.LoadTexture("You Won", SPRITE_WON_FILEPATH);
			this->_result.setTexture(this->_data->assets.GetTexture("You Won"));
		}
		else
		{
			this->_data->assets.LoadTexture("You Lose", SPRITE_LOSE_FILEPATH);
			this->_result.setTexture(this->_data->assets.GetTexture("You Lose"));
		}

		this->_retryButton.setPosition(this->_data->window.getSize().x / 2 - this->_retryButton.getLocalBounds().width / 2 - 170,
			                           this->_data->window.getSize().y / 2 + this->_retryButton.getLocalBounds().height / 2);
		this->_homeButton.setPosition(this->_data->window.getSize().x / 2 - this->_homeButton.getLocalBounds().width / 2 + 170,
			                          this->_data->window.getSize().y / 2 + this->_homeButton.getLocalBounds().height / 2);
		this->_result.setPosition(this->_data->window.getSize().x / 2 - this->_result.getLocalBounds().width / 2,
			                      this->_data->window.getSize().y / 2 - this->_result.getLocalBounds().height * 1.5);
	}

	void GameOverState::HandleInput()
	{
		sf::Event event;
		while (this->_data->window.pollEvent(event))
		{
			switch (event.type) {

			case sf::Event::Closed:
				this->_data->window.close();
				break;

			case sf::Event::MouseButtonPressed:
				if (this->_data->input.isAreaClicked(this->_retryButton, sf::Mouse::Left, this->_data->window))
					this->_data->machine.AddState(StateRef(new GameChooseState(_data)), true);

				if (this->_data->input.isAreaClicked(this->_homeButton, sf::Mouse::Left, this->_data->window))
					this->_data->machine.AddState(StateRef(new MainMenuState(_data)), true);

			default:
				break;
			}
		}
	}

	void GameOverState::Update(float dt)
	{

	}

	void GameOverState::Draw(float dt)
	{
		this->_data->window.clear(sf::Color(255, 36, 70, 1));
		this->_data->window.draw(this->_retryButton);
		this->_data->window.draw(this->_homeButton);
		this->_data->window.draw(this->_result);
		this->_data->window.display();
	}
}
