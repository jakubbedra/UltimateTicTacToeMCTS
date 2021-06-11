#pragma once 

#include <iostream>

#include "Board.h"
#include "Player.h"
#include "RandomBot.h"
#include "Human.h"
#include "MCTSBot.h"

#define HUMAN 0
#define RANDOM 1
#define MCTS 2

class Menu
{
private:
	int player1Type;
	int player2Type;
	int numberOfGames;
	bool showBoard;	//if set to false shows only the winner
	
	void generateBoardAndPlayers(Player** player1, Player** player2, Board** board)
	{
		switch (player1Type)
		{
		case HUMAN:
			*player1 = new Human('x');
			break;
		case RANDOM:
			*player1 = new RandomBot('x');
			break;
		case MCTS:
			*player1 = new MCTSBot('x');
			break;
		default:
			*player1 = new RandomBot('x');
			break;
		}

		switch (player2Type)
		{
		case HUMAN:
			*player2 = new Human('o');
			break;
		case RANDOM:
			*player2 = new RandomBot('o');
			break;
		case MCTS:
			*player2 = new MCTSBot('o');
			break;
		default:
			*player2 = new RandomBot('o');
			break;
		}

		*board = new Board(*player1, *player2);
		(*player1)->setBoard(*board);
		(*player2)->setBoard(*board);

		if (showBoard == false)
			(*board)->disablePrinting();
	}

public:
	Menu()
	{
		this->player1Type = HUMAN;
		this->player2Type = HUMAN;
		this->numberOfGames = 1;
		this->showBoard = true;
	}

	void start()
	{
		Player* player1 = nullptr;
		Player* player2 = nullptr;
		Board* board = nullptr;
		settings();

		int winsP1 = 0;
		int winsP2 = 0;

		for (int i = 0; i < numberOfGames; i++)
		{
			generateBoardAndPlayers(&player1, &player2, &board);

			while (board->getState() == IN_PROGRESS)
			{
				board->makeTurn();
			}

			if (board->getState() == PLAYER1)
				winsP1++;
			else if (board->getState() == PLAYER2)
				winsP2++;

			delete player1;
			delete player2;
			delete board;
		}

		std::cout << "x wins/o wins" << std::endl;
		std::cout << winsP1 << "/" << winsP2 << std::endl;
	}

	void settings()
	{
		std::cout << "Player1 (x):" << std::endl;
		std::cout << "0 - human; 1 - randomBot; 2 - MCTSBot" << std::endl;
		int choice;
		std::cin >> player1Type;

		system("cls");
		std::cout << "Player2 (o):" << std::endl;
		std::cout << "0 - human; 1 - randomBot; 2 - MCTSBot" << std::endl;
		std::cin >> player2Type;

		system("cls");
		std::cout << "Number of games to be played:" << std::endl;
		std::cin >> numberOfGames;

		system("cls");
		std::cout << "Show board? y/n" << std::endl;
		std::cin >> choice;
		if (choice == 'y')
			showBoard = true;
		else if (choice == 'n')
			showBoard = false;
		else
			showBoard = true;

		system("cls");
	}

};