#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <omp.h>
#include "SkittlesSim.h"

#define NUM_OF_SKITTLES 60
#define SIMS_ALLOWED 10000



int main() {

    /**
     * parallelize the whole freaking Sim
     * k then parallelize a simulation of simulations
     */

long double resultOfResults = 0;
omp_set_num_threads(20);
double simsdone = 0;

#pragma omp parallel for // TODO this doesn't work
    for (long simOfSims = 0; simOfSims < SIMS_ALLOWED - 1  ; ++simOfSims) {

        double result = 0; // result of SIMS_ALLOWED simulations ran

        // Executes SIMS_ALLOWED number of simulations  and returns the average
        for (short simNum = 0; simNum < SIMS_ALLOWED; ++simNum) {

            // set up dummy first node for chain of skittles bags
            SKITTLES_BAG *head = calloc(sizeof(SKITTLES_BAG), 1);
            bool dupeFound = false;
            short duplicate = 0;
//            srand(simNum * simOfSims + omp_get_thread_num());
            srand(simOfSims * simOfSims + simNum);

            // finish after simNum bags
            for (short currentBagNum = 1; !dupeFound; ++currentBagNum) {

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
                // if so, contribute the number of bags it took to the result and quit this sim
                while (!dupeFound && otherBag->nextBag != NULL) {
                    // if (otherBag->bagNumber == (simNum + 1))
                    if (strcmp(head->bag, otherBag->bag) == 0) // uncomment later
                    {
                        result += currentBagNum;
                        dupeFound = true;
                        duplicate = otherBag->bagNumber;
                    } else otherBag = otherBag->nextBag;
                }
            } // END of bag generation/comparison

            //prints the sub average after every bag count achieved
//            printf("    Simulation %lu-%lu complete: Bags %u and %u were duplicates\n"
//                   "\t    Sub Avg: %lf\n", simOfSims, simNum, duplicate, head->bagNumber,  result / (double)(simNum + 1));

            // free bag memory allocation
            SKITTLES_BAG *previousBag;
            while (head != NULL)
            {
                previousBag = head;
                head = head->nextBag;
                free(previousBag);
            }

        } // END of SIM

#pragma omp critical
        simsdone++;
        resultOfResults += result / SIMS_ALLOWED;
        printf("resultOfResults: %Lf\n", resultOfResults);
        printf("Simulation %lu complete:\n\tCurrent Avg: %lf\n", simOfSims,  (double) (resultOfResults / (simsdone)));
    } // END of SIM of SIMS
}