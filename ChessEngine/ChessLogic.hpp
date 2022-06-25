#pragma once
#include <array>
#include <stdint.h>
#include <bitset>
#include "Game/Game.hpp"

namespace ChessEngine {

	class ChessLogic {
	public:

		ChessLogic(ChessGUI::cppTable GUIArray);
		std::bitset<64> generateLegalMoves(int i, int j);
		std::vector<std::pair<int, int>> GUILegalMoves(int i, int j);
		void makeMove(int fromi, int fromj, int toi, int toj);


	private:

		struct Piece {
			std::bitset<64> attackMap;
			int8_t type;
			std::pair<int, int> position;
		};

		std::array<std::array<Piece, 8>, 8> _chessBoard;

		std::bitset<64> diagonalMoveCreation(int i, int j);
		std::bitset<64> horizontalMoveCreation(int i, int j);
		std::bitset<64> knightMoveCreation(int i, int j);
		std::bitset<64> aroundMoveCreation(int i, int j);
		std::bitset<64> kingMoveCreation(int i, int j);
		std::bitset<64> pawnMoveCreation(int i, int j);

		void resetAttackMaps();

		bool pieceColour(int piece);
		bool isBlocking(int i, int j, int anotheri, int anoterj);
		bool isEnemy(int i, int j, int anotheri, int anoterj);

	};
}