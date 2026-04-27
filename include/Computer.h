#ifndef COMPUTER_H
#define COMPUTER_H

#include <vector>
#include "Card.h"
#include "Deck.h"
#include "Player.h"
using namespace std;

class Computer : public Player{
private:  
    //child of the player class as it is an AI player
	bool isFlush(const vector<Card>& hand); //check if hand is a flush
	bool isStraight(const vector<Card>& hand); //check if hand is a straight
public:
	int handEvaluator(const vector<Card>& hand); //decide how good current hand is (used for betting and discarding/drawing decision-making)

};

#endif
