#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "SkittlesSim.h"

#define NUM_OF_SKITTLES 60
#define SIMS_ALLOWED 1000

int main() {

    /**
     * parallelize the whole freaking Sim
     */

    double simResults[SIMS_ALLOWED] = {0};

// #pragma omp parallel for
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    for (int simNum = 0; true ; ++simNum) {

        // set up dummy first node for chain of skittles bags
        SKITTLES_BAG *head = calloc(sizeof(SKITTLES_BAG), 1);
        bool dupeFound = false;
        srand(simNum);

        // finish after simNum bags
        for (int currentBagNum = 1; !dupeFound ; ++currentBagNum) {

            // generate a bag of skittles
            // make a skittles bag structure
            // initialize to "aaaaa"
            // set bag number to currentBagNum
            SKITTLES_BAG *newBag = calloc(sizeof(SKITTLES_BAG), 1);
            newBag->bagNumber = currentBagNum;
            strcpy(newBag->bag, "aaaaa\0");

            // for each skittle, give it an equal chance to be any color and add it to
            for (int i = 0; i < NUM_OF_SKITTLES; ++i) {
                newBag->bag[(rand() % NUM_OF_FLAVORS)]++;
            }

            // add new skittle bag to the current list, then compare

            newBag->nextBag = head;
            head = newBag;

            SKITTLES_BAG *otherBag = head->nextBag;

            // check newest bag against all previous bags to see if we've found a duplicate
            // if so, contribute the number of bags it took to the simResults array and quit this sim
            while (!dupeFound && otherBag->nextBag != NULL)
            {
                if (strcmp(head->bag, otherBag->bag) == 0)
                {
                    simResults[simNum] = currentBagNum;
                    dupeFound = true;
                }
            }
        } // END of bag generation/comparison

        // write final result to file

        // free bag memory allocation

    } // END of SIM
#pragma clang diagnostic pop


}