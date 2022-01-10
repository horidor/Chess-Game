#pragma once
#include <array>
#include <stdint.h>
#include <bitset>
#include "Game/Game.hpp"

namespace ChessEngine {

	class ChessLogic {
	public:

		ChessLogic(ChessGUI::cppTable GUIArray);
		ChessGUI::cppTable generateLegalMoves(int i, int j);
		std::vector<std::pair<int, int>> GUILegalMoves(int i, int j);
		void makeMove(int fromi, int fromj, int toi, int toj);

	private:

		ChessGUI::cppTable _chessBoard;
		ChessGUI::cppTable attackMoves;

		void diagonalMoveCreation(int i, int j);
		void horizontalMoveCreation(int i, int j);
		void knightMoveCreation(int i, int j);
		void aroundMoveCreation(int i, int j);
		void kingMoveCreation(int i, int j);
		void pawnMoveCreation(int i, int j);

		bool pieceColour(int piece);
		bool isBlocking(int i, int j, int anotheri, int anoterj);
		bool isEnemy(int i, int j, int anotheri, int anoterj);

		ChessGUI::cppTable _chessBoardOld;

	};
}