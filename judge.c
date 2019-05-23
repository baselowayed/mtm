#include "judge.h"

#define TOP_TEN 10

struct judge {

    char *name;
    int *votes;
};


/*this func checks if the char is a small letter*/
static bool checkIfLetter(char c) {
    if (((c >= 'a') && (c <= 'z')) || c == ' ')
        return true;
    return false;
}


/*checks if the name of the judge is legal*/
static bool checkIfNameIsLegal(char *name) {
    while (*name) {
        if (!checkIfLetter(*name))
            return false;
        name++;
    }
    return true;
}


/*creates new_judge with the given details and returns it
 and returns NULL in case of unaccessible address*/
Judge createJudge(char *name, int *votes, operation_result *result1) {
    Judge new_judge = malloc(sizeof(*new_judge));
    if (new_judge == NULL)
        return NULL;
    if (!checkIfNameIsLegal(name)) {
        if (result1) *result1 = JUDGE_INVALID_NAME;
        free(new_judge);
        return NULL;
    }
    new_judge->name = malloc(strlen(name) + 1);
    if (new_judge->name == NULL) {
        if (result1)
            *result1 = JUDGE_OUT_OF_MEMORY;
        free(new_judge);
        return NULL;
    }
    strcpy(new_judge->name, name);

    new_judge->votes = malloc(sizeof(votes) * TOP_TEN);
    if (new_judge->votes == NULL) {
        free(new_judge->name);
        free(new_judge);
        *result1 = JUDGE_OUT_OF_MEMORY;
        return NULL;
    }

    for (int i = 0; i < 10; i++) {
        new_judge->votes[i] = votes[i];
    }

    *result1 = JUDGE_SUCCESS;
    return new_judge;
}


/*destroys/frees the unwanted_judge
 note: it can't connect the next*/
void destroyJudge(Judge judge) {
    if (judge == NULL)
        return;

    free(judge->name);
    free(judge->votes);
    free(judge);


}

void destroyJudgeElment(MapDataElement unwanted_judge) {
    if (unwanted_judge == NULL)
        return;
    Judge temp_judge = (Judge) unwanted_judge;

    if (temp_judge->name != NULL)
        free(temp_judge->name);
    if (temp_judge->votes != NULL)
        free(temp_judge->votes);

    free(temp_judge);

}


/* receives a judge and returns his name and in case of error returns NULL*/
char *getJudgeName(Judge judge) {
    if (judge == NULL)
        return NULL;
    return judge->name;
}



/* receives a judge and returns his id and in case of error returns NULL*/



/*this function reccieves judge and returns his votes and
 in case of error returns NULL*/
int *getJudgeVotes(Judge judge) {
    if (judge == NULL)
        return NULL;

    return judge->votes;
}


/*it copies judge2 to judge1 and returns JUDGE_SUCCESS if done right
 and JUDGE_NULL_ARGUMENT if judge1/judge2 address isn't accessible*/
MapDataElement copyJudge(MapDataElement judge) {
    if (judge == NULL)
        return NULL;
    Judge temp_judge = (Judge) judge;
    if (!checkIfNameIsLegal(temp_judge->name))
        return NULL;
    operation_result result = JUDGE_SUCCESS;
    Judge copy = createJudge(temp_judge->name, temp_judge->votes, &result);
    if (copy == NULL) {
        return NULL;
    }
    if (result == JUDGE_SUCCESS)
        return copy;
    else {
        destroyJudge(copy);
        return NULL;
    }
}


bool JudgeVoted(Judge judge, int id) {
    for (int i = 0; i < TOP_TEN; i++) {
        if ((judge->votes)[i] == id)
            return true;
    }
    return false;


}
