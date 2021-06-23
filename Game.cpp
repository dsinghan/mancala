#include "Game.h"
#include <iostream>
using namespace std;

Game::Game(const Board& b, Player* south, Player* north): m_board(b)
{
	m_south = south;
	m_north = north;
	m_isGameOver = false;
	m_whoTurn = SOUTH; //south always starts the game
}

void Game::display() const
{
	cout << "~~" <<m_north->name() << "~~" << endl << "  "; //prints names and board
	for (int i = 1; i <= m_board.holes(); i++)
	{
		cout << m_board.beans(NORTH, i) << "  ";
	}
	cout << endl << m_board.beans(NORTH, 0);
	for (int i = 1; i <= m_board.holes(); i++)
	{
		cout << "   ";
	}
	cout << m_board.beans(SOUTH, 0) << endl << "  ";
	for (int i = 1; i <= m_board.holes(); i++)
	{
		cout << m_board.beans(SOUTH, i) << "  ";
	}
	cout << endl << "~~" << m_south->name() << "~~" << endl;
}

void Game::status(bool& over, bool& hasWinner, Side& winner) const
{
	if (!m_isGameOver) //game is not over
	{
		over = false;
		return;
	}
	else if (m_board.beans(NORTH, 0) == m_board.beans(SOUTH, 0))
	{
		cout << "Both players tied." << endl; //tie
		over = true;
		hasWinner = false;
		return;
	}
	else if (m_board.beans(NORTH, 0) > m_board.beans(SOUTH, 0))
	{
		cout << "The winner is " << m_north->name() << "." << endl; //north wins
		over = true;
		hasWinner = true;
		winner = NORTH;
		return;
	}
	else
	{
		cout << "The winner is " << m_south->name() << "." << endl; //south wins
		over = true;
		hasWinner = true;
		winner = SOUTH;
		return;
	}
}

bool Game::move()
{
	if (m_isGameOver)
	{
		return false;
	}
	string s;
	Side endSide;
	int endHole, choice, exit = 0;
	if (m_whoTurn == SOUTH)
	{
		while (exit == 0)
		{
			choice = m_south->chooseMove(m_board, SOUTH); //get players choice
			m_board.sow(SOUTH, choice, endSide, endHole); //sow
			if (endSide == SOUTH && endHole == 0 && m_board.beansInPlay(SOUTH) > 0 && m_board.beansInPlay(NORTH) > 0)
			{
				display();
				cout << m_south->name() << " gets another turn." << endl; //if last bean is in pot, get another turn
			}
			else if (endSide == SOUTH && endHole != 0 && m_board.beans(endSide, endHole) == 1 && m_board.beans(NORTH, endHole) > 0 && m_board.beansInPlay(SOUTH) > 0 && m_board.beansInPlay(NORTH) > 0)
			{
				m_board.moveToPot(SOUTH, endHole, SOUTH);
				m_board.moveToPot(NORTH, endHole, SOUTH);
				display();
				exit = 1; //capture
			}
			else
			{
				display();
				exit = 1; //regular move
			}
		}
		m_whoTurn = NORTH; //switch turns
	}
	else //same thing for south player's turn
	{
		while (exit == 0)
		{
			choice = m_north->chooseMove(m_board, NORTH);
			m_board.sow(NORTH, choice, endSide, endHole);
			if (endSide == NORTH && endHole == 0 && m_board.beansInPlay(SOUTH) > 0 && m_board.beansInPlay(NORTH) > 0)
			{
				display();
				cout << m_north->name() << " gets another turn." << endl;
			}
			else if (endSide == NORTH && endHole != 0 && m_board.beans(endSide, endHole) == 1 && m_board.beans(SOUTH, endHole) > 0 && m_board.beansInPlay(SOUTH) > 0 && m_board.beansInPlay(NORTH) > 0)
			{
				m_board.moveToPot(NORTH, endHole, NORTH);
				m_board.moveToPot(SOUTH, endHole, NORTH);
				display();
				exit = 1;
			}
			else
			{
				display();
				exit = 1;
			}
		}
		m_whoTurn = SOUTH;
	}
	if (m_board.beansInPlay(SOUTH) == 0 || m_board.beansInPlay(NORTH) == 0) //if game is over
	{
		if (m_board.beansInPlay(SOUTH) > 0 || m_board.beansInPlay(NORTH) > 0)
		{
			cout << "Sweeping remaining beans into respective pots." << endl; //empty the board
			for (int i = 1; i <= m_board.holes(); i++)
			{
				m_board.moveToPot(SOUTH, i, SOUTH);
				m_board.moveToPot(NORTH, i, NORTH);
			}
			display();
		}
		m_isGameOver = true; //set game to over
	}
	if (!m_south->isInteractive() && !m_north->isInteractive()) //if both players are computers let user see progress incrementally
	{
		getline(cin, s);
	}
	return true;
}

void Game::play()
{
	string s;
	bool over, hasWinner;
	Side winner;
	display();
	while (!m_isGameOver) //play while game is not over
	{
		move();
	}
	status(over, hasWinner, winner); //let game know it's over
}

int Game::beans(Side s, int hole) const
{
	return m_board.beans(s, hole);
}
