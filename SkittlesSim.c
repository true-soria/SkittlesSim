#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <omp.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
#define NUM_OF_SKITTLES 60
#define SIMS_ALLOWED 1000
#define BAGS_ALLOWED 1000
#define NUM_OF_FLAVORS 5


int main() {

    /**
     * parallelize the whole freaking Sim
     * k then parallelize a simulation of simulations
     */

    double resultOfResults = 0;
    int simSetNum = 0;
    double startTime = omp_get_wtime();

    while (true) {
#pragma omp parallel for
            for (int threadOfSims = 0; threadOfSims < omp_get_num_threads(); ++threadOfSims) {

                printf("%d\n", omp_get_thread_num());

                double result = 0; // result of SIMS_ALLOWED simulations ran
                char bagArray[BAGS_ALLOWED][NUM_OF_FLAVORS + 1];

                // Executes SIMS_ALLOWED number of simulations  and returns the average
                for (short simNum = 0; simNum < SIMS_ALLOWED; ++simNum) {

                    bool dupeFound = false;
                    srand(simNum * threadOfSims + omp_get_thread_num());

                    // finish after simNum bags
                    for (short currentBagNum = 0; !dupeFound; ++currentBagNum) {

                        // generate a bag of skittles
                        // make a skittles bag structure
                        // initialize to "aaaaa"
                        // set bag number to currentBagNum

                        // for each skittle, give it an equal chance to be any color and add it to
                        strcpy(bagArray[currentBagNum],"aaaaa\0");
                        for (short i = 0; i < NUM_OF_SKITTLES; ++i) {
                            bagArray[currentBagNum][(rand() % NUM_OF_FLAVORS)]++;
                        }

                        // check newest bag against all previous bags to see if we've found a duplicate
                        // if so, contribute the number of bags it took to the result and quit this sim
                        for (int j = 0; j < currentBagNum; ++j) {
                            // if (otherBag->bagNumber == (simNum + 1))
                            if (strcmp(bagArray[currentBagNum], bagArray[j]) == 0) // uncomment later
                            {
                                result += currentBagNum;
                                dupeFound = true;
                            }
                        }
                    } // END of bag generation/comparison

                } // END of individual SIM

#pragma omp critical
                resultOfResults += (result / (SIMS_ALLOWED * omp_get_num_threads()));
//                printf(" ");
            } // END of Thread set of Sims (and parallel section)

        simSetNum++;
        printf("Simulation set %d complete: \n\tCurrent Average: %lf\n\tCurrent Time: %lf\n\n", simSetNum,
               (double) (resultOfResults / simSetNum), omp_get_wtime() - startTime);
    }
}

#pragma clang diagnostic pop