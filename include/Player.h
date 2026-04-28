#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include "Card.h"
#include "Deck.h"
using namespace std;

class Computer;

class Player {
private:
    vector<Card> hand; //what cards the player has
    vector<Card> discardPile; //what cards the player is swapping out
    int tokens = 0; //secondary currency, awarded to the winner at showdown
    int currency = 0; //amount of chips a player has to bet 
    int betAmount; //default all in
    int attackCards = 0; //attack tarot card that allows player to change the value of a random card from opponent's hand, track amount
    int viewingCards = 0; //allows players to see a random card from the opponent's hand, track amount

public:
    void addCard(Card card); //draw a card
    void discardCard(int index); //swap out existing card in hand
    void changeCardValue(int index, const string& newValue, int newNumeric); //change the value of a card for special tarot system
    void changeCardSuit(int index, const string& newSuit); //change what suit the card has for special tarot system
    void displayHand() const; //show player hand 
    void returnAllToDeck(Deck& deck); //discard all cards at end 
    int handSize() const; //show how many cards the player has
    bool hasCards() const; //check if player has cards
    void setBetAmount(int amount); //set an initial bet amount (default all in)
    int getBetAmount(); //find how much the player has bet 
    int getCurrency(); //show how many chips the player has
    void setCurrency(int money); //change the amount of chips the player has
    vector<Card>& getHand(); //retrieve what cards the player has
    vector<Card>& getDiscardPile(); //retrieve what cards have been swapped out

    //use tarot cards
    void buyAttackCards(); //increase the amount of attack cards from shop
    void buyViewingCards(); //increase the amount of viewing cards from shop
    void useAttackCard(Computer& enemy);
    void useViewingCard(Computer& enemy);
    int getAttackCards() const;  //how many attack tarot cards the player owns
    int getViewingCards() const; //how many viewing tarot cards the player owns

    //tokens (secondary currency)
    int getTokens() const; //show how many tokens the player has
    int changeTokens(int amount); //increase/decrease the amount of tokens the player has
};

#endif
