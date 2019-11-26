# SkittlesSimulation
Computer Architecture Final Project By True Soria and Nathan Moreno

## Issue 1: Average Function
The average must not allow for 3 things:
 - Overflow
 - averaging 0 values
 - impricision

  If we assume it takes on average 400 bags to run the simulation, through traditional averaging when getting the sum we will overflow at about 5 million simulations using an int. This can be extended using a ulong, but we don't know how many sims can run in 2 minutes. A double would remove this issue, but precision may be lost.

## Issue 2: Memory
This is going to be a fine line to toe. We will definitely need to free memory at the end of each sim, but that is going to cost time. a necessary loss.
