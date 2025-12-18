# C++ Blackjack (Console)

A simple console-based Blackjack game written in C++. This project focuses on clean OOP structure (Player + Game), basic input validation, and core Blackjack gameplay flow (betting, hit, stand, double down).

## Features
- Place bets (table minimum: $5)
- Hit / Stand / Double Down (double down only on first move)
- Dealer hits on 16 and stands on 17
- Blackjack payout: 3:2
- Balance tracking + optional buy-in when you run out of money
- Randomized card dealing (simplified)

## Rules / Simplifications
This is a simplified Blackjack implementation:
- Hands are tracked as *totals only* (no individual card list)
- No insurance, no splits
- Deck is infinite (cards are generated randomly; no card removal)
- Dealer starts with one visible total in this version

## Project Structure
- `main.cpp` – UI + game loop (menus, betting, round control)
- `Player.h/.cpp` – Player balance and bet management
- `Game.h/.cpp` – Hand totals, dealing logic, win/draw/bust checks

## Build & Run

### macOS / Linux (g++)
```bash
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp Game.cpp Player.cpp Deck.cpp Input.cpp -o blackjack

./blackjack

### Windows (minGW)
```bash
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp Game.cpp Player.cpp Deck.cpp Input.cpp -o blackjack.exe
blackjack.exe



How to Play

Enter your name and buy-in amount.

Choose:

1 Place a bet (starts a hand)

2 Cash out (exit)

3 Check balance

During a hand choose:

1 Hit

2 Stand

3 Double down (first decision only, requires enough balance)