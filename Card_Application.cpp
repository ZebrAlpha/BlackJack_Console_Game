//============================================================================
// Name        : Card_Application.cpp
// Author      : Josiah Eakle
// Description : Application designed to create and maintain a deck of cards
//				 cards can be used to play blackjack.
//============================================================================

#include <iostream>     /* input, output from user */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

using namespace std;

const int AMOUNT_OF_CARDS = 52;

void gameMenu();
void playBlackJack();
void printHand(string *dArray, int *dOrder, int handAmt, string ownHand, bool isGameEnd);

int isPropInput(string input, int inputIndex);
int isYesNo(string input);

void initDeck(string *dArray, int *dOrder, int size_of_deck);
void shuffleDeck(string *dArray, int *dOrder, int size_of_deck);

void dealHand(string *dArray, int *dOrder, string *pHand, int *pOrder, int &deckIndex, int amt);
void addCard(string *dArray, int *dOrder, string *pHand, int *pOrder, int &deckIndex, int &handIndex);

void resetDeck(string *dArray);
void resetDeck(int *dArray);

string getWinner(int plrHandVal, int dlrHandVal, string playerName, string dlrName);

int getHandValue(int *dOrder, int handAmt);

void clearScreen();


int main() {

	/*
	 * 0-12 : Spades
	 * 13-25: Clubs
	 * 25-38: Diamonds
	 * 38-51: Hearts
	 */

	gameMenu();


}


// asks player if they would like to play, until player enters exit in main menu
void gameMenu() {

	string input;
	int inInt = 3;

	do {
		if(!inInt) cout << endl << endl << "~*~ Incorrect input, please try again... ~*~" << endl << endl << endl;

		// index 1 for isPropInput
		// only 1, and 2 are allowed
		cout << "~*~ BLACKJACK ~*~" << endl << endl;
		cout << "[1] Play Game" << endl;
		cout << "[2] Exit" << endl;;
		cout << "~*> ";
		cin >> input;
		inInt = isPropInput(input, 1);
	} while (!inInt);

	if(inInt == 1) playBlackJack();
	else if(inInt == 2) cout << "~*~ Thanks for playing Josiah's BlackJack! ~*~" << endl << endl;
	else cout << "SOMETHING WENT WRONG, GOODBYE!" << endl;

}

//where the game takes place,
//this will be a beefy function
void playBlackJack() {

	// Declare and initialize the two deck arrays
	// Two arrays - one for the card names, one for card values
	//NAME OF ARRAY RETURNS THE ADDRESS OF THE FIRST OF THE ARRAY


	string deck[AMOUNT_OF_CARDS];
	int deckOrder[AMOUNT_OF_CARDS];
	int deckIndex = 0;

	int plrCardAmt = 0;
	int plrHandVal = 0;
	int plrBank = 20;
	int plrBet;
	string plrDeck[12];
	int plrDeckOrdr[12];

	int dlrCardAmt = 0;
	int dlrHandVal = 0;
	string dlrDeck[12];
	int dlrDeckOrdr[12];

	string playerName, input;
	string dlrName = "House";
	int inInt = 5;

	clearScreen();

	cout << "~*~ BLACKJACK ~*~" << endl << endl;

	// GET PLAYER NAME
	do {
		if(inInt == -1) cout << "~*~ Incorrect input, please try again... ~*~" << endl << endl;
		cin.ignore(1000, '\n'); //IGNORES 1000 char or until a new line
		cin.clear(); //CLEARS THE ERROR FLAG STATE
		cout << "~*~ What is your name, player? *~~" << endl;
		cout << "~*> ";
		getline(cin, playerName);
		cout << endl << endl;
		cout << "~*~ Your name is " << playerName << "? (y/n) ~*~" << endl;
		cout << "~*> ";
		cin >> input;
		inInt = isYesNo(input);
		cout << endl << endl;
	} while (inInt != 1);


	bool playAgain = true;
	bool outOfMoney = false;
	bool placeBetBool = true;

	while (playAgain && !outOfMoney) {

		placeBetBool = true;
		deckIndex = 0;
		plrCardAmt = 0;
		plrHandVal = 0;
		dlrCardAmt = 0;
		dlrHandVal = 0;
		plrBet = 2;

		resetDeck(plrDeck);
		resetDeck(plrDeckOrdr);
		resetDeck(dlrDeck);
		resetDeck(dlrDeckOrdr);


		clearScreen();


		if(plrBank < 2) outOfMoney = true;

		inInt = -2;

		if (outOfMoney) placeBetBool = false;
		while(placeBetBool) {
			if(plrBet > plrBank) cout << "! YOU DON'T HAVE THAT MUCH TO BET, PLEASE TRY AGAIN... !" << endl << endl;
			else if (plrBet < 2) cout << "! The minimum bet is $2, please try again... !" << endl << endl;
			cout << "~*~ How much would you like to bet? (Min: $2) ~*~" << endl;
			cout << "BANK: $" <<plrBank << endl << endl;
			cout << "~*> $";
			cin >> input;

			inInt = isPropInput(input, 2);
			plrBet = inInt;

			if ((plrBet > 2) && (plrBet <= plrBank)) placeBetBool = false;
			else placeBetBool = true;
			clearScreen();
		}




		plrBank -= plrBet;


		// Initialize, shuffle, and deal to player and dealer
		cout << "~*~ INITIALIZING DECK ~*~ " << endl;
		initDeck(deck, deckOrder, AMOUNT_OF_CARDS);
		cout << "~*~  SHUFFLING  DECK  ~*~" << endl;
		shuffleDeck(deck, deckOrder, AMOUNT_OF_CARDS);
		cout << "~*~   DEALING CARDS   ~*~" << endl << endl;

		// Deals two cards into the dealer and player card arrays, adds 2 to the deck index twice
		dealHand(deck, deckOrder, plrDeck, plrDeckOrdr, deckIndex, 2);
		// To remember the amount of cards the player has
		plrCardAmt += 2;
		dealHand(deck, deckOrder, dlrDeck, dlrDeckOrdr, deckIndex, 2);
		dlrCardAmt += 2;

		plrHandVal = getHandValue(plrDeckOrdr, plrCardAmt);
		dlrHandVal = getHandValue(dlrDeckOrdr, dlrCardAmt);

		bool endBool = false;
		if (plrHandVal == 21 || dlrHandVal == 21) endBool = true;

		// THIS IS WHERE THE GAME TAKES PLACE
		while (!endBool && plrHandVal < 21) {


			inInt = -1;
			do {

				if(inInt == 0) cout << "~*~ Wrong input, please try again... ~*~" << endl << endl;
				printHand(dlrDeck, dlrDeckOrdr, dlrCardAmt, dlrName, endBool);
				printHand(plrDeck, plrDeckOrdr, plrCardAmt, playerName, endBool);

				cout << "~*~ What would you like to do? ~*~" << endl;
				cout << "[1] Hit " << endl;
				cout << "[2] Hold" << endl;
				cout << "~*> ";
				cin >> input;

				//0 is returned if wrong input
				inInt = isPropInput(input, 1);
			} while (!inInt);

			clearScreen();

			if(inInt == 1) {
				addCard(deck, deckOrder, plrDeck, plrDeckOrdr, deckIndex, plrCardAmt);
			} else if (inInt == 2) {
				endBool = true;
				while (dlrHandVal < 17) {
					addCard(deck, deckOrder, dlrDeck, dlrDeckOrdr, deckIndex, dlrCardAmt);
					dlrHandVal = getHandValue(dlrDeckOrdr, dlrCardAmt);
				}
			}





			plrHandVal = getHandValue(plrDeckOrdr, plrCardAmt);
			dlrHandVal = getHandValue(dlrDeckOrdr, dlrCardAmt);
			if(plrHandVal > 21) {
				endBool = true;
				while (dlrHandVal < 17) {
					addCard(deck, deckOrder, dlrDeck, dlrDeckOrdr, deckIndex, dlrCardAmt);
					dlrHandVal = getHandValue(dlrDeckOrdr, dlrCardAmt);
				}
			}




		}


		printHand(dlrDeck, dlrDeckOrdr, dlrCardAmt, dlrName, endBool);
		printHand(plrDeck, plrDeckOrdr, plrCardAmt, playerName, endBool);

		// CHECK WINNER HERE
		string winner = getWinner(plrHandVal, dlrHandVal, playerName, dlrName);
		cout << winner << endl;

		if (winner == playerName) {
			cout << "~*~ " << playerName << " Wins! ~*~" << endl << endl;
			plrBank += plrBet * 2;
		} else if (winner == dlrName) {
			cout << "~*~ " << playerName << " Lost, better luck next time! ~*~" << endl << endl;
		} else {
			cout << "~*~ TIE ~*~ " << endl << endl;
		}

		if(plrBank < 2) outOfMoney = true;

		inInt = -1;
		while (inInt == -1 && !outOfMoney){
			cout << "~*~ Would you like to play again? (y/n) ~*~" << endl;
			cout << "~*> ";
			cin >> input;
			inInt = isYesNo(input);
			if(inInt == 0) playAgain = false;
			else if(inInt == 1) playAgain = true;
		}

	}
	clearScreen();

	if (outOfMoney) cout << "~*~ YOU RAN OUT OF FUNDS, BETTER LUCK NEXT TIME... ~*~" << endl;
	else if (plrBank > 20) cout << "~*~ You won $" << (plrBank - 20) << ", great job! ~*~" << endl;
	else if (plrBank == 20) cout << "~*~ You broke even! ~*~" << endl << endl;
	else cout << "~*~ You lost " << abs(plrBank - 20) << ", better luck next time... ~*~" << endl;
	cout << "~*~ THANKS FOR PLAYING BLACKJACK! ~*~" << endl << endl;




//	for (int i = 0; i < plrCardAmt; i++) {
//		cout << plrDeck[i] << endl;
//	} 	cout << "VALUE: " << plrHandVal;




}

void resetDeck(string *dArray) {
	for(int i = 0; i<12; i++){
		dArray[i] = " ";
	}
}
void resetDeck(int *dArray) {
	for(int i = 0; i<12; i++){
		dArray[i] = -1;
	}
}




// returns the deck index, adds the prompted amt of cards into the two p arrays
void dealHand(string *dArray, int *dOrder, string *pHand, int *pOrder, int &deckIndex, int amt) {
	for (int i=0; i<amt; i++) {
		pHand[i]  = dArray[deckIndex];
		pOrder[i] = dOrder[deckIndex];
		deckIndex++;
	}
}

void addCard(string *dArray, int *dOrder, string *pHand, int *pOrder, int &deckIndex, int &handIndex) {
	pHand[handIndex] = dArray[deckIndex];
	pOrder[handIndex] = dOrder[deckIndex];
	handIndex++;
	deckIndex++;
}

int getHandValue(int *dOrder, int handAmt) {

	const int b = 21;
	int aceAmt = 0;
	int handVal = 0;
	int cardVal = 0;

	/*
	 * 0 - Ace
	 * 1 - Two
	 * 2 - Three
	 * 3 - Four
	 * 4 - Five
	 * 5 - Six
	 * 6 - Seven
	 * 7 - Eight
	 * 8 - Nine
	 * 9 - Ten
	 * 10 - Jack
	 * 11 - Queen
	 * 12 - King
	 */


	// ACE will always start as 11, once the handVal is over 21, we subtract 10 for each ace until we have not broken

	for (int i=0; i<handAmt; i++) {

		// GET THE VALUE OF EACH INDIVIDUAL CARD
		int placeholder;
		placeholder = (dOrder[i]%13);
		switch(placeholder) {
		case(0):
				cardVal = 11;
				aceAmt ++;
				break;
		case(1):
				cardVal = 2;
				break;
		case(2):
				cardVal = 3;
				break;
		case(3):
				cardVal = 4;
				break;
		case(4):
				cardVal = 5;
				break;
		case(5):
				cardVal = 6;
				break;
		case(6):
				cardVal = 7;
				break;
		case(7):
				cardVal = 8;
				break;
		case(8):
				cardVal = 9;
				break;
		case(9):
		case(10):
		case(11):
		case(12):
			cardVal = 10;
			break;
		}
		handVal += cardVal;
	}

	while((handVal > b) && aceAmt > 0) {
		handVal -= 10;
		aceAmt--;
	}

	return handVal;
}

void printHand(string *dArray, int *dOrder, int handAmt, string ownHand, bool isGameEnd) {

	int deckVal = getHandValue(dOrder, handAmt);

	cout << "~*~ " << ownHand << " ~*~" << endl;
	if(!isGameEnd && ownHand == "House") cout << "Total: [Hidden]" << endl << "~*~*~*~*~*~*~*~*~*~*~" << endl;
	else cout << "Total: " << deckVal << endl << "~*~*~*~*~*~*~*~*~*~*~" << endl;
	for(int i=0; i<handAmt; i++) {
		if(!isGameEnd && i==0 && ownHand == "House") {
			cout << "[Card Hidden]" << endl;
		} else {
		cout << dArray[i] << endl;
		}
	} cout << endl;


}


// return the name of the winner or returns tie if no winner
string getWinner(int plrHandVal, int dlrHandVal, string playerName, string dlrName) {
	if((plrHandVal > dlrHandVal) && plrHandVal <= 21) {
		return playerName;
	} else if (plrHandVal <=21 && dlrHandVal > 21) {
		return playerName;
	} else if((dlrHandVal > plrHandVal) && dlrHandVal <= 21) {
		return dlrName;
	} else if(dlrHandVal <=21 && plrHandVal > 21) {
		return dlrName;
	} else {
		return "TIE";
	}
}


//returns menu input or returns 0 if input is wrong
int isPropInput(string input, int inputIndex) {

	/*
	 * inputIndex used to choose which menu is being used
	 * 1 - first game menu: (1, 2)
	 *
	 *
	 *
	 */

	switch(inputIndex) {
	case(1):
			if(input.length() > 1) return 0;
			if(input[0] == '1') return 1;
			else if(input[0] == '2') return 2;
			else return 0;
			break;
	case(2):
			for(int i=0; i<input.length(); i++) {
				if (input[i] < 48 || input[i] > 57) return -1;
			}
			return stoi(input);
			break;
	}
	return 0;
}


// Checks if the input is one char
// If one char - checks if it is yes or no
// returns -1 if incorrect value
// returns 1 if yes
// returns 0 if no
int isYesNo(string input) {
	if(input.length() > 1) return -1;
	if     (input[0] == 'y' || input[0] == 'Y') return 1;
	else if(input[0] == 'n' || input[0] == 'N') return 0;
	else return -1;
}


// Parameters - the address of the begining of the array of cards, the size of the array of cards
// Sets strings as the names of the cards
void initDeck(string *dArray, int *dOrder, int size_of_deck) {
	string suit, value;
	int index;
	for (int i=0; i<size_of_deck; i++) {
		if     (i>=0  && i<13) suit = "Spades";
		else if(i>=13 && i<26) suit = "Clubs";
		else if(i>=26 && i<39) suit = "Diamonds";
		else if(i>=39 && i<52) suit = "Hearts";
		else                   cout << "Error in initializing deck!" << endl;
		index = i%13;
		switch(index) {
		case(0):
				value = "Ace  ";
				break;
		case(1):
				value = "Two  ";
				break;
		case(2):
				value = "Three";
				break;
		case(3):
				value = "Four ";
				break;
		case(4):
				value = "Five ";
				break;
		case(5):
				value = "Six  ";
				break;
		case(6):
				value = "Seven";
				break;
		case(7):
				value = "Eight";
				break;
		case(8):
				value = "Nine ";
				break;
		case(9):
				value = "Ten  ";
				break;
		case(10):
				value = "Jack ";
				break;
		case(11):
				value = "Queen";
				break;
		case(12):
				value = "King ";
				break;
		}
		dArray[i] = value + " of " + suit;
		dOrder[i] = i;
	}
}

// Parameters - the addresss of the beginning of the array of cards, the amount that is within the array
// For the whole array, sets one value as a new random value, replaces the new random value as the old one
// Basically exchanges two cards until the whole deck is changed
void shuffleDeck(string *dArray, int *dOrder, int size_of_deck) {

	int randNum, saveInt;
	string saveString;

	/* initialize random seed using time as the seed */
	srand (time(NULL));

	for(int i=0; i<size_of_deck; i++) {
		// randNum used to decide which card [i] will be replaced by
		/* generate secret number between 0 and 51: */
		randNum = rand() % 52;

		// set saveInt as the card being replaced
		saveInt = dOrder[i];
		saveString = dArray[i];
		// replace card with random card btween 0-51
		dOrder[i] = dOrder[randNum];
		dArray[i] = dArray[randNum];
		// set card i replaced to card i was initially
		dOrder[randNum] = saveInt;
		dArray[randNum] = saveString;

//		cout << saveString << " swapped with " << dArray[i];
//		cout << endl << endl;
	}

}



/*
 * 0  - Ace of Spades
 * 1  - Two of Spades
 * 2  - Three of Spades
 * 3  - Four of Spades
 * 4  - Five of Spades
 * 5  - Six of Spades
 * 6  - Seven of Spades
 * 7  - Eight of Spades
 * 8  - Nine of Spades
 * 9  - Ten of Spades
 * 10 - Jack of Spades
 * 11 - Queen of Spades
 * 12 - King of Spades
 *
 * 13 - Ace of Clubs
 * 14 - Two of Clubs
 * 15 - Three of Clubs
 * 16 - Four of Clubs
 * 17 - Five of Clubs
 * 18 - Six of Clubs
 * 19 - Seven of Clubs
 * 20 - Eight of Clubs
 * 21 - Nine of Clubs
 * 22 - Ten of Clubs
 * 23 - Jack of Clubs
 * 24 - Queen of Clubs
 * 25 - King of Clubs
 *
 * 26 - Ace of Diamonds
 * 27 - Two of Diamonds
 * 28 - Three of Diamonds
 * 29 - Four of Diamonds
 * 30 - Five of Diamonds
 * 31 - Six of Diamonds
 * 32 - Seven of Diamonds
 * 33 - Eight of Diamonds
 * 34 - Nine of Diamonds
 * 35 - Ten of Diamonds
 * 36 - Jack of Diamonds
 * 37 - Queen of Diamonds
 * 38 - King of Diamonds
 *
 * 39 - Ace of Hearts
 * 40 - Two of Hearts
 * 41 - Three of Hearts
 * 42 - Four of Hearts
 * 43 - Five of Hearts
 * 44 - Six of Hearts
 * 45 - Seven of Hearts
 * 46 - Eight of Hearts
 * 47 - Nine of Hearts
 * 48 - Ten of Hearts
 * 49 - Jack of Hearts
 * 50 - Queen of Hearts
 * 51 - King of Hearts
 */

void clearScreen() {
	for(int i=0; i<100; i++) cout << endl;
}
