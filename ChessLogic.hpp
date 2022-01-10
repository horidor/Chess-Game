#pragma once
#include <array>
#include <stdint.h>
#include <bitset>
#include "Game.hpp"

namespace ChessEngine {

	class ChessLogic {
	public:

		ChessLogic(ChessGUI::cppTable GUIArray);
		ChessGUI::cppTable generateLegalMoves(int i, int j);
		std::vector<std::pair<int, int>> GUILegalMoves(int i, int j);
		static bool pieceColour(int piece);

	private:

		ChessGUI::cppTable _chessBoard;
		ChessGUI::cppTable attackMoves;

		void diagonalMoveCreation(int i, int j);
		void horizontalMoveCreation(int i, int j);
		void knightMoveCreation(int i, int j);
		void aroundMoveCreation(int i, int j);
		void kingMoveCreation(int i, int j);
		void pawnMoveCreation(int i, int j);

		
		bool isBlocking(int i, int j, int anotheri, int anoterj);
		bool isEnemy(int i, int j, int anotheri, int anoterj);

		ChessGUI::cppTable _chessBoardOld;

	};
}