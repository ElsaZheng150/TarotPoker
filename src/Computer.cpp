#include "Computer.h"
#include "Card.h"
#include <iostream>
using namespace std;

void Computer::addCard(Card card) {
    hand.push_back(card);
}

// moves card to discard pile instead so it isn't just deleted.
void Computer::discardCard(int index) {
    discardPile.push_back(hand[index]);
    hand.erase(hand.begin() + index);
}


void Computer::changeCardValue(int index, const string& newValue, int newNumeric) {
    hand[index].value = newValue;
    hand[index].numericValue = newNumeric;
}

void Computer::changeCardSuit(int index, const string& newSuit) {
    hand[index].suit = newSuit;
}

void Computer::displayHand() const {
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

void Computer::returnAllToDeck(Deck& deck) {
    deck.returnCards(hand);
    deck.returnCards(discardPile);
}

int Computer::handSize() const {
    return (int)hand.size();
}

bool Computer::hasCards() const {
    return !hand.empty();
}

int Computer::changeChips(int amount) {
    chips += amount;
    return chips;
}

int Computer::getChips() const {
    return chips;
}

vector<Card>& Computer::getHand() {
    return hand;
}

vector<Card>& Computer::getDiscardPile() {
    return discardPile;
}

int Computer::getTokens() const {
    return tokens;
}

int Computer::changeTokens(int amount) {
    tokens += amount;
    return tokens;
}
