#pragma once

#include <conio.h>

#include "Player.h"



class Human : public Player
{
public:
	Human(char figure)
		: Player(figure)
	{}

	void makeMove(Position* move) override
	{
		bool moveAccepted = false;
		move->gX = 0;
		move->gY = 0;
		move->sbX = 0;
		move->sbY = 0;
		board->updateCursor(move);

		while (moveAccepted == false)
		{
			switch (_getch())
			{
			case 'w':
				board->updateMove(move, 'w');
				board->updateCursor(move);
				break;

			case 's':
				board->updateMove(move, 's');
				board->updateCursor(move);
				break;

			case 'a':
				board->updateMove(move, 'a');
				board->updateCursor(move);
				break;

			case 'd':
				board->updateMove(move, 'd');
				board->updateCursor(move);
				break;

			case '\r':
				moveAccepted = board->acceptMove(move);
				break;
			}
		}
	}

};