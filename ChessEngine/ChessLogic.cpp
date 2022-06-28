
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
				_chessBoard[i][j].pinMap.reset();
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

		if (_chessBoard[i][j].pinMap.any()) attackMoves &= _chessBoard[i][j].pinMap;
		
		return attackMoves;
	}

	void ChessLogic::resetMaps() {
		std::bitset<64> AuxMap;
		resetPawnMaps();
		generationPinMaps();

		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 8; j++) {
				AuxMap = generateMoves(i, j);
				if (abs(_chessBoard[i][j].type) != WHITE_KING) _chessBoard[i][j].attackMap = generateLegalMoves(i, j, AuxMap);
				else _chessBoard[i][j].attackMap = AuxMap;
			}

		AddCastlingMoves();
	}

	void ChessLogic::resetPawnMaps() {
		whitePawnMap.reset();
		blackPawnMap.reset();
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

	void ChessLogic::promoteTo(int i, int j, int type) {
		_chessBoard[i][j].type = type;
		
		castlingCheck(i, j);
		resetMaps();
		generationCheckMaps();
		castlingCheck(i, j);
		resetMaps();
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

	void ChessLogic::enPassantCheck(int fromi, int fromj, int toi, int toj) {
		enPassanttoPass = std::make_pair(-1, -1);
		if ((toi == enPassant.first - 1 || toi == enPassant.second + 1) && toj == enPassant.second && abs(_chessBoard[fromi][fromj].type == WHITE_PAWN)) {
			_chessBoard[enPassant.first][enPassant.second].type = EMPTY_PIECE;
			enPassanttoPass = enPassant;
		}
		else if (abs(_chessBoard[fromi][fromj].type) == WHITE_PAWN && abs(fromi - toi) == 2) {
			enPassant = std::make_pair(toi, toj);
			return;
		}
		enPassant = std::make_pair(-1, -1);
	}

	void ChessLogic::makeMove(int fromi, int fromj, int toi, int toj) {
		castlingMove = std::make_pair(-1, -1);
		if (_chessBoard[fromi][fromj].type == WHITE_KING) {
			WhiteKingPos = std::make_pair(toi, toj);
			if (abs(toj - fromj) == 2) PerformCastlingMoves(toi, toj);
		}
		if (_chessBoard[fromi][fromj].type == BLACK_KING) {
			BlackKingPos = std::make_pair(toi, toj);
			if (abs(toj - fromj) == 2) PerformCastlingMoves(toi, toj);
		}

		enPassantCheck(fromi, fromj, toi, toj);

		_chessBoard[toi][toj] = _chessBoard[fromi][fromj];
		_chessBoard[fromi][fromj].type = EMPTY_PIECE;
		_chessBoard[fromi][fromj].attackMap.reset();
		whoseTurn *= -1;

		castlingCheck(toi, toj, fromi, fromj);
		resetMaps();
		generationCheckMaps();

		castlingCheck(toi, toj, fromi, fromj);
		resetMaps();
	}

	void ChessLogic::PerformCastlingMoves(int toi, int toj) {
		if (whoseTurn == BLACK) {
			if (toj == 6) {
				_chessBoard[0][5].type = BLACK_ROOK;
				_chessBoard[0][7].type = EMPTY_PIECE;
				castlingMove = std::make_pair(0, 5);
			}
			else if (toj == 2) {
				_chessBoard[0][3].type = BLACK_ROOK;
				_chessBoard[0][0].type = EMPTY_PIECE;
				castlingMove = std::make_pair(0, 3);
			}
		}
		else if (whoseTurn == WHITE) {
			if (toj == 6) {
				_chessBoard[7][5].type = WHITE_ROOK;
				_chessBoard[7][7].type = EMPTY_PIECE;
				castlingMove = std::make_pair(7, 5);
			}
			else if (toj == 2) {
				_chessBoard[7][3].type = WHITE_ROOK;
				_chessBoard[7][0].type = EMPTY_PIECE;
				castlingMove = std::make_pair(7, 3);
			}
		}
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

	void ChessLogic::AddCastlingMoves() {
		if (castlingWhiteShort) _chessBoard[WhiteKingPos.first][WhiteKingPos.second].attackMap.set(7 * 8 + 6);
		if (castlingWhiteLong) _chessBoard[WhiteKingPos.first][WhiteKingPos.second].attackMap.set(7 * 8 + 2);
		if (castlingBlackShort) _chessBoard[BlackKingPos.first][BlackKingPos.second].attackMap.set(6);
		if (castlingBlackLong) _chessBoard[BlackKingPos.first][BlackKingPos.second].attackMap.set(2);
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
					if (!isBlocking(i + 2, j, i, j) && (!isBlocking(i + 1, j, i, j)))
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

				if (isEnemy(enPassant.first, enPassant.second, i, j)) {
					if (i == enPassant.first) {
						if (j == enPassant.second - 1 || j == enPassant.second + 1)
							attackMoves.set((enPassant.first + 1) * 8 + enPassant.second);
					}
				}
			}
			break;
		case WHITE:
			if (i > 0) {
				if (!isBlocking(i - 1, j, i, j))
					attackMoves.set((i - 1) * 8 + j);

				if (i == 6)
				{
					if (!isBlocking(i - 2, j, i, j) && (!isBlocking(i - 1, j, i, j)))
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

				if (isEnemy(enPassant.first, enPassant.second, i, j)) {
					if (i == enPassant.first) {
						if (j == enPassant.second - 1 || j == enPassant.second + 1)
							attackMoves.set((enPassant.first - 1) * 8 + enPassant.second);
					}
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

		while (!isEnemy(fromi + Accum * forI, fromj + Accum * forJ, fromi, fromj)) {
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

	void ChessLogic::generationCheckMaps() {
		std::bitset<64> CheckMap;

		CheckPhase = false;
		checkMaps.clear();

		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (_chessBoard[i][j].attackMap.test(WhiteKingPos.first * 8 + WhiteKingPos.second) && whoseTurn == WHITE) {
					if (_chessBoard[i][j].type == BLACK_BISHOP || _chessBoard[i][j].type == BLACK_ROOK || _chessBoard[i][j].type == BLACK_QUEEN)
						checkMaps.push_back(streamGenerator(i, j, WhiteKingPos.first, WhiteKingPos.second));

					else if (_chessBoard[i][j].type == BLACK_PAWN || _chessBoard[i][j].type == BLACK_KNIGHT) {
						CheckMap.set(i * 8 + j);
						checkMaps.push_back(CheckMap);
						CheckMap.reset();
					}
					
				}
				else if (_chessBoard[i][j].attackMap.test(BlackKingPos.first * 8 + BlackKingPos.second) && whoseTurn == BLACK) {
					if (_chessBoard[i][j].type == WHITE_BISHOP || _chessBoard[i][j].type == WHITE_ROOK || _chessBoard[i][j].type == WHITE_QUEEN)
						checkMaps.push_back(streamGenerator(i, j, BlackKingPos.first, BlackKingPos.second));

					else if (_chessBoard[i][j].type == WHITE_PAWN || _chessBoard[i][j].type == WHITE_KNIGHT) {
						CheckMap.set(i * 8 + j);
						checkMaps.push_back(CheckMap);
						CheckMap.reset();
					}
				}
					
			}
		}

		if (!checkMaps.empty()) CheckPhase = true;

	}

	void ChessLogic::generationPinMaps() {

		std::pair<int, int> allypos = std::make_pair(-1, -1);

		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				_chessBoard[i][j].pinMap.reset();
			}
		}
		
		int Accum;
		int allyCount;

		int i, j;
		if (whoseTurn == WHITE) {
			i = WhiteKingPos.first;
			j = WhiteKingPos.second;
		}
		else if (whoseTurn == BLACK) {
			i = BlackKingPos.first;
			j = BlackKingPos.second;
		}

		for (int fori = -1; fori < 2; fori++) {
			for (int forj = -1; forj < 2; forj++) {
				if ((fori == 0) and (forj == 0)) continue;

				Accum = 1;
				allyCount = 0;

				while (!isEnemy(i + Accum * fori, j + Accum * forj, i, j)) {
					if (isAlly(i + Accum * fori, j + Accum * forj, i, j)) {
						if (allyCount == 1) {
							allyCount++;
							break;
						}
						else {
							allyCount++;
							allypos = std::make_pair(i + Accum * fori, j + Accum * forj);
						}
					}
					if (i + Accum * fori >= 8 || i + Accum * fori < 0 || j + Accum * forj < 0 || j + Accum * forj >= 8) break;
					Accum++;
				}

				if (allyCount == 1) {
					if (isEnemy(i + Accum * fori, j + Accum * forj, i, j)) {
						if (abs(_chessBoard[i + Accum * fori][j + Accum * forj].type) == WHITE_QUEEN || abs(_chessBoard[i + Accum * fori][j + Accum * forj].type) == WHITE_ROOK 
							|| abs(_chessBoard[i + Accum * fori][j + Accum * forj].type) == WHITE_BISHOP)
							_chessBoard[allypos.first][allypos.second].pinMap = streamGenerator(i + Accum * fori, j + Accum * forj, i, j);
					}
				}
			}
		}

	}

	void ChessLogic::castlingCheck(int toi, int toj, int fromi, int fromj) {
		castlingBlackLong = false;
		castlingBlackShort = false;
		castlingWhiteLong = false;
		castlingWhiteShort = false;
		std::bitset<64> whiteAttackMap;
		std::bitset<64> blackAttackMap;

		for (int checki = 0; checki < 8; checki++) {
			for (int checkj = 0; checkj < 8; checkj++) {
				if (pieceColour(_chessBoard[checki][checkj].type) == WHITE)
					whiteAttackMap |= _chessBoard[checki][checkj].attackMap;
				else if (pieceColour(_chessBoard[checki][checkj].type) == BLACK)
					blackAttackMap |= _chessBoard[checki][checkj].attackMap;
			}
		}

		if (_chessBoard[toi][toj].type == BLACK_KING) {
			castlingBlackLongF = false;
			castlingBlackShortF = false;
		}
		if (_chessBoard[toi][toj].type == WHITE_KING) {
			castlingWhiteLongF = false;
			castlingWhiteShortF = false;
		}

		if (!whiteAttackMap.test(BlackKingPos.first * 8 + BlackKingPos.second)) {
			if (castlingBlackLongF) {
				if (_chessBoard[toi][toj].type == BLACK_ROOK && fromi == 0 && fromj == 0) {
					castlingBlackLongF = false;
				}
				else {
					castlingBlackLong = true;
					for (int j = BlackKingPos.second - 1; j > 0; j--) {
						if (isEnemy(BlackKingPos.first, j, BlackKingPos.first, BlackKingPos.second) || isAlly(BlackKingPos.first, j, BlackKingPos.first, BlackKingPos.second)) {
							castlingBlackLong = false;
							break;
						}

						if (whiteAttackMap.test(BlackKingPos.first * 8 + j)) {
							castlingBlackLong = false;
							break;
						}
					}
				}
			}

			if (castlingBlackShortF) {
				if (_chessBoard[toi][toj].type == BLACK_ROOK && fromi == 0 && fromj == 7) {
					castlingBlackShortF = false;
				}
				else {
					castlingBlackShort = true;
					for (int j = BlackKingPos.second + 1; j < 7; j++) {
						if (isEnemy(BlackKingPos.first, j, BlackKingPos.first, BlackKingPos.second) || isAlly(BlackKingPos.first, j, BlackKingPos.first, BlackKingPos.second)) {
							castlingBlackShort = false;
							break;
						}

						if (whiteAttackMap.test(BlackKingPos.first * 8 + j)) {
							castlingBlackShort = false;
							break;
						}
					}
				}
			}
		}

		if (!blackAttackMap.test(WhiteKingPos.first * 8 + WhiteKingPos.second)) {
			if (castlingWhiteLongF) {
				if (_chessBoard[toi][toj].type == WHITE_ROOK && fromi == 7 && fromj == 0) {
					castlingWhiteLongF = false;
				}
				else {
					castlingWhiteLong = true;
					for (int j = WhiteKingPos.second - 1; j > 0; j--) {
						if (isEnemy(WhiteKingPos.first, j, WhiteKingPos.first, WhiteKingPos.second) || isAlly(WhiteKingPos.first, j, WhiteKingPos.first, WhiteKingPos.second)) {
							castlingWhiteLong = false;
							break;
						}

						if (blackAttackMap.test(WhiteKingPos.first * 8 + j)) {
							castlingWhiteLong = false;
							break;
						}
					}
				}
			}

			if (castlingWhiteShortF) {
				if (_chessBoard[toi][toj].type == WHITE_ROOK && fromi == 7 && fromj == 7) {
					castlingWhiteShortF = false;
				}
				else {
					castlingWhiteShort = true;
					for (int j = WhiteKingPos.second + 1; j < 7; j++) {
						if (isEnemy(WhiteKingPos.first, j, WhiteKingPos.first, WhiteKingPos.second) || isAlly(WhiteKingPos.first, j, WhiteKingPos.first, WhiteKingPos.second)) {
							castlingWhiteShort = false;
							break;
						}

						if (blackAttackMap.test(WhiteKingPos.first * 8 + j)) {
							castlingWhiteShort = false;
							break;
						}
					}
				}
			}
		}
	}
}