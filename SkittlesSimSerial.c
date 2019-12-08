#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
#define NUM_OF_SKITTLES 60
#define SIMS_ALLOWED 10000
#define BAGS_ALLOWED 1000
#define NUM_OF_FLAVORS 5
#define THREADS 8

int main() {

    /** SECTION 1: SERIAL
     * This version of the code is an entirely serialized version
     * of the program. Though there are some optimizations, for
     * instance this can freely use rand(), this implementation
     * is nearly identical, but still in a way that does not
     * hinder its own performance.
     */
    long double resultOfResults = 0;
    int simSetNum = 0;
    double startTime = omp_get_wtime();

    while (true) {
        for (int threadOfSims = 0; threadOfSims < THREADS; ++threadOfSims) {

            double result = 0;

            for (short simNum = 0; simNum < SIMS_ALLOWED; ++simNum) {

                bool dupeFound = false;
                char bagArray[BAGS_ALLOWED][NUM_OF_FLAVORS + 1];
                srand((simNum + 1) * (threadOfSims + time(NULL)));

                for (short currentBagNum = 0; !dupeFound; ++currentBagNum) {

                    strcpy(bagArray[currentBagNum], "aaaaa\0");

                    for (short i = 0; i < NUM_OF_SKITTLES; ++i) {
                        bagArray[currentBagNum][(rand() % NUM_OF_FLAVORS)]++;
                    }

                    for (int j = 0; j < currentBagNum; ++j) {
                        if (strcmp(bagArray[currentBagNum], bagArray[j]) == 0) {
                            result += currentBagNum;
                            dupeFound = true;
                        }
                    } // END of bag generation/comparison

                }
            } // END of SIM

            resultOfResults += (result / (SIMS_ALLOWED * THREADS));
            printf("%d simulations ran.\n", SIMS_ALLOWED * (threadOfSims + 1));
        } // END of Thread set of Sims (and parallel section)

        simSetNum++;
        printf("Simulation set %d complete: \n\tCurrent Average: %lf\n\tCurrent Time: %lf\n\n", simSetNum,
               (double) (resultOfResults / simSetNum), omp_get_wtime() - startTime);
    }
}

#pragma clang diagnostic pop