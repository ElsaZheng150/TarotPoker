#ifndef COMPUTER_H
#define COMPUTER_H

#include <vector>
#include "Card.h"
#include "Deck.h"
using namespace std;

class Computer {
private:
    vector<Card> hand;
    vector<Card> discardPile;   
    int chips = 0;

public:
    void addCard(Card card);    
    void discardCard(int index);
    void changeCardValue(int index, const string& newValue, int newNumeric);
    void changeCardSuit(int index, const string& newSuit);
    void displayHand() const;
    void returnAllToDeck(Deck& deck);
    int handSize() const;
    bool hasCards() const;
	int changeChips(int amount);
	int getChips() const;
    vector<Card>& getHand();
    vector<Card>& getDiscardPile();
};

#endif
