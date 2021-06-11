#pragma once

#include "Position.h"
#include "Board.h"



class Board;

class Player
{
protected:
	char figure;
	int id;
	Board* board;

public:
	Player(char figure)
	{
		this->figure = figure;
	}

	virtual void makeMove(Position* move) = 0;

	char getFigure()
	{
		return this->figure;
	}

	int getID()
	{
		return this->id;
	}

	void setID(int id)
	{
		this->id = id;
	}

	void setBoard(Board* board)
	{
		this->board = board;
	}

};