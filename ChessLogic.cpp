#include "DEFINITIONS.hpp"
#include "ChessLogic.hpp"


namespace ChessEngine {

	ChessLogic::ChessLogic(ChessGUI::cppTable GUITable)
	{
		int bitboardAcc = 0;
		uint8_t temp;
		for (int i = 0; i < GRID_SIZE; i++) {
			for (int j = 0; j < GRID_SIZE; j++) {
				temp = translateGUItoEngine(GUITable[i][j]);
				for (int k = 0; k < 8; k++)
				{
					_bitBoard[bitboardAcc][k] = ((temp >> k) & 1U);
				}
				bitboardAcc++;
			}
		}
		attackMoves = new bool[64];
	}

	ChessLogic::~ChessLogic()
	{
		delete attackMoves;
	}

	bool* ChessLogic::generateLegalMoves(int i, int j)
	{

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