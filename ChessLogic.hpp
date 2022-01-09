#pragma once
#include <array>
#include <stdint.h>
#include <bitset>
#include "Game.hpp"

namespace ChessEngine {
	enum chessBits {
		QUEENBIT = 7,
		KINGBIT = 6,
		ROOKBIT = 5,
		KNIGHTBIT = 4,
		BISHOPBIT = 3,
		PAWNBIT = 2,
		WHITEBIT = 1,
		BLACKBIT = 0
	};

	class ChessLogic {
	public:

		ChessLogic(ChessGUI::cppTable GUIArray);
		std::bitset<64> generateLegalMoves(int i, int j);
		std::vector<std::pair<int, int>> GUILegalMoves(int i, int j);

	private:

		uint8_t translateGUItoEngine(int8_t GUIType);

		std::bitset<8> _bitBoard[64];
		std::bitset<64> attackMoves;

		std::bitset<64> diagonalMoveCreation(int position);
		std::bitset<64> horizontalMoveCreation(int position);
		std::bitset<64> knightMoveCreation(int position);
		std::bitset<64> aroundMoveCreation(int position);
		std::bitset<64> kingMoveCreation(int position);
		std::bitset<64> pawnMoveCreation(int position);

		std::bitset<8> _bitBoardOld[64];

	};
}