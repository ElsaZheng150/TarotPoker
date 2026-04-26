#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include "Card.h"
#include "Deck.h"
using namespace std;

class Player {
private:
    vector<Card> hand;
    vector<Card> discardPile;
    int tokens = 0; //secondary currency, awarded to the winner at showdown 
    int currency = 0; //amount of chips a player has to bet 
    int betAmount; //default all in

public:
    void addCard(Card card);
    void discardCard(int index);
    void changeCardValue(int index, const string& newValue, int newNumeric);
    void changeCardSuit(int index, const string& newSuit);
    void displayHand() const;
    void returnAllToDeck(Deck& deck);
    int handSize() const;
    bool hasCards() const;
    void setBetAmount(int amount);
    int getBetAmount();
    int getCurrency();
    void setCurrency(int money);
    vector<Card>& getHand();
    vector<Card>& getDiscardPile();

    //tokens (secondary currency)
    int getTokens() const;
    int changeTokens(int amount);
};

#endif
