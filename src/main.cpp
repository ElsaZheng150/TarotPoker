#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include "Deck.h"
#include "Player.h"
#include "Input.h"
using namespace std;

// redraws the entire UI after every action so it isn't as cluttered
void drawUI(Player& player, Deck& deck, int selected, const string& message) {
    clearScreen();
    cout << "=== Tarot Poker - Deck System ===" << endl;
    cout << endl;

    if (player.handSize() == 0) {
        cout << "Your hand is empty." << endl;
    } else {
        cout << "Your hand: ";
        for (int i = 0; i < player.handSize(); i++) {
            if (i == selected) {
                // ANSI inverse colors for highlighting
                cout << " \033[7m " << player.getHand()[i].display() << " \033[0m ";
            } else {
                cout << "  " << player.getHand()[i].display() << "  ";
            }
        }
        cout << endl;
    }

    cout << endl;
    cout << "Deck: " << deck.size() << "/" << deck.getTotal() << " cards" << endl;
    cout << endl;

    cout << "--- Controls ---" << endl;
    cout << "[<] [>]  Navigate hand" << endl;
    cout << "[1]      Draw a card" << endl;
    cout << "[2]      Discard selected card" << endl;
    cout << "[3]      Increase selected card value" << endl;
    cout << "[4]      Cycle selected card suit" << endl;
    cout << "[D]      View deck" << endl;
    cout << "[R]      End round (return cards)" << endl;
    cout << "[Q]      Quit" << endl;

    if (!message.empty()) {
        cout << endl << "> " << message << endl;
    }
}

int main() {
    //set console to utf-8 for suit symbols
    //system("chcp 65001 > nul"); removed because windows os only
    srand((unsigned int)time(0));

    Deck deck;
    deck.shuffle();
    Player player;

    int selected = 0;
    string message = "Welcome to Tarot Poker!";
    bool running = true;
    bool showingDeck = false;

    drawUI(player, deck, selected, message);

    while (running) {
        int key = readKey();

        
        if (showingDeck) {
            showingDeck = false;
            drawUI(player, deck, selected, message);
            continue;
        }

        switch (key) {
            case KEY_LEFT: {
                if (selected > 0) selected--;
                drawUI(player, deck, selected, message);
                break;
            }
            case KEY_RIGHT: {
                if (player.handSize() > 0 && selected < player.handSize() - 1)
                    selected++;
                drawUI(player, deck, selected, message);
                break;
            }
            case KEY_1: {
                if (deck.isEmpty()) {
                    message = "The deck is empty!";
                } else {
                    Card drawn = deck.draw();
                    message = "Drew: " + drawn.display();
                    player.addCard(drawn);
                    if (player.handSize() == 1) selected = 0;
                }
                drawUI(player, deck, selected, message);
                break;
            }
            case KEY_2: {
                if (!player.hasCards()) {
                    message = "Hand is empty. Nothing to discard.";
                } else {
                    message = "Discarded: " + player.getHand()[selected].display();
                    player.discardCard(selected);
                    if (selected >= player.handSize() && selected > 0)
                        selected--;
                }
                drawUI(player, deck, selected, message);
                break;
            }
            case KEY_3: {
                if (!player.hasCards()) {
                    message = "Hand is empty.";
                } else {
                    string oldDisplay = player.getHand()[selected].display();
                    player.getHand()[selected].incrementValue();
                    message = "Changed: " + oldDisplay + " -> " + player.getHand()[selected].display();
                }
                drawUI(player, deck, selected, message);
                break;
            }
            case KEY_4: {
                if (!player.hasCards()) {
                    message = "Hand is empty.";
                } else {
                    string oldDisplay = player.getHand()[selected].display();
                    player.getHand()[selected].cycleSuit();
                    message = "Changed: " + oldDisplay + " -> " + player.getHand()[selected].display();
                }
                drawUI(player, deck, selected, message);
                break;
            }
            case KEY_D: {
                showingDeck = true;
                clearScreen();
                deck.display();
                cout << endl << "Press any key to return..." << endl;
                break;
            }
            case KEY_R: {
                player.returnAllToDeck(deck);
                deck.shuffle();
                selected = 0;
                message = "All cards returned to deck and shuffled.";
                drawUI(player, deck, selected, message);
                break;
            }
            case KEY_Q: {
                running = false;
                break;
            }
        }
    }

    clearScreen();
    cout << "Goodbye!" << endl;

    return 0;
}
