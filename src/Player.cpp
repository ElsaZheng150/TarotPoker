#include "Player.h"
#include "Card.h"
#include <iostream>
using namespace std;

void Player::addCard(Card card) {
    hand.push_back(card);
}

// moves card to discard pile instead so it isn't just deleted.
void Player::discardCard(int index) {
    discardPile.push_back(hand[index]);
    hand.erase(hand.begin() + index);
}


void Player::changeCardValue(int index, const string& newValue, int newNumeric) {
    hand[index].value = newValue;
    hand[index].numericValue = newNumeric;
}

void Player::changeCardSuit(int index, const string& newSuit) {
    hand[index].suit = newSuit;
}

void Player::displayHand() const {
    if (hand.empty()) {
        cout << "Your hand is empty." << endl;
        return;
    }
    cout << "Your hand: ";
    for (int i = 0; i < (int)hand.size(); i++) {
        cout << "[" << i + 1 << "] " << hand[i].display() << "  ";
    }
    cout << endl;
}

void Player::returnAllToDeck(Deck& deck) {
    deck.returnCards(hand);
    deck.returnCards(discardPile);
}

int Player::handSize() const {
    return (int)hand.size();
}

bool Player::hasCards() const {
    return !hand.empty();
}

vector<Card>& Player::getHand() {
    return hand;
}

vector<Card>& Player::getDiscardPile() {
    return discardPile;
}

//check who has the greater hand to find the winner
void compareHands(){
    int isWinner; //check if the player won
    bool isFlush = true; //true until proven false

    //determine if hands are the same
    //Royal flush > Straight Flush > 4 of a kind > Full House > Flush >
    //Straight > 3 of a kind > 2 pair > pair > high card


    //judge by higher order if hand types are the same
    //sort by descending order
    //compare
    //important: suits do not break ties in standard 5 card poker

    //go through all possible hands and see which one is the highest
    displayWinner(isWinner); //show who won
}//end of compareHands

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
