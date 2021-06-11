#pragma once

#include <chrono>

#include "Player.h"
#include "Node.h"

using namespace std::chrono;


class MCTSBot : public Player
{
private:
	long long maxMillis;

public:
	MCTSBot(char figure)
		: Player(figure)
	{
		this->maxMillis = DEFAULT_MCTS_MILLIS;
	}

	MCTSBot(char figure, long long maxMillis)
		: Player(figure)
	{
		this->maxMillis = maxMillis;
	}

	void makeMove(Position* move) override
	{
		Node root(this->board);

		long long endMillis = this->maxMillis;
		auto time = system_clock::now();
		auto since_epoch = time.time_since_epoch();
		auto millis = duration_cast<milliseconds>(since_epoch);
		long long now = millis.count();
		endMillis += now;

		Node* bestNode;

		while (now < endMillis)
		{
			bestNode = root.selection();
			bestNode->enter();
			bestNode->expansion();
			bestNode = bestNode->getRandomChild();
			int winner = bestNode->simulation();
			bestNode->leave();
			bestNode->backpropagation(winner);

			time = system_clock::now();
			since_epoch = time.time_since_epoch();
			millis = duration_cast<milliseconds>(since_epoch);
			now = millis.count();
		}

		bestNode = root.getBestChild();

		bestNode->copyMove(move);
	}

};