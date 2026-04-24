#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <map>
#include <algorithm>
#include <chrono>
#include <thread>
#ifdef _WIN32
#include <windows.h> //Win32 Sleep — std::this_thread::sleep_for can hang under mingw win32 threading
#else
#include <unistd.h>
#endif
#include "Deck.h"
#include "Player.h"
#include "Computer.h"
#include "Input.h"
using namespace std;

//portable small sleep used for dealing/betting animations
void sleepMs(int ms) {
#ifdef _WIN32
    Sleep(ms);
#else
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
#endif
}

// struct to pass all parameters into drawGame so it isn't cluttered
struct InputParams {
    Player& player;
    Deck& deck;
    int& selected;
    string& message;
	string& message2;
	string& message3;
    Computer& opponent;
    int& pot;
    int& gameState; //used so drawGame can hide opponent hand until showdown
    int& discardsThisRound; //so the draw-phase hint can show progress
    int& drawsThisRound;
};

//short label for each gameState value (shown in the phase hint)
static string phaseName(int gameState) {
    switch (gameState) {
        case 0: return "Ante";
        case 1: return "Dealing";
        case 2: return "Betting Round 1";
        case 3: return "Draw/Discard";
        case 4: return "Betting Round 2";
        case 5: return "Showdown";
        default: return "Between Rounds";
    }
}

//phase-appropriate action hint shown at the bottom of the game screen
static string phaseHint(int gameState, int discardsThisRound, int drawsThisRound, int pot) {
    switch (gameState) {
        case 0: return "Press [SPACE] to deal cards.";
        case 1: return "Press [SPACE] to begin the first betting round.";
        case 2:
        case 4: return "Press [B] to bet +1 chip. Press [SPACE] when done betting.";
        case 3: {
            int owed = discardsThisRound - drawsThisRound;
            string base = "Press [2] to discard, [1] to draw replacements. ";
            base += "Discarded: " + to_string(discardsThisRound) + "/3";
            base += ", Drawn: " + to_string(drawsThisRound) + "/" + to_string(discardsThisRound);
            if (owed > 0) base += " (draw " + to_string(owed) + " more to continue)";
            else base += ". Press [SPACE] when done.";
            return base;
        }
        case 5:
            //pot is zeroed when R awards it, so use that as the "showdown resolved" signal
            if (pot == 0) return "Hand revealed. Press [SPACE] to enter the shop.";
            return "Press [R] to reveal hands and award the pot.";
        default: return "";
    }
}

// main menu: in-game controls are shown during play, so keep this screen minimal
void drawUI(Player& player, Deck& deck, int selected, const string& message) {
    clearScreen();
    cout << "=== Tarot Poker ===" << endl;
    cout << endl;
    cout << "Five-card draw poker against a computer opponent." << endl;
    cout << endl;
    cout << "[S]  Start game" << endl;
    cout << "[Q]  Quit" << endl;

    if (!message.empty()) {
        cout << endl << "> " << message << endl;
    }
}

void drawGame(const InputParams& inP) {
    clearScreen();
    cout << "=== Tarot Poker - In Game ===" << endl;
    cout << endl;
    cout << "Controls: [<]/[>] navigate  [SPACE] advance phase  [1] draw  [2] discard" << endl;
    cout << "          [B] bet +1 chip  [R] end round (showdown)  [Q] quit" << endl;
    cout << endl;

    cout << "Opponent chips: " << inP.opponent.getChips() << endl << endl;

    if (inP.opponent.handSize() == 0) {
        cout << "Opponent hand is empty." << endl;
    }
    else {
        //reveal opponent's hand only at showdown (gameState 5); never show the player's cursor on opponent cards
        bool reveal = (inP.gameState == 5);
        cout << "Opponent hand: \n\n";
        for (int i = 0; i < inP.opponent.handSize(); i++) {
            if (reveal) {
                cout << "  " << inP.opponent.getHand()[i].display() << "  ";
            } else {
                cout << " \xe2\x81\x87  ";
            }
        }
        cout << endl;
    }

    cout << endl;
	cout << "-----------------------------------------" << endl << endl;

    cout << "Your chips: " << inP.player.getChips() << endl << endl;

    if (inP.player.handSize() == 0) {
        cout << "Your hand is empty." << endl;
    }
    else {
        cout << "Your hand: \n\n";
        for (int i = 0; i < inP.player.handSize(); i++) {
            if (i == inP.selected) {
                // ANSI inverse colors for highlighting
                cout << " \033[7m " << inP.player.getHand()[i].display() << " \033[0m ";
            }
            else {
                cout << "  " << inP.player.getHand()[i].display() << "  ";
            }
        }
        cout << endl;
    }

    cout << endl;
    cout << "=========================================" << endl << endl;
    cout << "Total pot: " << inP.pot << endl << endl;

    cout << inP.message << endl;
    cout << inP.message2 << endl;
	cout << inP.message3 << endl;

    //phase label + action hint so the player always knows what to press next
    cout << endl;
    cout << "--- Phase " << inP.gameState << ": " << phaseName(inP.gameState) << " ---" << endl;
    cout << phaseHint(inP.gameState, inP.discardsThisRound, inP.drawsThisRound, inP.pot) << endl;
}

void drawShop(InputParams inP) {
    clearScreen();
    cout << "=== Tarot Poker - Shop ===" << endl;
    cout << endl;
    cout << "Controls: [SPACE] exit shop and start next round  [D] view deck  [Q] quit" << endl;
    cout << endl;
    cout << "|---| Item 1 |---|     |---| Item 2 |---|     |---| Item 3 |---|" << endl;
    cout << endl << endl << endl << endl << endl << endl << endl << endl;
    cout << endl;
    cout << "Press [SPACE] to return to the game and up the ante." << endl;
}

//display the player or computer that won using results from compareHands
//win (0), lose (1), draw(2)
void displayWinner(int whoWon, string& message){
    if(whoWon == 0){
		message = "Player Wins\n";
    }
    if(whoWon == 1){
        message = "Player Loses\n";
    }
    if(whoWon == 2){
		message = "Draw\n";
    }
}

/*
Keep in case do different method later but decided to go with easier code
//sort hand by ascending order using bubble sort
//pass by reference to modify vector
void sortHand(vector<int>& hand){
    for(size_t i=0; i<hand.size()-1; i++){
        for(size_t j=0; j<hand.size()-i-1; j++){
            //swap if in wrong order
            if(hand[j] > hand[j+1]){
                int temp = hand[j+1];
                hand[j+1] = hand[j];
                hand[j] = temp;
            }
        }
    }
}
*/

//check if all cards share the same suit — only meaningful for a full 5-card hand
bool isFlush(const vector<Card>& hand) {
    if (hand.size() < 5) {
        return false; //flush requires a full 5-card hand
    }

    string suit = hand[0].suit; //get suit

    for (size_t i = 1; i < hand.size(); i++) {
        if (hand[i].suit != suit) {
            return false; //not a flush
        }
    }
    return true;
}

//check if hand forms a straight — only meaningful for a full 5-card hand
bool isStraight(vector<int> values) {
    if (values.size() < 5) {
        return false; //straight requires 5 consecutive cards
    }

    sort(values.begin(), values.end()); //sort in ascending order

    for (size_t i = 1; i < values.size(); i++) { //ensure values are incrementing by only 1 value
        //not the next value or dupe will break
        if ((values[i] != values[i - 1] + 1) || values[i] == values[i - 1]) {
            return false; //leave if not
        }
    }
    return true;
}

//determine how good the player/opponent's hand is
//Straight Flush > 4 of a kind > Full House > Flush >
//Straight > 3 of a kind > 2 pair > pair > high card
int getHandRank(const vector<Card>& hand, vector<int>& sortedValues) {
    vector<int> cardValues; //numeric card values
    vector<string> suits; //the suit of the cards (for flush hands)

    //determine the numeric value of each card (how good it is)
    for(size_t i=0; i<hand.size(); i++) {
        cardValues.push_back(hand[i].numericValue);
        suits.push_back(hand[i].suit);
    }

    //sort the values in descending value using built in function from algorithm
    sort(cardValues.begin(), cardValues.end(), greater<int>());
    sortedValues = cardValues; //put sorted values into sortedValues vector

    //count the frequencies to determine hand type
    map<int, int> freq; //map to pair value to frequency
    for(size_t j=0; j<cardValues.size(); j++) {
        freq[cardValues[j]]++; //increment count for value at j from hand
    }

    bool flush = isFlush(hand); //check if it's a flush using helper
    bool straight = isStraight(cardValues); //check if it's a straight using helper
 
    int four = 0; //check for four of a kind
    int three = 0; //check for three of a kind
    int pairs = 0; //check for pairs

    //find frequencies to determine hand type
    for(auto k=freq.begin(); k!=freq.end(); k++) { //auto determine type
        if(k->second == 4){
            four = 1; //four of kind
        }
        else if(k->second == 3){
            three = 1; //three of a kind
        }
        else if(k->second == 2){
            pairs++; //pairs exist
        }
        else{
            continue; //keep looking/high card maybe
        }
    }

    //determine ranking based off of poker rules
    if(straight && flush){
        return 8; //straight flush
    }
    else if(four > 0){
        return 7; //four of a kind
    }
    else if(three>0 && pairs==1){
        return 6; //full house
    }
    else if(flush){
        return 5; //flush
    }
    else if(straight){
        return 4; //straight
    }
    else if(three > 0){
        return 3; //three of a kind but not full house
    }
    else if(pairs == 2){
        return 2; //2 pair
    }
    else if(pairs == 1){
        return 1; //pair
    }
    else{
        return 0; //high card
    }
}

//human-readable hand rank name (matches the 0-8 scale in getHandRank)
string handRankName(int rank) {
    switch (rank) {
        case 8: return "Straight Flush";
        case 7: return "Four of a Kind";
        case 6: return "Full House";
        case 5: return "Flush";
        case 4: return "Straight";
        case 3: return "Three of a Kind";
        case 2: return "Two Pair";
        case 1: return "Pair";
        case 0: return "High Card";
        default: return "Unknown";
    }
}

//computer AI: pick indices to discard this round.
//keeps cards that are part of a pair/better, chases 4-of-suit flushes,
//keeps high singletons (J+), dumps low singletons. caps at 3 per traditional 5-card draw.
vector<int> computerDecideDiscards(const vector<Card>& hand) {
    vector<int> discards;
    map<int, int> valueFreq;
    map<string, int> suitFreq;
    for (size_t i = 0; i < hand.size(); i++) {
        valueFreq[hand[i].numericValue]++;
        suitFreq[hand[i].suit]++;
    }

    //chasing a flush? keep all cards of the dominant suit, ditch the odd one out
    string flushSuit = "";
    for (auto it = suitFreq.begin(); it != suitFreq.end(); it++) {
        if (it->second >= 4) {
            flushSuit = it->first;
            break;
        }
    }

    for (int i = 0; i < (int)hand.size(); i++) {
        if (!flushSuit.empty()) {
            if (hand[i].suit != flushSuit) discards.push_back(i);
            continue;
        }
        if (valueFreq[hand[i].numericValue] >= 2) continue; //part of pair+
        if (hand[i].numericValue >= 11) continue; //high singleton
        discards.push_back(i);
    }

    if (discards.size() > 3) discards.resize(3);
    return discards;
}

//computer AI: size a bet based on hand strength. never returns > opponent's chips.
int computerDecideBet(const vector<Card>& hand, int ante, int availableChips) {
    vector<int> sorted;
    int rank = getHandRank(hand, sorted);
    int bet;
    if (rank >= 6) bet = 1 + ante * 3;          //full house or better: aggressive
    else if (rank >= 4) bet = 1 + ante * 2;     //straight/flush: strong
    else if (rank >= 2) bet = 1 + ante;         //two pair/trips: solid
    else if (rank >= 1) bet = 1;                //pair: cautious
    else bet = (rand() % 2);                    //high card: 0 or 1 (sometimes bluff)
    if (bet > availableChips) bet = availableChips;
    if (bet < 0) bet = 0;
    return bet;
}

//see which player (player vs computer) has the better hand.
//returns the winner code (0=player, 1=computer, 2=tie) and builds a message
//containing both hands' rank names plus the winner announcement.
int compareHands(Player& human, Computer& enemy, string& message) {
    vector<Card> player = human.getHand();
    vector<Card> opponent = enemy.getHand();
    vector<int> playerValues;
    vector<int> enemyValues;
    int playerHandRank = getHandRank(player, playerValues);
    int enemyHandRank = getHandRank(opponent, enemyValues);
    int isWinner = 2; //default tie

    // compare rank first
    if(playerHandRank > enemyHandRank){
        isWinner = 0; //player wins
    }
    else if(playerHandRank < enemyHandRank){
        isWinner = 1; //enemy wins
    }
    else{
        //high card to break ties
        for (int i = 0; i < (int)playerValues.size(); i++){
            if (playerValues[i] > enemyValues[i]){
                isWinner = 0;
                break;
            }
            if (playerValues[i] < enemyValues[i]){
                isWinner = 1;
                break;
            }
        }
    }

    //build message with hand ranks, then append winner announcement
    string winMsg;
    displayWinner(isWinner, winMsg);
    message = "Your hand: " + handRankName(playerHandRank) + "\n"
            + "Opponent hand: " + handRankName(enemyHandRank) + "\n"
            + winMsg;
    return isWinner;
}

int main() {
    //set console to utf-8 for suit symbols
    system("chcp 65001 > nul"); //removed because windows os only
    srand((unsigned int)time(0));

    Deck deck;
    deck.shuffle();
    Player player; 

    int selected = 0;
    string message = "Welcome to Tarot Poker!";
    string message2 = ""; // extra message if needed
	string message3 = ""; // used for messages like "You can't bet right now" & "You can't draw right now"
    bool running = true;
    bool showingDeck = false;

    int state = 0; //0 for draw/discard functionality, 1 for the actual game, 2 for shop
	int gameState = 0; //0 for initial bet, 1 for dealing, 2 for 1st bet, 3 for draw discard, 4 for 2nd bet, 5 for showdown
    int chips = 100;
	int betChips = 0; // tracks how many chips the player has bet in the current betting round
    int pot = 0; // total chips bet by all players
	int ante = 1; // initial bet amount
    int alreadyBet = false;
    int alreadyDrew = false;
    int opponentTurnOver = false;
	bool readyForNextGameState = false;
	bool readyForShop = false;
	int discardsThisRound = 0; //5-card draw exchange counters (reset each round)
	int drawsThisRound = 0;

    //intialize enemy but do not show hand
    Computer opponent;

	InputParams inP {player, deck, selected, message, message2, message3, opponent, pot, gameState, discardsThisRound, drawsThisRound};

    drawUI(player, deck, selected, message);

    player.changeChips(100);
	opponent.changeChips(100);

    while (running) {
        int key = readKey();

        //main menu: only S (start) and Q (quit) are live here now
        if (state == 0) {
            switch (key) {
                case KEY_S: {
                    state = 1;
                    //empty player hand before starting new game
                    if (player.handSize() > 0) {
                        player.returnAllToDeck(deck);
                    }
                    //reset UI state so the menu's cursor/messages don't bleed into the game screen
                    selected = 0;
                    message = "";
                    message2 = "";
                    message3 = "";
                    drawGame(inP);
                    break;
                }
                case KEY_Q: {
                    running = false;
                    break;
                }
            }

        }

        //started 5 card game w/ opponent (CPU)
		if (state == 1) {
            if (showingDeck) {
                showingDeck = false;
                drawGame(inP);
                continue;
            }

            message3 = "";

            switch (key) {
                case KEY_LEFT: {
                    if (selected > 0) selected--;
                    drawGame(inP);
                    break;
                }
                case KEY_RIGHT: {
                    if (player.handSize() > 0 && selected < player.handSize() - 1)
                        selected++;
                    drawGame(inP);
                    break;
                }
                case KEY_SPACE: {
					if (readyForNextGameState == false) {
                        message3 = "You can't progress the game state right now.";
                        drawGame(inP);
                        break;
                    }
                    //can't leave the draw phase with an incomplete exchange (partial hand)
                    if (gameState == 3 && drawsThisRound < discardsThisRound) {
                        int owed = discardsThisRound - drawsThisRound;
                        message3 = "Draw " + to_string(owed)
                                 + " more card(s) (press [1]) before advancing.";
                        drawGame(inP);
                        break;
                    }
                    else {
                        gameState++;
                        readyForNextGameState = false;
						opponentTurnOver = false;
						betChips = 0;
					}
                    if (readyForShop) {
                        //return the revealed hands to the deck before entering the shop
                        //so the deck-view shows the full 52 cards
                        player.returnAllToDeck(deck);
                        opponent.returnAllToDeck(deck);
                        deck.shuffle();
                        selected = 0;
                        state = 2;
                        drawShop(inP);
                    }
                    else if (!readyForShop) {
						drawGame(inP);
                    }
                    break;
                }
                case KEY_1: {
                    if (gameState != 3) {
                        message3 = "You can't draw cards right now.";
						drawGame(inP);
						break;
                    }
                    //exchange limits: you can only draw as many cards as you've discarded
                    if (drawsThisRound >= discardsThisRound) {
                        message3 = "You can only draw as many cards as you've discarded.";
                    }
                    else if (deck.isEmpty()) { //check if deck is empty before drawing
                        message3 = "The deck is empty!";
                    }
                    else if (player.handSize() >= 5) { //check if hand is full before drawing
                        message3 = "Hand is full. Discard a card before drawing.";
                    }
                    else {
                        Card drawn = deck.draw();
                        message2 = "Drew: " + drawn.display();
                        player.addCard(drawn);
                        drawsThisRound++;
                        if (player.handSize() == 1) selected = 0;
                    }
                    drawGame(inP);
                    break;
                }
                case KEY_2: {
                    if (gameState != 3) {
                        message3 = "You can't discard cards right now.";
                        drawGame(inP);
                        break;
                    }
                    //exchange limits: max 3 discards per round, no discarding after drawing starts
                    if (drawsThisRound > 0) {
                        message3 = "You've already started drawing — no more discards this round.";
                    }
                    else if (discardsThisRound >= 3) {
                        message3 = "You've already discarded the max of 3 cards this round.";
                    }
                    else if (!player.hasCards()) {
                        message3 = "Hand is empty. Nothing to discard.";
                    }
                    else {
                        message2 = "Discarded: " + player.getHand()[selected].display();
                        player.discardCard(selected);
                        discardsThisRound++;
                        if (selected >= player.handSize() && selected > 0)
                            selected--;
                    }
                    drawGame(inP);
                    break;
                }
                case KEY_3: {
                    break;
                }
                case KEY_B: {
                    if (!(gameState == 2 || gameState == 4)) {
						message3 = "You can't bet right now!";
						drawGame(inP);
                        break;
                    }
                    pot++;
					betChips++;
                    player.changeChips(-1);
                    if (betChips == 1) {
                        message2 = "Added 1 chip to the pot.\n";
                    }
                    else if (betChips > 1) {
                        message2 = "Added " + to_string(betChips) + " chips to the pot.\n";
                    }
                    drawGame(inP);
                    break;
                }
                case KEY_D: {
                    //deck view is shop-only now
                    message3 = "Deck viewing is only available in the shop.";
                    drawGame(inP);
                    break;
                }
                case KEY_R: {
                    if (gameState != 5) {
                        drawGame(inP);
                        break;
                    }
                    //player hands competes against the enemy
                    if (player.handSize() == 0 || opponent.handSize() == 0) {
                        message3 = "Both players needs cards in hand.";
                    }
                    else {
                        int winner = compareHands(player, opponent, message);
                        //award the pot to the winner (split on tie, odd chip to player)
                        if (winner == 0) {
                            player.changeChips(pot);
                            message += "You win " + to_string(pot) + " chips!\n";
                        } else if (winner == 1) {
                            opponent.changeChips(pot);
                            message += "Opponent wins " + to_string(pot) + " chips.\n";
                        } else {
                            int half = pot / 2;
                            int odd = pot - 2 * half;
                            player.changeChips(half + odd);
                            opponent.changeChips(half);
                            message += "Pot split: " + to_string(half + odd) + " to you, "
                                    + to_string(half) + " to opponent.\n";
                        }
                        pot = 0;
                        //hands are NOT returned to deck here so the reveal stays on screen;
                        //the shop-exit handler returns and reshuffles them for the next round
                        message2 = "Press [SPACE] to enter the shop.";
                        readyForNextGameState = true;
                    }
                    drawGame(inP);
                    break;
                }
                case KEY_Q: {
                    running = false;
                    break;
                }
			}

            if (gameState == 0 && alreadyBet == false) {
                //initial bet (automatic ante) — actually deduct from both players
                readyForNextGameState = true;
                int playerAnte = ante;
                int oppAnte = ante;
                if (playerAnte > player.getChips()) playerAnte = player.getChips();
                if (oppAnte > opponent.getChips()) oppAnte = opponent.getChips();
                player.changeChips(-playerAnte);
                opponent.changeChips(-oppAnte);
                pot += playerAnte + oppAnte;
                message = "Ante " + to_string(ante) + ": Initial bet of " + to_string(ante) + "\n";
                message += "Opponent bets " + to_string(oppAnte) + ".\nYou bet " + to_string(playerAnte) + ".\n";
                drawGame(inP);
                alreadyBet = true;
            }
            

            if (gameState == 1 && alreadyDrew == false) {
                //dealing phase — deal all five cards to each player silently
                readyForNextGameState = true;
                for (int i = 0; i < 5; i++) {
                    player.addCard(deck.draw());
                    opponent.addCard(deck.draw());
                }
                message = "";
                drawGame(inP);
                alreadyBet = false;
                alreadyDrew = true;
            }

            if (gameState == 2) {
                //1st betting phase
				readyForNextGameState = true;
                message = "Make a bet!";
                if (opponentTurnOver == false) {
                    //opponent sizes its bet based on hand strength (smart AI)
                    int bet = computerDecideBet(opponent.getHand(), ante, opponent.getChips());
                    for (int i = 0; i < bet; i++) {
                        opponent.changeChips(-1);
                        pot++;
                        drawGame(inP);
                        sleepMs(500); //disable if this breaks things
                    }
                    message2 = "Opponent bet " + to_string(bet) + " chips.\n";
                    opponentTurnOver = true;
                }
				drawGame(inP);
			}

            if (gameState == 3) {
                //draw/discard phase
				readyForNextGameState = true;
                message = "Draw or discard cards!";
                if (opponentTurnOver == false) {
                    //opponent uses heuristic discard (keep pairs, chase flushes, keep high singletons)
                    vector<int> toDiscard = computerDecideDiscards(opponent.getHand());
                    //sort descending so earlier indices stay valid while erasing
                    sort(toDiscard.begin(), toDiscard.end(), greater<int>());
                    for (size_t i = 0; i < toDiscard.size(); i++) {
                        opponent.discardCard(toDiscard[i]);
                        drawGame(inP);
                        sleepMs(500); //disable if this breaks things
                    }
                    for (size_t i = 0; i < toDiscard.size(); i++) {
                        if (deck.isEmpty()) break;
                        opponent.addCard(deck.draw());
                        drawGame(inP);
                        sleepMs(500); //disable if this breaks things
                    }
                    message2 = "Opponent drew and discarded " + to_string(toDiscard.size()) + " cards.\n";
                    opponentTurnOver = true;
                }
				drawGame(inP);
            }

            if (gameState == 4) {
                //2nd betting phase
				readyForNextGameState = true;
                message = "Make a bet!";
                if (opponentTurnOver == false) {
                    //same smart bet sizing as 1st betting phase
                    int bet = computerDecideBet(opponent.getHand(), ante, opponent.getChips());
                    for (int i = 0; i < bet; i++) {
                        opponent.changeChips(-1);
                        pot++;
                        drawGame(inP);
                        sleepMs(500); //disable if this breaks things
                    }
                    message2 = "Opponent bet " + to_string(bet) + " chips.\n";
                    opponentTurnOver = true;
                }

				drawGame(inP);
            }

            if (gameState == 5) {
                //showdown phase
				//TODO: reveal opponent's hand during showdown (currently hidden by ?? if the 'hide' code is not commented out)
				//TODO: show the player's and opponent's hand rank (straight, flush, etc) during showdown
                readyForShop = true;
                if (readyForNextGameState == false) {
                    message = "Press [R] to end the round and see who wins!";
                }
				
				drawGame(inP);
            }


        }

        //shop for tarot cards - lets the player manipulate their hand or oppnent's hand in a unique way
        else if (state == 2) {
            //returning from the deck view: clear the flag and redraw the shop
            if (showingDeck) {
                showingDeck = false;
                drawShop(inP);
                continue;
            }
            //TODO: add actual shop functionality (if time permits)
            switch (key) {
                case KEY_SPACE: { //will be used to exit the shop and start the next round
                    //hands were already returned to the deck when entering the shop
                    state = 1;
                    gameState = 0; //reset game state to 0 (for new round)
					readyForShop = false; //reset shop state

                    alreadyBet = false; //reset bet state
                    alreadyDrew = false; //reset draw state
					opponentTurnOver = false; //reset opponent turn state
                    readyForNextGameState = true; //ready for next state in new round
                    discardsThisRound = 0; //reset exchange counters for new round
                    drawsThisRound = 0;

                    //rest the message for the next round
					message = "Starting a new round! Adding to the ante...";
					message2 = "";
					message3 = "";

                    drawGame(inP);
                    break;
                }
                case KEY_D: {
                    //deck view moved here from the in-game loop
                    showingDeck = true;
                    clearScreen();
                    deck.display();
                    cout << endl << "Press any key to return to the shop..." << endl;
                    continue;
                }
                case KEY_Q: {
                    running = false;
                    break;
                }
                default: {
                    drawShop(inP);
                    break;
				}
            }

            // Possible tarots
            /*
            Strength: adds value of 1 to selected card
            The Devil: subtracts value of 1 from selected card

            The Hanged Man: force opponent to discard a card of your choice

            The Star: convert two cards in your hand to diamonds
            The Moon: convert two cards in your hand to clubs
            The Sun: convert two cards in your hand to hearts
            The World: convert two cards in your hand to spades

            Wheel of Fortune: reveal 1-5 of opponent's cards

            swap a card in your hand with a card in the opponent's hand
            convert a card in your hand to a wild card
            get 2 extra discard/draws
            */
        }
    }

    clearScreen();
    cout << "Goodbye!" << endl;

    return 0;
}

