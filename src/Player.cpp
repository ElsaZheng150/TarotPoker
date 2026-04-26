#include "Player.h"
#include "Card.h"
#include <vector>
#include <iostream>
using namespace std;

/*
    Function: addCard
    Purpose: add a card into the player's hand
    Parameter(s): the card to add
    Return: None
*/
void Player::addCard(Card card) {
    hand.push_back(card); //add card to end of hand
}

/*
    Function: discardCard
    Purpose: moves card to discard pile instead so it isn't just deleted
    Parameter(s): where the card to delete is
    Return: None
*/
void Player::discardCard(int index) {
    discardPile.push_back(hand[index]); //get card from hand and add it to discard pile
    hand.erase(hand.begin() + index); //remove card from hand and fill in any gaps
}

/*
    Function: changeCardValue
    Purpose: change what the card is (e.g., J->Q)
    Parameter(s): where the card to change is, the card to change to, new power/value of the card
    Return: None
*/
void Player::changeCardValue(int index, const string& newValue, int newNumeric) {
    //set the new value of the card and its strength
    hand[index].value = newValue; 
    hand[index].numericValue = newNumeric;
}

/*
    Function: changeCardSuit
    Purpose: alter the suit of the card
    Parameter(s): where the card to change is, the new suit
    Return: None
*/
void Player::changeCardSuit(int index, const string& newSuit) {
    hand[index].suit = newSuit; //set desired suit
}

/*
    Function: displayHand
    Purpose: show the player's hand
    Parameter(s): None
    Return: None, print to console
*/
void Player::displayHand() const {
    //input validation, ensure user has cards
    if (hand.empty()) { 
        cout << "Your hand is empty." << endl;
        return;
    }
    //display what cards the user has
    cout << "Your hand: ";
    //loops through hand and outputs cards
    for (int i = 0; i < (int)hand.size(); i++) {
        cout << "[" << i + 1 << "] " << hand[i].display() << "  ";
    }
    cout << endl;
}

/*
    Function: returnAllToDeck
    Purpose: send all cards in hand back to the deck
    Parameter(s): the deck to send cards back to
    Return: None
*/
void Player::returnAllToDeck(Deck& deck) {
    deck.returnCards(hand); //sends card user has back 
    deck.returnCards(discardPile); //sends card discarded back as well
}

/*
    Function: handSize
    Purpose: find how many cards the players has
    Parameter(s): None
    Return: amount of cards the player has
*/
int Player::handSize() const {
    return (int)hand.size();
}

/*
    Function: hasCards
    Purpose: check to see if player has cards
    Parameter(s): None
    Return: True if player has cards and False if player has no cards
*/
bool Player::hasCards() const {
    return !hand.empty(); 
}

/*
    Function: getHand
    Purpose: Get information on what cards the player has
    Parameter(s): None
    Return: player hand (vector)
*/
vector<Card>& Player::getHand() {
    return hand;
}

/*
    Function: getDiscardPile
    Purpose: get info on what cards have been discarded/swapped out
    Parameter(s): None
    Return: the discarded cards (vector)
*/
vector<Card>& Player::getDiscardPile() {
    return discardPile;
}

/*
    Function: getTokens
    Purpose: tell other functions/players how many shop tokens they have
    Parameter(s): None
    Return: the amount of tokens player has
*/
int Player::getTokens() const {
    return tokens;
}

/*
    Function: changeTokens
    Purpose: increase/decrease the amount of tokens the player has
    Parameter(s): amount of tokens to be added
    Return: the amount of tokens the player now has after update
*/
int Player::changeTokens(int amount) {
    tokens += amount; //increase tokens by amount
    return tokens;
}

/*
    Function: getCurrency
    Purpose: find out how many chips the player has (not for the tarot shop)
    Parameter(s): None
    Return: how many chips the player has
*/
int Player::getCurrency(){
    return currency;
}//end of getCurrency


/*
    Function: setCurrency
    Purpose: increase/decrease how many chips the player has
    Parameter(s): amount to be added
    Return: None
*/
void Player::setCurrency(int money){
    currency += money; //increase chips by x amount of money
}//end of setCurrency


/*  
    Function: getBetAmount
    Purpose: remind player how much they bet or for calculations
    Parameter(s): None
    Return: how much the player bet
*/
int Player::getBetAmount(){
    return betAmount;
}//end of getBetAmount

/*
    Function: setBetAmount
    Purpose: bet money for the game
    Parameter(s): how much currency the player wants to bet
    Return: None
*/
void Player::setBetAmount(int amount){
    betAmount = amount; //set new bet amount
}//end of setBetAmount
