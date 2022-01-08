#pragma once

#include <SFML/Graphics.hpp>
#include "Game.hpp"
#include <vector>

namespace Sonar
{
	class AI
	{
	public:
		AI(GameDataRef data);
		void PlacePiece(int (&gridArray)[8][8], int& column, int& row);
	private:
		GameDataRef _data;
		int MiniMax(int(&gridArray)[8][8], int depth, int alpha, int beta, bool isMaximizing);
		int HeuristicEvaluation(int(&gridArray)[8][8], int depth);
		int EvaluatePossible(int(&gridArray)[8][8]);
		std::vector<std::pair<int, int>> BlockBoundary(int(&gridArray)[8][8], int& i, int& j);
		bool BeenThere(int i, int j, const std::vector<std::pair<int, int>>& _gnThru);
	};
}
