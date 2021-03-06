#pragma once
#include <array>
#include <vector>
#include <stdint.h>
#include <bitset>
#include "Game/Game.hpp"

namespace ChessEngine {

	class ChessLogic {
	public:

		ChessLogic(ChessGUI::cppTable GUIArray);
		
		std::vector<std::pair<int, int>> GUILegalMoves(int i, int j);
		void makeMove(int fromi, int fromj, int toi, int toj);
		void promoteTo(int i, int j, int type);

		bool getEnd() { return isEnd(); }
		bool isCheck() { return CheckPhase; }

		std::pair<int, int> enPassantOuter() { return enPassanttoPass; }
		std::pair<int, int> castlingOuter() { return castlingMove;  }


	private:
		enum PieceColour {
			BLACK = -1,
			NONE = 0,
			WHITE = 1
		};
		int whoseTurn = WHITE;
		bool CheckPhase = false;
		std::pair<int, int> enPassant = std::make_pair(-1, -1);
		std::pair<int, int> enPassanttoPass = std::make_pair(-1, -1);
		std::pair<int, int> castlingMove = std::make_pair(-1, -1);

		bool castlingWhiteLong = false, castlingWhiteShort = false, castlingWhiteLongF = true, castlingWhiteShortF = true;
		bool castlingBlackLong = false, castlingBlackShort = false, castlingBlackLongF = true, castlingBlackShortF = true;

		std::vector<std::bitset<64>> checkMaps;

		std::pair<int, int> WhiteKingPos;
		std::pair<int, int> BlackKingPos;

		struct Piece {
			std::bitset<64> attackMap;
			int8_t type;
			std::pair<int, int> position;
			std::bitset<64> pinMap;
		};

		Piece checkCheckingPiece;

		typedef std::array<std::array<Piece, 8>, 8> pieceBoard;

		pieceBoard _chessBoard;

		std::bitset<64> generateMoves(int i, int j);
		std::bitset<64> diagonalMoveCreation(int i, int j);
		std::bitset<64> horizontalMoveCreation(int i, int j);
		std::bitset<64> knightMoveCreation(int i, int j);
		std::bitset<64> aroundMoveCreation(int i, int j);
		std::bitset<64> kingMoveCreation(int i, int j);
		std::bitset<64> pawnMoveCreation(int i, int j);
		std::bitset<64> generateLegalMoves(int i, int j, std::bitset<64> attackMoves);

		void resetMaps();
		void resetPawnMaps();

		std::bitset<64> whitePawnMap;
		std::bitset<64> blackPawnMap;

		std::bitset<64> whiteDefendMap;
		std::bitset<64> blackDefendMap;

		std::bitset<64> streamGenerator(int toi, int toj, int fromi, int fromj);

		bool isEnd();

		void generationCheckMaps();
		void generationPinMaps();

		void enPassantCheck(int fromi, int fromj, int toi, int toj);

		void castlingCheck(int toi, int toj, int fromi = -1, int fromj = -1);

		void AddCastlingMoves();
		void PerformCastlingMoves(int toi, int toj);


		int pieceColour(int piece);
		bool isBlocking(int i, int j, int anotheri, int anoterj);
		bool isEnemy(int i, int j, int anotheri, int anoterj);
		bool isAlly(int i, int j, int checkedfori, int checkedforj);
		void defendMapSetup(int i, int j, int checkedfori, int checkedforj);

	};
}