//
// Created by true on 11/14/19.
//

#ifndef SKITTLESSIM_SKITTLESSIM_H
#define SKITTLESSIM_SKITTLESSIM_H


// make a skittles bag node for chain of nodes
    // must contain char* representing count of 5

typedef struct skittlesBag
{
    char bag[6];
    struct skittlesBag *nextBag;
} SKITTLES_BAG;






#endif //SKITTLESSIM_SKITTLESSIM_H
