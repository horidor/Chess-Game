#pragma once

#define SCREEN_WIDTH 934
#define SCREEN_HEIGHT 852

#define SPLASH_STATE_SHOW_TIME 3
#define SPLASH_SCENE_BACKGROUND_FILEPATH "Resources\\Splash Background.png"
#define MAIN_MENU_BACKGROUND_FILEPATH "Resources\\Main Menu Background.png"
#define GAME_BACKGROUND_FILEPATH "Resources\\Main Menu Background.png"
#define PAUSE_BACKGROUND_FILEPATH "Resources\\Main Menu Background.png"

#define MAIN_MENU_TITLE_PATH "Resources\\Game Title.png"
#define MAIN_MENU_PLAY_BUTTON "Resources\\Play Button.png"
#define RESUME_BUTTON "Resources\\Resume Button.png"
#define HOME_BUTTON "Resources\\Home Button.png"
#define RETRY_BUTTON "Resources\\Retry Button.png"

#define MAIN_MENU_PLAY_BUTTON_OUTER "Resources\\Play Button Outer.png"
#define PAUSE_BUTTON "Resources\\Game Files\\Pause Button.png"

#define X_PIECE_FILEPATH "Resources\\X.png"
#define O_PIECE_FILEPATH "Resources\\O.png"
#define BLOCK_PIECE "Resources\\Block.png"

#define GRID_SPRITE_FILEPATH "Resources\\Game Files\\Chess Grid.png"

#define CHESS_PIECE_ARRAY "Resources\\Game Files\\Chess Pieces Array.png"

#define COURIERNEW_FONT "Resources\\Fonts\\couriernew.ttf"
#define COURIERNEW_BOLD_FONT "Resources\\Fonts\\couriernewbold.ttf"

#define WHITE_TILE "Resources\\Game Files\\Tile White.png"
#define BLACK_TILE "Resources\\Game Files\\Tile Black.png"

#define EMPTY_PIECE -16
#define CAN_MOVE -2
#define CANT_MOVE -3

#define WHITE_PIECE		-4
#define WHITE_QUEEN		-5
#define WHITE_ROOK		-6
#define WHITE_BISHOP	-7
#define WHITE_KNIGHT	-8
#define WHITE_PAWN		-9

#define BLACK_PIECE		-10
#define BLACK_QUEEN		-11
#define BLACK_ROOK		-12
#define BLACK_BISHOP	-13
#define BLACK_KNIGHT	-14
#define BLACK_PAWN		-15

#define STATE_PLAYING 94
#define STATE_PAUSED 95
#define STATE_WON 96
#define STATE_LOSE 97
#define STATE_PLAYER_CHOOSING 98
#define STATE_PLAYER_MOVING 99
#define STATE_ANOTHER_CHOOSING 100
#define STATE_ANOTHER_MOVING 101

#define TIME_BEFORE_SHOWING_GAME_OVER 1
#define SPRITE_WON_FILEPATH "Resources\\Win.png"
#define SPRITE_LOSE_FILEPATH "Resources\\Lose.png"

#define INITIAL_DEPTH 5
#define INF 64 * INITIAL_DEPTH
#define CHILDREN_NUMBER 10
