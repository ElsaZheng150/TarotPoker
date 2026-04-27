#include "Computer.h"
#include "Card.h"
#include <iostream>
#include <map>
#include <algorithm>
using namespace std;

//subclass (child) of Player class
bool Computer::isFlush(const vector<Card>& hand) {
	if(hand.empty() || hand.size()!=5){
		return false; //safe guard against empty hand
	}
	string suit = hand[0].suit; //get suit
	//loop through the hand to check if suit matches 
	for(const auto& card : hand) {
		if(card.suit != suit) {
			return false; //not a flush
		}
	}
	return true; //default return
}

bool Computer::isStraight(const vector<Card>& hand) {
	if(hand.empty() || hand.size()<5){
		return false; //safe guard against empty hand or any hand less than 5
	}
	vector<int> values; //numeric card values
	for(const auto& card : hand) {
		values.push_back(card.numericValue);
	}
	sort(values.begin(), values.end()); //sort in ascending order
	for (int i = 0; i < values.size() - 1; i++) { //ensure values are incrementing by only 1 value
		if (values[i + 1] != values[i] + 1) {
			return false; //leave if not
		}
	}
	return true;
}

int Computer::handEvaluator(const vector<Card>& hand) {
	map<int, int> freq; //map to hold frequencies of card values for hand evaluation
	for (const auto& card : hand) {
		freq[card.numericValue]++; //count frequencies of each card value
	}//end of for loop

	int pairs = 0; //check for pairs
	bool threeOfKind = false; //check for three of a kind
	bool fourOfKind = false; //check for four of a kind
	bool flush = isFlush(hand); //check if it's a flush using helper
	bool straight = isStraight(hand); //check if it's a straight using helper

	for (auto const& [value, count] : freq) { //loop through frequency map to determine hand type
		if(count == 4) {
			fourOfKind = true; //four of a kind
		}//end of if
		else if(count == 3) {
			threeOfKind = true; //three of a kind
		}//end of else if
		else if(count == 2) {
			pairs++; //pair exists
		}//end of else if
	}//end of for loop

	//evaluate hand strength and return a corresponding int value for the hand type
	if (straight && flush) return 8; //straight flush
	if (fourOfKind) return 7; //four of a kind
	if (threeOfKind && pairs >= 1) return 6; //full house
	if (flush) return 5; //flush
	if (straight) return 4; //straight
	if (threeOfKind) return 3; //three of a kind but not full house
	if (pairs >= 2) return 2; //two pair
	if (pairs == 1) return 1; //one pair
	return 0; //high card

}