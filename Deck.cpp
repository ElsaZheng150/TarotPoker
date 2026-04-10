#include "Deck.h"
#include <iostream>
#include <cstdlib>
using namespace std;

Deck::Deck() {
    string suits[] = {"Hearts", "Diamonds", "Clubs", "Spades"};
    string values[] = {"2", "3", "4", "5", "6", "7", "8", "9", "10",
                        "Jack", "Queen", "King", "Ace"};
    int numerics[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};

    for (int s = 0; s < 4; s++) {
        for (int v = 0; v < 13; v++) {
            cards.push_back(Card(suits[s], values[v], numerics[v]));
        }
    }
    totalCards = cards.size();
}

void Deck::shuffle() {
    for (int i = cards.size() - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Card temp = cards[i];
        cards[i] = cards[j];
        cards[j] = temp;
    }
}

Card Deck::draw() {
    Card top = cards.back();
    cards.pop_back();
    return top;
}

void Deck::addCard(Card card) {
    cards.push_back(card);
    totalCards++;
}

void Deck::returnCard(Card card) {
    cards.push_back(card);
}

void Deck::returnCards(vector<Card>& pile) {
    for (int i = 0; i < (int)pile.size(); i++) {
        cards.push_back(pile[i]);
    }
    pile.clear();
}

// displays deck as a grid organized by suit and value
void Deck::display() const {
    string suits[] = {"Hearts", "Diamonds", "Clubs", "Spades"};
    string suitSymbols[] = {"\xe2\x99\xa5", "\xe2\x99\xa6", "\xe2\x99\xa3", "\xe2\x99\xa0"};
    string values[] = {"2", "3", "4", "5", "6", "7", "8", "9", "10",
                        "Jack", "Queen", "King", "Ace"};
    string shortVals[] = {"2", "3", "4", "5", "6", "7", "8", "9", "10",
                           "J", "Q", "K", "A"};

    cout << "Cards in deck (" << cards.size() << "/" << totalCards << "):" << endl;
    cout << endl;

    // column headers, ace to 2
    cout << "      ";
    for (int v = 12; v >= 0; v--) {
        string header = shortVals[v];
        while ((int)header.size() < 6) header += " ";
        cout << header;
    }
    cout << endl;

    for (int s = 0; s < 4; s++) {
        cout << "  " << suitSymbols[s] << "   ";
        for (int v = 12; v >= 0; v--) {
            int count = 0;
            for (int c = 0; c < (int)cards.size(); c++) {
                if (cards[c].suit == suits[s] && cards[c].value == values[v]) {
                    count++;
                }
            }
            string cell;
            int displayLen;
            if (count == 0) {
                cell = "--";
                displayLen = 2;
            } else if (count == 1) {
                cell = suitSymbols[s] + shortVals[v];
                displayLen = (int)shortVals[v].size() + 1;
            } else {
                // show duplicates like ♥Kx2
                cell = suitSymbols[s] + shortVals[v] + "x" + to_string(count);
                displayLen = (int)shortVals[v].size() + 1 + 1 + (int)to_string(count).size();
            }
            while (displayLen < 6) { cell += " "; displayLen++; }
            cout << cell;
        }
        cout << endl;
    }

    // For future tarot implementation (15th card value or custom suit)
    bool hasExtra = false;
    for (int c = 0; c < (int)cards.size(); c++) {
        bool standard = false;
        for (int s = 0; s < 4; s++) {
            for (int v = 0; v < 13; v++) {
                if (cards[c].suit == suits[s] && cards[c].value == values[v]) {
                    standard = true;
                }
            }
        }
        if (!standard) {
            if (!hasExtra) {
                cout << endl << "  Other cards:" << endl;
                hasExtra = true;
            }
            cout << "    " << cards[c].display() << endl;
        }
    }
}

bool Deck::isEmpty() const {
    return cards.empty();
}

int Deck::size() const {
    return (int)cards.size();
}

int Deck::getTotal() const {
    return totalCards;
}
