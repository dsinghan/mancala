#include "Board.h"

Board::Board(int nHoles, int nInitialBeansPerHole)
{
	if (nHoles <= 0)
	{
		nHoles = 1; //lowest number of holes
	}
	if (nInitialBeansPerHole < 0)
	{
		nInitialBeansPerHole = 0; //lowest number of beans per hole
	}
	m_holes = nHoles;
	m_beans = 2 * nHoles * nInitialBeansPerHole;
	southPot = 0;
	northPot = 0;
	for (int i = 0; i < nHoles; i++)
	{
		m_northHoles.push_back(nInitialBeansPerHole); //create elements in both vectors
		m_southHoles.push_back(nInitialBeansPerHole);
	}
}

Board::Board(const Board &oldBoard)
{
	m_holes = oldBoard.m_holes; //copies all of old information into new
	m_beans = oldBoard.m_beans;
	southPot = oldBoard.southPot;
	northPot = oldBoard.northPot;
	for (int i = 0; i < m_holes; i++)
	{
		m_northHoles.push_back(oldBoard.m_northHoles[i]);
		m_southHoles.push_back(oldBoard.m_southHoles[i]);
	}
}

int Board::holes() const
{
	return m_holes;
}

int Board::beans(Side s, int hole) const
{
	if (hole == 0) //hole 0 corresponds to pots
	{
		if (s == Side::NORTH)
		{
			return northPot;
		}
		else if (s == Side::SOUTH)
		{
			return southPot;
		}
	}
	else if (hole <= m_holes)
	{
		if (s == Side::NORTH)
		{
			return m_northHoles[hole-1];
		}
		else if (s == Side::SOUTH)
		{
			return m_southHoles[hole-1];
		}
	}
	return -1; //if hole isn't a part of board
}

int Board::beansInPlay(Side s) const
{
	int count = 0;
	if (s == Side::NORTH)
	{
		for (int i = 0; i < m_holes; i++)
		{
			count += m_northHoles[i]; //counts all the beans in all holes in a side
		}
	}
	else if (s == Side::SOUTH)
	{
		for (int i = 0; i < m_holes; i++)
		{
			count += m_southHoles[i];
		}
	}
	return count;
}

int Board::totalBeans() const
{
	return m_beans;
}

bool Board::sow(Side s, int hole, Side& endSide, int& endHole)
{
	if (hole <= 0 || hole > m_holes || beans(s, hole) == 0)
	{
		return false; //hole has to be in bounds
	}
	int b = beans(s, hole);
	if (s == Side::NORTH)
	{
		m_northHoles[hole - 1] = 0;
		for (int i = hole - 1; i >= 1 && b > 0; i--) //move counterclockwise
		{
			m_northHoles[i - 1]++; //add a bean to each hole
			b--; //decrement the number of beans to sow once placed
			if (b == 0)
			{
				endSide = NORTH;
				endHole = i;
				return true;
			}
		}
		while (b > 0)
		{
			northPot++;
			b--;
			if (b == 0)
			{
				endSide = NORTH;
				endHole = 0;
				return true;
			}
			for (int i = 1; i <= m_southHoles.size() && b > 0; i++) //continue moving counterclockwise
			{
				m_southHoles[i - 1]++;
				b--;
				if (b == 0)
				{
					endSide = SOUTH;
					endHole = i;
					return true;
				}
			}
			for (int i = m_northHoles.size(); i >= 1 && b > 0; i--) //continue moving
			{
				m_northHoles[i - 1]++;
				b--;
				if (b == 0)
				{
					endSide = NORTH;
					endHole = i;
					return true;
				}
			}
		}
	}
	else if (s == Side::SOUTH) //same thing for south
	{
		m_southHoles[hole - 1] = 0;
		for (int i = hole + 1; i <= m_southHoles.size() && b > 0; i++)
		{
			m_southHoles[i - 1]++;
			b--;
			if (b == 0)
			{
				endSide = SOUTH;
				endHole = i;
				return true;
			}
		}
		while (b > 0)
		{
			southPot++;
			b--;
			if (b == 0)
			{
				endSide = SOUTH;
				endHole = 0;
				return true;
			}
			for (int i = m_northHoles.size(); i >= 1 && b > 0; i--)
			{
				m_northHoles[i - 1]++;
				b--;
				if (b == 0)
				{
					endSide = NORTH;
					endHole = i;
					return true;
				}
			}
			for (int i = 1; i <= m_southHoles.size() && b > 0; i++)
			{
				m_southHoles[i - 1]++;
				b--;
				if (b == 0)
				{
					endSide = SOUTH;
					endHole = i;
					return true;
				}
			}
		}
	}
	return false;
}

bool Board::moveToPot(Side s, int hole, Side potOwner)
{
	if (hole <= 0 || hole > m_holes)
	{
		return false; //has to be in bounds
	}
	if (s == Side::NORTH && potOwner == Side::NORTH)
	{
		northPot += m_northHoles[hole - 1]; //add beans to the pot
		m_northHoles[hole - 1] = 0; //change beans in hole to 0
		return true;
	}
	else if (s == Side::SOUTH && potOwner == Side::SOUTH)
	{
		southPot += m_southHoles[hole - 1];
		m_southHoles[hole - 1] = 0;
		return true;
	}
	else if (s == Side::NORTH && potOwner == Side::SOUTH)
	{
		southPot += m_northHoles[hole - 1];
		m_northHoles[hole - 1] = 0;
		return true;
	}
	else if (s == Side::SOUTH && potOwner == Side::NORTH)
	{
		northPot += m_southHoles[hole - 1];
		m_southHoles[hole - 1] = 0;
		return true;
	}
	return false;
}

bool Board::setBeans(Side s, int hole, int beans)
{
	if (hole <= 0 || hole > m_holes)
	{
		return false;
	}
	if (beans != this->beans(s, hole))
	{
		m_beans += (beans - this->beans(s, hole)); //add or subtract total number of beans
	}
	if (s == Side::NORTH)
	{
		m_northHoles[hole - 1] = beans; //set new number of beans at hole
		return true;
	}
	else if (s == Side::SOUTH)
	{
		m_southHoles[hole - 1] = beans;
		return true;
	}
	return false;
}
