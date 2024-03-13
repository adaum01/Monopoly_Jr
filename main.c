// ECE 209, Spring 2021 - Program 2
// Monopoly Junior board game
// Up to four players, rules adapted from standard Monopoly Junior from Hasbro
//Alex Daum

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "random.h"
#include "monopoly.h"

int main() {
    setupBoard(4, 19);
    printf("%d\n", peekTopDeck());
    printf("%d\n", playerBalance(0));
    printf("%d\n", playerBalance(1));
    printf("%d\n", playerBalance(2));
    printf("%d\n", playerBalance(3));

    movePlayer(2, 3);

    printf("%s\n", positionName(playerPosition(2)));

    printf("%d\n", playerBalance(0));
    printf("%d\n", playerBalance(1));
    printf("%d\n", playerBalance(2));
    printf("%d\n", playerBalance(3));
}