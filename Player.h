#pragma once
#include <iostream>
#include <string>
using namespace std;

class Player
{
private:
	string name;
	double bet, balance;
	const double MIN_BET = 5.0;

public:
	Player(string = "Player 1", double = 100.0);
	void setBet(double);
	double getBet();
	void setBalance(double);
	double getBalance();

};


