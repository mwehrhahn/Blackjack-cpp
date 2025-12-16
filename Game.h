#pragma once
#include <iostream>
#include <string>

class Game
{
private:
	int userHand;
	int dealerHand;
	bool over21;

public:
	Game(int uH = 0, int dH = 0);
	void dealInitialHands();
	void setHand(int&);
	void setUserHand();
	int getUserHand();
	void printUserHand();
	void setDealerHand();
	int getDealerHand();
	void printDealerHand();
	bool isOver21(int uH);
	bool isWinner(int uH, int dH, bool bust);
	bool isDraw(int uh, int dH, bool bust);
	void newHand();
};