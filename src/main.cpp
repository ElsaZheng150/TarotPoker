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
#include <windows.h> //Win32 Sleep — std::this_thread::sleep_for hangs under mingw win32 threads
#else
#include <unistd.h>
#endif
#include "Deck.h"
#include "Player.h"
#include "Computer.h"
#include "Input.h"
using namespace std;

void sleepMs(int ms) {
#ifdef _WIN32
    Sleep(ms);
#else
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
#endif
}

//bundled params for drawGame
struct InputParams {
    Player& player;
    Deck& deck;
    int& selected;
    string& message;
	string& message2;
	string& message3;
    Computer& opponent;
    int& pot;
    int& gameState; //drawGame reveals the opponent hand only at showdown
    int& discardsThisRound;
    int& drawsThisRound;
};

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
            //pot is zeroed by the R handler, so use it as the "showdown resolved" signal
            if (pot == 0) return "Hand revealed. Press [SPACE] to enter the shop.";
            return "Press [R] to reveal hands and award the pot.";
        default: return "";
    }
}

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

    cout << "Opponent chips: " << inP.opponent.getChips()
         << "   Opponent tokens: " << inP.opponent.getTokens() << endl << endl;

    if (inP.opponent.handSize() == 0) {
        cout << "Opponent hand is empty." << endl;
    }
    else {
        //reveal only at showdown; no cursor highlight on opponent cards
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

    cout << "Your chips: " << inP.player.getChips()
         << "   Your tokens: " << inP.player.getTokens() << endl << endl;

    if (inP.player.handSize() == 0) {
        cout << "Your hand is empty." << endl;
    }
    else {
        cout << "Your hand: \n\n";
        for (int i = 0; i < inP.player.handSize(); i++) {
            if (i == inP.selected) {
                //ANSI inverse highlight
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

    cout << endl;
    cout << "--- Phase " << inP.gameState << ": " << phaseName(inP.gameState) << " ---" << endl;
    cout << phaseHint(inP.gameState, inP.discardsThisRound, inP.drawsThisRound, inP.pot) << endl;
}

void drawShop(InputParams inP) {
    clearScreen();
    cout << "=== Tarot Poker - Shop ===" << endl;
    cout << endl;
    cout << "Controls: [1] buy Item 1 (test)  [SPACE] exit shop  [D] view deck  [Q] quit" << endl;
    cout << endl;
    cout << "Your chips: " << inP.player.getChips()
         << "   Your tokens: " << inP.player.getTokens() << endl;
    cout << endl;
    cout << "|---| Item 1 |---|     |---| Item 2 |---|     |---| Item 3 |---|" << endl;
    cout << "| 5 tokens      |     |   (coming soon) |     |   (coming soon) |" << endl;
    cout << "| test purchase |" << endl;
    cout << endl << endl << endl << endl << endl << endl;
    cout << endl;
    cout << "Press [SPACE] to return to the game and up the ante." << endl;
    if (!inP.message3.empty()) {
        cout << endl << "> " << inP.message3 << endl;
    }
}

//0 = player wins, 1 = enemy wins, 2 = tie
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

//flush check — only meaningful with a full 5-card hand
bool isFlush(const vector<Card>& hand) {
    if (hand.size() < 5) {
        return false;
    }
    string suit = hand[0].suit;
    for (size_t i = 1; i < hand.size(); i++) {
        if (hand[i].suit != suit) {
            return false;
        }
    }
    return true;
}

//straight check — only meaningful with a full 5-card hand
bool isStraight(vector<int> values) {
    if (values.size() < 5) {
        return false;
    }
    sort(values.begin(), values.end());
    for (size_t i = 1; i < values.size(); i++) {
        if ((values[i] != values[i - 1] + 1) || values[i] == values[i - 1]) {
            return false;
        }
    }
    return true;
}

//rank: 8=straight flush, 7=four of a kind, 6=full house, 5=flush,
//4=straight, 3=three of a kind, 2=two pair, 1=pair, 0=high card
int getHandRank(const vector<Card>& hand, vector<int>& sortedValues) {
    vector<int> cardValues;
    for(size_t i=0; i<hand.size(); i++) {
        cardValues.push_back(hand[i].numericValue);
    }

    map<int, int> freq;
    for(size_t j=0; j<cardValues.size(); j++) {
        freq[cardValues[j]]++;
    }

    //sort by (frequency desc, value desc) so pair/trip/quad values lead sortedValues.
    //without this tiebreak, a K-pair loses to a Q-pair whose kicker is an Ace.
    sort(cardValues.begin(), cardValues.end(), [&freq](int a, int b) {
        if (freq[a] != freq[b]) return freq[a] > freq[b];
        return a > b;
    });
    sortedValues = cardValues;

    bool flush = isFlush(hand);
    bool straight = isStraight(cardValues);

    int four = 0;
    int three = 0;
    int pairs = 0;
    for(auto k=freq.begin(); k!=freq.end(); k++) {
        if(k->second == 4) four = 1;
        else if(k->second == 3) three = 1;
        else if(k->second == 2) pairs++;
    }

//see which player (player vs computer) has the better hand
void compareHands(Player& human, Computer& enemy) {
    vector<Card> player = human.getHand(); //get player hand
    vector<Card> opponent = enemy.getHand(); //get computer opponent hand
    vector<int> playerValues; //numeric values of player's hand
    vector<int> enemyValues; //numeric values of enemy's hand
    //determine how good each hand is
    int playerHandRank = getHandRank(player, playerValues); 
    int enemyHandRank = getHandRank(opponent, enemyValues);
    int isWinner = 2; //default tie (no added or lost currency)

    if(playerHandRank > enemyHandRank){
        isWinner = 0; //player wins
        //add bet amount onto total currency
        human.setCurrency(human.getCurrency() + human.getBetAmount()); 
    }//end of if
    else if(playerHandRank < enemyHandRank){
        isWinner = 1; //enemy wins
        //take out bet amount during loss
        human.setCurrency(human.getCurrency() - human.getBetAmount()); 
    }//end of else if
    else{
        //same rank: walk sortedValues (already freq-then-value ordered) for the tiebreak
        for (int i = 0; i < (int)playerValues.size(); i++){
            if (playerValues[i] > enemyValues[i]){
                isWinner = 0; //player wins
                //add bet amount onto total currency
                human.setCurrency(human.getCurrency() + human.getBetAmount()); 
                break;
            }
            if (playerValues[i] < enemyValues[i]){
                isWinner = 1; //enemy wins
                //take out bet amount during loss
                human.setCurrency(human.getCurrency() - human.getBetAmount());
                break;
            }
        }
    }

    string winMsg;
    displayWinner(isWinner, winMsg);
    message = "Your hand: " + handRankName(playerHandRank) + "\n"
            + "Opponent hand: " + handRankName(enemyHandRank) + "\n"
            + winMsg;
    return isWinner;
}

//base 10 + 5 per hand rank
int tokenPayoutForRank(int rank) {
    return 10 + 5 * rank;
}

int main() {
    //UTF-8 console for suit symbols (Windows only)
    system("chcp 65001 > nul");
    srand((unsigned int)time(0));

    Deck deck;
    deck.shuffle();
    Player player;

    int selected = 0;
    string message = "Welcome to Tarot Poker!";
    string message2 = "";
	string message3 = ""; //transient error hint ("You can't X right now")
    bool running = true;
    bool showingDeck = false;

    int state = 0;      //0=menu, 1=game, 2=shop
	int gameState = 0;  //0=ante, 1=deal, 2=bet1, 3=draw/discard, 4=bet2, 5=showdown
    int chips = 100;
	int betChips = 0;   //chips bet this round
    int pot = 0;
	int ante = 1;
    int alreadyBet = false;
    int alreadyDrew = false;
    int opponentTurnOver = false;
	bool readyForNextGameState = false;
	bool readyForShop = false;
	int discardsThisRound = 0; //exchange counters, reset each round
	int drawsThisRound = 0;

    Computer opponent;

	InputParams inP {player, deck, selected, message, message2, message3, opponent, pot, gameState, discardsThisRound, drawsThisRound};

    drawUI(player, deck, selected, message);

    player.changeChips(100);
	opponent.changeChips(100);

    while (running) {
        int key = readKey();

        //main menu: only S (start) and Q (quit)
        if (state == 0) {
            switch (key) {
                case KEY_S: {
                    state = 1;
                    if (player.handSize() > 0) {
                        player.returnAllToDeck(deck);
                    }
                    //reset cursor/messages so menu state doesn't leak into the game screen
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

        //active round
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
                    //can't leave the draw phase with a partial hand
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
                        //return hands to the deck entering the shop so deck-view shows all 52
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
                    //draws <= discards (exchange cap)
                    if (drawsThisRound >= discardsThisRound) {
                        message3 = "You can only draw as many cards as you've discarded.";
                    }
                    else if (deck.isEmpty()) {
                        message3 = "The deck is empty!";
                    }
                    else if (player.handSize() >= 5) {
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
                    //max 3 discards per round; no discards after drawing starts
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
                    message3 = "Deck viewing is only available in the shop.";
                    drawGame(inP);
                    break;
                }
                case KEY_R: {
                    if (gameState != 5) {
                        drawGame(inP);
                        break;
                    }
                    if (player.handSize() == 0 || opponent.handSize() == 0) {
                        message3 = "Both players needs cards in hand.";
                    }
                    else {
                        //both ranks used for token payout
                        vector<int> pv; vector<int> ov;
                        int playerRank = getHandRank(player.getHand(), pv);
                        int opponentRank = getHandRank(opponent.getHand(), ov);

                        int winner = compareHands(player, opponent, message);
                        //winner takes pot; tie splits with odd chip going to the player
                        if (winner == 0) {
                            player.changeChips(pot);
                            int payout = tokenPayoutForRank(playerRank);
                            player.changeTokens(payout);
                            message += "You win " + to_string(pot) + " chips and "
                                    + to_string(payout) + " tokens!\n";
                        } else if (winner == 1) {
                            opponent.changeChips(pot);
                            int payout = tokenPayoutForRank(opponentRank);
                            opponent.changeTokens(payout);
                            message += "Opponent wins " + to_string(pot) + " chips and "
                                    + to_string(payout) + " tokens.\n";
                        } else {
                            int half = pot / 2;
                            int odd = pot - 2 * half;
                            player.changeChips(half + odd);
                            opponent.changeChips(half);
                            int payout = tokenPayoutForRank(playerRank);
                            player.changeTokens(payout);
                            opponent.changeTokens(payout);
                            message += "Pot split: " + to_string(half + odd) + " to you, "
                                    + to_string(half) + " to opponent. Both earn "
                                    + to_string(payout) + " tokens.\n";
                        }
                        pot = 0;
                        //leave hands on screen after R; the shop-exit handler clears them
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

            //ante
            if (gameState == 0 && alreadyBet == false) {
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


            //deal 5 cards to each silently
            if (gameState == 1 && alreadyDrew == false) {
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

            //1st betting round
            if (gameState == 2) {
				readyForNextGameState = true;
                message = "Make a bet!";
                if (opponentTurnOver == false) {
                    int bet = computerDecideBet(opponent.getHand(), ante, opponent.getChips());
                    for (int i = 0; i < bet; i++) {
                        opponent.changeChips(-1);
                        pot++;
                        drawGame(inP);
                        sleepMs(500);
                    }
                    message2 = "Opponent bet " + to_string(bet) + " chips.\n";
                    opponentTurnOver = true;
                }
				drawGame(inP);
			}

            //draw/discard
            if (gameState == 3) {
				readyForNextGameState = true;
                message = "Draw or discard cards!";
                if (opponentTurnOver == false) {
                    vector<int> toDiscard = computerDecideDiscards(opponent.getHand());
                    //erase highest indices first so the remaining ones stay valid
                    sort(toDiscard.begin(), toDiscard.end(), greater<int>());
                    for (size_t i = 0; i < toDiscard.size(); i++) {
                        opponent.discardCard(toDiscard[i]);
                        drawGame(inP);
                        sleepMs(500);
                    }
                    for (size_t i = 0; i < toDiscard.size(); i++) {
                        if (deck.isEmpty()) break;
                        opponent.addCard(deck.draw());
                        drawGame(inP);
                        sleepMs(500);
                    }
                    message2 = "Opponent drew and discarded " + to_string(toDiscard.size()) + " cards.\n";
                    opponentTurnOver = true;
                }
				drawGame(inP);
            }

            //2nd betting round
            if (gameState == 4) {
				readyForNextGameState = true;
                message = "Make a bet!";
                if (opponentTurnOver == false) {
                    int bet = computerDecideBet(opponent.getHand(), ante, opponent.getChips());
                    for (int i = 0; i < bet; i++) {
                        opponent.changeChips(-1);
                        pot++;
                        drawGame(inP);
                        sleepMs(500);
                    }
                    message2 = "Opponent bet " + to_string(bet) + " chips.\n";
                    opponentTurnOver = true;
                }

				drawGame(inP);
            }

            //showdown
            if (gameState == 5) {
                readyForShop = true;
                if (readyForNextGameState == false) {
                    message = "Press [R] to end the round and see who wins!";
                }

				drawGame(inP);
            }


        }

        //shop (tarot purchasing; not wired up yet)
        else if (state == 2) {
            if (showingDeck) {
                showingDeck = false;
                drawShop(inP);
                continue;
            }
            //TODO: real shop inventory
            switch (key) {
                case KEY_SPACE: { //exit shop, start next round
                    //hands were already returned to the deck on shop entry
                    state = 1;
                    gameState = 0;
					readyForShop = false;

                    alreadyBet = false;
                    alreadyDrew = false;
					opponentTurnOver = false;
                    readyForNextGameState = true;
                    discardsThisRound = 0;
                    drawsThisRound = 0;

					message = "Starting a new round! Adding to the ante...";
					message2 = "";
					message3 = "";

                    drawGame(inP);
                    break;
                }
                case KEY_D: {
                    showingDeck = true;
                    clearScreen();
                    deck.display();
                    cout << endl << "Press any key to return to the shop..." << endl;
                    continue;
                }
                case KEY_1: {
                    //no-op purchase just to exercise the token flow
                    const int cost = 5;
                    if (player.getTokens() < cost) {
                        message3 = "Not enough tokens (need " + to_string(cost)
                                 + ", have " + to_string(player.getTokens()) + ").";
                    } else {
                        player.changeTokens(-cost);
                        message3 = "Bought Item 1 for " + to_string(cost) + " tokens.";
                    }
                    drawShop(inP);
                    break;
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

