#include "InputManager.hpp"

namespace Sonar
{
	bool InputManager::isAreaClicked(sf::Sprite object, sf::Mouse::Button button, sf::RenderWindow& window)
	{
		if (sf::Mouse::isButtonPressed(button))
		{
			sf::Vector2i mousePos = GetMousePosition(window);
			sf::IntRect tempRect(object.getPosition().x, object.getPosition().y, object.getGlobalBounds().width, object.getGlobalBounds().height);
			
			if (tempRect.contains(mousePos))
			{
				return true;
			}
		}

		return false;
	}

	bool InputManager::isAreaClicked(sf::IntRect object, sf::Mouse::Button button, sf::RenderWindow& window)
	{
		if (sf::Mouse::isButtonPressed(button))
		{
			sf::Vector2i mousePos = GetMousePosition(window);
			if (object.contains(mousePos))
			{
				return true;
			}
		}

		return false;
	}

	sf::Vector2i InputManager::GetMousePosition(sf::RenderWindow& window)
	{
		return sf::Mouse::getPosition(window);
	}
}
