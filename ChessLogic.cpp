#include "DEFINITIONS.hpp"
#include "ChessLogic.hpp"
#include <iostream>

using namespace ChessGUI;

namespace ChessEngine {

	enum PieceColour {
		BLACK = true,
		WHITE = false
	};

	ChessLogic::ChessLogic(cppTable GUITable)
	{
		_chessBoard = GUITable;
	}

	cppTable ChessLogic::generateLegalMoves(int i, int j)
	{
		for (auto& row : attackMoves) row.fill(CANT_MOVE);

		switch (_chessBoard[i][j])
		{
		case WHITE_QUEEN:
		case BLACK_QUEEN:
			horizontalMoveCreation(i, j);
			diagonalMoveCreation(i, j);
			aroundMoveCreation(i, j);
			break;
		case WHITE_KING:
		case BLACK_KING:
			kingMoveCreation(i, j);
			break;
		case WHITE_ROOK:
		case BLACK_ROOK:
			horizontalMoveCreation(i, j);
			break;
		case WHITE_KNIGHT:
		case BLACK_KNIGHT:
			knightMoveCreation(i, j);
			break;
		case WHITE_BISHOP:
		case BLACK_BISHOP:
			diagonalMoveCreation(i, j);
			break;
		case WHITE_PAWN:
		case BLACK_PAWN:
			pawnMoveCreation(i, j);
			break;
		}

		return attackMoves;
	}

	std::vector<std::pair<int, int>> ChessLogic::GUILegalMoves(int i, int j) {
		cppTable legalMoves = generateLegalMoves(i, j);
		std::vector < std::pair <int, int>> legalMovesGUI;

		for (auto& it : legalMoves) {
			for (auto iter : it)
				std::cout << iter << "  ";
			std::cout << "\n";
		}

		for (int _i = 0; _i < GRID_SIZE; _i++)
			for (int _j = 0; _j < GRID_SIZE; _j++)
				if (legalMoves[_i][_j] == CAN_MOVE)
					legalMovesGUI.push_back(std::make_pair(_i, _j));
		
		return legalMovesGUI;
	}

	bool ChessLogic::pieceColour(int piece) {
		if (piece > 0) return WHITE;
		return BLACK;
	}

	bool ChessLogic::isBlocking(int i, int j, int checkedfori, int checkedforj) {
		if ((_chessBoard[i][j] == EMPTY_PIECE) || (isEnemy(i, j, checkedfori, checkedforj)))
			return false;
		return true;
	}

	bool ChessLogic::isEnemy(int i, int j, int checkedfori, int checkedforj) {
		if (pieceColour(_chessBoard[i][j]) != (pieceColour(_chessBoard[checkedfori][checkedforj])) && _chessBoard[i][j] != EMPTY_PIECE)
			return true;
		return false;
	}

	void ChessLogic::horizontalMoveCreation(int i, int j) {
		int verticalAccumN = i;
		if (i <= 0) verticalAccumN += 1;
		int verticalAccumP = i;
		if (i >= 7) verticalAccumP -= 1;

		int horizontalAccumN = j;
		if (j <= 0) horizontalAccumN += 1;
		int horizontalAccumP = j;
		if (j >= 7) horizontalAccumP -= 1;

		while (verticalAccumN - 1 > 0 && _chessBoard[verticalAccumN - 1][j] == EMPTY_PIECE)
		{
			verticalAccumN--;
			attackMoves[verticalAccumN][j] = CAN_MOVE;
		}

		verticalAccumN--;
		if (!isBlocking(verticalAccumN, j, i, j)) 
			attackMoves[verticalAccumN][j] = CAN_MOVE;

		while (verticalAccumP+1 < 7 && _chessBoard[verticalAccumP + 1][j] == EMPTY_PIECE)
		{
			attackMoves[verticalAccumP][j] = CAN_MOVE;
			verticalAccumP++;
		}

		verticalAccumP++;
		if (!isBlocking(verticalAccumP, j, i, j))
			attackMoves[verticalAccumP][j] = CAN_MOVE;

		while (horizontalAccumN-1 > 0 && _chessBoard[i][horizontalAccumN - 1] == EMPTY_PIECE)
		{
			attackMoves[i][horizontalAccumP] = CAN_MOVE;
			horizontalAccumN--;
		}

		horizontalAccumN--;
		if (!isBlocking(i, horizontalAccumN, i, j))
			attackMoves[i][horizontalAccumN] = CAN_MOVE;

		while (horizontalAccumP+1 < 7 && _chessBoard[i][horizontalAccumN + 1] == EMPTY_PIECE)
		{
			attackMoves[i][horizontalAccumP] = CAN_MOVE;
			horizontalAccumP++;
		}

		horizontalAccumP++;
		if (!isBlocking(i, horizontalAccumP, i, j))
			attackMoves[i][horizontalAccumP] = CAN_MOVE;
	}

	void ChessLogic::diagonalMoveCreation(int i, int j) {
		int Accum = 0;
		if (i <= 0 || j <= 0) Accum -= 1;
		

		while ((j - Accum - 1 > 0 && i - Accum - 1 > 0) && _chessBoard[i - Accum - 1][j - Accum - 1]  == EMPTY_PIECE)
		{
			Accum++;
			attackMoves[i - Accum][j - Accum] = CAN_MOVE;
		}

		Accum++;
		if (!isBlocking(i-Accum, j-Accum, i, j))
			attackMoves[i - Accum][j - Accum] = CAN_MOVE;

		Accum = 0;
		if (i <= 0 || j >= 7) Accum -= 1;

		while ((j + Accum + 1 < 7 && i - Accum - 1 > 0) && _chessBoard[i - Accum - 1][j + Accum + 1] == EMPTY_PIECE)
		{
			Accum++;
			attackMoves[i - Accum][j + Accum] = CAN_MOVE;
		}

		Accum++;
		if (!isBlocking(i - Accum, j + Accum, i, j))
			attackMoves[i - Accum][j + Accum] = CAN_MOVE;


		Accum = 0;
		if (i >= 7 || j <= 0) Accum -= 1;

		while ((j - Accum - 1 > 0 && i + Accum + 1 < 7) && _chessBoard[i + Accum + 1][j - Accum - 1] == EMPTY_PIECE)
		{
			Accum++;
			attackMoves[i + Accum][j - Accum] = CAN_MOVE;
		}

		Accum++;
		if (!isBlocking(i + Accum, j - Accum, i, j))
			attackMoves[i + Accum][j - Accum] = CAN_MOVE;

		
		Accum = 0;
		if (i >= 7 || j >= 7) Accum -= 1;

		while ((j + Accum + 1 < 7 && i + Accum + 1 < 7) && _chessBoard[i + Accum + 1][j + Accum + 1] == EMPTY_PIECE)
		{
			Accum++;
			_chessBoard[i + Accum][j + Accum] = CAN_MOVE;
		}

		Accum++;
		if (!isBlocking(i + Accum, j + Accum, i, j))
			attackMoves[Accum + i][Accum + j] = CAN_MOVE;

	}

	void ChessLogic::aroundMoveCreation(int i, int j) {
		for (int c = i - 1; c < i + 2; c++) {
			for (int k = j - 1; k < j + 2; k++) {
				if (c >= 0 && c < 8 && k >= 0 && k < 8) {
					if (!isBlocking(c, k, i, j))
						attackMoves[c][k] = CAN_MOVE;
				}
			}
		}
	}

	void ChessLogic::knightMoveCreation(int i, int j) {
		for (int c = i - 2; c <= i + 2; c += 4) {
			for (int k = j - 1; k < j + 2; k += 2) {
				if (c >= 0 && c < 8 && k >= 0 && k < 8) {
					if (!isBlocking(c, k, i, j))
						attackMoves[c][k] = CAN_MOVE;
				}
			}
		}

		for (int c = i - 1; c <= i + 2; c += 2) {
			for (int k = j - 2; k < j + 2; k += 4) {
				if (c >= 0 && c < 8 && k >= 0 && k < 8) {
					if (!isBlocking(c, k, i, j))
						attackMoves[c][k] = CAN_MOVE;
				}
			}
		}
	}

	void ChessLogic::kingMoveCreation(int i, int j) {
		aroundMoveCreation(i, j);
	}

	void ChessLogic::pawnMoveCreation(int i, int j) {
		switch (pieceColour(_chessBoard[i][j])) {
		case BLACK:
			if (i < 7) {
				if (!isBlocking(i + 1, j, i, j))
					attackMoves[i + 1][j] = CAN_MOVE;
			
				if (j < 7) {
					if (isEnemy(i + 1, j + 1, i, j))
						attackMoves[i + 1][j + 1] = CAN_MOVE;
				}
				
				if (j > 0) {
					if (isEnemy(i + 1, j - 1, i, j))
						attackMoves[i + 1][j - 1] = CAN_MOVE;
				}
			}
			break;
		case WHITE:
			if (i > 0) {
				if (!isBlocking(i - 1, j, i, j))
					attackMoves[i - 1][j] = CAN_MOVE;
				
				if (j < 7) {
					if (isEnemy(i - 1, j + 1, i, j) && j < 7)
						attackMoves[i - 1][j + 1] = CAN_MOVE;
				}
				
				if (j > 0) {
					if (isEnemy(i - 1, j - 1, i, j) && j > 0)
						attackMoves[i - 1][j - 1] = CAN_MOVE;
				}
			}
		}
	}

}