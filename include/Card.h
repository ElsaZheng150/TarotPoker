#ifndef CARD_H
#define CARD_H

#include <string>
using namespace std;


struct Card {
    string suit;
    string value;
    int numericValue;    // numeric version for comparisons (2-14, ace is 14, king is 13, queen is 12, jack is 11)

    Card(string suit, string value, int numericValue);
    string suitSymbol() const;
    string shortValue() const;
    string display() const;

    void incrementValue();  // wraps ace back to 2
    void cycleSuit();       // Hearts -> Diamonds -> Clubs -> Spades -> Hearts
};

int valueToNumeric(const string& value);
string numericToValue(int numeric);
bool isValidSuit(const string& suit);

#endif
