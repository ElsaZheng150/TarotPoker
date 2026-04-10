#ifndef DECK_H
#define DECK_H

#include <vector>
#include "Card.h"
using namespace std;

class Deck {
private:
    vector<Card> cards;
    int totalCards;   

public:
    Deck();
    void shuffle();
    Card draw();
    void addCard(Card card);             
    void returnCard(Card card);          
    void returnCards(vector<Card>& pile);
    void display() const;
    bool isEmpty() const;
    int size() const;
    int getTotal() const;
};

#endif
