#ifndef DECK_H
#define DECK_H

#include <vector>
#include "Card.h"
using namespace std;

class Deck {
private:
    vector<Card> cards; //holds the cards available to play with 
    int totalCards; //how many cards in the vector cards and are available to be dealt

public:
    Deck(); //constructor
    void shuffle(); //randomly mix up the cards before dealing
    Card draw(); //allow the player to take cards if they don't have 5 cards
    void addCard(Card card); //add card into the deck
    void returnCard(Card card); //send card back into the deck 
    void returnCards(vector<Card>& pile); //send call cards back after game
    void display() const; //show the cards to the player
    bool isEmpty() const; //check if the deck has any cards
    int size() const; //check the amount of cards in a deck
    int getTotal() const; //return the number of available cards
};

#endif
