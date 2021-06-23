#include "Player.h"
#include <string>
#include <iostream>
using namespace std;

Player::Player(std::string name)
{
	m_name = name;
}

Player::Player(const Player& old)
{
	m_name = old.m_name;
}

std::string Player::name() const
{
	return m_name;
}

bool Player::isInteractive() const
{
	return true;
}

Player::~Player()
{

}

HumanPlayer::HumanPlayer(std::string name): Player(name)
{

}

bool HumanPlayer::isInteractive() const
{
	return true;
}

int HumanPlayer::chooseMove(const Board& b, Side s) const
{
	int move = 0, exit = 0;
	while (exit == 0)
	{
		cout << "Select a hole, " << name() << ": "; //prompt the user for an int
		cin >> move;
		if (move > b.holes() || move <= 0) //has to be in bounds
		{
			cout << "The hole number must be from 1 to " << b.holes() << "." << endl;
		}
		else if (b.beans(s, move) == 0) //has to have beans in the hole
		{
			cout << "There are no beans in that hole." << endl;
		}
		else
		{
			exit = 1;
		}
	}
	return move;
}

HumanPlayer::~HumanPlayer()
{

}

BadPlayer::BadPlayer(std::string name) : Player(name)
{

}

bool BadPlayer::isInteractive() const
{
	return false;
}

int BadPlayer::chooseMove(const Board& b, Side s) const
{
	for (int i = 1; i <= b.holes(); i++)
	{
		if (b.beans(s, i) != 0)
		{
			cout << name() << " chooses hole " << i << endl;
			return i; //return first hole that has beans
		}
	}
	return -1;
}

BadPlayer::~BadPlayer()
{

}

SmartPlayer::SmartPlayer(std::string name) : Player(name)
{

}

bool SmartPlayer::isInteractive() const
{
	return false;
}

int SmartPlayer::chooseMove(const Board& b, Side s) const
{
	int bestHole = 1, value;
	chooseMoves(b, s, bestHole, value, 3); //call helper function with depth 3
	return bestHole;
}

SmartPlayer::~SmartPlayer()
{

}

void SmartPlayer::chooseMoves(const Board& b, Side s, int& bestHole, int& value, int times) const
{
	Side endSide, t;
	int endHole, h2, v2;
	if (s == NORTH) //finds opposite side
	{
		t = SOUTH;
	}
	else
	{
		t = NORTH;
	}

	if (b.beansInPlay(s) == 0) //if game is over return the outcome
	{
		if (b.beans(s, 0) > (b.beans(t, 0) + b.beansInPlay(t)))
		{
			bestHole = -1;
			value = 1000;
			return;
		}
		else if (b.beans(s, 0) < (b.beans(t, 0) + b.beansInPlay(t)))
		{
			bestHole = -1;
			value = -1000;
			return;
		}
		else
		{
			bestHole = -1;
			value = 0;
			return;
		}
	}

	if (times == 0) //if reached final depth return the value of heuristic
	{
		value = b.beans(t, 0) - b.beans(s, 0);
		return;
	}

	for (int i = 1; i <= b.holes(); i++)
	{
		if (b.beans(s, i) != 0) //finds holes with beans in it
		{
			Board c = b; //makes copy of the board
			c.sow(s, i, endSide, endHole); //sows the new board
			chooseMoves(c, t, h2, v2, times-1); //runs again with opposite side and one less depth
			if (v2 >= value) //if new value is the greatest encountered
			{
				bestHole = i; //set bestHole and value to the new move
				value = v2;
			}
		}
	}
	return;
}
© 2021 GitHub, Inc.
