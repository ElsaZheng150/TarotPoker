#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <map>
#include <algorithm>
#include <chrono>
#include <thread>
#include "Deck.h"
#include "Player.h"
#include "Computer.h"
#include "Input.h"
using namespace std;

// struct to pass all parameters into drawGame so it isn't cluttered
struct InputParams {
    Player& player;
    Deck& deck;
    int& selected;
    string& message;
	string& message2;
	string& message3;
    Computer& opponent;
    int& pot; 
};

/*
    Function: drawUI
    Purpose: redraws the entire UI after every action so it isn't as cluttered
    Parameters: the player, the deck, things to display
    Return: None, print to console
*/
void drawUI(Player& player, Deck& deck, int selected, const string& message) {
    clearScreen();
    std::cout << "=== Tarot Poker - Deck System ===" << endl;
    std::cout << endl;

    //display what cards the player has if any
    if (player.handSize() == 0) {
        std::cout << "Your hand is empty." << endl;
    } else {
        std::cout << "Your hand: ";
		for (int i = 0; i < player.handSize(); i++) { //loop through hand and display cards
            if (i == selected) {
                // ANSI inverse colors for highlighting
                std::cout << " \033[7m " << player.getHand()[i].display() << " \033[0m ";
            } else {
                std::cout << "  " << player.getHand()[i].display() << "  ";
            }
        }
        std::cout << endl;
    }

	//menu to let user know what actions they can perform to play the game
    std::cout << endl;
    std::cout << "Deck: " << deck.size() << "/" << deck.getTotal() << " cards" << endl;
    std::cout << endl;

    std::cout << "--- Controls ---" << endl;
    std::cout << "[<] [>]  Navigate hand" << endl;
    std::cout << "[1]      Draw a card" << endl;
    std::cout << "[2]      Discard selected card" << endl;
    std::cout << "[3]      Increase selected card value" << endl;
    std::cout << "[4]      Cycle selected card suit" << endl;
    std::cout << "[D]      View deck" << endl;
    std::cout << "[R]      End round (return cards)" << endl; //will check who wins (player v computer)
	std::cout << "[S]      Start game" << endl;
    std::cout << "[Q]      Quit" << endl;

    if (!message.empty()) {
        std::cout << endl << "> " << message << endl;
    }
}

/*
    Function: drawGame
    Purpose: Output the game to the player on the console
    Parameters: inP (the Input Parameters)
    Return: None, output to console
*/
void drawGame(const InputParams& inP) {
	clearScreen(); //clear any previous text
	//print menu and instrictions for the game
    std::cout << "=== Tarot Poker - In Game ===" << endl;
    std::cout << endl;
    std::cout << "[<] [>]  Navigate hand" << endl;
	std::cout << "[SPACE]  Progress game state" << endl;
    std::cout << "[1]      Draw a card" << endl;
    std::cout << "[2]      Discard selected card" << endl;
	std::cout << "[B]      Bet +1 chip" << endl;
	std::cout << "[D]      View deck" << endl;
	std::cout << "[Q]      Quit" << endl << endl;

	//output opponent's status
    cout << "Opponent chips: " << inP.opponent.getCurrency() << endl << endl;

    if (inP.opponent.handSize() == 0) {
        std::cout << "Opponent hand is empty." << endl;
    }
    else {
        std::cout << "Opponent hand: \n\n";
        for (int i = 0; i < inP.opponent.handSize(); i++) {
            if (i == inP.selected) {
                // ANSI inverse colors for highlighting
                std::cout << " \033[7m " << inP.opponent.getHand()[i].display() << " \033[0m ";
				// 'hide' code: Replaces the oppenet's hand with ?? to hide the suit and value from the player (highlighted)
				// For testing purposes, the opponent's hand will be shown but the code is here to make the switch
                // std::cout << " \033[7m \xe2\x81\x87  \033[0m ";
                
            }
            else {
                std::cout << "  " << inP.opponent.getHand()[i].display() << "  ";
				// Replaces the oppenet's hand with ?? to hide the suit and value from the player
				//std::cout << " \xe2\x81\x87  ";
            }
        }
        std::cout << endl;
    }

    //formatting
    std::cout << endl;
	std::cout << "-----------------------------------------" << endl << endl;

    std::cout << "Your chips: " << inP.player.getCurrency() << endl << endl;

    //show player their cards if there are cards
    if (inP.player.handSize() == 0) {
        std::cout << "Your hand is empty." << endl;
    }
    else {
        std::cout << "Your hand: \n\n";
        for (int i = 0; i < inP.player.handSize(); i++) {
            if (i == inP.selected) {
                // ANSI inverse colors for highlighting
                std::cout << " \033[7m " << inP.player.getHand()[i].display() << " \033[0m ";
            }
            else {
                std::cout << "  " << inP.player.getHand()[i].display() << "  ";
            }
        }
        std::cout << endl;
    }

    std::cout << endl;
    std::cout << "=========================================" << endl << endl;
    std::cout << "Total pot: " << inP.pot << endl << endl;

    std::cout << inP.message << endl;
    std::cout << inP.message2 << endl;
	std::cout << inP.message3 << endl;
}

/*
    Function: drawShop
    Purpose: output the shop for users to buy tarot cards
    Parameters: standard ones listed at top
    Return: None, print to console
*/
void drawShop(InputParams inP) {
    clearScreen();
    std::cout << "=== Tarot Poker - Shop ===" << endl;
    std::cout << endl;
    std::cout << "|---| Item 1 |---|     |---| Item 2 |---|     |---| Item 3 |---|" << endl;
    std::cout << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl;
    std::cout << endl;
    std::cout << "Press [SPACE] to return to the game and up the ante." << endl;
}

/*
    Function: displayWinner
    win (0), lose (1), draw(2)
    Purpose: display the player or computer that won using results from compareHands
    Parameters: number corresponding to player win, tie, player loss, what to show the player
    Return: None
*/
void displayWinner(int whoWon, string& message){
    //set message based on win number
    if(whoWon == 0){
		message = "Player Wins\n";
    }//end of if
    if(whoWon == 1){
        message = "Player Loses\n";
    }//end of if
    if(whoWon == 2){
		message = "Draw\n";
    }//end of if
}//end of displayWinner

//handle the playerBet
void handlePlayerBet(Player& player, int&pot, string& message, int ALL = 777){

    std::cout << "Place CUSTOM BET or ALL IN [777]: ";
    int numBetMoney;
    if (!(cin >> numBetMoney)) {//take in user input
        cin.clear();
        cin.ignore(1000, '\n'); //clear input buffer if invalid input
        numBetMoney = 0; //default to 0 if invalid input
    }
    else {
        //valid bet is greater than 0 and less than or equal to all currency (can't go over)
        //default all in if no valid bet is placed
        if (numBetMoney > 0 && numBetMoney <= player.getCurrency()) {
            player.setBetAmount(numBetMoney); //set amount if valid
            player.setCurrency(player.getCurrency() - numBetMoney); //subtract from player's currency
            pot += player.getBetAmount(); //add to pot
        } //end of if
        else if (numBetMoney == ALL) {
            player.setBetAmount(player.getCurrency()); //set all in if 777 is entered
            player.setCurrency(0); //subtract all from player's currency
            pot += player.getBetAmount(); //add all in to pot
        }
        else {
            //invalid bet, default 0
            player.setBetAmount(0);
            message = "No chips added to the pot.";
        }
	}//end of else

}//end of handlePlayerBet

/*
Keep in case do different method later but decided to go with easier code
//sort hand by ascending order using bubble sort
//pass by reference to modify vector
void sortHand(vector<int>& hand){
    for(size_t i=0; i<hand.size()-1; i++){
        for(size_t j=0; j<hand.size()-i-1; j++){
            //swap if in wrong order
            if(hand[j] > hand[j+1]){
                int temp = hand[j+1];
                hand[j+1] = hand[j];
                hand[j] = temp;
            }//end of if
        }//end of inner for loop
    }//end of outer for loop
}//end of sortHand
*/

/*
    Function: isFlush
    Purpose: check if all cards share the same suit (flush)
    Parameters: the hand (player or enemy's)
    Return: True if the hand has a flush, False if the hand has differing suits
*/
bool isFlush(const vector<Card>& hand) {
    //do nothing if hnad is empty or does not have 5 cards
    if(hand.empty() || hand.size()!=5){
        return false; //safe guard against empty hand
    }//end of if

    string suit = hand[0].suit; //get suit

    //loop through the hand to check if suit matches 
    for(size_t i=1; i<hand.size(); i++) {
        if(hand[i].suit != suit) {
            return false; //not a flush
        }//end of if
    }//end of for loop

    return true; //default return
}//end of isFlush

/*
    Function: isStraight
    Purpose: check if hand forms a straight (values differ by 1)
    Parameters: a vector containing the strength values of cards from the hand
    Return: True if hand contains a straight, False if there is no straight
*/
bool isStraight(vector<int> values) {
    if(values.empty() || values.size()<5){
        return false; //safe guard against empty hand or any hand less than 5
    }//end of if

    sort(values.begin(), values.end()); //sort in ascending order

    for(size_t i=1; i<values.size(); i++) { //ensure values are incrementing by only 1 value
        //not the next value or dupe will break
        if (values[i] != values[i - 1] + 1) {
            return false; //leave if not
            break;
        }//end of if statement
    }//end of for loop
    return true;
}//end of isStraight

/*
    Function: getHandRank
    Purpose: determine how good the player/opponent's hand is
    Straight Flush > 4 of a kind > Full House > Flush > Straight > 3 of a kind > 2 pair > pair > high card
    Parameters: a vector containing the cards in the hand, the sorted strength values of the hand
    Return: the rank of the hand (how strong it is)
*/
int getHandRank(const vector<Card>& hand, vector<int>& sortedValues) {
    vector<int> cardValues; //numeric card values
    vector<string> suits; //the suit of the cards (for flush hands)

    //determine the numeric value of each card (how good it is)
    for(size_t i=0; i<hand.size(); i++) {
        cardValues.push_back(hand[i].numericValue);
        suits.push_back(hand[i].suit);
    }//end of for loop

    //sort the values in descending value using built in function from algorithm
    sort(cardValues.begin(), cardValues.end(), greater<int>());
    sortedValues = cardValues; //put sorted values into sortedValues vector

    //count the frequencies to determine hand type
    map<int, int> freq; //map to pair value to frequency
    for(size_t j=0; j<cardValues.size(); j++) {
        freq[cardValues[j]]++; //increment count for value at j from hand
    }//end of for loop

    bool flush = isFlush(hand); //check if it's a flush using helper
    bool straight = isStraight(cardValues); //check if it's a straight using helper
 
    int four = 0; //check for four of a kind
    int three = 0; //check for three of a kind
    int pairs = 0; //check for pairs

    //find frequencies to determine hand type
    for(auto k=freq.begin(); k!=freq.end(); k++) { //auto determine type
        if(k->second == 4){
            four = 1; //four of kind
        }//end of if
        else if(k->second == 3){
            three = 1; //three of a kind
        }//end of else if
        else if(k->second == 2){
            pairs++; //pairs exist
        }//end of else if
        else{
            continue; //keep looking/high card maybe
        }//end of else
    }//end of for loop

    //determine ranking based off of poker rules
    if(straight && flush){
        return 8; //straight flush
    }//end of if
    else if(four > 0){
        return 7; //four of a kind
    }//end of else if
    else if(three>0 && pairs==1){
        return 6; //full house
    }//end of else if
    else if(flush){
        return 5; //flush
    }//end of else if
    else if(straight){
        return 4; //straight
    }//end of else if
    else if(three > 0){
        return 3; //three of a kind but not full house
    }//end of else if
    else if(pairs == 2){
        return 2; //2 pair
    }//end of else if
    else if(pairs == 1){
        return 1; //pair
    }//end of else if
    else{
        return 0; //high card
    }//end of else
}//end of getHandRank

/*
    Function: compareHands
    Purpose: see which player (player vs computer) has the better hand
    Parameters: the player, the computer (AI enemy), and what message to print
    Return: 
*/
void compareHands(Player& human, Computer& enemy, string& message) {
    vector<Card> player = human.getHand(); //get player hand
    vector<Card> opponent = enemy.getHand(); //get computer opponent hand
    vector<int> playerValues; //numeric values of player's hand
    vector<int> enemyValues; //numeric values of enemy's hand
    //determine how good each hand is
    int playerHandRank = getHandRank(player, playerValues); 
    int enemyHandRank = getHandRank(opponent, enemyValues);
    int isWinner = 2; //default tie (no added or lost currency)

    // compare rank first
    if(playerHandRank > enemyHandRank){
        isWinner = 0; //player wins
        //add bet amount onto total currency
        human.setCurrency(human.getCurrency() + human.getBetAmount()); 
    }//end of if
    else if(playerHandRank < enemyHandRank){
        isWinner = 1; //enemy wins
        //take out bet amount during loss
        human.setCurrency(human.getCurrency() - human.getBetAmount()); 
    }//end of else if
    else{
        //high card to break ties
        //loop through hands to determine who wins
        for (int i = 0; i < (int)playerValues.size(); i++){
            if (playerValues[i] > enemyValues[i]){
                isWinner = 0; //player wins
                //add bet amount onto total currency
                human.setCurrency(human.getCurrency() + human.getBetAmount()); 
                break;
            }//end of if
            if (playerValues[i] < enemyValues[i]){
                isWinner = 1; //enemy wins
                //take out bet amount during loss
                human.setCurrency(human.getCurrency() - human.getBetAmount());
                break;
            }//end of if
        }//end of for loop
    }//end of else
    displayWinner(isWinner, message); //display who the winner is
}//end of compareHands

/*
    Function: main
    Purpose: allows users to play the game
    Parameters: None
    Return: int 0
*/
int main() {
    //set console to utf-8 for suit symbols
    system("chcp 65001 > nul"); //removed because windows os only
    srand((unsigned int)time(0));

    Deck deck;
    deck.shuffle();
    Player player; 

    int selected = 0;
    string message = "Welcome to Tarot Poker!";
    string message2 = ""; // extra message if needed
	string message3 = ""; // used for messages like "You can't bet right now" & "You can't draw right now"
    bool running = true;
    bool showingDeck = false;

    int state = 0; //0 for draw/discard functionality, 1 for the actual game, 2 for shop
	int gameState = 0; //0 for initial bet, 1 for dealing, 2 for 1st bet, 3 for draw discard, 4 for 2nd bet, 5 for showdown
    int chips = 100; 
	int betChips = 0; // tracks how many chips the player has bet in the current betting round
    int pot = 0; // total chips bet by all players
	int ante = 1; // initial bet amount 
    int alreadyBet = false; //check betting status
    int alreadyDrew = false; //check status of drawing cards
    int opponentTurnOver = false; //check what computer is doing
	bool readyForNextGameState = false; //check if game can move on
	bool readyForShop = false; //check if can use shop

    //intialize enemy but do not show hand
    Computer opponent;

    //declare parameters
	InputParams inP {player, deck, selected, message, message2, message3, opponent, pot};

    //display the game
    drawUI(player, deck, selected, message);

    player.setCurrency(100);
	opponent.setCurrency(100);

    //game to continue until we reach a stopping point
    while (running) { 
        int key = readKey(); //read in user input

		//main menu for testing deck and hand functionality (preview)
        if (running && state == 0) {
            if (showingDeck) {
                showingDeck = false;
                drawUI(player, deck, selected, message);
                continue;
            }

            switch (key) {
                case KEY_LEFT: {
                    if (selected > 0) selected--;
                    drawUI(player, deck, selected, message);
                    break;
                }
                case KEY_RIGHT: {
                    if (player.handSize() > 0 && selected < player.handSize() - 1)
                        selected++;
                    drawUI(player, deck, selected, message);
                    break;
                }
                case KEY_1: {
                    if (deck.isEmpty()) {
                        message = "The deck is empty!";
                    }
                    else {
                        Card drawn = deck.draw();
                        message = "Drew: " + drawn.display();
                        player.addCard(drawn);
                        if (player.handSize() == 1) selected = 0;
                    }
                    drawUI(player, deck, selected, message);
                    break;
                }
                case KEY_2: {
                    if (!player.hasCards()) {
                        message = "Hand is empty. Nothing to discard.";
                    }
                    else {
                        message = "Discarded: " + player.getHand()[selected].display();
                        player.discardCard(selected);
                        if (selected >= player.handSize() && selected > 0)
                            selected--;
                    }
                    drawUI(player, deck, selected, message);
                    break;
                }
                case KEY_3: {
                    if (!player.hasCards()) {
                        message = "Hand is empty.";
                    }
                    else {
                        string oldDisplay = player.getHand()[selected].display();
                        player.getHand()[selected].incrementValue();
                        message = "Changed: " + oldDisplay + " -> " + player.getHand()[selected].display();
                    }
                    drawUI(player, deck, selected, message);
                    break;
                }
                case KEY_4: {
                    if (!player.hasCards()) {
                        message = "Hand is empty.";
                    }
                    else {
                        string oldDisplay = player.getHand()[selected].display();
                        player.getHand()[selected].cycleSuit();
                        message = "Changed: " + oldDisplay + " -> " + player.getHand()[selected].display();
                    }
                    drawUI(player, deck, selected, message);
                    break;
                }
                case KEY_D: {
                    showingDeck = true;
                    clearScreen();
                    deck.display();
                    std::cout << endl << "Press any key to return..." << endl;
                    break;
                }
                case KEY_R: {
                    if (player.handSize() == 0 || opponent.handSize() == 0) {
                        message = "Both players needs cards in hand.";
                    }
                    else {
                        //player hands competes against the enemy
                        compareHands(player, opponent, message);
                        //returns all to deck
                        player.returnAllToDeck(deck);
                        deck.shuffle();
                        selected = 0;
                        message = "All cards returned to deck and shuffled.";
                    }
                    drawUI(player, deck, selected, message);
                    break;
                }
                case KEY_S: {
                    state = 1;
                    //empty player hand before starting new game
                    if (player.handSize() > 0) {
                        player.returnAllToDeck(deck);
                    }
					drawGame(inP);
                    break;
                }
                case KEY_Q: {
                    running = false;
                    break;
                }
            }

        }

        //started 5 card game w/ opponent (CPU)
		if (running && state == 1) {
            if (showingDeck) {
                showingDeck = false;
                drawGame(inP);
                continue;
            }

            message3 = "";

            switch (key) {
                case KEY_LEFT: {
                    if (selected > 0) selected--;
                    drawGame(inP);
                    break;
                }
                case KEY_RIGHT: {
                    if (player.handSize() > 0 && selected < player.handSize() - 1)
                        selected++;
                    drawGame(inP);
                    break;
                }
                case KEY_SPACE: {
                    if(gameState == 0 && !alreadyBet){ //ante is automatic, so we dont need to use SPACE to advance to ante
                        //allow player to read initial message about ante before progressing
                        drawGame(inP);
                        break;
					}
					if (readyForNextGameState == false) {
                        message3 = "You can't progress the game state right now.";
                        drawGame(inP);
                        break;
                    }
                    else {
                        gameState++;
                        readyForNextGameState = false;
						opponentTurnOver = false;
						betChips = 0;
					}
                    if (readyForShop) {
                        state = 2;
                        drawShop(inP);
                    }
                    else if (!readyForShop) {
						drawGame(inP);
                    }
                    break;
                }
                case KEY_1: {
                    if (gameState != 3) {
                        message3 = "You can't draw cards right now.";
						drawGame(inP);
						break;
                    }
					if (deck.isEmpty()) { //check if deck is empty before drawing
                        message3 = "The deck is empty!";
                    }
                    //comment out if testing requires more than 5 cards
					if (player.handSize() >= 5) { //check if hand is full before drawing
                        message3 = "Hand is full. Discard a card before drawing.";
                    }
                    else {
                        Card drawn = deck.draw();
                        message2 = "Drew: " + drawn.display();
                        player.addCard(drawn);
                        if (player.handSize() == 1) selected = 0;
                    }
                    drawGame(inP);
                    break;
                }
                case KEY_2: {
                    if (gameState != 3) {
                        message3 = "You can't discard cards right now.";
                        drawGame(inP);
                        break;
                    }
                    if (!player.hasCards()) {
                        message3 = "Hand is empty. Nothing to discard.";
                    }
                    else {
                        message2 = "Discarded: " + player.getHand()[selected].display();
                        player.discardCard(selected);
                        if (selected >= player.handSize() && selected > 0)
                            selected--;
                        //TODO: add a counter to keep track of how many cards the player has discarded (or use an existing var/func)
                        //TODO: update draw() or KEY_1 case to not allow the player to draw more than discarded
                        //TODO: limit the amount of times a player can exchange to once per round (currently they can keep discarding and drawing)

                    }
                    drawGame(inP);
                    break;
                }
                case KEY_3: {
                    break;
                }
                case KEY_D: {
                    showingDeck = true;
                    clearScreen();
                    deck.display();
                    std::cout << endl << "Press any key to return..." << endl;
                    continue;
                }
                case KEY_R: {
                    if (gameState != 5) {
                        drawGame(inP);
                        break;
                    }
                    //player hands competes against the enemy
                    if (player.handSize() == 0 || opponent.handSize() == 0) {
                        message3 = "Both players needs cards in hand.";
                    }
                    else {
                        compareHands(player, opponent, message);
                        //returns all to deck
                        player.returnAllToDeck(deck);
                        opponent.returnAllToDeck(deck);
                        deck.shuffle();
                        selected = 0;
                        message2 = "All cards returned to deck and shuffled.\nPress SPACE to enter the shop.";
						readyForNextGameState = true;
                    }
                    drawGame(inP);
                    break;
                }
                case KEY_Q: {
                    running = false;
                    break;
                }
			}

            if (gameState == 0 && alreadyBet == false) {
                //initial bet (automatic ante)
                message3 = ""; //reset message 3 (error)
                if (player.getCurrency() <= 0) { //check if player has sufficient funds to continue before ante
                    message = "You are out of funds. Get out. NOW!";
                    drawGame(inP);
                    std::this_thread::sleep_for(std::chrono::milliseconds(3000)); //pause to let player read message
                    running = false;
                    break;
                }
				if (opponent.getCurrency() <= 0) { //check if opponent has sufficient funds to continue before ante
                    message = "Opponent is out of funds. You win! Get out while you're ahead!";
                    drawGame(inP);
                    std::this_thread::sleep_for(std::chrono::milliseconds(2000)); //pause to let player read message
                    running = false;
                    break;
                }
                readyForNextGameState = true;
                pot += ante * 2;
                message = "Ante " + to_string(ante) + ": Initial bet of " + to_string(ante) + "\n";
                message += "Opponent bets 1.\nYou bet 1.\n";
                drawGame(inP);
                alreadyBet = true;
            }
            

            if (gameState == 1 && alreadyDrew == false) {
                //dealing phase
				readyForNextGameState = true;
                message = "Dealing cards...\n";
                for (int i = 0; i < 5; i++) {
                    Card drawn = deck.draw();
                    message += "Dealt: " + drawn.display() + "\n";
                    player.addCard(drawn);
                    drawGame(inP);

                    std::this_thread::sleep_for(std::chrono::milliseconds(100)); //disable if this breaks things

                    opponent.addCard(deck.draw());
                    drawGame(inP);
                    std::this_thread::sleep_for(std::chrono::milliseconds(100)); //disable if this breaks things
                }
				message = "Cards dealt!\n";
                drawGame(inP);
				alreadyBet = false;
                alreadyDrew = true;
            }

            if (gameState == 2) {
                //1st betting phase
                //opponent goes first
                readyForNextGameState = true;
                message = "Opponent is betting...";
                if (opponentTurnOver == false) {
                    int bet = 1 + rand() % ante;
                    for (int i = 0; i < bet; i++) {
                        opponent.setCurrency(opponent.getCurrency() - bet);
                        pot++;
                        drawGame(inP);
                        std::this_thread::sleep_for(std::chrono::milliseconds(500)); //disable if this breaks things
                    }
                    message = "Opponent bet " + to_string(bet) + " chips.";
                    opponentTurnOver = true;
                    drawGame(inP);
                }

				//player turn to bet; call handlePlayerBet function to take in custom bet amount or all in
                message2 = "Your turn to bet!";
				drawGame(inP);
                if (alreadyBet == false) {
                    handlePlayerBet(player, pot, message);
                    alreadyBet = true;
                    message2 = "Player bet: " + to_string(player.getBetAmount()) + " chips.\nBets closed.";
                    drawGame(inP);
                }
			}

            if (gameState == 3) {
                //draw/discard phase
				readyForNextGameState = true;
				alreadyBet = false; //reset bet state for next betting round
                message2 = "";
                message = "Draw or discard cards!";
				//TODO: make opponent's draw/discard smarter (currently random num of cards discarded/drawn)
                if (opponentTurnOver == false) {
                    int cards = rand() % 4;
                    for (int i = 0; i < cards; i++) {
                        opponent.discardCard(rand() % opponent.handSize());
						drawGame(inP);
                        std::this_thread::sleep_for(std::chrono::milliseconds(500)); //disable if this breaks things

                    }
                    for (int i = 0; i < cards; i++) {
                        opponent.addCard(deck.draw());
						drawGame(inP);
                        std::this_thread::sleep_for(std::chrono::milliseconds(500)); //disable if this breaks things
					}
					message2 = "Opponent drew and discarded " + to_string(cards) + " cards.\n";
					opponentTurnOver = true;
                }
				drawGame(inP);
            }

            if (gameState == 4) {
                //2nd betting phase
				//Opponent goes first
                //TODO: make opponent's betting smarter
                readyForNextGameState = true;
                message = "Opponent is betting...";
                if (opponentTurnOver == false) {
                    int bet = 1 + rand() % ante;
                    for (int i = 0; i < bet; i++) {
                        opponent.setCurrency(opponent.getCurrency() - bet);
                        pot++;
                        drawGame(inP);
                        std::this_thread::sleep_for(std::chrono::milliseconds(500)); //disable if this breaks things
                    }
                    message = "Opponent bet " + to_string(bet) + " chips.";
                    opponentTurnOver = true;
                    drawGame(inP);
                }

                //player turn to bet; call handlePlayerBet function to take in custom bet amount or all in
                message2 = "Your turn to bet!";
                drawGame(inP);
                if (alreadyBet == false) {
                    handlePlayerBet(player, pot, message);
                    alreadyBet = true;
                    message2 = "Player bet: " + to_string(player.getBetAmount()) + " chips.\nBets closed.";
                    drawGame(inP);
                }
            }

            if (gameState == 5) {
                //showdown phase
				//TODO: reveal opponent's hand during showdown (currently hidden by ?? if the 'hide' code is not commented out)
				//TODO: show the player's and opponent's hand rank (straight, flush, etc) during showdown
                message2 = "";
                readyForShop = true;
                if (readyForNextGameState == false) {
                    message = "Press [R] to end the round and see who wins!";
                }
				
				drawGame(inP);
            }


        }
        //shop for tarot cards - lets the player manipulate their hand or oppnent's hand in a unique way
        else if (running && state == 2) {
            //TODO: add actual shop functionality (if time permits)
            switch (key) {
                case KEY_SPACE: { //will be used to exit the shop and start the next round
                    state = 1;
                    gameState = 0; //reset game state to 0 (for new round)
					readyForShop = false; //reset shop state

                    alreadyBet = false; //reset bet state
                    alreadyDrew = false; //reset draw state
					opponentTurnOver = false; //reset opponent turn state
                    readyForNextGameState = false; //reset for next state in new round

                    //rest the message for the next round
					message = "Starting a new round! Adding to the ante...";
					message2 = "";
					message3 = "";

                    drawGame(inP);
                    break;
                }
                default: {
                    drawShop(inP);
                    break;
				}
            }

            // Possible tarots
            /*
            Strength: adds value of 1 to selected card
            The Devil: subtracts value of 1 from selected card

            The Hanged Man: force opponent to discard a card of your choice

            The Star: convert two cards in your hand to diamonds
            The Moon: convert two cards in your hand to clubs
            The Sun: convert two cards in your hand to hearts
            The World: convert two cards in your hand to spades

            Wheel of Fortune: reveal 1-5 of opponent's cards

            swap a card in your hand with a card in the opponent's hand
            convert a card in your hand to a wild card
            get 2 extra discard/draws
            */
        }
    }

    clearScreen();
    std::cout << "Goodbye!" << endl;

    return 0;
}

