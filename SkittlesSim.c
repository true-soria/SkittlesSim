#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "SkittlesSim.h"

#define NUM_OF_SKITTLES 60
#define SIMS_ALLOWED 10
#define SUB_AVG_SIZE 4

int main() {

    /**
     * parallelize the whole freaking Sim
     */

    double simResults[SIMS_ALLOWED] = {0};

// #pragma omp parallel for
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    for (ulong simNum = 0; true ; ++simNum) {

        // set up dummy first node for chain of skittles bags
        SKITTLES_BAG *head = calloc(sizeof(SKITTLES_BAG), 1);
        bool dupeFound = false;
        ushort duplicate = 0;
        srand(simNum);

        // finish after simNum bags
        for (ushort currentBagNum = 1; !dupeFound ; ++currentBagNum) {

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
                if (otherBag->bagNumber == (simNum + 1))
                // if (strcmp(head->bag, otherBag->bag) == 0) // uncomment later
                {
                    simResults[simNum] = currentBagNum;
                    dupeFound = true;
                    duplicate = otherBag->bagNumber;
                } else otherBag = otherBag->nextBag;
            }
        } // END of bag generation/comparison

        // Calculate Current Average

        double average = 0;
        ulong averageCount = 0;


        for (ulong j = 0; j < simNum; ++j) {
            if (simResults[j] == 0)
            {
                average += simResults[j];
                averageCount ++;
            }
        }

        average /= averageCount;


        // write final result to file
        /*
#pragma omp critical
        {

        };
        */

        // Example output:

        printf("Simulation %lu complete: Bags %u and %u were duplicates\n"
               "\tCurrent Avg: %lf\n", simNum, duplicate, head->bagNumber, average);

        // free bag memory allocation

    } // END of SIM
#pragma clang diagnostic pop


}