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


int main() {

    /**
     * parallelize the whole freaking Sim
     * k then parallelize a simulation of simulations
     */

//    omp_set_num_threads(6);
    double resultOfResults = 0;
    int simSetNum = 0;
    double startTime = omp_get_wtime();

    // seed random numbers
    uint seed[10];
    for (int k = 0; k < 10; ++k) {
        seed[k] = rand();
    }


    while (true) {
        //
#pragma omp parallel shared(seed)
        {
            int numOfThreads = omp_get_num_threads();
//            printf("%d\n", omp_get_thread_num());

#pragma omp for reduction(+: resultOfResults) schedule(static)
            for (int threadOfSims = 0; threadOfSims < numOfThreads; ++threadOfSims) {
                for (int simNum = 0; simNum < SIMS_ALLOWED; ++simNum) {

                    double result = 0; // result of SIMS_ALLOWED simulations ran
                    char bagArray[BAGS_ALLOWED][NUM_OF_FLAVORS + 1];
                    bool dupeFound = false;
                    int thisThread = omp_get_thread_num();

                    // finish after simNum bags
//                    printf("dupeFound = %d, %d\n", dupeFound, true);
                    for (short currentBagNum = 0; !dupeFound; ++currentBagNum) {

                        // generate a bag of skittles
                        // make a skittles bag structure
                        // initialize to "aaaaa"
                        // set bag number to currentBagNum

                        // for each skittle, give it an equal chance to be any color and add it to
                        strcpy(bagArray[currentBagNum], "aaaaa\0");
                        for (short i = 0; i < NUM_OF_SKITTLES; ++i) {
                            bagArray[currentBagNum][(seed[thisThread] % NUM_OF_FLAVORS)]++;
                            seed[thisThread] = ((seed[thisThread] * 1103515245U) + 12345U) & 0x7fffffff;

//                            bagArray[currentBagNum][(rand() % NUM_OF_FLAVORS)]++;

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

//                    printf("thread %d has done %d simulations: result = %.0lf \n", omp_get_thread_num(), simNum + 1, result);
                    resultOfResults += (result / ((SIMS_ALLOWED) * numOfThreads));

                }
            }
        }// END of Thread set of Sims (and parallel section)

        simSetNum++;
        printf("Simulation set %d complete:\n\tSimulations Ran: %d per thread \n\tCurrent Average: %lf\n\tCurrent Time: %lf\n\n",
                simSetNum, SIMS_ALLOWED * simSetNum,(double) (resultOfResults / simSetNum), omp_get_wtime() - startTime);
    }
}

#pragma clang diagnostic pop