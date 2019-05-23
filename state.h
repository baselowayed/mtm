//
// Created by Caroline Jubran on 2019-04-23.
//

#ifndef HW1_STATE_H
#define HW1_STATE_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "map.h"
#include "list.h"

/**
*
* Implements a state container type.
* The state has 3 parameters.
* the functions we built help us get the internal Data ,update it and check if it is legal
*
* The following functions are available:
*
*   StateCreate		        - Creates a new State
*   DestroyState            - Deletes an existing State and frees all resources
*   copyIntDataElement      - Returns an existing State
*   freeIntDataElement	    - frees an int data element
*   compareIntKeyElement	- compares 2 integers
*   copyIntElement  	    - Returns an int element
*	freeIntElement          -frees an int element
*   DestroyStateElemnt		-Deletes an existing MapDataElement and frees all resources
*   StateGetName            -Returns the name of the State
*   GetMapOfVotes			-Returns the citizen votes map of the State
*   findFavoriteTen         -Returns an array that includes top ten states id's sorted
 *                          based on the state citizens votes and if there is less
 *                          than 10 it fills the others with -1
*/

/** Type for defining the State */
typedef struct state_t* State;

/** Type used for returning error codes from State functions */
typedef enum {
    STATE_SUCSESS,
    STATE_OUT_OF_MEMORY,
    STATE_NULL_ARGUMENT,
    STATE_INVALID_ID,
    STATE_INVALID_NAME,
    STATE_NOT_EXIST,
    SAME_STATE,
}StateResult;





/**
* StateCreate: Allocates a new empty state with the details that are given and update
*               the result according to the arguments and allocations
* @param state_name - a pointer to a string which updated to the state name we allocated
* @param state_song- a pointer to a string which updated to the state song we allocated
* @param result - a StateResult pointer we update it depending on the operation result
* @return
* 	NULL - if one of the parameters is NULL or allocations failed.
* 	new_state - in case we successfully allocated the new State
*/
State StateCreate(char *state_name,char *state_song,StateResult *result);

/**
 * DestroyState: Deletes an existing State and frees all resources
 * @param state  -the State that the function destroys
 * return -void
 */
void DestroyState(State state);

/**
 * copyIntDataElement:  Returns an existing State
 * @param stateKeyElement  -the state we return its type is MapDataElement
 * return                  NULL - if the parameter is NULL or the allocation failed.
 *                         new_element -a copy of the param which was allocated
 */
MapDataElement copyIntDataElement(MapDataElement  stateKeyElement);

/**
 * freeIntDataElement: frees a MapKeyElement after it converts the param to int type
 * @param stateKeyElement  -the Element that we free
 * return: -void
 */
void freeIntDataElement(MapDataElement stateKeyElement);


/**
 * compareIntKeyElement: compares 2 integers
 * @param Key1  -pointer to an integer
 * @param Key2  -pointer to another integer
 * return  - 0: in case the 2 integers equal
 *         - 1: in case the first integer is greater than the second
 *         - -1: in case the second integer is greater than the first integer
 */
int compareIntKeyElement(MapKeyElement Key1,MapKeyElement Key2);



/**
 * copyIntElement  	   - Returns an int element which is a copy of the stateKeyElement
 * @param stateKeyElement
 * return       NULL - if the parameter is NULL or the allocation failed.
 *              new_element- the copy of the param
 */
MapKeyElement copyIntElement(MapKeyElement stateKeyElement);


/**
 * freeIntElement: frees a MapKeyElement after it converts the param to int type
 * @param stateKeyElement  -the Element that we free
 * return: -void
 */
void freeIntElement(MapKeyElement stateKeyElement);


/**
 * DestroyState: Deletes an existing State and frees all resources
 * @param state  -the state that we free its elements
 * return  void
 */
void DestroyStateElemnt(MapDataElement state);

/**
 * stateGetName: returns the name of a state
 * @param state  -the state that we return its name
 * return  NULL - if the parameter is NULL
 *          state->state_name -the name of the state parameter
 */
char *StateGetName(State state);


/**
 *  GetMapOfVotes:  receives a state and returns its citizen votes map
 * @param state
 * return       NULL - if the parameter is NULL
 *              -state->citizen_votes: returns the citizen votes map of the state
 */
Map GetMapOfVotes(State state);


/**
 * findFavoriteTen: receives a state and Returns an array that includes top ten states id's sorted
 *                          based on the state citizens votes first and secondary by id and if there is less
 *                          than 10 it fills the others with -1
 * @param state    -the state that we search it in order to find its most ten votes states by its citizen
 * return          NULL - if the parameter is NULL or the allocation failed.
 *                 favorite- array that includes the top ten states id'es
 */
int* findFavoriteTen(State state);


/**
 * stateCopy: creats a copy of a given state.
 * @param state   -this is the state that we copy and returns the copy of it
 * @return        NULL - if the parameter is NULL or the allocation failed.
 *                copy_state -a copy of the state that we allocated
 */
MapDataElement StateCopy(MapDataElement state);


#endif //HW1_STATE_H
