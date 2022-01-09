#pragma once
class ChessLogic {
public:

	ChessLogic(int*);
	bool* generateLegalMoves(int i, int j);



private:
	bool _bitBoard [64][8];

};