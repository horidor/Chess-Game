#include "AI.hpp"
#include "DEFINITIONS.hpp"
#include <map>

namespace Sonar
{
	AI::AI(GameDataRef data)
	{
		this->_data = data;
	}

	void AI::PlacePiece(int(&gridArray)[8][8], int& column, int& row)
	{
		int maxEvaluation = -(INF + 1);

		int evaluated_turns = EvaluatePossible(gridArray);

		std::vector < std::pair<int, int>> _gnThru;

		for (int count = 0; count < evaluated_turns; count++)
		{
			int i, j;
			do
			{
				i = rand() % 8;
				j = rand() % 8;
			} while (gridArray[i][j] == CANT_MOVE || BeenThere(i, j, _gnThru));
			_gnThru.push_back(std::make_pair(i, j));
			gridArray[i][j] = CANT_MOVE;
			std::vector<std::pair<int, int>> boundary = BlockBoundary(gridArray, i, j);
			int evaluation = MiniMax(gridArray, INITIAL_DEPTH, -(INF + 1), INF + 1, false);
			gridArray[i][j] = EMPTY_PIECE;

			for (int k = 0; k < boundary.size(); k++)
			{
				gridArray[boundary[k].first][boundary[k].second] = EMPTY_PIECE;
			}
			if (evaluation > maxEvaluation)
			{
				maxEvaluation = evaluation;
				column = i;
				row = j;
			}
		}
	}

	int AI::MiniMax(int(&gridArray)[8][8], int depth, int alpha, int beta, bool isMaximizing)
	{
		int staticEvaluation = this->HeuristicEvaluation(gridArray, depth + 1);
		if (depth == 0 || abs(staticEvaluation) == ((INF/INITIAL_DEPTH)*(depth+1)))
		{
			return staticEvaluation;
		}

		int evaluated_turns = EvaluatePossible(gridArray);

		std::vector < std::pair<int, int>> _gnThru;

		if (isMaximizing)
		{
			int maxEvaluation = -INF - 1;
			for (int count = 0; count < evaluated_turns; count++)
			{

				int i = -1, j = -1;
				do
				{
					i = rand() % 8;
					j = rand() % 8;
				} while (gridArray[i][j] == CANT_MOVE || BeenThere(i, j, _gnThru));

				_gnThru.push_back(std::make_pair(i, j));
				gridArray[i][j] = CANT_MOVE;

				std::vector<std::pair<int, int>> boundary = BlockBoundary(gridArray, i, j);

				int evaluation = MiniMax(gridArray, depth - 1, alpha, beta, false);
				maxEvaluation = std::max(evaluation, maxEvaluation);
				alpha = std::max(alpha, evaluation);

				gridArray[i][j] = EMPTY_PIECE;

				for (int k = 0; k < boundary.size(); k++)
				{
					gridArray[boundary[k].first][boundary[k].second] = EMPTY_PIECE;
				}

				if (beta <= alpha)
				{
					return maxEvaluation;
				}
			}
			return maxEvaluation;
		}
		else
		{
			int minEvaluation = INF + 1;

			for (int count = 0; count < evaluated_turns; count++)
			{

				int i = -1, j = -1;
				do
				{
					i = rand() % 8;
					j = rand() % 8;
				} while (gridArray[i][j] == CANT_MOVE || BeenThere(i, j, _gnThru));
				_gnThru.push_back(std::make_pair(i, j));

				gridArray[i][j] = CANT_MOVE;

				std::vector<std::pair<int, int>> boundary = BlockBoundary(gridArray, i, j);

				int evaluation = MiniMax(gridArray, depth - 1, alpha, beta, true);
				minEvaluation = std::min(evaluation, minEvaluation);
				beta = std::min(beta, evaluation);

				gridArray[i][j] = EMPTY_PIECE;

				for (int k = 0; k < boundary.size(); k++)
				{
					gridArray[boundary[k].first][boundary[k].second] = EMPTY_PIECE;
				}

				if (beta <= alpha)
				{
					return minEvaluation;
				}
			}

			return minEvaluation;
		}
	}

	int AI::HeuristicEvaluation(int(&gridArray)[8][8], int depth)
	{
		int score = 0;

		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				if (gridArray[i][j] == CANT_MOVE)
				{
					++score;
				}
			}
		}
		return score*depth;
	}

	std::vector<std::pair<int, int>> AI::BlockBoundary(int(&gridArray)[8][8], int& i, int& j)
	{
		gridArray[i][j] = CANT_MOVE;

		std::vector<std::pair<int, int>> boundary;

		for (int c = i - 1; c < i + 2; c++)
		{
			for (int r = j - 1; r < j + 2; r++)
			{
				if (c >= 0 && c <= 7 && r >= 0 && r <= 7)
				{
					if (gridArray[c][r] != CANT_MOVE)
					{
						gridArray[c][r] = CANT_MOVE;
						std::pair<int, int> coord;
						coord.first = c;
						coord.second = r;
						boundary.push_back(coord);
					}
				}
			}
		}
		return boundary;
	}

	bool AI::BeenThere(int i, int j, const std::vector<std::pair<int, int>>& _gnThru)
	{
		for (auto it = _gnThru.begin(); it != _gnThru.end(); it++)
		{
			if (it->first == i && it->second == j) return true;
		}
		return false;
	}

	int AI::EvaluatePossible(int(&gridArray)[8][8])
	{
		int score = 0;

		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				if (gridArray[i][j] == EMPTY_PIECE)
				{
					++score;
				}
			}
		}

		return std::min(score, CHILDREN_NUMBER);
	}

}


