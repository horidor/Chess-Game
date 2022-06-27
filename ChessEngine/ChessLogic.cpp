
#include "DEFINITIONS.hpp"
#include "ChessLogic.hpp"
#include <iostream>

using namespace ChessGUI;

namespace ChessEngine {

	ChessLogic::ChessLogic(cppTable GUITable)
	{
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (GUITable[i][j] != EMPTY_PIECE) {
					_chessBoard[i][j].type = GUITable[i][j];
					_chessBoard[i][j].position = std::make_pair(i, j);
				}
				else {
					_chessBoard[i][j].type = EMPTY_PIECE;
				}

				if (_chessBoard[i][j].type == WHITE_KING) WhiteKingPos = std::make_pair(i, j);
				if (_chessBoard[i][j].type == BLACK_KING) BlackKingPos = std::make_pair(i, j);
			}
		}

		resetPawnMaps();
		resetMaps();
	}

	std::bitset<64> ChessLogic::generateMoves(int i, int j)
	{
		std::bitset<64> attackMoves;

		switch (_chessBoard[i][j].type)
		{
		case WHITE_QUEEN:
		case BLACK_QUEEN:
			attackMoves |= horizontalMoveCreation(i, j);
			attackMoves |= diagonalMoveCreation(i, j);
			attackMoves |= aroundMoveCreation(i, j);
			break;
		case WHITE_KING:
		case BLACK_KING:
			attackMoves |= kingMoveCreation(i, j);
			break;
		case WHITE_ROOK:
		case BLACK_ROOK:
			attackMoves |= horizontalMoveCreation(i, j);
			break;
		case WHITE_KNIGHT:
		case BLACK_KNIGHT:
			attackMoves |= knightMoveCreation(i, j);
			break;
		case WHITE_BISHOP:
		case BLACK_BISHOP:
			attackMoves |= diagonalMoveCreation(i, j);
			break;
		case WHITE_PAWN:
		case BLACK_PAWN:
			attackMoves |= pawnMoveCreation(i, j);
			break;
		case EMPTY_PIECE:
			break;
		}

		return attackMoves;
	}

	std::bitset<64> ChessLogic::generateLegalMoves(int i, int j, std::bitset<64> attackMoves) {
		if ((CheckPhase) && (checkMaps.size() >= 2) && (pieceColour(_chessBoard[i][j].type) == whoseTurn)) {
			attackMoves.reset();
		}
		else if ((CheckPhase) && (checkMaps.size() == 1 && (pieceColour(_chessBoard[i][j].type) == whoseTurn))) {
			attackMoves &= checkMaps[0];
		}
		
		return attackMoves;
	}

	void ChessLogic::resetMaps() {
		std::bitset<64> AuxMap;
		resetPawnMaps();

		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 8; j++) {
				AuxMap = generateMoves(i, j);
				if (abs(_chessBoard[i][j].type) != WHITE_KING) _chessBoard[i][j].attackMap = generateLegalMoves(i, j, AuxMap);
				else _chessBoard[i][j].attackMap = AuxMap;
			}
	}

	void ChessLogic::resetPawnMaps() {
		for (int i = 0; i < 8; i++) 
			for (int j = 0; j < 8; j++) {
				if (_chessBoard[i][j].type == WHITE_PAWN) {
					if (i != 0) {
						whitePawnMap.set((i - 1) * 8 + j + 1, true);
						whitePawnMap.set((i - 1) * 8 + j - 1, true);
					}
				}
				else if (_chessBoard[i][j].type == BLACK_PAWN) {
					if (i != 7) {
						blackPawnMap.set((i + 1) * 8 + j + 1, true);
						blackPawnMap.set((i + 1) * 8 + j - 1, true);
					}
				}
			}
		
	}

	void ChessLogic::defendMapSetup(int i, int j, int checkedfori, int checkedforj) {
		if (isAlly(i, j, checkedfori, checkedforj)) {
			if (pieceColour(_chessBoard[checkedfori][checkedforj].type) == WHITE)
				whiteDefendMap.set(i * 8 + j);
			else if (pieceColour(_chessBoard[checkedfori][checkedforj].type) == BLACK)
				blackDefendMap.set(i * 8 + j);
		}
	}

	std::vector<std::pair<int, int>> ChessLogic::GUILegalMoves(int i, int j) {
		std::bitset<64> legalMoves = _chessBoard[i][j].attackMap;
		std::vector < std::pair <int, int>> legalMovesGUI;

		for (int _i = 0; _i < GRID_SIZE; _i++)
			for (int _j = 0; _j < GRID_SIZE; _j++)
				if (legalMoves.test(_i * 8 + _j))
					legalMovesGUI.push_back(std::make_pair(_i, _j));

		return legalMovesGUI;
	}

	int ChessLogic::pieceColour(int piece) {
		if (piece > 0) return WHITE;
		else if (piece < 0) return BLACK;
		return NONE;
	}

	bool ChessLogic::isBlocking(int i, int j, int checkedfori, int checkedforj) {
		if ((i < 8 && i >= 0) && (j < 8 && j >= 0)) {
			if ((_chessBoard[i][j].type == EMPTY_PIECE) || ((abs(_chessBoard[i][j].type) == WHITE_KING) && (isEnemy(i, j, checkedfori, checkedforj))))
				return false;
		}
		return true;
	}

	bool ChessLogic::isEnemy(int i, int j, int checkedfori, int checkedforj) {
		if ((i < 8 && i >= 0) && (j < 8 && j >= 0)) {
			if (pieceColour(_chessBoard[i][j].type) != (pieceColour(_chessBoard[checkedfori][checkedforj].type)) && _chessBoard[i][j].type != EMPTY_PIECE)
				return true;
		}
		return false;
	}

	bool ChessLogic::isAlly(int i, int j, int checkedfori, int checkedforj) {
		if ((i < 8 && i >= 0) && (j < 8 && j >= 0)) {
			if (pieceColour(_chessBoard[i][j].type) == (pieceColour(_chessBoard[checkedfori][checkedforj].type)) && _chessBoard[i][j].type != EMPTY_PIECE)
				return true;
		}
		return false;
	}

	void ChessLogic::makeMove(int fromi, int fromj, int toi, int toj) {
		if (_chessBoard[toi][toj].type == WHITE_KING) WhiteKingPos = std::make_pair(toi, toj);
		if (_chessBoard[toi][toj].type == BLACK_KING) BlackKingPos = std::make_pair(toi, toj);
		_chessBoard[toi][toj] = _chessBoard[fromi][fromj];
		_chessBoard[fromi][fromj].type = EMPTY_PIECE;
		_chessBoard[fromi][fromj].attackMap.reset();
		whoseTurn *= -1;
		resetMaps();
		generationCheckMaps(_chessBoard);
		resetMaps();
	}

	std::bitset<64> ChessLogic::horizontalMoveCreation(int i, int j) {
		int verticalAccumN = i - 1;
		int verticalAccumP = i + 1;

		int horizontalAccumN = j - 1;
		int horizontalAccumP = j + 1;

		std::bitset<64> attackMoves;

		while (!isBlocking(verticalAccumN, j, i, j))
		{
			attackMoves.set((verticalAccumN) * 8 + j);
			verticalAccumN--;
		}

		if (isEnemy(verticalAccumN, j, i, j))
			attackMoves.set((verticalAccumN) * 8 + j);
		else defendMapSetup(verticalAccumN, j, i, j);

		while (!isBlocking(verticalAccumP, j, i, j))
		{
			attackMoves.set((verticalAccumP) * 8 + j);
			verticalAccumP++;
		}

		if (isEnemy(verticalAccumP, j, i, j))
			attackMoves.set((verticalAccumP) * 8 + j);
		else defendMapSetup(verticalAccumP, j, i, j);

		while (!isBlocking(i, horizontalAccumN, i, j))
		{
			attackMoves.set(i * 8 + horizontalAccumN);
			horizontalAccumN--;
		}

		if (isEnemy(i, horizontalAccumN, i, j))
			attackMoves.set(i * 8 + horizontalAccumN);

		else defendMapSetup(i, horizontalAccumN, i, j);

		while (!isBlocking(i, horizontalAccumP, i, j))
		{
			attackMoves.set(i * 8 + horizontalAccumP);
			horizontalAccumP++;
		}

		if (isEnemy(i, horizontalAccumP, i, j))
			attackMoves.set(i * 8 + horizontalAccumP);

		else defendMapSetup(i, horizontalAccumP, i, j);

		return attackMoves;
	}

	std::bitset<64> ChessLogic::diagonalMoveCreation(int i, int j) {
		int Accum = 1;

		std::bitset<64> attackMoves;

		while (!isBlocking(i - Accum, j - Accum, i, j))
		{
			attackMoves.set((i - Accum) * 8 + (j - Accum));
			Accum++;
		}

		if (isEnemy(i - Accum, j - Accum, i, j))
			attackMoves.set((i - Accum) * 8 + (j - Accum));

		else defendMapSetup(i - Accum, j - Accum, i, j);

		Accum = 1;

		while (!isBlocking(i - Accum, j + Accum, i, j))
		{
			attackMoves.set((i - Accum) * 8 + (j + Accum));
			Accum++;
		}

		if (isEnemy(i - Accum, j + Accum, i, j))
			attackMoves.set((i - Accum) * 8 + (j + Accum));

		else defendMapSetup(i - Accum, j + Accum, i, j);

		Accum = 1;

		while (!isBlocking(i + Accum, j - Accum, i, j))
		{
			attackMoves.set((i + Accum) * 8 + (j - Accum));
			Accum++;
		}

		if (isEnemy(i + Accum, j - Accum, i, j))
			attackMoves.set((i + Accum) * 8 + (j - Accum));

		else defendMapSetup(i + Accum, j - Accum, i, j);

		Accum = 1;

		while (!isBlocking(i + Accum, j + Accum, i, j))
		{
			attackMoves.set((i + Accum) * 8 + (j + Accum));
			Accum++;
		}

		if (isEnemy(i + Accum, j + Accum, i, j))
			attackMoves.set((i + Accum) * 8 + (j + Accum));

		else defendMapSetup(i + Accum, j + Accum, i, j);

		return attackMoves;

	}

	std::bitset<64> ChessLogic::aroundMoveCreation(int i, int j) {
		std::bitset<64> attackMoves;

		for (int c = i - 1; c < i + 2; c++) {
			if (c >= 0 && c < 8) {
				for (int k = j - 1; k < j + 2; k++) {
					if (k >= 0 && k < 8) {
						if (!isBlocking(c, k, i, j) || isEnemy(c, k, i, j)) {
							attackMoves.set(c * 8 + k);
							defendMapSetup(c, k, i, j);
						}
					}
				}
			}
		}

		return attackMoves;
	}

	std::bitset<64> ChessLogic::knightMoveCreation(int i, int j) {
		std::bitset<64> attackMoves;

		for (int c = i - 2; c <= i + 2; c += 4) {
			if (c >= 0 && c < 8) {
				for (int k = j - 1; k < j + 2; k += 2) {
					if (k >= 0 && k < 8) {
						if (!isBlocking(c, k, i, j) || isEnemy(c, k, i, j)) {
							attackMoves.set(c * 8 + k);
							defendMapSetup(c, k, i, j);
						}	
					}
				}
			}
		}

		for (int c = i - 1; c <= i + 2; c += 2) {
			if (c >= 0 && c < 8) {
				for (int k = j - 2; k <= j + 2; k += 4) {
					if (k >= 0 && k < 8) {
						if (!isBlocking(c, k, i, j) || isEnemy(c, k, i, j)) {
							attackMoves.set(c * 8 + k);
							defendMapSetup(c, k, i, j);
						}
					}
				}
			}
		}

		return attackMoves;
	}

	std::bitset<64> ChessLogic::kingMoveCreation(int i, int j) {
		std::bitset<64> attackMoves;
		std::bitset<64> checkMoves;

		attackMoves |= aroundMoveCreation(i, j);

		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 8; j++) {
				if ((pieceColour(_chessBoard[i][j].type) != whoseTurn) && (abs(_chessBoard[i][j].type) != WHITE_PAWN) && (abs(_chessBoard[i][j].type) != WHITE_KING))
					checkMoves |= _chessBoard[i][j].attackMap;

				else if ((pieceColour(_chessBoard[i][j].type) != whoseTurn) && (abs(_chessBoard[i][j].type) == WHITE_KING))
					checkMoves |= aroundMoveCreation(i, j);
			}

		checkMoves.flip();
		attackMoves &= checkMoves;

		if (whoseTurn == WHITE) {
			checkMoves = blackPawnMap;
			checkMoves |= blackDefendMap;
			checkMoves.flip();
			attackMoves &= checkMoves;
		}
		else {
			checkMoves = whitePawnMap;
			checkMoves |= whiteDefendMap;
			checkMoves.flip();
			attackMoves &= checkMoves;
		}

		return attackMoves;
	}

	std::bitset<64> ChessLogic::pawnMoveCreation(int i, int j) {
		std::bitset<64> attackMoves;

		switch (pieceColour(_chessBoard[i][j].type)) {
		case BLACK:
			if (i < 7) {
				if (!isBlocking(i + 1, j, i, j))
					attackMoves.set((i + 1) * 8 + j);

				if (i == 1)
				{
					if (!isBlocking(i + 2, j, i, j))
						attackMoves.set((i + 2) * 8 + j);
				}

				if (j < 7) {
					if (isEnemy(i + 1, j + 1, i, j))
						attackMoves.set((i + 1) * 8 + j + 1);

				}

				if (j > 0) {
					if (isEnemy(i + 1, j - 1, i, j))
						attackMoves.set((i + 1) * 8 + j - 1);
				}
			}
			break;
		case WHITE:
			if (i > 0) {
				if (!isBlocking(i - 1, j, i, j))
					attackMoves.set((i - 1) * 8 + j);

				if (i == 6)
				{
					if (!isBlocking(i - 2, j, i, j))
						attackMoves.set((i - 2) * 8 + j);
				}

				if (j < 7) {
					if (isEnemy(i - 1, j + 1, i, j) && j < 7)
						attackMoves.set((i - 1) * 8 + j + 1);
				}

				if (j > 0) {
					if (isEnemy(i - 1, j - 1, i, j) && j > 0)
						attackMoves.set((i - 1) * 8 + j - 1);
				}
			}
		}

		return attackMoves;
	}


	std::bitset<64> ChessLogic::streamGenerator(int toi, int toj, int fromi, int fromj) {
		int forI, forJ;
		std::bitset<64> streamMove;

		if (toi == fromi)
			forI = 0;
		else if (toi < fromi)
			forI = -1;
		else
			forI = 1;

		if (toj == fromj)
			forJ = 0;
		else if (toj < fromj)
			forJ = -1;
		else
			forJ = 1;

		int Accum = 1;

		while (!isBlocking(fromi + Accum * forI, fromj + Accum * forJ, fromi, fromj)) {
			streamMove.set((fromi + Accum * forI) * 8 + (fromj + Accum * forJ));
			Accum++;
		}

		if (isEnemy(fromi + Accum * forI, fromj + Accum * forJ, fromi, fromj))
			streamMove.set((fromi + Accum * forI) * 8 + (fromj + Accum * forJ));

		return streamMove;
	}

	bool ChessLogic::isEnd() {
		std::bitset<64> checkForEnd;

		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (pieceColour(_chessBoard[i][j].type) == whoseTurn)
					checkForEnd |= _chessBoard[i][j].attackMap;

			}
		}

		if (checkForEnd.none())
			return true;

		return false;

	}

	void ChessLogic::generationCheckMaps(pieceBoard& boardToCheck) {
		std::bitset<64> CheckMap;

		CheckPhase = false;
		checkMaps.clear();

		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (boardToCheck[i][j].attackMap.test(WhiteKingPos.first * 8 + WhiteKingPos.second) && whoseTurn == WHITE) {
					if (boardToCheck[i][j].type == BLACK_BISHOP || boardToCheck[i][j].type == BLACK_ROOK || boardToCheck[i][j].type == BLACK_QUEEN)
						checkMaps.push_back(streamGenerator(i, j, WhiteKingPos.first, WhiteKingPos.second));

					else if (boardToCheck[i][j].type == BLACK_PAWN || boardToCheck[i][j].type == BLACK_KNIGHT) {
						CheckMap.set(i * 8 + j);
						checkMaps.push_back(CheckMap);
						CheckMap.reset();
					}
					
				}
				else if (boardToCheck[i][j].attackMap.test(BlackKingPos.first * 8 + BlackKingPos.second) && whoseTurn == BLACK) {
					if (boardToCheck[i][j].type == WHITE_BISHOP || boardToCheck[i][j].type == WHITE_ROOK || boardToCheck[i][j].type == WHITE_QUEEN)
						checkMaps.push_back(streamGenerator(i, j, BlackKingPos.first, BlackKingPos.second));

					else if (boardToCheck[i][j].type == WHITE_PAWN || boardToCheck[i][j].type == WHITE_KNIGHT) {
						CheckMap.set(i * 8 + j);
						checkMaps.push_back(CheckMap);
						CheckMap.reset();
					}
				}
					
			}
		}

		if (!checkMaps.empty()) CheckPhase = true;

	}
}