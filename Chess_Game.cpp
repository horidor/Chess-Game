#include <iostream>
#include "Game/Game.hpp"
#include "DEFINITIONS.hpp"

int main()
{
    ChessGUI::Game game(SCREEN_WIDTH, SCREEN_HEIGHT, "Chess Game");
    return EXIT_SUCCESS;
}
