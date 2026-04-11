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

public:
    void addCard(Card card);
    void discardCard(int index);
    void changeCardValue(int index, const string& newValue, int newNumeric);
    void changeCardSuit(int index, const string& newSuit);
    void displayHand() const;
    void returnAllToDeck(Deck& deck);
    int handSize() const;
    bool hasCards() const;
    vector<Card>& getHand();
    vector<Card>& getDiscardPile();
    void displayWinner(int whoWon); //display the player or computer that won
    void compareHands(); //check who has the greater hand to find the winner
};

#endif
