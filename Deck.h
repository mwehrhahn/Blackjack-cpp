#pragma once
#include <vector>
#include <random> 
#include "Card.h"

class Deck {
public:
    Deck();
    void shuffle();
    Card draw();
    std::size_t size() const;

private:
    std::vector<Card> cards;
    std::mt19937 rng;
};