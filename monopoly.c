// ECE 209, Spring 2021 - Program 2
// Monopoly Junior board game
// Implements functions given in monopoly.h file which will allow game to function properly
//  Includes various definitions for properties relating to the initialization and manipulation of the players,
//  board elements, and the chance deck, from specifications given in the boardData.c file
// Alexander Daum 3/27/21

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "random.h"
#include "boardData.h"
#include "monopoly.h"


//GLOBAL VARIABLES

// these store data for each player that is referenced in multiple functions, each element represents a separate player: 0->Player 0, 3->Player 3, etc.
// pPos tracks the current position of all players, pBalance tracks each player's current balance, pNames holds the string data of every player's name
int pPos[4] = {-1, -1, -1, -1};         //initialized to -1 to be able to determine non-initialized players later
int pBalance[4];
char pNames[4][8];

// these maintain data on jail-free cards and jail status for each player, and holds data that must be remembered outside of any one individual function
// JFcard array tracks whether a player is currently holding the jail-free card (0->No Card, 1->Holding Card)
// inJail array tracks whether a player is currently in jail (0->Not in Jail, 1->In Jail)
int JFcard[4] = {0};
int inJail[4] = {0};

// these store/track data related to the board and cards in the game, and are referenced throughout the program
// chanceDeck tracks the current deck of cards by and the arrangement of cards within the deck by card ID
// deckSize tracks the current size of the deck (if the Jail-Free card has been drawn, this will be reduced by one until returned)
// propOwned tracks the ownership status of all tiles on the board (if unowned or not a property->-1, if owned->Player ID of Owner)
int chanceDeck[MAX_CARDS];
int deckSize = MAX_CARDS;
int propOwned[MAX_BOARD_SPACES];

// This acts a holder for string data derived from cards/property information
// arrPosName and arrPosColor hold the string data of each each tile location after it has been called by its respective function, and allows the returned pointer to still function properly after it has been created
// arrPosRent holds the rent value of each property after it has been called by its respective function
// arrCardText and arrCardMid hold the string data discovered by the cardText function after it has been called
char arrPosName[MAX_BOARD_SPACES][100];
char arrPosColor[MAX_BOARD_SPACES][20];
int arrPosRent[MAX_BOARD_SPACES];
char arrCardText[MAX_CARDS][100];
char arrCardMid[MAX_CARDS][20];



//FUNCTIONS

// returns nothing, function sets up starting player balances, positions, names, sets all properties to unowned,
// as well as creates and shuffles a deck of chance cards from the provided board data and a given number of players
// input number of players (int numPlayers), and a seed for the random number generator (unsigned int seed)
void setupBoard(int numPlayers, unsigned int seed){
    seedRandom(seed);

    int startBal;
    switch(numPlayers){             //determine starting balance for each player based on total players: 2 players -> $20, 3 players -> $18, 4 players -> $16
        case 2: startBal = 20;
        break;
        case 3: startBal = 18;
        break;
        case 4: startBal = 16;
        break;
    }

    char* names[] = {"Hazel", "Scottie", "Top Hat", "Toy Car"};     //array of potential names

    for(int i = 0; i < numPlayers; i++){    //set up information for each player that is playing
        pPos[i] = 0;                        //set starting position to 0 (GO)
        pBalance[i] = startBal;             //set starting balance to the determined starting balance

        int nCounter = 0;
        while (names[i][nCounter]){         //copy over the particular name associated with the current player ID element by element into the appropriate index of the global pNames array
            pNames[i][nCounter] = names[i][nCounter];
            nCounter++;
        }
    }

    for(int i = 0; i < MAX_BOARD_SPACES; i++){      //set all tiles to un-owned (-1)
        propOwned[i] = -1;
    }

    for (int i = 0; i < MAX_CARDS; i++){            //create a chance deck consisting of card IDs (indexes) 0-19
        chanceDeck[i] = i;
    }
    for (int i = MAX_CARDS-1; i >= 1; i--) {        //shuffle deck of card ID's
        int j = getRandom(i+1);                //retrieve a random index of the deck array j
        int temp = chanceDeck[i];                   //store cardID value in a temp variable
        chanceDeck[i] = chanceDeck[j];              //assign card ID at the deck's index j to current index of the deck
        chanceDeck[j] = temp;                       //assign stored card ID to the deck's index of j to complete the swap
    }
}

// returns nothing, function moves player and performs/calls all necessary functions and calculations associated with moving around the board and landing on tiles
// input player id (int p), and number of spaces to move (spaces)
void movePlayer(int p, int spaces){
    //in jail
    if((pPos[p] == 6) && (inJail[p] == 1)){     //check that player is at jail tile and not "just visiting"
        printf("In jail, must first pay or use card to leave\n");

        if(JFcard[p] == 1){     //check if player is holding a jail-free card
            JFcard[p] = 0;      //remove card
            replaceJailCard();  //return it to the bottom of the chance deck
            inJail[p] = 0;      //remove jail status
            printf("player has used their get out of jail free card\n");
        }
        else{                   //player is not holding a jail-free card
            pBalance[p] -= 1;   //pay $1 fine
            printf("player does not have a get out of jail free card, pay $1\n");
        }
    }

    pPos[p] += spaces;  //move player "spaces" tiles forward.

    //pass go
    if(pPos[p] > MAX_BOARD_SPACES-1){       //check if player passed or landed on go
        pBalance[p] += 2;                   //collect $2 for passing go
        pPos[p] %= MAX_BOARD_SPACES;        //player went around entire board, begin from start (0) again
    }

    //property tiles
    if(pPos[p] %3 != 0){                                    //true if player has landed on a property tile
        printf("landed on property tile\n");
        if(propOwned[pPos[p]] == -1){                       //check if property is un-owned
            printf("property is unowned, ");
            if(pBalance[p] >= positionRent(pPos[p])) {      //check if player can afford to buy property
                propOwned[pPos[p]] = p;                     //property now belongs to player
                pBalance[p] -= positionRent(pPos[p]);       //pay rent value to purchase
                printf("player buys property for listed price\n");
            }
            else{
                printf("player cannot afford property\n");
            }
        }
        //paying rent
        if((propOwned[pPos[p]] != -1) && (propOwned[pPos[p]] != p)){          //check if property tile is already owned, and owned by another player
            printf("property is owned by another player, must pay rent\n");
            int rent = positionRent(pPos[p]);           //retrieve rent value of property

            pBalance[p] -= rent;                        //reduce player balance by rent
            pBalance[propOwned[pPos[p]]] += rent;       //owner receives rent money
        }
        //player owns property
        else if (propOwned[pPos[p]] != -1){     //property is owned, and not by another player
            printf("player owns this property\n");
        }
    }

    //non-property tiles
    else{
        //go to jail
        if(pPos[p] == 18){      //check if player has landed on "go to jail tile"
            pPos[p] = 6;        //send player directly to jail, does not pass go
            inJail[p] = 1;      //set jail status to 1
            printf("player has landed on go to jail, go directly to jail, do not pass go, do not collect $2\n");
        }
        //chance
        if(strstr(positionName(pPos[p]), "Chance") != NULL){      //check if player has landed on a chance tile
            printf("player has landed on chance, draw a card\n");
            int cardID = drawCard();                        //draw a card from the deck
            printf("%s\n", cardText(cardID));       //print out drawn card's text

            if(cardID == 0){        //if card ID is 0, drawn card is the jail-free card
                JFcard[p] = 1;      //give player the jail-free card
            }
            if(chanceCards[cardID][0] == '+'){              //if chance card identifier is '+', chance card is a "move player" card
                int moveVal = atoi(arrCardMid[cardID]);     //retrieve movement value from card
                movePlayer(p, moveVal);                     //move player forward by specified number of tiles
            }
            if(chanceCards[cardID][0] == 'g'){                    //if chance card identifier is 'g', chance card is an "advance to" card
                if ((atoi(arrCardMid[cardID]) % 3) != 0) {        //check if designated tile is a property tile
                    if (pPos[p] > atoi(arrCardMid[cardID])) {     //check if player receives $2 passing or landing on go to reach destination
                        pBalance[p] += 2;
                        printf("passed go, collect $2\n");
                    }
                    pPos[p] = atoi(arrCardMid[cardID]);     //move player to designated tile

                    if(propOwned[pPos[p]] == -1){           //if property is unowned, player receives property for free
                        propOwned[pPos[p]] = p;
                        printf("player now owns %s\n", positionName(pPos[p]));
                    }
                    else if(propOwned[pPos[p]] != p){                   //check if property is owned by another player
                        printf("property is owned by another player, pay rent\n");
                        pBalance[p] -= positionRent(pPos[p]);           //player pays rent, owner receives the rent
                        pBalance[propOwned[pPos[p]]] += positionRent(pPos[p]);
                    }
                    else{                                               //player already owns the property
                        printf("player already owns this property");
                    }
                }
                else{       //designated tile is a nonproperty tile
                    movePlayer(p, ((MAX_BOARD_SPACES - pPos[p]) + atoi(arrCardMid[cardID])));   //move player as normal and perform associated actions (player distance from 0 + designated tile position)
                }
            }
            if(chanceCards[cardID][0] == '$'){              //if chance card identifier is '$', chance card is a "money" card
                int moneyVal = atoi(arrCardMid[cardID]);    //retrieve money value from card

                if(moneyVal == 1){                  //special case if money value of card is 1
                    int i = 0;
                    int moneySum = 0;
                    while((pPos[i] != -1) && (i < 4)){      //if player does not exist position is -1, loop until all existing players have been affected
                        pBalance[i] -= moneyVal;            //reduce all player balances by the money value
                        moneySum += moneyVal;               //collect the sum of these payments
                        i++;
                    }
                    pBalance[p] += moneySum;        //give player the collected sum
                }
                else {
                    pBalance[p] += moneyVal;
                }
            }
            if(chanceCards[cardID][0] == 'm') {             //if chance card identifier is 'm', chance card is a "move to" card
                if (strstr(arrCardMid[cardID], "free") != NULL) {      //move to next unowned property
                    int count = 0;
                    int posFound = 0;                       //value is 0 until unowned property is found
                    while((posFound == 0) && (count < MAX_BOARD_SPACES)){                           //loop until an unowned property tile has been found or all properties have been checked
                        if((propOwned[pPos[p] + count] == -1) && (((pPos[p] + count) % 3) != 0)){   //check if tile is both unowned and a property tile
                            posFound = 1;                   //nearest unowned property found, end loop

                            propOwned[pPos[p] + count] = p;     //player now owns the property
                            if((pPos[p] + count) > (MAX_BOARD_SPACES - 1)){
                                pBalance[p] += 2;               //if player passes go (pPos exceeds 23) to reach new property, collect $2
                                printf("passed go, collect $2\n");
                            }
                            pPos[p] += count;                   //move player to new property
                            pPos[p] %= MAX_BOARD_SPACES;        //allows player to start from go if position exceeds 23, otherwise does nothing
                            printf("player now owns %s\n", positionName(pPos[p]));
                        }
                        count++;    //evaluate next property if unowned property has not been found
                    }
                }
                else{   //move to color property
                    int count = 0;
                    int posFound = 0;                       //value is 0 until property of specified color has been found
                    while((count < MAX_BOARD_SPACES) && (posFound == 0)){                           //loop through entire board until tile of specified color has been reached or entire board has been evaluated
                        if(strcmp(arrCardMid[cardID], positionColor(pPos[p] + count)) == 0) {   //check if specified color matches property color
                            posFound = 1;                   //property has been found

                            if (propOwned[pPos[p] + count] != -1) {          //if first tile of specified color is owned, check other tile
                                printf("first %s tile is owned, checking next tile\n", arrCardMid[cardID]);
                                count++;

                                if (propOwned[pPos[p] + count] != -1) {         //if other tile of specified color is owned too, pay rent to owner of tile
                                    if ((pPos[p] + count) > (MAX_BOARD_SPACES - 1)) {
                                        pBalance[p] += 2;                       //if player passes go to reach new property, collect $2
                                        printf("passed go, collect $2\n");
                                    }
                                    pPos[p] += count;                   //move player to new property
                                    pPos[p] %= MAX_BOARD_SPACES;        //if player passes go, continue moving from 0, otherwise does nothing

                                    printf("second %s tile is owned by another player as well, must pay rent\n", arrCardMid[cardID]);
                                    int rent = positionRent(pPos[p]);           //get value of property rent

                                    pBalance[p] -= rent;                        //player pays rent
                                    pBalance[propOwned[pPos[p]]] += rent;       //property owner receives rent
                                }
                                else {
                                    propOwned[pPos[p] + count] = p;     //player now owns the property
                                    if ((pPos[p] + count) > (MAX_BOARD_SPACES - 1)) {
                                        pBalance[p] += 2;               //if player passes go to reach new property, collect $2
                                        printf("passed go, collect $2\n");
                                    }
                                    pPos[p] += count;                   //move player to new property
                                    pPos[p] %= MAX_BOARD_SPACES;        //if player passes go, continue moving from 0, otherwise does nothing
                                    printf("second %s tile is unowned, player now owns %s\n", arrCardMid[cardID], positionName(pPos[p]));
                                }
                            }
                            else {
                                propOwned[pPos[p] + count] = p;     //player now owns the property
                                if ((pPos[p] + count) > (MAX_BOARD_SPACES - 1)) {
                                    pBalance[p] += 2;               //if player passes go to reach new property, collect $2
                                    printf("passed go, collect $2\n");
                                }
                                pPos[p] += count;                   //move player to new property
                                pPos[p] %= MAX_BOARD_SPACES;        //if player passes go, continue moving from 0, otherwise does nothing
                                printf("%s tile is unowned, player now owns %s\n", arrCardMid[cardID], positionName(pPos[p]));
                            }
                        }
                    count++;    //evaluate next tile if specified color has not been found
                    }
                }
            }
        }
    }
}

//returns player ID of the first player with highest balance as winner once a player or players have a balance equal to or less than zero, otherwise returns -1 to show there is no winner yet
//input number of current players (int numPlayers)
int findWinner(int numPlayers){
    int testPosBal = 1;        //if value is 1 after the for loop, all players have a positive balance, set to 0 if any player has a zero or negative balance
    int pIDMaxBal = 0;         //variable holds player ID of player with the highest balance

    for(int i = 0; i < numPlayers; i++){
        if(pBalance[i] <= 0)        //if a player has a zero or negative balance, set testZeroBal to zero
            testPosBal = 0;
        if(pBalance[i] > pBalance[pIDMaxBal])       //set pIDMaxBal to the ID of the player with the highest balance found so far
            pIDMaxBal = i;
    }
    if(testPosBal == 0){   //if testZeroBal = 0, one or more players have a zero/negative balance, return ID of player with highest balance
        return pIDMaxBal;
    }
    else{                   //if testZeroBal = 1, all players are still playing with a positive balance, no winner yet, return -1
        return -1;
    }
}

// player info functions

// returns a char pointer pointing to the name associated with the specified player ID
// input player id (int p)
const char * playerName(int p){
    return pNames[p];       //returns name stored in global array at the player ID
}

// returns the balance associated with the specified player ID
// input player id (int p)
int playerBalance(int p){
    return pBalance[p];     //returns balance stored in global array at the player ID
}

// returns the position of the player associated with the specified player ID
// input player id (int p)
int playerPosition(int p){
    return pPos[p];         //returns position stored in global array at the player ID
}



// board info functions

// returns a char pointer pointing to the name associated with a given tile of the board
// input board position (int pos)
const char * positionName(int pos) {
    int counter = 0;            //counter to move through the boardSpaces array
    int i = 0;                  //counter to move through array holding desired information

    while (boardSpaces[pos][counter] != ':') {       //finds position of first colon, stores in counter
        counter++;
    }
    counter++;                                       //move counter 1 to the right of the first colon
    while (boardSpaces[pos][counter] != ':') {       //finds position of second colon, stores in counter
        counter++;
    }
    counter++;                                       //move counter 1 to the right of the second colon

    while (boardSpaces[pos][counter]) {              //copy every character to global temp array until the null terminator is reached to find the position name
        arrPosName[pos][i] = boardSpaces[pos][counter];
        i++;
        counter++;
    }
    arrPosName[pos][i] = '\0';              //add null char to end of the string
    char *posNamePtr = arrPosName[pos];     //set a pointer to the global array
    return posNamePtr;                      //return pointer
}

// returns integer ownership value of a given tile of the board (-1->Unowned or Not a Property Tile, 0-3->Player ID of Owner)
// input board position (int pos)
int positionOwner(int pos){
    return propOwned[pos];      //returns player ID (or -1 value if unowned/not property) stored in global array
}

// returns integer rent value of a given tile of the board
// input board position (int pos)
int positionRent(int pos){
    int counter = 0;        //counter to count through boardSpaces array
    int i = 0;              //counter to move through and assign to the indexes of the tempArray
    char tempArr[25];       //temporary array to hold chars of rent value
    while (boardSpaces[pos][counter] != ':') {       //finds position of first colon, stores in counter
        counter++;
    }
    counter++;

    while (boardSpaces[pos][counter] != ':') {       //copy every character to global rent array until the second colon is reached to find the rent chars
        tempArr[i] = boardSpaces[pos][counter];
        i++;            //increment tempArray counter
        counter++;      //increment boardSpaces counter
    }
    tempArr[i] = '\0';              //add null char to end of the string
    int rentVal = atoi(tempArr);    //convert chars in tempArray holder to an integer value

    if ((propOwned[pos] == propOwned[pos - 1] || propOwned[pos] == propOwned[pos + 1]) && propOwned[pos] != -1) {   //checks owners of tiles to the left and right of current tile
        rentVal *= 2;                                                                                               //if both adjacent property tiles are owned by the same player, pay double rent
        printf("another player owns both tiles of this color, pay double rent\n");
    }
    arrPosRent[pos] = rentVal;  //store value of property's rent into the global array

    return arrPosRent[pos];     //return rent value
}

// returns a char pointer pointing to the color associated with a given tile of the board
// input board position (int pos)
const char * positionColor(int pos){
    int i = 0;
    while(boardSpaces[pos][i] != ':') {              //copy every character to global temp array until the first colon to find color.
        arrPosColor[pos][i] = boardSpaces[pos][i];
        i++;
    }
    arrPosColor[pos][i] = '\0';               //add null char to end of the string
    char* posColorPtr = arrPosColor[pos];     //set a pointer to the global array
    return posColorPtr;                       //return pointer
}


// chance card functions

// returns integer card ID of the top card of the deck and returns it to the bottom, if Jail-Free card is drawn, "remove" it from the deck
// no input
int drawCard(){
    int topCard = chanceDeck[0];            //hold on to top card
    for(int i = 1; i < deckSize; i++){
        chanceDeck[i-1] = chanceDeck[i];    //shift all cards 1 spot up
    }
    chanceDeck[deckSize-1] = topCard;       //place drawn card at the bottom of the deck
    if(topCard == 0){                       //if get out of jail free card drawn, "keep" the card
        deckSize -= 1;                      //reduce size of deck by one, so drawn jail-free card is not included
    }

    return topCard;         //return drawn card's ID;
}

// returns a char pointer pointing to the card text associated with a given chance card's ID, retrieves and stores middle text of the card data in the process as well
// input card ID (int card)
const char * cardText(int card){
    int counter = 0;                //counter to move through chanceCard array data at specific index
    while(chanceCards[card][counter] != ':'){   //increment counter until first colon is reached
        counter++;
    }
    counter++;                      //move counter past first colon

    int i = 0;                      //initialize counter to store into indexes of global data arrays
    while(chanceCards[card][counter] != ':'){
        arrCardMid[card][i] = chanceCards[card][counter];   //store chars of middle text into arrCardMid global array until second colon is reached
        counter++;                  //increment counters
        i++;
    }
    arrCardMid[card][i] = '\0';     //add null terminator to the end of the middle card text stored in global array
    counter++;                      //move counter past second colon

    i = 0;                          //reset global array index counter back to 0
    while(chanceCards[card][counter]){                          //move counter through chanceCard data at card index until null terminator at end is reached
        arrCardText[card][i] = chanceCards[card][counter];      //copy data into card text global data array as counter increments
        counter++;                  //increment counters
        i++;
    }
    arrCardText[card][i] = '\0';            //add null terminator to the end of the card text stored in global array
    char* cardTextPtr = arrCardText[card];  //set pointer to point to the new card text data stored in global array
    return cardTextPtr;                     //return pointer
}

// "returns" the Jail-Free card to the deck
// no input
void replaceJailCard(){
    deckSize += 1;          //return size of deck to original size, will now include jail-free card at bottom as part of the deck again
}


// functions for testing purposes

//returns nothing, sets a player's location to a specified value
//input player ID (int player), and position value (int pos)
void setPlayerPosition(int player, int pos){
    pPos[player] = pos;
}

//returns nothing, sets a player's balance to a specified value
//input player ID (int player), and balance value (int dollars)
void setPlayerBalance(int player, int dollars){
    pBalance[player] = dollars;
}

//returns nothing, sets a property's ownership value to a specified value (player ID or -1)
//input tile position (int pos), tile value (int player)
void setPositionOwner(int pos, int player){
    propOwned[pos] = player;
}

//returns card ID of card at the top of the deck
//input nothing
int peekTopDeck(){
    return chanceDeck[0];       //return card ID of card at deck index 0
}

//returns card ID of card at the bottom of the deck
//input nothing
int peekBottomDeck(){
    return chanceDeck[deckSize-1];      //return card ID of the last card ID element in the current working deck
}

//returns card ID of card at specific index of the deck
//input index of deck (int c)
int peekDeck(int c){
    return chanceDeck[c];       //return card ID of card at the specific deck index c
}