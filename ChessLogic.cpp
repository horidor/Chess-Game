#include "DEFINITIONS.hpp"
#include "ChessLogic.hpp"


namespace ChessEngine {

	ChessLogic::ChessLogic(ChessGUI::cppTable GUITable)
	{
		int bitboardAcc = 0;
		for (int i = 0; i < GRID_SIZE; i++) {
			for (int j = 0; j < GRID_SIZE; j++) {
				_bitBoard[bitboardAcc] = translateGUItoEngine(GUITable[i][j]);
				bitboardAcc++;
			}
		}
	}

	uint64_t ChessLogic::generateLegalMoves(int i, int j)
	{
		int currentPos = i * 8 + j;


		if (QUEENBIT & _bitBoard[currentPos]) {
			attackMoves = horizontalMoveCreation(currentPos);
			attackMoves |= aroundMoveCreation(currentPos);
			attackMoves |= diagonalMoveCreation(currentPos);
		}
		else if (KINGBIT & _bitBoard[currentPos]) {
			attackMoves = kingMoveCreation(currentPos);
		}
		else if (ROOKBIT & _bitBoard[currentPos]) {
			attackMoves = horizontalMoveCreation(currentPos);
		}
		else if (KNIGHTBIT & _bitBoard[currentPos]) {
			attackMoves = knightMoveCreation(currentPos);
		}
		else if (BISHOPBIT & _bitBoard[currentPos]) {
			attackMoves = diagonalMoveCreation(currentPos);
		}
		else if (PAWNBIT & _bitBoard[currentPos]) {
			attackMoves = pawnMoveCreation(currentPos);
		}

		return attackMoves;
	}

	uint64_t ChessLogic::horizontalMoveCreation(int position) {
		int verticalAccumP = position + 8;
		int verticalAccumM = position - 8;

		int horizontalAccumP = position + 1;
		int horizontalAccumM = position - 1;
		int horizontalBoundary = position / 8;

		while (verticalAccumP < 64 && _bitBoard[verticalAccumP] == 0)
		{
			attackMoves |= (uint64_t)1 << (64 - verticalAccumP - 1);
			verticalAccumP += 8;
		}

		while (verticalAccumM >= 0 && _bitBoard[verticalAccumM] == 0)
		{
			attackMoves |= (uint64_t)1 << (64 - verticalAccumM - 1);
			verticalAccumM -= 8;
		}

		while (horizontalAccumP < (horizontalBoundary + 1) * 8 && _bitBoard[horizontalAccumP] == 0)
		{
			attackMoves |= (uint64_t)1 << (64 - horizontalAccumP - 1);
			horizontalAccumP += 1;
		}

		while (horizontalAccumM > horizontalBoundary * 8 && _bitBoard[horizontalAccumM] == 0)
		{
			attackMoves |= (uint64_t)1 << (64 - horizontalAccumM - 1);
			horizontalAccumP -= 1;
		}

		return attackMoves;
	}

	uint64_t ChessLogic::diagonalMoveCreation(int position) {
		int topLeftAccum = position - 9;
		int topRightAccum = position - 7;

		int botLeftAccum = position + 7;
		int botRightAccum = position + 9;

		while (topLeftAccum % 8 != 0 && _bitBoard[topLeftAccum] == 0)
		{
			attackMoves |= (uint64_t)1 << (64 - topLeftAccum - 1);
			topLeftAccum -= 9;
		}

		while ((topRightAccum+1) % 8 != 0 && _bitBoard[topRightAccum] == 0)
		{
			attackMoves |= (uint64_t)1 << (64 - topRightAccum - 1);
			topRightAccum -= 7;
		}

		while (botLeftAccum % 8 != 0 && _bitBoard[botLeftAccum] == 0)
		{
			attackMoves |= (uint64_t)1 << (64 - botLeftAccum - 1);
			botLeftAccum += 7;
		}

		while ((botRightAccum+1) % 8 != 0 && _bitBoard[botRightAccum] == 0)
		{
			attackMoves |= (uint64_t)1 << (64 - botRightAccum - 1);
			botRightAccum += 9;
		}
	}

	uint64_t ChessLogic::aroundMoveCreation(int position) {
		
	}

	uint64_t ChessLogic::knightMoveCreation(int position) {

	}

	uint64_t ChessLogic::kingMoveCreation(int position) {

	}

	uint64_t ChessLogic::pawnMoveCreation(int position) {

	}



	uint8_t ChessLogic::translateGUItoEngine(int8_t GUIType)
	{
		uint8_t type = 0;
		if (GUIType > BLACK_QUEEN) type |= WHITEBIT;

		else if (GUIType < BLACK_QUEEN)
		{
			type |= BLACKBIT;
			GUIType += 5;
		}

		switch (GUIType)
		{
		case (WHITE_QUEEN):
			type |= QUEENBIT;
			break;
		case (WHITE_KING):
			type |= KINGBIT;
			break;
		case (WHITE_ROOK):
			type |= ROOKBIT;
			break;
		case (WHITE_KNIGHT):
			type |= KNIGHTBIT;
			break;
		case (WHITE_BISHOP):
			type |= BISHOPBIT;
			break;
		case (WHITE_PAWN):
			type |= PAWNBIT;
			break;
		}

		return GUIType;
	}

}