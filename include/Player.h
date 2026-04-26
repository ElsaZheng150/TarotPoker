#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include "Card.h"
#include "Deck.h"
using namespace std;

class Player {
private:
    vector<Card> hand; //cards the player has (5 card max)
    vector<Card> discardPile; //cards the player swaps out for cards from deck
    int tokens = 0; //secondary currency, awarded to the winner at showdown 
    int currency = 100; //amount of chips a player has to bet 
    int betAmount; //default all in

public:
    void addCard(Card card); //draw a card
    void discardCard(int index); //swap out existing card in hand
    void changeCardValue(int index, const string& newValue, int newNumeric); //change the value of a card for special tarot system
    void changeCardSuit(int index, const string& newSuit); //change what suit the card has for special tarot system
    void displayHand() const; //show player hand 
    void returnAllToDeck(Deck& deck); //discard all cards at end 
    int handSize() const; //show how many cards the player has
    bool hasCards() const; //check if player has cards
    void setBetAmount(int amount); //set an initial bet amount (default all in)
    int getBetAmount(); //find how much the player has bet 
    int getCurrency(); //show how many chips the player has
    void setCurrency(int money); //change the amount of chips the player has
    vector<Card>& getHand(); //retrieve what cards the player has
    vector<Card>& getDiscardPile(); //retrieve what cards have been swapped out

    //tokens (secondary currency)
    int getTokens() const; //show how many tokens the player has
    int changeTokens(int amount); //increase/decrease the amount of tokens the player has
};

#endif
