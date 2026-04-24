#include "Player.h"
#include "Card.h"
#include <vector>
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

int Player::changeChips(int amount) {
    chips += amount;
    return chips;
}

int Player::getChips() const {
    return chips;
}

vector<Card>& Player::getHand() {
    return hand;
}

vector<Card>& Player::getDiscardPile() {
    return discardPile;
}

int Player::getTokens() const {
    return tokens;
}

int Player::changeTokens(int amount) {
    tokens += amount;
    return tokens;
}
