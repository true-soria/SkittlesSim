#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <omp.h>
#include "SkittlesSim.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
#define NUM_OF_SKITTLES 60
#define SIMS_ALLOWED 1000

int main() {

    /**
     * parallelize the whole freaking Sim
     * k then parallelize a simulation of simulations
     */

    double resultOfResults = 0;
    int simSetNum = 0;

    while (true) {
#pragma omp parallel for
            for (int threadOfSims = 0; threadOfSims < omp_get_num_threads(); ++threadOfSims) {

                printf("%d\n", omp_get_num_threads());

                double result = 0; // result of SIMS_ALLOWED simulations ran

                // Executes SIMS_ALLOWED number of simulations  and returns the average
                for (short simNum = 0; simNum < SIMS_ALLOWED; ++simNum) {

                    // set up dummy first node for chain of skittles bags
                    SKITTLES_BAG *head = calloc(sizeof(SKITTLES_BAG), 1);
                    bool dupeFound = false;
                    srand(simNum * threadOfSims + omp_get_thread_num());

                    // finish after simNum bags
                    for (short currentBagNum = 1; !dupeFound; ++currentBagNum) {

                        // generate a bag of skittles
                        // make a skittles bag structure
                        // initialize to "aaaaa"
                        // set bag number to currentBagNum

                        SKITTLES_BAG *newBag = calloc(sizeof(SKITTLES_BAG), 1);
                        strcpy(newBag->bag, "aaaaa\0");

                        // for each skittle, give it an equal chance to be any color and add it to
                        for (short i = 0; i < NUM_OF_SKITTLES; ++i) {
                            newBag->bag[(rand() % NUM_OF_FLAVORS)]++;
                        }

                        // add new skittle bag to the current list, then compare

                        newBag->nextBag = head;
                        head = newBag;

                        SKITTLES_BAG *otherBag = head->nextBag;

                        // check newest bag against all previous bags to see if we've found a duplicate
                        // if so, contribute the number of bags it took to the result and quit this sim
                        while (!dupeFound && otherBag->nextBag != NULL) {
                            // if (otherBag->bagNumber == (simNum + 1))
                            if (strcmp(head->bag, otherBag->bag) == 0) // uncomment later
                            {
                                result += currentBagNum;
                                dupeFound = true;
                            } else otherBag = otherBag->nextBag;
                        }
                    } // END of bag generation/comparison

                    // free bag memory allocation
                    SKITTLES_BAG *previousBag;
                    while (head != NULL) {
                        previousBag = head;
                        head = head->nextBag;
                        free(previousBag);
                    }

                } // END of SIM

#pragma omp atomic update
                resultOfResults += (result / (SIMS_ALLOWED * omp_get_num_threads()));
            } // END of Thread set of Sims (and parallel section)

        simSetNum++;
        printf("Simulation set %d complete: \n\tCurrent Average: %lf\n\n", simSetNum,
               (double) (resultOfResults / simSetNum));
    }
}

#pragma clang diagnostic pop