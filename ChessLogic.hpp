#pragma once
#include <array>
#include <stdint.h>
#include "Game.hpp"

namespace ChessEngine {
	enum chessBits {
		QUEENBIT = (1U << 7),
		KINGBIT = (1U << 6),
		ROOKBIT = (1U << 5),
		KNIGHTBIT = (1U << 4),
		BISHOPBIT = (1U << 3),
		PAWNBIT = (1U << 2),
		WHITEBIT = (1U << 1),
		BLACKBIT = (1U)
	};

	class ChessLogic {
	public:

		ChessLogic(ChessGUI::cppTable GUIArray);
		~ChessLogic();
		uint64_t generateLegalMoves(int i, int j);
		std::vector<int> GUILegalMoves(int i, int j);

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