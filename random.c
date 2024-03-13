// ECE 209, Spring 2021 - Program 2
// random numbers

#include "random.h"

// global variable to keep state for random number generator
static int lfsr = 0;

// set starting value for LFSR-based random number generator
void seedRandom(unsigned int seed) {
    if (seed) lfsr = seed;
}

// return a random integer between 0 and limit-1
unsigned int getRandom(unsigned int limit) {
    return lfsrNext() % limit;  //
}

// lfsrNext -- function to advance an LFSR for pseudorandom number generation
// uses global variable (lfsr)
// code from: https://en.wikipedia.org/wiki/Linear-feedback_shift_register
unsigned int lfsrNext(void) {
    lfsr ^= lfsr >> 7;
    lfsr ^= lfsr << 9;
    lfsr ^= lfsr >> 13;
    return lfsr;
}
