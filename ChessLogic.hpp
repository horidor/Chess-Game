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
		uint64_t generateLegalMoves(int i, int j);
		std::vector<std::pair<int, int>> GUILegalMoves(int i, int j);

	private:

		uint8_t translateGUItoEngine(int8_t GUIType);

		uint8_t _bitBoard[64];
		uint64_t attackMoves;

		uint64_t diagonalMoveCreation(int position);
		uint64_t horizontalMoveCreation(int position);
		uint64_t knightMoveCreation(int position);
		uint64_t aroundMoveCreation(int position);

		uint64_t kingMoveCreation(int position);

		uint64_t pawnMoveCreation(int position);

		uint8_t _bitBoardOld[64];

	};
}