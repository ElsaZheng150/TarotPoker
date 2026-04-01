# TarotPoker
Five card draw poker with tarot cards to influence player and opponent cards

Game Description
Five card draw poker
Game play: We are using mostly standard five card draw poker rules. Each player gets five cards and can place afterwards. Then they can replace or keep the cards they have. In our game, they will also have tarot cards that can affect your cards or the cards of your opponent(s). Before hands are revealed, players place bets again. The player with the best hand or last one remaining if everyone else has folded is the winner.

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

For decks we would still need to figure out whether or not it gets reset every round so player changes don’t affect the next round or if all cards keep their changed states and get re-shuffled in the deck. 
If we were to not reset the cards in the deck eventually the suits and values will be skewed which would make certain hands such as flushes or 5 of a kind more easy to achieve therefore making those hands overvalued. 
We could either not do this or for each round calculate the probability of each hand being drawn and then rank them again based upon rarity. 
(we could keep their rankings hidden from the players so they don’t end up having a cycle where they switch their cards between two different types of hands
This would also add another aspect as to not create too many of the same card as it would also help your opponents win not just yourself.
External Libraries
string library: to use string variables for console output
iostream library: for standard input and output
vector library: use vectors to keep track of cards in the deck and what the players have
cstdlib: for random number generation
