#pragma once

#include <list>
#include <cstdlib>

#include "Player.h"



class RandomBot : public Player
{
public:
	RandomBot(char figure)
		: Player(figure)
	{}

	void makeMove(Position* move) override
	{
		std::list<Position> possibleMoves = board->getFreeGrids();
		int ind = rand() % possibleMoves.size();
		auto randomPosition = possibleMoves.begin();
		std::advance(randomPosition, ind);

		move->gX = randomPosition->gX;
		move->gY = randomPosition->gY;
		move->sbX = randomPosition->sbX;
		move->sbY = randomPosition->sbY;
	}

};