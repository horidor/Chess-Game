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
		bool* generateLegalMoves(int i, int j);
		std::vector<int> GUILegalMoves(int i, int j);

	private:

		uint8_t translateGUItoEngine(int8_t GUIType);

		bool _bitBoard[64][8];
		bool* attackMoves;

		std::array<bool, 64> diagonalMoveCreation(const bool**);
		std::array<bool, 64> horizontalMoveCreation(const bool**);
		std::array<bool, 64> knightMoveCreation(const bool**);
		std::array<bool, 64> kingaroundMoveCreation(const bool**);
		std::array<bool, 64> pawnMoveCreation(const bool**);

		bool _bitBoardOld[64][8];

	};
}