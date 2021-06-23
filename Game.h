#ifndef GAME_H
#define GAME_H

#include "Side.h"
#include "Board.h"
#include "Player.h"

class Game
{
public:
	Game(const Board& b, Player* south, Player* north);
	void display() const;
	void status(bool& over, bool& hasWinner, Side& winner) const;
	bool move();
	void play();
	int beans(Side s, int hole) const;
private:
	Board m_board;
	Player* m_south;
	Player* m_north;
	bool m_isGameOver;
	Side m_whoTurn;
};

#endif
