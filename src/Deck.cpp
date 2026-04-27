#include "Deck.h"
#include <iostream>
#include <cstdlib>
using namespace std;

/*
    Purpose: Default Constructor for Deck class
    Add all cards into the deck
    Standard deck rules, tarot cards are separate
*/
Deck::Deck() {
    //values of the cards for the deck
    string suits[] = {"Hearts", "Diamonds", "Clubs", "Spades"};
    string values[] = {"2", "3", "4", "5", "6", "7", "8", "9", "10",
                        "Jack", "Queen", "King", "Ace"};
    int numerics[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};

    //initalize all the cards in the deck and add them in
    for (int s = 0; s < 4; s++) {
        for (int v = 0; v < 13; v++) {
            cards.push_back(Card(suits[s], values[v], numerics[v]));
        }
    }
    totalCards = cards.size(); //set amount of cards in the deck
}

/*
    Function: shuffle
    Purpose: Mix up the cards for randomness when cards are dealt
    Parameter(s): None
    Return: None
*/
void Deck::shuffle() {
    //goes through the deck and shifts where the card is in the deck
    for (int i = cards.size() - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        //swapping places
        Card temp = cards[i]; //placeholder
        cards[i] = cards[j];
        cards[j] = temp;
    }
}

/*
    Function: draw
    Purpose: Draws a card from the top of the deck
    Parameter(s): None
    Return: the card drawn
*/
Card Deck::draw() {
    Card top = cards.back(); //the card that would be at the top of the deck in real life
    cards.pop_back(); //remove from deck
    return top; //give card to the player's hand
}

/*
    Function: addCard
    Purpose: Add a card back into the deck
    Parameter(s): The card to be added
    Return: None
*/
void Deck::addCard(Card card) {
    cards.push_back(card); //add card to the deck
    totalCards++; //increase count of how many cards the deck has pre dealing
}

/*
    Function: returnCard
    Purpose: Send a card back to the deck
    Parameter(s): the card to be sent back
    Return: None
*/
void Deck::returnCard(Card card) {
    cards.push_back(card); //add card back to deck
}

/*
    Function: returnCards
    Purpose: Return all cards in a hand to the deck (at end or if user discards all cards)
    Parameter(s): player's hand
    Return: none
*/
void Deck::returnCards(vector<Card>& pile) {
    //go through the returned cards and add them back into the deck
    for (int i = 0; i < (int)pile.size(); i++) {
        cards.push_back(pile[i]);
    }
    pile.clear(); //clear the pile so that there are no more cards in the discard pile
}

/*
    Function: display
    Purpose: Displays deck as a grid organized by suit and value
    Parameter(s): None
    Return: None
*/
void Deck::display() const {
    string suits[] = {"Hearts", "Diamonds", "Clubs", "Spades"};
    string suitSymbols[] = {"\xe2\x99\xa5", "\xe2\x99\xa6", "\xe2\x99\xa3", "\xe2\x99\xa0"};
    string values[] = {"2", "3", "4", "5", "6", "7", "8", "9", "10",
                        "Jack", "Queen", "King", "Ace"};
    string shortVals[] = {"2", "3", "4", "5", "6", "7", "8", "9", "10",
                           "J", "Q", "K", "A"};

    //console display what cards are in the deck
    cout << "Cards in deck (" << cards.size() << "/" << totalCards << "):" << endl;
    cout << endl;

    // Print column headers (card values from Ace down to 2)
    cout << "      ";
    for (int v = 12; v >= 0; v--) {
        string header = shortVals[v];
        // Pad each header to a fixed width (6 characters) for alignment
        while ((int)header.size() < 6) header += " ";
        cout << header;
    }
    cout << endl;

    // Loop through each suit (row)
    for (int s = 0; s < 4; s++) {
        // Print the suit symbol at the start of the row
        cout << "  " << suitSymbols[s] << "   ";
        // Loop through each value (column), again from Ace to 2
        for (int v = 12; v >= 0; v--) {
            int count = 0;
            // Count how many cards of this suit and value exist in the deck
            for (int c = 0; c < (int)cards.size(); c++) {
                if (cards[c].suit == suits[s] && cards[c].value == values[v]) {
                    count++;
                }
            }
            string cell;
            int displayLen;

            // Determine what to display in the cell based on count
            if (count == 0) { // No cards of this type
                cell = "--";
                displayLen = 2;
            } else if (count == 1) { // One card: show symbol + short value (e.g., ♥K)
                cell = suitSymbols[s] + shortVals[v];
                displayLen = (int)shortVals[v].size() + 1;
            } else { // Multiple cards: show count (e.g., ♥Kx2)
                // show duplicates like ♥Kx2
                cell = suitSymbols[s] + shortVals[v] + "x" + to_string(count);
                displayLen = (int)shortVals[v].size() + 1 + 1 + (int)to_string(count).size();
            }
            // Pad each cell to a fixed width (6 characters) for table alignment
            while (displayLen < 6) { cell += " "; displayLen++; }

            // Output the formatted cell
            cout << cell;
        }
        cout << endl; // Move to the next row after finishing a suit
    }

    // For future tarot implementation (15th card value or custom suit)
    bool hasExtra = false;
    for (int c = 0; c < (int)cards.size(); c++) {
        bool standard = false;
        // Check if this card matches any standard suit/value combination
        for (int s = 0; s < 4; s++) {
            for (int v = 0; v < 13; v++) {
                if (cards[c].suit == suits[s] && cards[c].value == values[v]) {
                    standard = true;
                }
            }
        }
        // If not standard, list it under "Other cards"
        if (!standard) {
            if (!hasExtra) {
                cout << endl << "  Other cards:" << endl;
                hasExtra = true;
            }
            cout << "    " << cards[c].display() << endl;
        }
    }
}

/*
    Function: isEmpty
    Purpose: check if the deck has cards left
    Parameter(s): None
    Return: True if deck has no more cards, False if deck has at least 1 card
*/
bool Deck::isEmpty() const {
    return cards.empty(); 
}

/*
    Function: size
    Purpose: find how many cards the deck still has
    Parameter(s): None
    Return: amount of cards left in the deck
*/
int Deck::size() const {
    return (int)cards.size(); 
}

/*
    Function: getTotal
    Purpose: find how many cards are in the deck
    Parameter(s): None
    Return: number of cards in the deck in total
*/
int Deck::getTotal() const {
    return totalCards; 
}
