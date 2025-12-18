#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "Card.h"
#include "Deck.h"

class Game
{
private:
	Deck deck;
	std::vector<Card> userCards;
	std::vector<Card> dealerCards;
	bool over21;

	int scoreHand(const std::vector<Card>& hand) const;

public:
	Game(int uH = 0, int dH = 0);
	const std::vector<Card>& getUserCards() const;
	const std::vector<Card>& getDealerCards() const;
	void dealInitialHands();
	void hitUser();
	int getUserHand() const;
	void printUserHand() const;
	void setDealerHand();
	int getDealerHand() const;
	void printDealerHand(bool hideHoleCard = false) const;
	bool isOver21(int uH);
	bool isWinner(int uH, int dH, bool bust);
	bool isDraw(int uh, int dH, bool bust);
	bool dealerShowsAce() const;
	bool dealerHasBlackjack() const;
	void newHand();
};