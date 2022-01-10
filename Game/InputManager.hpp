#pragma once

#include <SFML/Graphics.hpp>

namespace ChessGUI
{
	class InputManager
	{
	public:
		InputManager() {};
		~InputManager() {};
		bool isAreaClicked(sf::Sprite object, sf::Mouse::Button button, sf::RenderWindow& window);
		bool isAreaClicked(sf::IntRect object, sf::Mouse::Button button, sf::RenderWindow& window);
		sf::Vector2i GetMousePosition(sf::RenderWindow& window);
	};
}
