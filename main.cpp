#include <iostream>																					//Required for cin and cout
#include <cstdlib>																					//Required for random number generation
#include <ctime>																					//Required for seeding randomness with srand()
#include <limits>
#include <string>
#include "Player.h"																					//Include header file that contains the Player class
#include "Game.h"																					//Include header file that contains the Game class
using namespace std;																				//Using standard namesapce

//Print Welcome screen explaining table rules
void printWelcome()
{
	cout << "Welcome to the Wehrhahn Casino Blackjack Table" << endl;
	cout << "The table minimum is $5 with no maximum" << endl;
	cout << "Dealer hits on all 16s and stands on all 17s" << endl;
	cout << "Blackjack pays out 3:2" << endl;
	cout << "No insurance" << endl;
}

//Print menu in between hands
void printOptions()
{
	cout << "What would you like to do?" << endl;
	cout << "1. Place a bet" << endl;
	cout << "2. Cash Out" << endl;
	cout << "3. Check Balance" << endl;
}

//Print menu during hands
void printMenu()
{
	cout << "Please make a choice:" << endl;
	cout << "1. Hit\n2. Stand\n3. Double down" << endl;
}

//Checks if user has enough funds to place a minimum bet before each hand
void checkEnoughBalance(Player& player1) 
{
	if (player1.getBalance() < 5.0)
	{
		double addBalance;
		int addChoice;
		cout << "\n\nYou are out of money!" << endl;
		cout << "Would you like to add more?" << endl;
		cout << "1. Yes\n2. No" << endl;
		cin >> addChoice;
		
		while (cin.fail() || addChoice < 1 || addChoice > 2)	//Check for valid input
		{
			cin.clear();
			cin.ignore(10000, '\n');
			cout << "Please enter 1 or 2: ";
			cin >> addChoice;
		}

		if (addChoice == 1)										//Add balance to user balance
		{
			cout << "How much would you like to add? ";
			cin >> addBalance;

			while (cin.fail() || addBalance <= 0)				//Check for valid input
			{
				cin.clear();
				cin.ignore(10000, '\n');
				cout << "Please enter a valid amount: ";
				cin >> addBalance;
			}

			player1.setBalance(player1.getBalance() + addBalance);
			cout << "Your new balance is $" << player1.getBalance() << endl;
		}
		else
		{
			cout << "Thanks for playing! Come back when you have more money." << endl;
			exit(0);	//ends the program
		}
	}

}

// Validate input for buy-in
int readIntInRange(const std::string& prompt, int min, int max) {
	while (true) {
		std::cout << prompt;
		int value;

		if (std::cin >> value && value >= min && value <= max) {
			return value;
		}

		std::cout << "invalid input. Enter a number from " << min << " to " << max << ".\n";
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
}

// Validate input for bet
double readDoubleMin(const std::string& prompt, double min) {
	while (true) {
		std::cout << prompt;
		double value;

		if (std::cin >> value && value >= min) {
			return value;
		}

		std::cout << "Invalid input. Enter a number >= " << min << ".\n";
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
}

int main()
{
	srand(time(NULL));	//Sets random seed to guarantee rand() gives different results each time

	//Declare and initialize player variables
	string name;
	double balance, bet;
	int choice, option, turn;	//Variables for menu options and to check if doubling down is possible
	bool standing = false;		//Variable to check if user chose stand option

	//Print initial message explaining the table rules and asking user to buy in
	printWelcome();
	cout << "\n\nWhat is your name: ";
	cin >> name;
	balance = readDoubleMin("How much would you like to buy in with? $", 1.0);

	//Create a new player object with name and balance
	Player player1(name, balance);
	Game game;

	//Start of do while loop that ends when user leaves table
	do {
		bool validOption = false;

		//Start of do while loop that makes sure user inputs a valid option
		do {
			option = readIntInRange("Enter menu option (1-3): ", 1, 3);
		} while (!validOption);		//End of valid option do while loop

		//Start of switch for user's option
		switch (option)
		{
		case 1:	//Set the player's bet
			standing = false;	//Player needs cards before standing
			turn = 0;			//First turn
			
			bet = readDoubleMin("Enter your bet: $", 5.0);

			if (player1.getBalance() >= bet)		//Checks if user has enough money to place bet
			{
				//Set the bet and subtract from balance
				player1.setBet(bet);
				player1.setBalance((player1.getBalance() - player1.getBet()));
			}
			else	//Player has insufficient funds for bet. Loops back to printOptions();
			{
				cout << "You do not have enough money to place that bet!" << endl;
				break;
			}

			game.dealInitialHands();	//Start of a hand

			if (game.getUserHand() == 21)		//Check for blackjack
			{
				cout << "Blackjack! Winner winner chicken dinner!" << endl;
				player1.setBalance((player1.getBalance() + (player1.getBet() * 2.5)));
				break;
			}

			//Gives user option to hit, stand, or double down
			choice = readIntInRange("Choose: 1) Hit  2) Stand. 3) Double Down: ", 1, 3);

			//Start of do while loop that handles the hand
			do
			{
				//Start of switch that handles the user's options
				switch (choice)
				{
				case 1:	// Option: Hit
					game.setUserHand();							//Give user another card

					if (game.isOver21(game.getUserHand()))		//Check for bust
					{
						cout << "Bust! Over 21 sorry" << endl;
					}
					else										//No bust
					{
						//Print updated user hand and dealer hand and give user options to hit, stand, or double down and loop back to start of switch
						game.printUserHand();
						game.printDealerHand();
						printMenu();
						cin >> choice;
					}
				
					break;
				
				case 2:	//Option: stand
					//Deal a card to dealer
					game.setDealerHand();
					game.printDealerHand();

					//Deal dealer's hand until they reach at least 17
					while (game.getDealerHand() < 17)
					{
						game.setDealerHand();
						game.printDealerHand();

					}
					
					if (game.isOver21(game.getDealerHand()))	//Check for dealer bust
					{
						cout << "Dealer bust! You win!" << endl;
						player1.setBalance(player1.getBalance() + player1.getBet() * 2);	//Payout original bet * 2
						
					}
					else if (game.isWinner(game.getUserHand(), game.getDealerHand(), game.isOver21(game.getUserHand())))	// Check if user hand > dealer hand
					{
						cout << "You win!" << endl;
						player1.setBalance(player1.getBalance() + player1.getBet() * 2);	//Payout original bet * 2
					}
					else if (game.isDraw(game.getUserHand(), game.getDealerHand(), game.isOver21(game.getUserHand())))		//Check if user hand == dealer hand
					{
						cout << "It's a draw. All bets push" << endl;
						player1.setBalance(player1.getBalance() + player1.getBet());		//Payout original bet
					}
					else																	//Dealer hand > user Hand
					{
						cout << "Sorry, you lose." << endl;

					}

					standing = true;	//Set standing option to true to exit do while loop

					//Reset game
					game.newHand();

					break;

				case 3:	//Option: Double down
					//Check if its the user's first turn, you can only double down on the first turn
					if (turn != 0)
					{
						cout << "You can only double down on your first move." << endl;
						printMenu();
						cin >> choice;
						break;
					}

					//Check if user has sufficient funds
					if (player1.getBalance() >= player1.getBet())
					{
						player1.setBalance(player1.getBalance() - player1.getBet());	//Subtract original bet again
						player1.setBet(player1.getBet() * 2);							//Double original bet
						game.setUserHand();												//Give user one more card only
						game.printUserHand();
					}
					else																//User has insufficient funds and is prompted to hit, stand, or double down again
					{
						cout << "You don't have enough balance to double down." << endl;
						printMenu();
						cin >> choice;
						break;
					}
					
					if (game.isOver21(game.getUserHand()))			//Check for busst
					{
						cout << "Bust! Over 21 sorry" << endl;

						//Reset game
						game.newHand();
						standing = true;		//Set stand to true to exit loop
						break;
					}
					else
					{
						//Deal dealer's hand until they reach at least 17
						while (game.getDealerHand() < 17)
						{
							game.setDealerHand();
							game.printDealerHand();

							if (game.isOver21(game.getDealerHand()))								//Check for dealer bust
							{
								cout << "Dealer bust! You win!" << endl;
								player1.setBalance(player1.getBalance() + player1.getBet() * 2);	//Payout original bet * 2

								//Reset game
								game.newHand();
								standing = true;													//Set stand to true to exit loop
								break;
							}
						}

						if (game.isWinner(game.getUserHand(), game.getDealerHand(), game.isOver21(game.getUserHand())))			//Check if user hand > dealer hand
						{
							cout << "You win!" << endl;
							player1.setBalance(player1.getBalance() + player1.getBet() * 2);									//Payout original bet * 2
						}
						else if (game.isDraw(game.getUserHand(), game.getDealerHand(), game.isOver21(game.getUserHand())))		//Check if user hand == dealer hand
						{
							cout << "It's a draw. All bets push" << endl;
							player1.setBalance(player1.getBalance() + player1.getBet());										//Payout original bet 
						}
						else																									//Dealer hand > user hand
						{
							cout << "Sorry, you lose." << endl;

						}

						//Reset game
						game.newHand();
						standing = true;	//Set stand to true to exit loop
					}
					break;

				default:	//Invalid option
					cout << "Please enter a valid choice!" << endl;
					break;
				}

				turn++;	//Increment turn
			} while (!game.isOver21(game.getUserHand()) && !standing);

			break;	//End of option 1

		case 2:	//Option 2: Cash out displays user's final balance and ends the program
			cout << "Thank you for playing! You cashed out with $" << player1.getBalance() << endl;
			break;

		case 3:	//Option 3: prints out user's current balance
			cout << "Your balance is $" << player1.getBalance() << endl;
			break;

		}	//End of switch

	} while (option != 2);	//End of while

	return 0;
}	//End of main