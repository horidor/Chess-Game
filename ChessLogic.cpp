#include "DEFINITIONS.hpp"
#include "ChessLogic.hpp"
#include <iostream>


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

		for (int i = 0; i < 64; i++)
		{
			std::cout << _bitBoard[i] << " ";
			if ((i + 1) % 8 == 0) std::cout << std::endl;
		}
	}

	std::vector<std::pair<int, int>> ChessLogic::GUILegalMoves(int i, int j) {
		std::bitset<64> temp = generateLegalMoves(i, j);
		std::vector<std::pair<int, int>> whatToReturn;

		for (int i = 0; i < 64; i++) {
			if (temp.test(i)) {
				whatToReturn.push_back(std::make_pair((63 - i) / 8, (63 - i) % 8));
				
			}
			std::cout << temp[i] << " ";
			if ((i + 1) % 8 == 0) std::cout << std::endl;
		}

		return whatToReturn;
	}

	std::bitset<64> ChessLogic::generateLegalMoves(int i, int j)
	{
		int currentPos = i * 8 + j;


		if (_bitBoard[currentPos].test(QUEENBIT)) {
			attackMoves = horizontalMoveCreation(currentPos);
			attackMoves |= aroundMoveCreation(currentPos);
			attackMoves |= diagonalMoveCreation(currentPos);
		}
		else if (_bitBoard[currentPos].test(KINGBIT)) {
			attackMoves = kingMoveCreation(currentPos);
		}
		else if (_bitBoard[currentPos].test(ROOKBIT)) {
			attackMoves = horizontalMoveCreation(currentPos);
		}
		else if (_bitBoard[currentPos].test(KNIGHTBIT)) {
			attackMoves = knightMoveCreation(currentPos);
		}
		else if (_bitBoard[currentPos].test(BISHOPBIT)) {
			attackMoves = diagonalMoveCreation(currentPos);
		}
		else if (_bitBoard[currentPos].test(PAWNBIT)) {
			attackMoves = pawnMoveCreation(currentPos);
		}

		return attackMoves;
	}

	std::bitset<64> ChessLogic::horizontalMoveCreation(int position) {
		int verticalAccumP = position + 8;
		int verticalAccumM = position - 8;

		int horizontalAccumP = position + 1;
		int horizontalAccumM = position - 1;
		int horizontalBoundary = position / 8;

		while (verticalAccumP < 64 && _bitBoard[verticalAccumP] == 0)
		{
			attackMoves.set(64 - verticalAccumP);
			verticalAccumP += 8;
		}

		while (verticalAccumM >= 0 && _bitBoard[verticalAccumM] == 0)
		{
			attackMoves.set(64 - verticalAccumM);
			verticalAccumM -= 8;
		}

		while (horizontalAccumP < (horizontalBoundary + 1) * 8 && _bitBoard[horizontalAccumP] == 0)
		{
			attackMoves.set(64 - horizontalAccumP);
			horizontalAccumP += 1;
		}

		while (horizontalAccumM > horizontalBoundary * 8 && _bitBoard[horizontalAccumM] == 0)
		{
			attackMoves.set(64 - horizontalAccumM);
			horizontalAccumP -= 1;
		}

		return attackMoves;
	}

	std::bitset<64> ChessLogic::diagonalMoveCreation(int position) {
		int topLeftAccum = position - 9;
		int topRightAccum = position - 7;

		int botLeftAccum = position + 7;
		int botRightAccum = position + 9;

		while (topLeftAccum % 8 != 0 && _bitBoard[topLeftAccum] == 0)
		{
			attackMoves.set(64 - topLeftAccum);
			topLeftAccum -= 9;
		}

		while ((topRightAccum+1) % 8 != 0 && _bitBoard[topRightAccum] == 0)
		{
			attackMoves.set(64 - topRightAccum);
			topRightAccum -= 7;
		}

		while (botLeftAccum % 8 != 0 && _bitBoard[botLeftAccum] == 0)
		{
			attackMoves.set(64 - botLeftAccum);
			botLeftAccum += 7;
		}

		while ((botRightAccum+1) % 8 != 0 && _bitBoard[botRightAccum] == 0)
		{
			attackMoves.set(64 - botRightAccum);
			botRightAccum += 9;
		}

		return attackMoves;
	}

	std::bitset<64> ChessLogic::aroundMoveCreation(int position) {
		if (position >= 8) {
			if (position % 8 != 0) attackMoves.set(64 - position - 9);
			attackMoves.set(64 - position - 8 - 1);
			if ((position+1) % 8 != 0) attackMoves.set(64 - position - 7);
		}

		if (position < 56) {
			if (position % 8 != 0) attackMoves.set(64 - position + 7);
			attackMoves.set(64 - position + 8);
			if ((position + 1) % 8 != 0) attackMoves.set(64 - position + 9);
		}

		if (position % 8 != 0) attackMoves.set(64 - position - 1);
		else if ((position+1) % 8 != 0) attackMoves.set(64 - position + 1);

		return attackMoves;

	}

	std::bitset<64> ChessLogic::knightMoveCreation(int position) {
		if (position >= 16) {
			attackMoves.set(64 - position - 17);
			attackMoves.set(64 - position - 15);
		}

		if (position < 48) {
			attackMoves.set(64 - position + 15);
			attackMoves.set(64 - position + 17);
		}

		if ((position - 1) % 8 != 0) {
			attackMoves.set(64 - position - 10);
			attackMoves.set(64 - position + 6);
		}

		if ((position + 2) % 8 != 0) {
			attackMoves.set(64 - position + 10);
			attackMoves.set(64 - position - 6);
		}


		return attackMoves;
	}

	std::bitset<64> ChessLogic::kingMoveCreation(int position) {
		if (position >= 8) {
			if (position % 8 != 0) attackMoves.set(64 - position - 9);
			attackMoves.set(64 - position - 8 - 1);
			if ((position + 1) % 8 != 0) attackMoves.set(64 - position - 7);
		}

		if (position < 56) {
			if (position % 8 != 0) attackMoves.set(64 - position + 7);
			attackMoves.set(64 - position + 8);
			if ((position + 1) % 8 != 0) attackMoves.set(64 - position + 9);
		}

		if (position % 8 != 0) attackMoves.set(64 - position - 1);
		else if ((position + 1) % 8 != 0) attackMoves.set(64 - position + 1);

		std::cout << attackMoves << std::endl;

		return attackMoves;
	}

	std::bitset<64> ChessLogic::pawnMoveCreation(int position) {
		if (position >= 8) {
			if (position % 8 != 0 && _bitBoard[position - 9] != 0) attackMoves.set(64 - position - 9);
			attackMoves.set(64 - position - 8);
			if ((position + 1) % 8 != 0 && _bitBoard[position - 7] != 0) attackMoves.set(64 - position - 7);
		}


		return attackMoves;
	}



	uint8_t ChessLogic::translateGUItoEngine(int8_t GUIType)
	{
		uint8_t type = 0;
		if (GUIType == EMPTY_PIECE) return 0;

		if (GUIType > BLACK_QUEEN) type |= (1U<<WHITEBIT);

		else if (GUIType <= BLACK_QUEEN)
		{
			type |= (1U<<BLACKBIT);
			GUIType += 6;
		}

		switch (GUIType)
		{
		case (WHITE_QUEEN):
			type |= (1U<<QUEENBIT);
			break;
		case (WHITE_KING):
			type |= (1U<<KINGBIT);
			break;
		case (WHITE_ROOK):
			type |= (1U<<ROOKBIT);
			break;
		case (WHITE_KNIGHT):
			type |= (1U<<KNIGHTBIT);
			break;
		case (WHITE_BISHOP):
			type |= (1U<<BISHOPBIT);
			break;
		case (WHITE_PAWN):
			type |= (1U<<PAWNBIT);
			break;
		}

		return type;
	}

}