#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <omp.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
#define NUM_OF_SKITTLES 60
#define SIMS_ALLOWED 10000
#define BAGS_ALLOWED 1000
#define NUM_OF_FLAVORS 5
#define NUM_OF_SEEDS 32


int main() {

    /** SECTION 1: SERIAL
     * In this section, start time is initialized as well as
     * counters for the number of sets completed and the
     * overall running average known as resultOfResults.
     */
    long double resultOfResults = 0;
    int simSetNum = 0;
    double startTime = omp_get_wtime();

    /** RANDOM
     * because of the undefined interaction between parallel programs
     * and rand(), this function is needed for generating random numbers.
     * each thread gets its own unique index from the seed array and
     * every time it is used, the seed is updated using gcc's normal
     * rand() function's algorithm.
     */
    uint seed[NUM_OF_SEEDS];
    for (int k = 0; k < NUM_OF_SEEDS; ++k) {
        seed[k] = rand();
    }

    /**
     * Batches of parallelized simulations are ran infinitely
     */
    while (true) {
#pragma omp parallel shared(seed)
        {
            int numOfThreads = omp_get_num_threads();
//            printf("%d\n", omp_get_thread_num()); // for demonstration purposes only

            /** SECTION 2: SIMULATIONS
             * Each thread runs the defined number of simulations and contributes
             * the final average to resultOfResults. this is done using omp's
             * reduction function. schedule is set to static because we want
             * each thread to get exactly one set of sims.
             */

#pragma omp for reduction(+: resultOfResults) schedule(static)
            for (int threadOfSims = 0; threadOfSims < numOfThreads; ++threadOfSims) {
                for (int simNum = 0; simNum < SIMS_ALLOWED; ++simNum) {

                    double result = 0;
                    char bagArray[BAGS_ALLOWED][NUM_OF_FLAVORS + 1];
                    bool dupeFound = false;
                    int thisThread = omp_get_thread_num();

                    /** SECTION 3: THE SIMULATION
                     * a running tally known as results keeps track of the
                     * sum of number of bags it took before there was a
                     * duplicate per run. It then gets divided by the number
                     * of simulations this thread ran before being added to
                     * resultOfResult.
                     */
                    for (short currentBagNum = 0; !dupeFound; ++currentBagNum) {

                        /**
                         * This runs for loop until a duplicate is found.
                         * The idea is to store the count of "skittles"
                         * to a string "bag" and compare it with previously
                         * generated bags.
                         *
                         * This is where the above described RANDOM is used.
                         * for each skittle, give it an equal chance to be
                         * any color and add it to the bag.
                         */
                        strcpy(bagArray[currentBagNum], "aaaaa\0");
                        for (short i = 0; i < NUM_OF_SKITTLES; ++i) {
                            bagArray[currentBagNum][(seed[thisThread] % NUM_OF_FLAVORS)]++;
                            seed[thisThread] = ((seed[thisThread] * 1103515245U) + 12345U) & 0x7fffffff;
                        }

                         /** check newest bag against all previous bags to see
                         * if we've found a duplicate if so, contribute the
                         * number of bags it took to the result and quit this sim */
                        for (int j = 0; j < currentBagNum; ++j) {
                            if (strcmp(bagArray[currentBagNum], bagArray[j]) == 0)
                            {
                                result += currentBagNum;
                                dupeFound = true;
                            }
                        }
                    } // END of SECTION 3: a defined number of simulations

//                    printf("thread %d has done %d simulations: result = %.0lf \n", omp_get_thread_num(), simNum + 1, result); // debug print statement
                    resultOfResults += (result / ((SIMS_ALLOWED) * numOfThreads));

                }
            }
        } // END of SECTION 2: Thread set of Sims (and parallel section)

        /** SECTION 1 CONT.
         * Here, the results of the thread set of simulations is reported and
         * the count of sets ran is updated. The running average gets printed
         * here.
         *
         * side node: for some odd reason, the first version of printf nearly
         * halves the speed of the program.
         */

        simSetNum++;
        printf("Simulation set %d complete:\n",
                simSetNum);
        printf("\tSimulations Ran: %d per thread \n\tCurrent Average: %lf\n\tCurrent Time: %lf\n\n",
                SIMS_ALLOWED * simSetNum,(double) (resultOfResults / simSetNum), omp_get_wtime() - startTime);
//        printf("Simulation set %d complete:\n\tCurrent Average: %lf\n\tCurrent Time: %lf\n\n",
//               simSetNum, (double) (resultOfResults / simSetNum), omp_get_wtime() - startTime);
    }
}

#pragma clang diagnostic pop