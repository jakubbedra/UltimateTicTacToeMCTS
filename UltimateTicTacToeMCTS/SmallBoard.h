#pragma once

#include <Windows.h>

#include "constants.h"
#include "Position.h"
#include "Board.h"
#include "Player.h"


class SmallBoard
{
private:
	int gameState;
	int board[BOARD_SIZE][BOARD_SIZE];
	Position position;
	Player* player1;
	Player* player2;

	bool winner[BOARD_SIZE][BOARD_SIZE];	//used for marking the winners in print method
	bool enabled;							//set true if player can make a move on this smallboard

public:
	SmallBoard(Position position, Player* player1, Player* player2)
	{
		this->player1 = player1;
		this->player2 = player2;

		this->position.sbX = position.sbX;
		this->position.sbY = position.sbY;
		this->position.gX = position.gX;
		this->position.gY = position.gY;

		this->gameState = IN_PROGRESS;
		this->enabled = true;

		for (int i = 0; i < BOARD_SIZE; i++)
			for (int j = 0; j < BOARD_SIZE; j++)
			{
				this->board[i][j] = 0;
				this->winner[i][j] = false;
			}
	}
	//(re)prints the 9 board grids
	void print()
	{
		for (int i = 0; i < BOARD_SIZE; i++)
		{
			for (int j = 0; j < BOARD_SIZE; j++)
			{
				DWORD dw;
				COORD here{ position.sbX * 8 + 2 * j + 2,
							position.sbY * 4 + i + 1 };
				HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
				WORD color = 0x07;

				if (winner[i][j])
				{
					if (gameState == PLAYER1)
						color = 0x2A;
					else color = 0x4C;
				}

				wchar_t wtext[2] = { 0x0000, 0x0000 };
				void* a = &wtext;
				char figure;

				if (board[i][j] == 1)
					figure = player1->getFigure();
				else if (board[i][j] == 2)
					figure = player2->getFigure();
				else figure = ' ';

				*(char*)(a) = figure;
				LPCWSTR ptr = wtext;

				WriteConsoleOutputAttribute(hStdOut, &color, 1, here, &dw);
				WriteConsoleOutputCharacter(hStdOut, ptr, 1, here, &dw);
			}
		}
	}

	void resetWinnerGrids()
	{
		for (int i = 0; i < BOARD_SIZE; i++)
			for (int j = 0; j < BOARD_SIZE; j++)
				this->winner[i][j] = false;
	}

	void checkWinningConditions(int id)
	{
		//checking for winning
		for (int i = 0; i < BOARD_SIZE; i++)
			if (board[i][0] == id && board[i][1] == id && board[i][2] == id)
			{
				winner[i][0] = true; winner[i][1] = true; winner[i][2] = true;
				this->gameState = id;
				return;
			}
		for (int i = 0; i < BOARD_SIZE; i++)
			if (board[0][i] == id && board[1][i] == id && board[2][i] == id)
			{
				winner[0][i] = true; winner[1][i] = true; winner[2][i] = true;
				this->gameState = id;
				return;
			}
		if (board[0][0] == id && board[1][1] == id && board[2][2] == id)
		{
			winner[0][0] = true; winner[1][1] = true; winner[2][2] = true;
			this->gameState = id;
			return;
		}
		if (board[2][0] == id && board[1][1] == id && board[0][2] == id)
		{
			winner[2][0] = true; winner[1][1] = true; winner[0][2] = true;
			this->gameState = id;
			return;
		}
		//checking for draw
		bool draw = true;
		for (int i = 0; i < BOARD_SIZE; i++)
			for (int j = 0; j < BOARD_SIZE; j++)
				if (board[i][j] == 0)
					draw = false;
		if (draw == true)
			this->gameState = DRAW;
	}

	void makeMove(Position* move, int id)
	{
		board[move->gY][move->gX] = id;
	}
	
	void getFreeGrids(std::list<Position>* freeGrids)
	{
		for (int i = 0; i < BOARD_SIZE; i++)
			for (int j = 0; j < BOARD_SIZE; j++)
				if (board[i][j] == 0)
					freeGrids->push_back(Position{j, i, this->position.sbX, this->position.sbY});
	}

	bool isFreeGrid(Position* position)
	{
		return (this->board[position->gY][position->gX] == 0 
			&& this->gameState == IN_PROGRESS);
	}

	int getState()
	{
		return this->gameState;
	}

	void setState(int gameState)
	{
		this->gameState = gameState;
	}

	void enable()
	{
		this->enabled = true;
	}

	void disable()
	{
		this->enabled = false;
	}
	
	bool isEnabled()
	{
		return this->enabled;
	}

};