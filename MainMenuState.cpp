#include <sstream>
#include <iostream>
#include "MainMenuState.hpp"
#include "DEFINITIONS.hpp"
#include "GameChooseState.hpp"

namespace ChessGUI
{
	MainMenuState::MainMenuState(GameDataRef data)
	{
		this->_data = data;
	}

	void MainMenuState::Init()
	{
		this->_data->assets.LoadTexture("Background", MAIN_MENU_BACKGROUND_FILEPATH);
		this->_data->assets.LoadTexture("Play Button", MAIN_MENU_PLAY_BUTTON);
		this->_data->assets.LoadTexture("Play Button Outer", MAIN_MENU_PLAY_BUTTON_OUTER);
		this->_data->assets.LoadFont("Courier New Bold", COURIERNEW_BOLD_FONT);
		this->_background.setTexture(this->_data->assets.GetTexture("Background"));
		this->_playButton.setTexture(this->_data->assets.GetTexture("Play Button"));
		this->_playButtonOuter.setTexture(this->_data->assets.GetTexture("Play Button Outer"));

		this->_title.setFont(this->_data->assets.GetFont("Courier New Bold"));
		this->_title.setCharacterSize(72);
		this->_title.setFillColor(sf::Color::White);
		this->_title.setString("Chess Game");

		this->_playButton.setPosition(SCREEN_WIDTH / 2 - this->_playButton.getGlobalBounds().width / 2,
			                          SCREEN_HEIGHT / 2 - this->_playButton.getGlobalBounds().height / 2);
		this->_playButtonOuter.setPosition(SCREEN_WIDTH / 2 - this->_playButtonOuter.getGlobalBounds().width / 2,
			                               SCREEN_HEIGHT / 2 - this->_playButtonOuter.getGlobalBounds().height / 2);
		this->_title.setPosition(SCREEN_WIDTH / 2 - this->_playButtonOuter.getGlobalBounds().width / 2 - 50, 
								SCREEN_HEIGHT / 2 - SCREEN_HEIGHT / 4);
	}

	void MainMenuState::HandleInput()
	{
		sf::Event event;
		while (this->_data->window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				this->_data->window.close();
				break;

			case sf::Event::MouseButtonPressed:
				if (this->_data->input.isAreaClicked(this->_playButton, sf::Mouse::Left, this->_data->window))
					this->_data->machine.AddState(StateRef(new GameChooseState(_data)), true);
				break;

			default:
				break;
			}
		}
	}

	void MainMenuState::Update(float dt)
	{

	}

	void MainMenuState::Draw(float dt)
	{
		this->_data->window.clear();
		this->_data->window.draw(this->_background);
		this->_data->window.draw(this->_playButton);
		this->_data->window.draw(this->_playButtonOuter);
		this->_data->window.draw(this->_title);
		this->_data->window.display();
	}
}
