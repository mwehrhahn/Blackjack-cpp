#include <iostream>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <string>
#include "Player.h"
#include "Game.h"

// ---------------- UI helpers ----------------

void printWelcome() {
    std::cout << "Welcome to the Wehrhahn Casino Blackjack Table\n";
    std::cout << "The table minimum is $5 with no maximum\n";
    std::cout << "Dealer hits on all 16s and stands on all 17s\n";
    std::cout << "Blackjack pays out 3:2\n";
}

void printOptions() {
    std::cout << "\nWhat would you like to do?\n";
    std::cout << "1. Place a bet\n";
    std::cout << "2. Cash Out\n";
    std::cout << "3. Check Balance\n";
}

int readIntInRange(const std::string& prompt, int min, int max) {
    while (true) {
        std::cout << prompt;
        int value;
        if (std::cin >> value && value >= min && value <= max) {
            return value;
        }
        std::cout << "Invalid input.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

double readDoubleMin(const std::string& prompt, double min) {
    while (true) {
        std::cout << prompt;
        double value;
        if (std::cin >> value && value >= min) {
            return value;
        }
        std::cout << "Invalid input.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

void checkEnoughBalance(Player& p) {
    if (p.getBalance() >= 5.0) return;

    std::cout << "\nYou are out of money.\n";
    int choice = readIntInRange("Add more funds? 1) Yes  2) No: ", 1, 2);

    if (choice == 1) {
        double add = readDoubleMin("Amount to add: $", 1.0);
        p.setBalance(p.getBalance() + add);
    } else {
        std::cout << "Thanks for playing.\n";
        exit(0);
    }
}

// ---------------- Main ----------------

int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    std::string name;
    double balance, bet;

    printWelcome();
    std::cout << "\nWhat is your name: ";
    std::cin >> name;
    balance = readDoubleMin("How much would you like to buy in with? $", 1.0);

    Player player(name, balance);
    Game game;

    int option;

    do {
        printOptions();
        option = readIntInRange("Enter menu option (1-3): ", 1, 3);

        switch (option) {

        // ========================= PLAY HAND =========================
        case 1: {
            checkEnoughBalance(player);

            bet = readDoubleMin("Enter your bet: $", 5.0);
            if (player.getBalance() < bet) {
                std::cout << "Insufficient balance.\n";
                break;
            }

            player.setBet(bet);
            player.setBalance(player.getBalance() - bet);

            game.dealInitialHands();
            game.printUserHand();
            game.printDealerHand(true);

            // ---------- Insurance ----------
            double insuranceBet = 0.0;

            if (game.dealerShowsAce()) {
                int ins = readIntInRange(
                    "Dealer shows an Ace. Buy insurance?\n1) Yes  2) No: ", 1, 2);

                if (ins == 1) {
                    double maxIns = std::min(player.getBet() / 2.0, player.getBalance());
                    insuranceBet = readDoubleMin(
                        "Insurance amount (0 to half bet): $", 0.0);

                    while (insuranceBet > maxIns) {
                        std::cout << "Max insurance is $" << maxIns << "\n";
                        insuranceBet = readDoubleMin("Insurance amount: $", 0.0);
                    }
                    player.setBalance(player.getBalance() - insuranceBet);
                }

                if (game.dealerHasBlackjack()) {
                    game.printDealerHand(false);
                    std::cout << "Dealer has blackjack.\n";
                    if (insuranceBet > 0) {
                        std::cout << "Insurance pays 2:1.\n";
                        player.setBalance(player.getBalance() + insuranceBet * 3);
                    }
                    game.newHand();
                    break;
                }
            }

            if (game.getUserHand() == 21) {
                std::cout << "Blackjack! You win!\n";
                player.setBalance(player.getBalance() + player.getBet() * 2.5);
                game.newHand();
                break;
            }

            // ---------- Player Turn ----------
            bool standing = false;
            int turn = 0;

            while (!standing && !game.isOver21(game.getUserHand())) {

                int choice = (turn == 0)
                    ? readIntInRange("Choose: 1) Hit  2) Stand  3) Double Down: ", 1, 3)
                    : readIntInRange("Choose: 1) Hit  2) Stand: ", 1, 2);

                // ---- HIT ----
                if (choice == 1) {
                    game.hitUser();
                    game.printUserHand();
                    game.printDealerHand(true);
                }

                // ---- STAND ----
                else if (choice == 2) {
                    game.printDealerHand(false);
                    while (game.getDealerHand() < 17) {
                        game.setDealerHand();
                        game.printDealerHand(false);
                    }

                    if (game.isOver21(game.getDealerHand()) ||
                        game.isWinner(game.getUserHand(), game.getDealerHand(), false)) {
                        std::cout << "You win!\n";
                        player.setBalance(player.getBalance() + player.getBet() * 2);
                    } else if (game.isDraw(game.getUserHand(), game.getDealerHand(), false)) {
                        std::cout << "Push.\n";
                        player.setBalance(player.getBalance() + player.getBet());
                    } else {
                        std::cout << "You lose.\n";
                    }

                    game.newHand();
                    standing = true;
                }

                // ---- DOUBLE DOWN ----
                else {
                    if (turn != 0 || player.getBalance() < player.getBet()) {
                        std::cout << "Cannot double down.\n";
                        continue;
                    }

                    player.setBalance(player.getBalance() - player.getBet());
                    player.setBet(player.getBet() * 2);

                    game.hitUser();
                    game.printUserHand();

                    if (game.isOver21(game.getUserHand())) {
                        std::cout << "Bust.\n";
                        game.newHand();
                        break;
                    }

                    game.printDealerHand(false);
                    while (game.getDealerHand() < 17) {
                        game.setDealerHand();
                        game.printDealerHand(false);
                    }

                    if (game.isOver21(game.getDealerHand()) ||
                        game.isWinner(game.getUserHand(), game.getDealerHand(), false)) {
                        std::cout << "You win!\n";
                        player.setBalance(player.getBalance() + player.getBet() * 2);
                    } else if (game.isDraw(game.getUserHand(), game.getDealerHand(), false)) {
                        std::cout << "Push.\n";
                        player.setBalance(player.getBalance() + player.getBet());
                    } else {
                        std::cout << "You lose.\n";
                    }

                    game.newHand();
                    standing = true;
                }

                turn++;
            }

            break;
        }

        // ========================= CASH OUT =========================
        case 2:
            std::cout << "You cash out with $" << player.getBalance() << "\n";
            break;

        // ========================= BALANCE =========================
        case 3:
            std::cout << "Current balance: $" << player.getBalance() << "\n";
            break;
        }

    } while (option != 2);

    return 0;
}
