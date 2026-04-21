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

// redraws the entire UI after every action so it isn't as cluttered
void drawUI(Player& player, Deck& deck, int selected, const string& message) {
    clearScreen();
    cout << "=== Tarot Poker - Deck System ===" << endl;
    cout << endl;

    if (player.handSize() == 0) {
        cout << "Your hand is empty." << endl;
    } else {
        cout << "Your hand: ";
        for (int i = 0; i < player.handSize(); i++) {
            if (i == selected) {
                // ANSI inverse colors for highlighting
                cout << " \033[7m " << player.getHand()[i].display() << " \033[0m ";
            } else {
                cout << "  " << player.getHand()[i].display() << "  ";
            }
        }
        cout << endl;
    }

    cout << endl;
    cout << "Deck: " << deck.size() << "/" << deck.getTotal() << " cards" << endl;
    cout << endl;

    cout << "--- Controls ---" << endl;
    cout << "[<] [>]  Navigate hand" << endl;
    cout << "[1]      Draw a card" << endl;
    cout << "[2]      Discard selected card" << endl;
    cout << "[3]      Increase selected card value" << endl;
    cout << "[4]      Cycle selected card suit" << endl;
    cout << "[D]      View deck" << endl;
    cout << "[R]      End round (return cards)" << endl; //will check who wins (player v computer)
	cout << "[S]      Start game" << endl;
    cout << "[Q]      Quit" << endl;

    if (!message.empty()) {
        cout << endl << "> " << message << endl;
    }
}

void drawGame(InputParams inP) {
    clearScreen();
    cout << "=== Tarot Poker - In Game ===" << endl;
    cout << endl;
    cout << "[<] [>]  Navigate hand" << endl;
	cout << "[SPACE]  Progress game state" << endl;
    cout << "[1]      Draw a card" << endl;
    cout << "[2]      Discard selected card" << endl;
	cout << "[B]      Bet +1 chip" << endl;
	cout << "[D]      View deck" << endl;
	cout << "[Q]      Quit" << endl << endl;


    cout << "Opponent chips: " << inP.opponent.getChips() << endl << endl;

    if (inP.opponent.handSize() == 0) {
        cout << "Opponent hand is empty." << endl;
    }
    else {
        cout << "Opponent hand: \n\n";
        for (int i = 0; i < inP.opponent.handSize(); i++) {
            if (i == inP.selected) {
                // ANSI inverse colors for highlighting
                cout << " \033[7m " << inP.opponent.getHand()[i].display() << " \033[0m ";
            }
            else {
                cout << "  " << inP.opponent.getHand()[i].display() << "  ";
            }
        }
        cout << endl;
    }

    cout << endl;
	cout << "-----------------------------------------" << endl << endl;

    cout << "Your chips: " << inP.player.getChips() << endl << endl;

    if (inP.player.handSize() == 0) {
        cout << "Your hand is empty." << endl;
    }
    else {
        cout << "Your hand: \n\n";
        for (int i = 0; i < inP.player.handSize(); i++) {
            if (i == inP.selected) {
                // ANSI inverse colors for highlighting
                cout << " \033[7m " << inP.player.getHand()[i].display() << " \033[0m ";
            }
            else {
                cout << "  " << inP.player.getHand()[i].display() << "  ";
            }
        }
        cout << endl;
    }

    cout << endl;
    cout << "=========================================" << endl << endl;
    cout << "Total pot: " << inP.pot << endl << endl;

    cout << inP.message << endl;
    cout << inP.message2 << endl;
	cout << inP.message3 << endl;
}

void drawShop(InputParams inP) {
    clearScreen();
    cout << "=== Tarot Poker - Shop ===" << endl;
    cout << endl;
    cout << "|---| Item 1 |---|     |---| Item 2 |---|     |---| Item 3 |---|" << endl;
    cout << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl;
    cout << endl;
    cout << "Press [SPACE] to return to the game and up the ante." << endl;
}

//display the player or computer that won using results from compareHands
//win (0), lose (1), draw(2)
void displayWinner(int whoWon, string& message){
    if(whoWon == 0){
		message = "Player Wins\n";
    }
    if(whoWon == 1){
        message = "Player Loses\n";
    }
    if(whoWon == 2){
		message = "Draw\n";
    }
}//end of displayWinner

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

//check if all cards share the same suit 
bool isFlush(const vector<Card>& hand) {
    string suit = hand[0].suit; //get suit

    if(hand.empty()){
        return false; //safe guard against empty hand
    }//end of if

    for (size_t i = 1; i < hand.size(); i++) {
        if (hand[i].suit != suit) {
            return false; //not a flush
        }//end of if
    }//end of for loop
    return true; 
}//end of isFlush

//check if hand forms a straight
bool isStraight(vector<int> values) {
    if(values.empty()){
        return false; //safe guard against empty hand
    }//end of if

    sort(values.begin(), values.end()); //sort in ascending order

    for (size_t i = 1; i < values.size(); i++) { //ensure values are incrementing by only 1 value
        //not the next value or dupe will break
        if ((values[i] != values[i - 1] + 1) || values[i] == values[i - 1]) {
            return false; //leave if not
        }//end of if statement
    }//end of for loop
    return true;
}//end of isStraight

//determine how good the player/opponent's hand is
//Straight Flush > 4 of a kind > Full House > Flush >
//Straight > 3 of a kind > 2 pair > pair > high card
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

//see which player (player vs computer) has the better hand
void compareHands(Player& human, Computer& enemy, string& message) {
    vector<Card> player = human.getHand();
    vector<Card> opponent = enemy.getHand();
    vector<int> playerValues;
    vector<int> enemyValues;
    int playerHandRank = getHandRank(player, playerValues);
    int enemyHandRank = getHandRank(opponent, enemyValues);
    int isWinner = 2; //default tie

    // compare rank first
    if(playerHandRank > enemyHandRank){
        isWinner = 0; //player wins
    }//end of if
    else if(playerHandRank < enemyHandRank){
        isWinner = 1; //enemy wins
    }//end of else if
    else{
        //high card to break ties
        for (int i = 0; i < (int)playerValues.size(); i++){
            if (playerValues[i] > enemyValues[i]){
                isWinner = 0;
                break;
            }//end of if
            if (playerValues[i] < enemyValues[i]){
                isWinner = 1;
                break;
            }//end of if
        }//end of for loop
    }//end of else
    displayWinner(isWinner, message); //display who the winner is
}//end of compareHands

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
    int alreadyBet = false;
    int alreadyDrew = false;
    int opponentTurnOver = false;
	bool readyForNextGameState = false;
	bool readyForShop = false;

    //intialize enemy but do not show hand
    Computer opponent; 

	InputParams inP {player, deck, selected, message, message2, message3, opponent, pot};

    drawUI(player, deck, selected, message);

    player.changeChips(100);
	opponent.changeChips(100);

    while (running) {
        int key = readKey();

        
        if (state == 0) {
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
                    cout << endl << "Press any key to return..." << endl;
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
					drawGame(inP);
                    break;
                }
                case KEY_Q: {
                    running = false;
                    break;
                }
            }

        }
		if (state == 1) {
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
                    if (deck.isEmpty()) {
                        message3 = "The deck is empty!";
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
                    }
                    drawGame(inP);
                    break;
                }
                case KEY_3: {
                    break;
                }
                case KEY_B: {
                    if (!(gameState == 2 || gameState == 4)) {
						message3 = "You can't bet right now!";
						drawGame(inP);
                        break;
                    }
                    pot++;
					betChips++;
                    player.changeChips(-1);
                    if (betChips == 1) {
                        message2 = "Added 1 chip to the pot.\n";
                    }
                    else if (betChips > 1) {
                        message2 = "Added " + to_string(betChips) + " chips to the pot.\n";
                    }
                    drawGame(inP);
                    break;
                }
                case KEY_D: {
                    showingDeck = true;
                    clearScreen();
                    deck.display();
                    cout << endl << "Press any key to return..." << endl;
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
                        message2 = "All cards returned to deck and shuffled.";
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
                //initial bet phase
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
                drawGame(inP);
				alreadyBet = false;
                alreadyDrew = true;
            }

            if (gameState == 2) {
                //1st betting phase
				readyForNextGameState = true;
                message = "Make a bet!";
                if (opponentTurnOver == false) {
                    int bet = 1 + rand() % ante;
                    for (int i = 0; i < bet; i++) {
                        opponent.changeChips(-1);
                        pot++;
                        drawGame(inP);
                        std::this_thread::sleep_for(std::chrono::milliseconds(500)); //disable if this breaks things
                    }
                    message2 = "Opponent bet " + to_string(bet) + " chips.\n";
                    opponentTurnOver = true;
                }
				drawGame(inP);
			}

            if (gameState == 3) {
                //draw/discard phase
				readyForNextGameState = true;
                message = "Draw or discard cards!";
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
				readyForNextGameState = true;
                message = "Make a bet!";
                if (opponentTurnOver == false) {
                    int bet = 1 + rand() % ante;
                    for (int i = 0; i < bet; i++) {
                        opponent.changeChips(-1);
                        pot++;
                        drawGame(inP);
                        std::this_thread::sleep_for(std::chrono::milliseconds(500)); //disable if this breaks things
                    }
                    message2 = "Opponent bet " + to_string(bet) + " chips.\n";
                    opponentTurnOver = true;
                }

				drawGame(inP);
            }

            if (gameState == 5) {
                //showdown phase
                readyForShop = true;
                if (readyForNextGameState == false) {
                    message = "Press [R] to end the round and see who wins!";
                }
				
				drawGame(inP);
            }


        }

        else if (state == 2) {
            // DOES NOT PROPERLY LOOP BACK TO THE BEGINNING OF THE ROUND
            switch (key) {
                case KEY_SPACE: {
                    state = 1;
                    gameState = 0;
                    alreadyBet = false;
                    alreadyDrew = false;
                    opponentTurnOver = false;
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
    cout << "Goodbye!" << endl;

    return 0;
}

