#pragma once

#include <list>
#include <limits>
#include <cmath>

#include "Board.h"
#include "Constants.h"


class Node
{
private:
	Node* parent;
	std::list<Node*> children;

	Board* board;

	double n;
	double w;

	int startPlayer;

	//game state in the node
	int playerID;
	Position recentlyPlaced;
	int boardGameState;
	bool availableBoards[BOARD_SIZE][BOARD_SIZE];
	int boardStates[BOARD_SIZE][BOARD_SIZE];

public:
	Node(Board* board)
	{
		this->board = board;
		board->copyLastMove(&recentlyPlaced);
		this->boardGameState = board->getState();
		this->playerID = board->getOpponentPlayerID();
		this->startPlayer = playerID;
		this->n = 0.0;
		this->w = 0.0;
		this->parent = nullptr;
		board->copyBoardsAvailability(availableBoards);
		board->copyBoardStates(boardStates);
	}

	Node(Board* board, Node* parent, Position* recentlyPlaced)
	{
		this->board = board;
		this->boardGameState = board->getState();
		this->recentlyPlaced.sbX = recentlyPlaced->sbX;
		this->recentlyPlaced.sbY = recentlyPlaced->sbY;
		this->recentlyPlaced.gX = recentlyPlaced->gX;
		this->recentlyPlaced.gY = recentlyPlaced->gY;
		this->n = 0.0;
		this->w = 0.0;
		this->parent = parent;
		this->startPlayer = parent->startPlayer;
		this->playerID = board->getOpponentPlayerID();
		board->copyBoardsAvailability(availableBoards);
		board->copyBoardStates(boardStates);
	}

	~Node()
	{
		for (Node* c : this->children)
			delete c;
	}

	Node* selection()
	{
		if (this->children.size() == 0) return this;
		else return this->getBestUCTChild()->selection();
	}

	void expansion()
	{
		std::list<Position> childPositions;
		childPositions = board->getFreeGrids();

		for (Position p : childPositions)
		{
			board->simulateMove(&p);
			if (board->getState() == startPlayer)
				this->w = -DBL_MAX;
			this->children.push_back(new Node(this->board, this, &p));
			board->revertMove(&p, this->boardGameState);
			board->setBoardsAvailability(this->availableBoards);
			board->setBoardStates(boardStates);
		}
		if (this->w == -DBL_MAX)
		{
			for (Node* n : this->children)
				delete n;
			while (children.empty() == false)
				children.pop_front();
		}
	}

	int simulation()
	{
		std::list<Position> randomMoves;
		std::list<int> boardStates;
		std::list<int> smallBoardStates;
		while (board->getState() == IN_PROGRESS)
		{
			std::list<Position> possibleMoves = board->getFreeGrids();
			int ind = rand() % possibleMoves.size();
			auto randomMove = possibleMoves.begin();
			std::advance(randomMove, ind);

			Position nextMove;
			nextMove.gX = randomMove->gX;
			nextMove.gY = randomMove->gY;
			nextMove.sbX = randomMove->sbX;
			nextMove.sbY = randomMove->sbY;

			boardStates.push_back(board->getState());
			smallBoardStates.push_back(board->getState(&nextMove));
			randomMoves.push_back(nextMove);
			board->simulateMove(&nextMove);
		}

		int finalState = board->getState();

		while (randomMoves.empty() == false)
		{
			board->revertMove(&randomMoves.back(), boardStates.back());
			randomMoves.pop_back();
			smallBoardStates.pop_back();
			boardStates.pop_back();
		}
		board->setBoardsAvailability(this->availableBoards);
		board->setBoardStates(this->boardStates);
		return finalState;
	}

	void backpropagation(int winner)
	{
		this->n += 1.0;
		if (winner == playerID)
			this->w += 1.0;

		if (this->parent == nullptr)
			return;
		this->parent->backpropagation(winner);
	}

	void enter()
	{
		if (this->parent != nullptr)
			this->parent->enter();
		if (this->parent == nullptr)
			return;

		board->simulateMove(&this->recentlyPlaced);
	}

	void leave()
	{
		if (this->parent == nullptr)
		{
			board->setBoardsAvailability(this->availableBoards);
			board->setBoardStates(this->boardStates);
			return;
		}
		board->revertMove(&this->recentlyPlaced, this->boardGameState);
		parent->leave();
	}

	double getUCT()
	{
		if (this->parent == nullptr)
			return w / n;
		if (this->n == 0.0)
			return std::numeric_limits<double>::infinity();
		else return w / n + sqrt(2.0) * sqrt(log(parent->n) / n);
	}
	//returns random child and simulates its move
	Node* getRandomChild()
	{
		if (children.size() == 0)
			return this;

		int ind = rand() % children.size();
		auto randomChild = children.begin();
		std::advance(randomChild, ind);

		Node* returnNode = *randomChild;
		board->simulateMove(&returnNode->recentlyPlaced);

		return returnNode;
	}
	//return child with the most simulations
	Node* getBestChild()
	{
		if (children.size() == 0)
			return this;

		double bestN = -1.0;
		Node* returnNode = nullptr;

		for (Node* c : children)
		{
			if (c->n > bestN)
			{
				bestN = c->n;
				returnNode = c;
			}
		}

		return returnNode;
	}

	Node* getBestUCTChild()
	{
		double bestUCT = -DBL_MAX;
		Node* returnNode = nullptr;

		for (Node* c : children)
		{
			if (c->getUCT() >= bestUCT)
			{
				bestUCT = c->getUCT();
				returnNode = c;
			}
		}

		return returnNode;
	}

	void copyMove(Position* move)
	{
		move->gX = recentlyPlaced.gX;
		move->gY = recentlyPlaced.gY;
		move->sbX = recentlyPlaced.sbX;
		move->sbY = recentlyPlaced.sbY;
	}

};