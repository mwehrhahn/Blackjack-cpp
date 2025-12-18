#include "Game.h"	//Include header file with variables and all function prototypes
#include <algorithm>

//constructor
Game::Game(int uH, int dH) : over21(false) {
	(void)uH;
	(void)dH;
}	

int Game::scoreHand(const std::vector<Card>& hand) const {
	int total = 0;
	int aces = 0;

	for (const auto& c : hand) {
		total += c.baseValue();
		if (c.rank == 1) aces++;
	}

	// Convert Aces from 11 to 1 as needed
	while (total > 21 && aces > 0) {
		total -= 10;
		aces--;
	}
	return total;
}

const std::vector<Card>& Game::getUserCards() const {
	return userCards;
}

const std::vector<Card>& Game::getDealerCards() const {
	return dealerCards;
}

void Game::dealInitialHands() {
	over21 = false;
	userCards.clear();
	dealerCards.clear();

	userCards.push_back(deck.draw());
	dealerCards.push_back(deck.draw());
	userCards.push_back(deck.draw());
	dealerCards.push_back(deck.draw());
}

void Game::hitUser() {
	userCards.push_back(deck.draw());
}

void Game::setDealerHand() {
	dealerCards.push_back(deck.draw());
}

int Game::getUserHand() const {
	return scoreHand(userCards);
}

int Game::getDealerHand() const{
	return scoreHand(dealerCards);
}

// Displays user hand
void Game::printUserHand() const {
	std::cout << "User cards: ";
	for (const auto& c : userCards) {
		std::cout << "[" << c.toString() << "] ";
	}
	std::cout << "Total: " << getUserHand() << "\n";
}

// Displays dealer hand
void Game::printDealerHand(bool hideHoleCard) const {		
	std::cout << "Dealer cards: ";

	for (std::size_t i = 0; i < dealerCards.size(); i++) {
		if (hideHoleCard && i == 1) {
			std::cout << "[Hidden] ";
		} else {
			std::cout << "[" << dealerCards[i].toString() << "] ";
		}
	}

	if (!hideHoleCard) { 
		std::cout << "Total: " << getDealerHand();
	}

	std::cout << "\n";
}

// Checks for bust
bool Game::isOver21(int uH)	{
	over21= (uH > 21);
	return over21;
}

// Checks if user is a winner
bool Game::isWinner(int uH, int dH, bool bust)
{
	if (bust) return false;
	if (dH > 21) return true;
	return uH > dH;

}

//Checks if userHand == dealerHand
bool Game::isDraw(int uH, int dH, bool bust)
{
	if (bust) return false;
	return uH == dH;
}

bool Game::dealerShowsAce() const {
	if (dealerCards.empty()) return false;
	return dealerCards[0].rank == 1; // Ace
}

bool Game::dealerHasBlackjack() const {
	if (dealerCards.size() < 2) return false;
	return scoreHand(dealerCards) == 21;
}

// Resets hands for a new game
void Game::newHand()
{
	userCards.clear();
	dealerCards.clear();
	over21 = false;
}