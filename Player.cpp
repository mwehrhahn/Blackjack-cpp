#include "Player.h"		//Include header file with variables and all function prototypes

Player::Player(string nm, double bal)	//Constructor
{
	name = nm;
	balance = bal;
	bet = 5.0;
}

void Player::setBet(double bt)		//Sets user bet
{
	if (bt > balance)
	{
		bt = balance;
	}
	else if (bt < MIN_BET)		//If bet is under minimum, default bet of minimum is set
	{
		bet = MIN_BET;
	}
	else
	{
		bet = bt;
	}
}

double Player::getBet()				//Returns bet
{
	return bet;
}

void Player::setBalance(double bal)	//Sets user's balance
{
	balance = bal;
}

double Player::getBalance()			//Returns user's balance
{
	return balance;
}