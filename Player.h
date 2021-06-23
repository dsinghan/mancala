#ifndef PLAYER_H
#define PLAYER_H

#include "Side.h"
#include <string>
#include "Board.h"
#include <chrono>
#include <future>
#include <atomic>

class Player
{
public:
	Player(std::string name);
	Player(const Player& old);
	std::string name() const;
	virtual bool isInteractive() const;
	virtual int chooseMove(const Board& b, Side s) const = 0;
	virtual ~Player();
private:
	std::string m_name;
};

class HumanPlayer: public Player
{
public:
	HumanPlayer(std::string name);
	virtual bool isInteractive() const;
	virtual int chooseMove(const Board& b, Side s) const;
	virtual ~HumanPlayer();
};

class BadPlayer: public Player
{
public:
	BadPlayer(std::string name);
	virtual bool isInteractive() const;
	virtual int chooseMove(const Board& b, Side s) const;
	virtual ~BadPlayer();
};

class SmartPlayer: public Player
{
public:
	SmartPlayer(std::string name);
	virtual bool isInteractive() const;
	virtual int chooseMove(const Board& b, Side s) const;
	virtual ~SmartPlayer();
private:
	void chooseMoves(const Board& b, Side s, int& bestHole, int& value, int times) const;
};

class AlarmClock
{
public:
	AlarmClock(int ms)
	{
		m_timedOut = false;
		m_isRunning = true;
		m_alarmClockFuture = std::async([=]() {
			for (int k = 0; k < ms && m_isRunning; k++)
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
			if (m_isRunning)
				m_timedOut = true;
		});
	}

	~AlarmClock()
	{
		m_isRunning = false;
		m_alarmClockFuture.get();
	}

	bool timedOut() const
	{
		return m_timedOut;
	}

	AlarmClock(const AlarmClock&) = delete;
	AlarmClock& operator=(const AlarmClock&) = delete;
private:
	std::atomic<bool> m_isRunning;
	std::atomic<bool> m_timedOut;
	std::future<void> m_alarmClockFuture;
};

#endif
