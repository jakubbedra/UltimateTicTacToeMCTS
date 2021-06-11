#pragma once

#include <iostream>
#include <cstdlib>
#include <list>

#include "constants.h"
#include "Smallboard.h"
#include "Player.h"



class Board
{
private:
	int gameState;
	SmallBoard* board[BOARD_SIZE][BOARD_SIZE];
	Player* player1;
	Player* player2;
	Player* currentPlayer;

	Position lastMove;	//last placed figure position
	char lastFigure;	//last placed figure (for print method)

	bool doPrint;

public:
	Board(Player* player1, Player* player2)
	{
		this->player1 = player1;
		this->player2 = player2;
		player1->setID(PLAYER1);
		player2->setID(PLAYER2);

		this->gameState = IN_PROGRESS;

		for (int i = 0; i < BOARD_SIZE; i++)
			for (int j = 0; j < BOARD_SIZE; j++)
				this->board[i][j] = new SmallBoard(Position{0, 0, j, i}, player1, player2);

		if (rand() % 2 == 0)
			this->currentPlayer = player1;
		else this->currentPlayer = player2;

		player1->setBoard(this);
		player2->setBoard(this);

		this->lastMove.gX = -1;
		this->lastMove.gY = -1;
		this->lastMove.sbX = -1;
		this->lastMove.sbY = -1;
		this->lastFigure = ' ';
		this->doPrint = true;
	}

	~Board()
	{
		for (int i = 0; i < BOARD_SIZE; i++)
			for (int j = 0; j < BOARD_SIZE; j++)
				delete this->board[i][j];
	}

	void makeTurn()
	{
		Position move;
		print(); 
		
		currentPlayer->makeMove(&move);
		makeMove(&move);
		
		lastMove.gX = move.gX;
		lastMove.gY = move.gY;
		lastMove.sbX = move.sbX;
		lastMove.sbY = move.sbY;
		print();

		updateSmallBoards(&move);
		checkWinningConditions();
		
		lastFigure = currentPlayer->getFigure();

		if (gameState == DRAW)
		{
			this->print();
			std::cout << "Game over! Draw." << std::endl;
			return;
		}
		else if (gameState == PLAYER1)
		{
			this->print();
			std::cout << "Game over! Player " << player1->getFigure()
				<< " has won!" << std::endl;
			return;
		}
		else if (gameState == PLAYER2)
		{
			this->print();
			std::cout << "Game over! Player " << player2->getFigure()
				<< " has won!" << std::endl;
			return;
		}

		togglePlayer();
	}

	void makeMove(Position* move)
	{
		board[move->sbY][move->sbX]->makeMove(move, currentPlayer->getID());
	}
	
	void checkWinningConditions()
	{
		int id = currentPlayer->getID();
		//checking for winning
		for (int i = 0; i < BOARD_SIZE; i++)
			if (board[i][0]->getState() == id && board[i][1]->getState() == id && board[i][2]->getState() == id)
			{
				this->gameState = currentPlayer->getID();
				return;
			}
		for (int i = 0; i < BOARD_SIZE; i++)
			if (board[0][i]->getState() == id && board[1][i]->getState() == id && board[2][i]->getState() == id)
			{
				this->gameState = currentPlayer->getID();
				return;
			}
		if (board[0][0]->getState() == id && board[1][1]->getState() == id && board[2][2]->getState() == id)
		{
			this->gameState = currentPlayer->getID();
			return;
		}
		if (board[2][0]->getState() == id && board[1][1]->getState() == id && board[0][2]->getState() == id)
		{
			this->gameState = currentPlayer->getID();
			return;
		}
		//checking for draw
		bool draw = true;
		for (int i = 0; i < BOARD_SIZE; i++)
			for (int j = 0; j < BOARD_SIZE; j++)
				if (board[i][j]->getState() == 0)
					draw = false;
		if (draw == true)
			this->gameState = DRAW;
	}
	//chcecks winning conditions for the last modified board and enables the board(s) for next turn
	void updateSmallBoards(Position* position)
	{
		board[position->sbY][position->sbX]->checkWinningConditions(currentPlayer->getID());

		for (int i = 0; i < BOARD_SIZE; i++)
			for (int j = 0; j < BOARD_SIZE; j++)
				board[i][j]->disable();

		if (board[position->gY][position->gX]->getState() != IN_PROGRESS)
		{
			for (int i = 0; i < BOARD_SIZE; i++)
				for (int j = 0; j < BOARD_SIZE; j++)
					if (board[i][j]->getState() == IN_PROGRESS)
						board[i][j]->enable();
		}
		else board[position->gY][position->gX]->enable();
	}

	void togglePlayer()
	{
		if (this->currentPlayer == this->player1)
			this->currentPlayer = this->player2;
		else this->currentPlayer = this->player1;
	}
	
	//prints the board walls (only in first turn) and calls SmallBoard print methods
	void print()
	{
		if (doPrint == false)
			return;

		if (this->lastMove.gX == -1)
		{
			std::cout << "#########################" << std::endl;
			std::cout << "#| | | |#| | | |#| | | |#" << std::endl;
			std::cout << "#| | | |#| | | |#| | | |#" << std::endl;
			std::cout << "#| | | |#| | | |#| | | |#" << std::endl;
			std::cout << "#########################" << std::endl;
			std::cout << "#| | | |#| | | |#| | | |#" << std::endl;
			std::cout << "#| | | |#| | | |#| | | |#" << std::endl;
			std::cout << "#| | | |#| | | |#| | | |#" << std::endl;
			std::cout << "#########################" << std::endl;
			std::cout << "#| | | |#| | | |#| | | |#" << std::endl;
			std::cout << "#| | | |#| | | |#| | | |#" << std::endl;
			std::cout << "#| | | |#| | | |#| | | |#" << std::endl;
			std::cout << "#########################" << std::endl;
		}
		for (int i = 0; i < BOARD_SIZE; i++)
			for (int j = 0; j < BOARD_SIZE; j++)
				board[i][j]->print();

		lightUpLastGrid();
	}
	//used to tell the (human) player were the last move was
	void lightUpLastGrid()
	{
		//exception
		if (lastMove.gX == -1)
			return;

		DWORD dw;
		COORD here{ lastMove.sbX * 8 + 2 * lastMove.gX + 2,
					lastMove.sbY * 4 + lastMove.gY + 1 };

		HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
		WORD color = 0x8F;

		wchar_t wtext[2] = { 0x0000, 0x0000 };
		void* a = &wtext;
		*(char*)(a) = lastFigure;
		LPCWSTR ptr = wtext;

		WriteConsoleOutputAttribute(hStdOut, &color, 1, here, &dw);
		WriteConsoleOutputCharacter(hStdOut, ptr, 1, here, &dw);
	}

	void updateMove(Position* move, char direction)
	{
		board[move->sbY][move->sbX]->print();

		switch (direction)
		{
		case 'w':
			move->gY--;
			if (move->gY < 0)
			{
				move->gY = 2;
				move->sbY--;
				if (move->sbY < 0)
					move->sbY = 2;
			}
			break;

		case 's':
			move->gY++;
			if (move->gY > 2)
			{
				move->gY = 0;
				move->sbY++;
				if (move->sbY > 2)
					move->sbY = 0;
			}
			break;

		case 'a':
			move->gX--;
			if (move->gX < 0)
			{
				move->gX = 2;
				move->sbX--;
				if (move->sbX < 0)
					move->sbX = 2;
			}
			break;

		case 'd':
			move->gX++;
			if (move->gX > 2)
			{
				move->gX = 0;
				move->sbX++;
				if (move->sbX > 2)
					move->sbX = 0;
			}
			break;
		}
	}

	void updateCursor(Position* move)
	{
		board[move->sbY][move->sbX]->print();
		lightUpLastGrid();

		DWORD dw;
		COORD here{ move->sbX * 8 + 2 * move->gX + 2,
					move->sbY * 4 + move->gY + 1 };

		HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
		WORD color = 0x70;

		wchar_t wtext[2] = { 0x0000, 0x0000 };
		void* a = &wtext;
		*(char*)(a) = currentPlayer->getFigure();
		LPCWSTR ptr = wtext;

		WriteConsoleOutputAttribute(hStdOut, &color, 1, here, &dw);
		WriteConsoleOutputCharacter(hStdOut, ptr, 1, here, &dw);
	}

	bool acceptMove(Position* move)
	{
		if (board[move->sbY][move->sbX]->isEnabled())
			return board[move->sbY][move->sbX]->isFreeGrid(move);
		return false;
	}

	void simulateMove(Position* move)
	{
		makeMove(move);
		updateSmallBoards(move);
		checkWinningConditions();
		togglePlayer();
	}

	void revertMove(Position* move, int boardState)
	{
		board[move->sbY][move->sbX]->makeMove(move, 0);	//reseting grid
		board[move->sbY][move->sbX]->resetWinnerGrids();
		this->gameState = boardState;
		togglePlayer();
	}

	void copyBoardsAvailability(bool availableBoards[BOARD_SIZE][BOARD_SIZE])
	{
		for (int i = 0; i < BOARD_SIZE; i++)
			for (int j = 0; j < BOARD_SIZE; j++)
				availableBoards[i][j] = board[i][j]->isEnabled();
	}

	void setBoardsAvailability(bool availableBoards[BOARD_SIZE][BOARD_SIZE])
	{
		for (int i = 0; i < BOARD_SIZE; i++)
			for (int j = 0; j < BOARD_SIZE; j++)
				if (availableBoards[i][j] == true) board[i][j]->enable();
				else board[i][j]->disable();
	}

	void copyBoardStates(int boardStates[BOARD_SIZE][BOARD_SIZE])
	{
		for (int i = 0; i < BOARD_SIZE; i++)
			for (int j = 0; j < BOARD_SIZE; j++)
				boardStates[i][j] = board[i][j]->getState();
	}

	void setBoardStates(int boardStates[BOARD_SIZE][BOARD_SIZE])
	{
		for (int i = 0; i < BOARD_SIZE; i++)
			for (int j = 0; j < BOARD_SIZE; j++)
				board[i][j]->setState(boardStates[i][j]);
	}

	void copyLastMove(Position* position)
	{
		position->gX = lastMove.gX;
		position->gY = lastMove.gY;
		position->sbX = lastMove.sbX;
		position->sbY = lastMove.sbY;
	}

	std::list<Position> getFreeGrids()
	{
		std::list<Position> freeGrids;
		
		for (int i = 0; i < BOARD_SIZE; i++)
			for (int j = 0; j < BOARD_SIZE; j++)
				if (board[i][j]->getState() == IN_PROGRESS && board[i][j]->isEnabled())
					board[i][j]->getFreeGrids(&freeGrids);

		return freeGrids;
	}

	bool isFreeGrid(Position* position)
	{
		return this->board[position->sbY][position->sbX]->isFreeGrid(position);
	}

	int getState()
	{
		return this->gameState;
	}
	//returns small board state
	int getState(Position* position)
	{
		return board[position->sbY][position->sbX]->getState();
	}

	int getCurrentPlayerID()
	{
		return this->currentPlayer->getID();
	}

	int getOpponentPlayerID()
	{
		if (this->currentPlayer->getID() == 1)
			return 2;
		else return 1;
	}

	int getOpponentPlayerID(int playerID)
	{
		if (playerID == 1)
			return 2;
		else return 1;
	}

	void disablePrinting()
	{
		this->doPrint = false;
	}

	void enablePrinting()
	{
		this->doPrint = true;
	}

};