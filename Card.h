#pragma once
#include <string>

enum class Suit { Hearts, Diamonds, Clubs, Spades };

struct Card {
    Suit suit;
    int rank; // 1=Ace, 2-10, 11=Jack, 12=Queen, 13=King

    int baseValue() const {
        if (rank ==1) return 11;    // Ace initially counts as 11
        if (rank >= 11) return 10;  // Face cards
        return rank;                // 2-10
    }

    std::string toString() const {
        static const char* suitNames[] = {"Hearts", "Diamonds", "Clubs", "Spades"};
        std::string r;
        switch (rank) {
            case 1: r = "A"; break;
            case 11: r = "J"; break;
            case 12: r = "Q"; break;
            case 13: r = "K"; break;
            default: r = std::to_string(rank); break;
        }
        return r + " of " + suitNames[static_cast<int>(suit)];
    }
};