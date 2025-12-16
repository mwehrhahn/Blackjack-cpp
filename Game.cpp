#include "Game.h"	//Include header file with variables and all function prototypes

Game::Game(int uH, int dH) : userHand(uH), dealerHand(dH), over21(false) {}	//constructor

void Game::dealInitialHands()	//deals initial hands for user and dealer at the start of a game
{
	userHand = 0;
	dealerHand = 0;

	setUserHand();
	setUserHand();
	printUserHand();

	setDealerHand();
	printDealerHand();
}

void Game::setHand(int& hand)	//deals a card to whichever hand is in the parameter
{
	int add = (rand() % 13 + 1);
	if (add >= 11 && add <= 13)		//checks for jack, queen, or king
	{
		add = 10;					//all jacks, queens, and kings have a value of 10
	}
	else if (add == 1)				//checks for ace
	{
		if (hand + 11 < 22)			//checks if ace can be 11
		{
			add = 11;
		}
		else						//ace needs to be 1
		{
			add = 1;
		}
	}

	hand += add;					//adds card to hand
}

void Game::setUserHand()			//Function to set userHand. Used in dealInitialHands()
{
	setHand(userHand);
}

int Game::getUserHand()				//Returns user hand
{
	return userHand;
}

void Game::printUserHand()			//Displays user hand
{
	std::cout << "User: " << getUserHand() << std::endl;
}

void Game::setDealerHand()			//Function to set dealerHand. Used in dealInitialHands()
{
	setHand(dealerHand);
}

int Game::getDealerHand()			//Returns dealer hand
{
	return dealerHand;
}

void Game::printDealerHand()		//Displays dealer hand
{
	std::cout << "Dealer: " << getDealerHand() << std::endl;
}

bool Game::isOver21(int uH)			//Checks for bust
{
	if (uH > 21)
	{
		over21 = true;
	}
	else
	{
		over21 = false;
	}

	return over21;
}

bool Game::isWinner(int uH, int dH, bool bust)	//Checks if user is a winner
{
	if (bust)
	{
		return false;
	}

	else if (dH > 21)
	{
		return true;
	}
	else
	{
		return uH > dH;
	}

}

bool Game::isDraw(int uH, int dH, bool bust)	//Checks if userHand == dealerHand
{
	if (bust)
	{
		return false;
	}
	else
	{
		return uH == dH;
	}
}

void Game::newHand()							//Resets hands for a new game
{
	userHand = 0;
	dealerHand = 0;
	over21 = false;
}