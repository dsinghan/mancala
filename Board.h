#ifndef BOARD_H
#define BOARD_H

#include "Side.h"
#include <vector>

class Board
{
public:
	Board(int nHoles, int nInitialBeansPerHole);
	Board(const Board &oldBoard);
	int holes() const;
	int beans(Side s, int hole) const;
	int beansInPlay(Side s) const;
	int totalBeans() const;
	bool sow(Side s, int hole, Side& endSide, int& endHole);
	bool moveToPot(Side s, int hole, Side potOwner);
	bool setBeans(Side s, int hole, int beans);
private:
	int m_holes, m_beans, southPot, northPot;
	std::vector<int> m_northHoles, m_southHoles;
};

#endif
