#include "Card.h"
using namespace std;

/*
    Purpose: Constructor for Card class
    Each card takes in a suit (diamonds, hearts, spades, clubs),
    a value (ace, 2-10, Jack, Queen, King),
    and the strength (value) of the card in standard poker rules
*/
Card::Card(string suit, string value, int numericValue)
    : suit(suit), value(value), numericValue(numericValue) {}

/*
    Function: suitSymbol
    Purpose: get unicode suit symbol
    Parameter(s): none
    Return: suit symbol of the card
*/
string Card::suitSymbol() const {
    if (suit == "Hearts")   return "\xe2\x99\xa5";
    if (suit == "Diamonds") return "\xe2\x99\xa6";
    if (suit == "Clubs")    return "\xe2\x99\xa3";
    if (suit == "Spades")   return "\xe2\x99\xa0";
    return suit;
}

/*
    Function: shortValue
    Purpose: Contains the simplified (standard) names for each non numeric card
    Parameter(s): None
    Return: the value of the card (numeric or simplified character form)
*/
string Card::shortValue() const {
    if (value == "Jack")  return "J";
    if (value == "Queen") return "Q";
    if (value == "King")  return "K";
    if (value == "Ace")   return "A";
    return value;
}

/*
    Function: display
    Purpose: To print out the card for the user to see
    Parameter(s): none
    Returns card suit and value
*/
string Card::display() const {
    return suitSymbol() + shortValue();
}

/*
    Function: incrementValue
    Purpose: Change the value of the card by one
    Will wrap ace back to 2
    Parameter(s): none
    Return: none
*/
void Card::incrementValue() {
    if (numericValue >= 14) {
        numericValue = 2; //wrap around if needed
    } else {
        numericValue++; //change the value of the card by 1
    }
    value = numericToValue(numericValue);
}

/*
    Function: cycleSuit
    Purpose: Shift card suit to the next suit
    Hearts -> Diamonds -> Clubs -> Spades -> Hearts
    Parameter(s): None
    Return: None
*/
void Card::cycleSuit() {
    if (suit == "Hearts")        suit = "Diamonds";
    else if (suit == "Diamonds") suit = "Clubs";
    else if (suit == "Clubs")    suit = "Spades";
    else                         suit = "Hearts";
}

/*
    Function: valueToNumeric
    Purpose: Get the strength (numeric value) of the card under standard poker rules
    Parameter(s): the actual card (e.g., King, Ace, 2, etc)
    Return: the strength of the card 
*/
int valueToNumeric(const string& value) {
    if (value == "2") return 2;
    if (value == "3") return 3;
    if (value == "4") return 4;
    if (value == "5") return 5;
    if (value == "6") return 6;
    if (value == "7") return 7;
    if (value == "8") return 8;
    if (value == "9") return 9;
    if (value == "10") return 10;
    if (value == "Jack") return 11;
    if (value == "Queen") return 12;
    if (value == "King") return 13;
    if (value == "Ace") return 14;
    return -1;
}

/*
    Function: numericToValue
    Purpose: Obtain the actual card using conversion from strength
    Parameter(s): the strength of the card
    Return: the card itself without the suit
*/
string numericToValue(int numeric) {
    switch (numeric) {
        case 2:  return "2";
        case 3:  return "3";
        case 4:  return "4";
        case 5:  return "5";
        case 6:  return "6";
        case 7:  return "7";
        case 8:  return "8";
        case 9:  return "9";
        case 10: return "10";
        case 11: return "Jack";
        case 12: return "Queen";
        case 13: return "King";
        case 14: return "Ace";
        default: return "?";
    }
}

/*
    Function: isValidSuit
    Purpose: Check to see if a card suit is valid
    Argument: the assigned suit of a card
    Return: True if it's a heart, diamond, club, or spade and False otherwise
*/
bool isValidSuit(const string& suit) {
    return suit == "Hearts" || suit == "Diamonds" || suit == "Clubs" || suit == "Spades";
}
