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
   - variables for suit (string) and value (string) of the card, numeric value for how strong the card is 2-14
   - constructor for a standard card with parameters: suit, value, and numeric value
   - Methods:
       - suitSymbol: to get the suit of the card
       - shortValue: to get what the card is (e.g., King, Ace, 10)
       - display: to show the card
       - incrementValue: to increase the value of the card when making the deck
       - cycleSuit: to change the card's suit to the next one
       - valueToNumeric: get the numeric value (how strong it is) of the card for             comparison purposes
       - numericToValue: get what the card is (e.g., King) based on the numeric               value to display the card properly
       - isValidSuit: check if the card has a valid suit, ensure no non standard              cards are used
2. Deck: what cards are available in a standard poker deck with option to expand with tarot cards
   - Private members:
       - vector of type Card to store the 52 standard cards
       - the number of cards that are still in the vector and can be dealt, not the           size of the deck
   - Public members:
       - default constructor
       - Methods:
           - shuffle: randomly mix up the cards before dealing to ensure fairness
           - draw: allow the player to a random if they don't have 5 cards
           - addCard: add card into the deck
           - returnCard: send card back into the deck
           - returnCards: send call cards back after game to start over
           - display: show hand to the player 
           - isEmpty: check if the deck has any cards (can the game still be played?)
           - size: check the amount of cards in a deck
           - getTotal: return the number of available card
3. Player: variables and functions for the player (hand, cards, currency)
   - private members:
       - vector of type card with a size of 5 to represent the player's hand
       - vector of type card to represent a discard pile where cards the player does          not want will go
       - number of tokens (secondary currency for the tarot shop)
       - currency (chips) are basically real world money gained for winning
       - amount to bet, must bet (poker rules)
       - number of attack tarot cards
       - number of viewing tarot cards
   - public members:
       - addCard: draw a card and put into hand
       - discardCard(int index): swap out existing card in hand, discarded card goes          to the discard pile
       - changeCardValue: change the value of a card for special tarot system
       - changeCardSuit: change what suit the card has for special tarot system
       - displayHand: show player hand (either to the player or at the end
       - returnAllToDeck: discard all cards at end when the game is over
       - handSize: show how many cards the player has, prevent too many cards
       - hasCards: check if player has cards
       - setBetAmount: set an initial bet amount (default all in, all players must            bet under poker rules)
       - getBetAmount: return how much the player has bet
       - getCurrency: show how many chips the player has and can bet
       - setCurrency: change the amount of chips the player has after winning/losing          game
       - getHand: return what cards the player has
       - getDiscardPile: return what cards have been swapped out
       - buyAttackCards: increase the amount of attack cards by buying from shop
       - buyViewingCards(): increase the amount of viewing cards by buying from shop
       - useAttackCard: decreases amount of attack cards in possession and remove             card from enemy's hand
       - useViewingCard: decreases amount of attack cards in possession and reveals           one random card from enemy's hand
       - getAttackCards: return how many attack tarot cards the player owns
       - getViewingCards: return how many viewing tarot cards the player owns
       - getTokens: return how many tokens the player has, for the shop
       - changeTokens: increase/decrease the amount of tokens the player has based            on results of the game and if the player buys tarot cards
4. Computer: a child of Player, differs by functions for basic game AI
   - inherits most functions from the player as they must complete the same moves
   - private members only
   - isFlush: to check if hand is a flush
   - isStraight: to check if the hand is straight
   - handEvaluator: determine how good the hand is before deciding to discard cards       and how much to bet
5. Input: for reading in user input using keys
   - enum key for what inputs the player may give
   - int readKey: take in user input
   - clearScreen: wipe all console text and start the game over, maintain clean console and UI
6. Main: where the main game code resides, contains code to compare hands and to run the game by outputing text on the console (game and shop)
   a) Game starts by displaying welcoming message, menu, and option to look at instructions
   b) Game starts. Both players (player and computer) are given 5 random cards and         100 chips to start off.
   c) Enemy(computer) will discard cards and make bets behind the scenes
   d) Player can now choose to discard cards as their hand is displayed
   e) Bets can be placed. Player must match bet of the computer or fold
   f) Tarot cards can be used if any and player can makes adjustments based on             results of tarot card usage.
   g) Showdown where hands and the winner is revealed. Chips are distributed based         on how much was bet on the pot. Tokens are distributed based on the strength         of the hand for fairness.
   h) Option for tarot shop is opened. Player can buy cards if they want and/or can.
   i) Player can continue playing until they decide to quit or are broke. 

External Libraries:
   - \<string\>: to use string variables for console output
   - \<iostream\>: for standard input and output
   - \<cstdlib\>: for random number generation
   - \<ctime\>: for a unique seed based on current time for random number generation
   - \<vector\>: use vectors to keep track of cards in the deck and what the players have
   - \<map\>: to keep track of card frequencies for determining hand ranks
   - \<algorithm\>: for sorting to determine hand ranks and to help with opponent AI
   - \<chrono\>: provides time measurements that allow for pausing the program for a specified amount of time
   - \<thread\>: provides the actual pausing functionality for the above

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
