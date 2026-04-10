#include "Card.h"
using namespace std;

Card::Card(string suit, string value, int numericValue)
    : suit(suit), value(value), numericValue(numericValue) {}

// unicode suit symbol
string Card::suitSymbol() const {
    if (suit == "Hearts")   return "\xe2\x99\xa5";
    if (suit == "Diamonds") return "\xe2\x99\xa6";
    if (suit == "Clubs")    return "\xe2\x99\xa3";
    if (suit == "Spades")   return "\xe2\x99\xa0";
    return suit;
}

string Card::shortValue() const {
    if (value == "Jack")  return "J";
    if (value == "Queen") return "Q";
    if (value == "King")  return "K";
    if (value == "Ace")   return "A";
    return value;
}

string Card::display() const {
    return suitSymbol() + shortValue();
}

void Card::incrementValue() {
    if (numericValue >= 14) {
        numericValue = 2;
    } else {
        numericValue++;
    }
    value = numericToValue(numericValue);
}

void Card::cycleSuit() {
    if (suit == "Hearts")        suit = "Diamonds";
    else if (suit == "Diamonds") suit = "Clubs";
    else if (suit == "Clubs")    suit = "Spades";
    else                         suit = "Hearts";
}

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

bool isValidSuit(const string& suit) {
    return suit == "Hearts" || suit == "Diamonds" || suit == "Clubs" || suit == "Spades";
}
