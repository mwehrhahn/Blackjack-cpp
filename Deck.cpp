#include "Deck.h"
#include <algorithm>
#include <stdexcept>

Deck::Deck() : rng(std::random_device{}()) {
    cards.reserve(52);
    for (int s = 0; s < 4; s++) {
        for (int r = 1; r <= 13; r++) {
            cards.push_back(Card{static_cast<Suit>(s), r});
        }
    }
    shuffle();
}

void Deck::shuffle() {
    std::shuffle(cards.begin(), cards.end(), rng);
}

Card Deck::draw() {
    if (cards.empty()) {
        *this = Deck();
    }
    Card c = cards.back();
    cards.pop_back();
    return c;
}

std::size_t Deck::size() const {
    return cards.size();
}