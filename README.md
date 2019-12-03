# SkittlesSimulation
Computer Architecture Final Project By True Soria and Nathan Moreno

## Issue 1: Average Function
The average must not allow for 3 things:
 - Overflow
 - averaging 0 values
 - impricision

If we assume it takes on average 400 bags to run the simulation, through traditional averaging when getting the sum we will overflow at about 5 million simulations using an int. This can be extended using a ulong, but we don't know how many sims can run in 2 minutes. A double would remove this issue, but precision may be lost.
  
We have two options for storing results:

option 1:
store results to an outside array where each index is specific to a sim. This leads to no race conditions, but it does leave some indexes blank while they are still processing. This is the most accurate while the number of simulations is still small, but it becomes slower and runs into memory issues when the number of sims scales higher. 

option 2:
store results to an outside array where each index represents a result and its value a frequency distribution. This will lead to an array with a static and likely guaranteed size. Memory will be a minimal issue. It ideally then has a static efficiency of O(n) with very minimal computation in every step. This will take a lot of brain power, if it's even possible to properly implement.

## Issue 2: Random
"Random" number generation is based on an algorithm. With no call to srand(), the sequence is constantly the same. Generally, the seed chosen is the current time, but it is possible that while running in parallel two threads will check the time at the same time. We also have access to the Sim number and thread number for random generation. 

## Issue 3: Memory
This is going to be a fine line to toe. We will definitely need to free memory at the end of each sim, but that is going to cost time. a necessary loss.

# Update 12/3

## Issue 4: Parallelization
while many issues have been resolved, the parallelization needs to be worked out. You can't parallelize an infinitely looped for loop. Will need to look into solutions.
