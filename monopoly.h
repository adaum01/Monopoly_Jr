// ECE 209, Spring 2021 - Program 2
// Monopoly Junior -- game board and functions

#ifndef MONOPOLY_H
#define MONOPOLY_H

// game play functions
void setupBoard(int numPlayers, unsigned int seed);  // initialize board, cards, players
void movePlayer(int p, int spaces);   // move player p forward by specified # spaces
// assume 0 <= p && p < numPlayers
int findWinner(int numPlayers);     // return winning player id, or -1 if no winner

// player info functions
// assume 0 <= p && p < numPlayers
const char * playerName(int p);  // return token string for player p
int playerBalance(int p);
int playerPosition(int p);

// board info functions
// assume 0 <= pos && pos < board spaces
const char * positionName(int pos);
int positionOwner(int pos);
int positionRent(int pos);
const char * positionColor(int pos);

// chance card functions
// assume 0 <= card and card < number of cards
int drawCard();   // returns ID of top card on the Chance stack
// and moves that card to the bottom of the stack
const char * cardText(int card);   // return the message of a card
void replaceJailCard();   // player returns get-out-of-jail card to bottom of stack

// functions for testing purposes
// (You may use these in your other functions, but the main purpose
// is to allow me to set up the board/cards for testing.)
void setPlayerPosition(int player, int pos);  // just put the player here; don't do anything
void setPlayerBalance(int player, int dollars);  // override player's balance
void setPositionOwner(int pos, int player);  // overwrite position owner
int peekTopDeck();   // view top card without removing it
int peekBottomDeck();  // view bottom card without removing it
int peekDeck(int c);   // view card c from the top of deck

#endif //MONOPOLY_H
