#pragma once
#include <iostream>
#include <string>

class Player
{
private:
	std::string name;
	double bet, balance;
	const double MIN_BET = 5.0;

public:
	Player(std::string = "Player 1", double = 100.0);
	void setBet(double);
	double getBet();
	void setBalance(double);
	double getBalance();

};


