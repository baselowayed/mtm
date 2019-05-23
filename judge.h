
#ifndef JUDGE_LIBRARY_H
#define JUDGE_LIBRARY_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include "map.h"
/**
*
* Implements a state container type.
* The state has 3 parameters.
* the functions we built help us get the internal Data ,update it and check if it is legal
*
* The following functions are available:
*
*   createJudge               - Creates a new judge
*   destroyJudge            - Deletes an existing judge and frees all resources
*   copyJudge               - Returns a copy of an existing judge
*   destroyJudgeElment     -Deletes/frees an existing MapDataElement
*   getJudgeVotes          -Returns the votes of the judge
*/



/** Type for defining the judge */
typedef struct judge *Judge;


/** Type used for returning error codes from judge functions */
typedef enum return_values {
    JUDGE_SUCCESS,             /* Operation succeeded*/
    JUDGE_NULL_ARGUMENT,       /* A NULL argument was passed*/
    JUDGE_OUT_OF_MEMORY,        /*an allocation failed*/
    JUDGE_INVALID_NAME,        /* judge's name is invalid*/
    JUDGE_INVALID_ID,       /* judge's ID is invalid*/
}operation_result;




/**
 * createJudge: Allocates a new judge with the details that are given and update
*               the result according to the arguments and allocations
 * @param name  -the name of the judge we allocated
 * @param votes  - 10 elements array includes the id'es of the states that he wants
 *                  to give points according to the index
*               i=0 -> 12points   i=1 -> 10 points   i=2 -> 8  i=3 -> 7 ....
 * @param result1  -it is updated to the result of the operation
 *                  -JUDGE_NULL_ARGUMENT:in case we received a null argument
 *                  -JUDGE_INVALID_NAME: in case the name we got was invalid
 *                  -JUDGE_OUT_OF_MEMORY:in case an allocation failed
 *                  -JUDGE_SUCCESS: in case we aloocated a new judge successfully
 * return           new_judge: if we allocated a new judge successfully
 *                  NULL - if one of the parameters is NULL or allocations failed.
 */
Judge createJudge(char* name, int* votes,operation_result* result1);



/**
 * destroyJudge : Deletes an existing judge and frees all resources
 * @param judge - the judge we wanna free after we free its elements
 */
void destroyJudge(Judge judge);

/**
 * copyJudge         - Returns a copy of an existing judge
 * @param judge      -the judge we want to copy
 * @return           -a copy of the judge we created/allocated
 */
MapDataElement copyJudge(MapDataElement judge);


/**
 * destroyJudgeElment : Deletes/frees an existing MapDataElement
 * @param unwanted_judge   -this is the elment that the func frees
 */
void destroyJudgeElment(MapDataElement unwanted_judge);


/**
 * getJudgeVotes:Returns the votes of the judge
 * @param judge  -the judge we search for his votes and returns it
 * @return       -judge->votes:if its not NULL we return it
 */
int* getJudgeVotes(Judge judge);

/**
 * JudgeVoted:Checks if the judge voted to a specific id of state
 * @param judge  -the judge
 *               - the id of the state
 * @return       -true if the judge has voted
 *               -false if the judge has not voted.
 */
bool JudgeVoted(Judge judge,int id);


#endif //JUDGE_LIBRARY_H
