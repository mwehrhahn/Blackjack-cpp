#include <iostream>
#include <limits>
#include <string>
#include <algorithm>
#include "Player.h"
#include "Game.h"
#include "Input.h"

// ---------------- UI helpers ----------------

static void printWelcome() {
    std::cout << "Welcome to the Wehrhahn Casino Blackjack Table\n"
              << "The table minimum is $5 with no maximum\n"
              << "Dealer hits on all 16s and stands on all 17s\n"
              << "Blackjack pays out 3:2\n";
}

static void printOptions() {
    std::cout << "\nWhat would you like to do?\n"
              << "1. Place a bet\n"
              << "2. Cash Out\n"
              << "3. Check Balance\n";
}

// --------------- Input helpers ---------------

static void checkEnoughBalanceOrExit(Player& p) {
    if (p.getBalance() >= 5.0) return;

    std::cout << "\nYou are out of money.\n";
    int choice = readIntInRange("Add more funds? 1) Yes  2) No: ", 1, 2);

    if (choice == 1) {
        double add = readDoubleMin("Amount to add: $", 1.0);
        p.setBalance(p.getBalance() + add);
        std::cout << "New balance: $" << p.getBalance() << "\n";
        return;
    }

    std::cout << "Thanks for playing.\n";
    std::exit(0);
}

// --------------- Hand flow helpers ---------------

static int promptAction(int turn) {
    if (turn == 0) {
        return readIntInRange("Choose: 1) Hit  2) Stand  3) Double Down: ", 1, 3);
    }
    return readIntInRange("Choose: 1) Hit  2) Stand: ", 1, 2);
}

static void payoutWin(Player& p) {
    p.setBalance(p.getBalance() + p.getBet() * 2); // return bet + winnings
}

static void payoutPush(Player& p) {
    p.setBalance(p.getBalance() + p.getBet()); // return bet only
}

// Reveal dealer, hit to 17, settle outcome. Returns after resetting hand.
static void resolveDealerAndSettle(Game& game, Player& player) {
    game.printDealerHand(false);
    while (game.getDealerHand() < 17) {
        game.setDealerHand();
        game.printDealerHand(false);
    }

    const int u = game.getUserHand();
    const int d = game.getDealerHand();

    if (game.isOver21(d)) {
        std::cout << "Dealer bust! You win!\n";
        payoutWin(player);
    } else if (game.isWinner(u, d, false)) {
        std::cout << "You win!\n";
        payoutWin(player);
    } else if (game.isDraw(u, d, false)) {
        std::cout << "Push.\n";
        payoutPush(player);
    } else {
        std::cout << "You lose.\n";
    }

    game.newHand();
}

// Returns true if hand ended immediately (dealer blackjack).
static bool offerInsurance(Game& game, Player& player) {
    if (!game.dealerShowsAce()) return false;

    double insuranceBet = 0.0;

    int ins = readIntInRange("Dealer shows an Ace. Buy insurance?\n1) Yes  2) No: ", 1, 2);
    if (ins == 1) {
        double maxIns = std::min(player.getBet() / 2.0, player.getBalance());
        insuranceBet = readDoubleMin("Insurance amount (0 to half bet): $", 0.0);

        while (insuranceBet > maxIns) {
            std::cout << "Max insurance is $" << maxIns << "\n";
            insuranceBet = readDoubleMin("Insurance amount: $", 0.0);
        }

        player.setBalance(player.getBalance() - insuranceBet);
    }

    if (game.dealerHasBlackjack()) {
        game.printDealerHand(false);
        std::cout << "Dealer has blackjack.\n";

        if (insuranceBet > 0.0) {
            std::cout << "Insurance pays 2:1.\n";
            player.setBalance(player.getBalance() + insuranceBet * 3); // return + profit
        }

        game.newHand();
        return true;
    }

    return false;
}

static void playHand(Game& game, Player& player) {
    checkEnoughBalanceOrExit(player);

    double bet = readDoubleMin("Enter your bet: $", 5.0);
    if (player.getBalance() < bet) {
        std::cout << "Insufficient balance.\n";
        return;
    }

    player.setBet(bet);
    player.setBalance(player.getBalance() - bet);

    game.dealInitialHands();
    game.printUserHand();
    game.printDealerHand(true);

    // Insurance + dealer blackjack early exit
    if (offerInsurance(game, player)) return;

    // Player blackjack
    if (game.getUserHand() == 21) {
        std::cout << "Blackjack! You win!\n";
        player.setBalance(player.getBalance() + player.getBet() * 2.5);
        game.newHand();
        return;
    }

    // Player turn
    int turn = 0;
    while (!game.isOver21(game.getUserHand())) {
        int action = promptAction(turn);

        if (action == 1) { // Hit
            game.hitUser();
            game.printUserHand();
            game.printDealerHand(true);
            turn++;
            continue;
        }

        if (action == 2) { // Stand
            resolveDealerAndSettle(game, player);
            return;
        }

        // Double down
        if (turn != 0) {
            std::cout << "You can only double down on your first move.\n";
            continue;
        }

        if (player.getBalance() < player.getBet()) {
            std::cout << "Cannot double down (insufficient balance).\n";
            continue;
        }

        player.setBalance(player.getBalance() - player.getBet());
        player.setBet(player.getBet() * 2);

        game.hitUser();
        game.printUserHand();

        if (game.isOver21(game.getUserHand())) {
            std::cout << "Bust.\n";
            game.newHand();
            return;
        }

        resolveDealerAndSettle(game, player);
        return;
    }

    // If we exit loop, player busted
    std::cout << "Bust! Over 21 sorry\n";
    game.newHand();
}

// ---------------- main ----------------

int main() {
    printWelcome();

    std::string name = readLine("\nWhat is your name: ");

    double balance = readDoubleMin("How much would you like to buy in with? $", 1.0);

    Player player(name, balance);
    Game game;

    int option = 0;
    do {
        printOptions();
        option = readIntInRange("Enter menu option (1-3): ", 1, 3);

        switch (option) {
            case 1:
                playHand(game, player);
                break;
            case 2:
                std::cout << "You cash out with $" << player.getBalance() << "\n";
                break;
            case 3:
                std::cout << "Current balance: $" << player.getBalance() << "\n";
                break;
        }

    } while (option != 2);

    return 0;
}
