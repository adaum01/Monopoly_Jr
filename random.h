// ECE 209, Spring 2021 - Program 2
// random numbers

#ifndef RANDOM_H
#define RANDOM_H

// set starting value for LFSR-based random number generator
void seedRandom(unsigned int seed);

// return a random integer between 0 and limit-1
unsigned int getRandom(unsigned int limit);

// lfsrNext -- function to advance an LFSR for pseudorandom number generation
// uses global variable (lfsr)
// code from: https://en.wikipedia.org/wiki/Linear-feedback_shift_register
unsigned int lfsrNext();


#endif // RANDOM_H
