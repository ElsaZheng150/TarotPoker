# TarotPoker
Five card draw poker with tarot cards to influence player and opponent cards

Game Description
Five card draw poker (Not Texas Hold em)
Game play: We are using mostly standard five card draw poker rules. Each player gets five cards and can place afterwards. Then they can replace or keep the cards they have. In our game, they will also have tarot cards that can affect your cards or the cards of your opponent(s). Before hands are revealed, players place bets again. The player with the best hand or last one remaining if everyone else has folded is the winner.

RULES:
1. Five cards are dealt from a standard 52 card deck. Player and Enemy AI both get 100 chips to start off with. The player does not get any tokens for the tarot shop to start off. 
2. Player and computer have 1 chance to swap out cards. 
3. Before placing bets, the player may use an attack card to hurt the computer's hand or a viewing card to see the computer's hand to help them decide on what to bet.
4. At this point bets will be placed and they must be the same. Both are allowed to increase bets as they see fit, but bets must be equal. Players are forced to bet to increase how competitive the game is.
5. Hands are compared and whoever wins gets the pot (chips and tokens).
*The tarot shop is open for shopping if the player has tokens.

Planned Features (game mechanics, formats, AI, interfaces) 
tarot cards that can affect the your or other player’s card (like Balatro)
For example you can change suits, increase the card value, view one card from another player, etc.
text based interface 
There is ASCII text for the different suits already. Most Tarot symbols already have ASCII code for them.
Another option is to use ascii art.
currency system for chips
There will be a shop like in balatro 
Two different currencies( chips and tokens)
Chips would be actual real world money like in normal poker
Tokens would be used to buy tarot cards before the game
Each ante would have a set amount of tokens you can win based upon which place you get each round. (1st gets 10 tokens, 2nd gets 5 tokens, etc.) 
We would need to do play testing to figure out what values would be best for both the prices of tarot cards and the rewarded tokens.
opponent is computer based (no multiplayer planned)
Different types of bots such as one which goes for a particular hand type
Some would be more defensive (using tarot cards for their own benefit)
Or offensive (using tarot cards to change opponent’s cards to sabotage)

Classes:
1. Card: how each card in the deck looks with its info (type, suit, numeric/strength value)
2. Deck: what cards are available in a standard poker deck with option to expand with tarot cards
3. Player: variables and functions for the player (hand, cards, currency)
5. Computer: a child of Player, differs by functions for basic game AI
6. Input: for reading in user input using keys
7. Main: where the main game code resides, contains code to compare hands and to run the game by outputing text on the console (game and shop)

External Libraries
string library: to use string variables for console output
iostream library: for standard input and output
vector library: use vectors to keep track of cards in the deck and what the players have
cstdlib: for random number generation

How to compile and run the program:
1. Clone the repository on Github
2. Open on VS Code
3. Install a C++ compiler if needed
4. Set up any configurations needed
5. Run the game using VS Code

How to play the game:
1. Ensure the previous set of instructions regarding compiling has been met
2. Hit run
3. Follow the console instructions and have fun!
