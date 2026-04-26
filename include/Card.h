#ifndef CARD_H
#define CARD_H

#include <string>
using namespace std;


struct Card {
    string suit; //hearts, diamonds, spades, clubs
    string value; //Ace, 2-10, Jack, Queen, King 
    int numericValue;    // numeric version for comparisons (2-14, ace is 14, king is 13, queen is 12, jack is 11)

    Card(string suit, string value, int numericValue); //constructor
    string suitSymbol() const; //what suit the card has
    string shortValue() const; //what the card is
    string display() const; //display the card

    void incrementValue();  // wraps ace back to 2
    void cycleSuit();       // Hearts -> Diamonds -> Clubs -> Spades -> Hearts
};

int valueToNumeric(const string& value); //get the value of the card (how strong it is)
string numericToValue(int numeric); //convert the value to the card itself for display purposes
bool isValidSuit(const string& suit); //checks if the suit is valid (must be a heart, diamond, spade, or club)

#endif
