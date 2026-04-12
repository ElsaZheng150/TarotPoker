#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <map>
#include <algorithm>
#include "Deck.h"
#include "Player.h"
#include "Computer.h"
#include "Input.h"
using namespace std;

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
    cout << "[Q]      Quit" << endl;

    if (!message.empty()) {
        cout << endl << "> " << message << endl;
    }
}

//display the player or computer that won using results from compareHands
//win (0), lose (1), draw(2)
void displayWinner(int whoWon){
    if(whoWon == 0){
        cout << "Player Wins" << endl;
    }
    if(whoWon == 1){
        cout << "Player Loses" << endl;
    }
    if(whoWon == 2){
        cout << "Draw" << endl;
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

bool isFlush(const vector<Card>& hand) {
    if (hand.empty()) return false;

    string suit = hand[0].suit;
    for (size_t i = 1; i < hand.size(); i++) {
        if (hand[i].suit != suit) return false;
    }
    return true;
}
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
void compareHands(Player& human, Computer& enemy) {
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
    displayWinner(isWinner); //display who the winner is
}//end of compareHands

int main() {
    //set console to utf-8 for suit symbols
    //system("chcp 65001 > nul"); removed because windows os only
    srand((unsigned int)time(0));

    Deck deck;
    deck.shuffle();
    Player player; 

    int selected = 0;
    string message = "Welcome to Tarot Poker!";
    bool running = true;
    bool showingDeck = false;

    //intialize enemy but do not show hand
    Computer opponent; 

    drawUI(player, deck, selected, message);

    while (running) {
        int key = readKey();

        
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
                } else {
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
                } else {
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
                } else {
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
                } else {
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
                //player hands competes against the enemy
                compareHands(player, opponent); 
                //returns all to deck
                player.returnAllToDeck(deck);
                deck.shuffle();
                selected = 0;
                message = "All cards returned to deck and shuffled.";
                drawUI(player, deck, selected, message);
                break;
            }
            case KEY_Q: {
                running = false;
                break;
            }
        }
    }

    clearScreen();
    cout << "Goodbye!" << endl;

    return 0;
}
