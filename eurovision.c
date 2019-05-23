#include <stdio.h>
#include "eurovision.h"
#include <stdlib.h>
#include "map.h"
#include <stdbool.h>
#include "state.h"
#include "judge.h"
#include <string.h>
#include "list.h"
#define NOVOTE 0
#define TOP_TEN 10
#define HUNDRED 100

struct eurovision_t {
    Map states;
    Map judges;
};

static double stateAveragePlaceByCitizens(Eurovision eurovision,
                                          MapKeyElement key);

static double stateFinalPoints(Eurovision eurovision,
        MapKeyElement key, int audiencePercent);

static StateResult addingState(Map map, int id_1, int id_2);

static double stateAveragePlaceByJudge(Eurovision eurovision
,MapKeyElement key);

static int convertI(int i);


Eurovision eurovisionCreate() {
    Eurovision eurovision1 = malloc(sizeof(*eurovision1));
    if (eurovision1 == NULL)
        return NULL;

    eurovision1->judges = mapCreate(&copyJudge,
                                    &copyIntElement,
                                    &destroyJudgeElment,
                                    &freeIntElement,
                                    &compareIntKeyElement);
    if (eurovision1->judges == NULL) {
        free(eurovision1);
        return NULL;
    }
    eurovision1->states = mapCreate(&StateCopy,
                                    &copyIntElement,
                                    &DestroyStateElemnt,
                                    &freeIntElement,
                                    &compareIntKeyElement);
    if (eurovision1->states == NULL) {
        mapDestroy(eurovision1->judges);
        free(eurovision1);
        return NULL;
    }
    return eurovision1;
}

void eurovisionDestroy(Eurovision eurovision) {
    if (eurovision == NULL)
        return;
    mapDestroy(eurovision->judges);
    mapDestroy(eurovision->states);
    free(eurovision);
}

EurovisionResult eurovisionAddState(Eurovision eurovision, int stateId
        , const char *stateName, const char *songName) {
    if (eurovision == NULL || stateName == NULL || songName == NULL)
        return EUROVISION_NULL_ARGUMENT;
    if (stateId < 0)
        return EUROVISION_INVALID_ID;

    StateResult status = STATE_SUCSESS;
    int tmp = stateId;
    int *tmp_id = &tmp;

    if (mapContains(eurovision->states, tmp_id)) {
        return EUROVISION_STATE_ALREADY_EXIST;
    }
    State state = StateCreate((char *) stateName, (char *) songName, &status);
    if (status == STATE_NULL_ARGUMENT) {
        return EUROVISION_NULL_ARGUMENT;
    }
    if (status == STATE_INVALID_NAME) {
        DestroyState(state);
        return EUROVISION_INVALID_NAME;
    }
    if (status == STATE_INVALID_ID) {

        DestroyState(state);
        return EUROVISION_INVALID_ID;
    }
    if (status == STATE_OUT_OF_MEMORY) {

        DestroyState(state);
        return EUROVISION_OUT_OF_MEMORY;
    }

    MapResult status1 = mapPut(eurovision->states, tmp_id, state);
    if (status1 == MAP_OUT_OF_MEMORY) {

        DestroyState(state);
        return EUROVISION_OUT_OF_MEMORY;
    }
    if (status1 == MAP_NULL_ARGUMENT) {

        DestroyState(state);
        return EUROVISION_NULL_ARGUMENT;
    }
    MAP_FOREACH(int*, iterator1, eurovision->states) {
        if ((*iterator1) != stateId) {
            addingState(eurovision->states, stateId, *iterator1);
        }
    }
    DestroyState(state);
    return EUROVISION_SUCCESS;
}

static StateResult addingState(Map map, int id_1, int id_2) {
    int vote = NOVOTE;
    State data1 = mapGet(map, &id_1);
    MapResult result1 = mapPut(GetMapOfVotes(data1), &id_2, &vote);
    if (result1 == MAP_NULL_ARGUMENT) {

        return STATE_NULL_ARGUMENT;
    }
    if (result1 == MAP_OUT_OF_MEMORY) {

        return STATE_OUT_OF_MEMORY;
    }
    State data2 = mapGet(map, &id_2);
    Map map2 = GetMapOfVotes(data2);
    MapResult result2 = mapPut(map2, &id_1, &vote);
    if (result2 == MAP_NULL_ARGUMENT) {
        return STATE_NULL_ARGUMENT;
    }
    if (result2 == MAP_OUT_OF_MEMORY) {
        return STATE_OUT_OF_MEMORY;
    }
    return STATE_SUCSESS;
}

EurovisionResult eurovisionAddJudge(Eurovision eurovision, int judgeId,
                                    const char *judgeName,
                                    int *judgeResults) {
    if (eurovision == NULL || judgeName == NULL || judgeResults == NULL)
        return EUROVISION_NULL_ARGUMENT;
    if (judgeId < 0)
        return EUROVISION_INVALID_ID;
    int tmp = judgeId;
    int *tmp_id = &tmp;
    if (mapContains(eurovision->judges, tmp_id)) {
        return EUROVISION_JUDGE_ALREADY_EXIST;
    }
    for (int i = 0; i < TOP_TEN; i++) {
        if (judgeResults[i] < 0) {
            return EUROVISION_INVALID_ID;
        }
        for (int j = i + 1; j < TOP_TEN; j++) {
            if (judgeResults[i] == judgeResults[j]) {
                return EUROVISION_INVALID_ID;
            }
        }
        int *temp_result2 = &judgeResults[i];
        if (!mapContains(eurovision->states, temp_result2)) {
            return EUROVISION_STATE_NOT_EXIST;
        }
    }
    operation_result result1 = JUDGE_SUCCESS;
    Judge tmp_judge = createJudge((char *) judgeName, judgeResults, &result1);
    if (tmp_judge == NULL && result1 != JUDGE_INVALID_NAME) {
        eurovisionDestroy(eurovision);
        return EUROVISION_OUT_OF_MEMORY;
    }
    if (result1 == JUDGE_NULL_ARGUMENT) {
        destroyJudge(tmp_judge);
        return EUROVISION_NULL_ARGUMENT;
    }
    if (result1 == JUDGE_INVALID_NAME) {
        destroyJudge(tmp_judge);
        return EUROVISION_INVALID_NAME;
    }
    if (result1 == JUDGE_INVALID_ID) {

        destroyJudge(tmp_judge);
        return EUROVISION_INVALID_ID;
    }
    if (result1 == JUDGE_OUT_OF_MEMORY) {
        destroyJudge(tmp_judge);
        eurovisionDestroy(eurovision);
        return EUROVISION_OUT_OF_MEMORY;
    }
    MapResult final_result = mapPut(eurovision->judges, tmp_id, tmp_judge);
    if (final_result == MAP_NULL_ARGUMENT) {
        destroyJudge(tmp_judge);
        return EUROVISION_OUT_OF_MEMORY;
    }
    if (final_result == MAP_OUT_OF_MEMORY) {
        destroyJudge(tmp_judge);
        eurovisionDestroy(eurovision);
        return EUROVISION_OUT_OF_MEMORY;
    }
    destroyJudge(tmp_judge);
    return EUROVISION_SUCCESS;
}


EurovisionResult eurovisionRemoveState(Eurovision eurovision, int stateId){
    if (eurovision == NULL)
        return EUROVISION_NULL_ARGUMENT;
    if (stateId < 0)
        return EUROVISION_INVALID_ID;
    int tmp = stateId;
    int *tmp_id = &tmp;
    if (!mapContains(eurovision->states, tmp_id))
        return EUROVISION_STATE_NOT_EXIST;
    int *iterator = mapGetFirst(eurovision->judges);
    while (iterator != NULL) {
        Judge data = mapGet((eurovision->judges), iterator);
        if (JudgeVoted(data, stateId)) {
            eurovisionRemoveJudge(eurovision, *iterator);
            iterator = mapGetFirst(eurovision->judges);
            continue;
        }
        iterator = mapGetNext(eurovision->judges);
    }
    MAP_FOREACH(int*, iter, eurovision->states) {
        State data = mapGet(eurovision->states, iter);
        if (mapContains(GetMapOfVotes(data), tmp_id)) {
            mapRemove(GetMapOfVotes(data), tmp_id);
        }
    }
    mapRemove(eurovision->states, tmp_id);
    return EUROVISION_SUCCESS;
}


EurovisionResult eurovisionRemoveJudge(Eurovision eurovision, int judgeId){

    if (eurovision == NULL)
        return EUROVISION_NULL_ARGUMENT;

    if (judgeId < 0)
        return EUROVISION_INVALID_ID;
    int tmp = judgeId;
    int *tmp_id = &tmp;

    if (!mapContains(eurovision->judges, tmp_id))
        return EUROVISION_JUDGE_NOT_EXIST;

    MapResult result2 = mapRemove(eurovision->judges, tmp_id);
    if (result2 == MAP_NULL_ARGUMENT)
        return EUROVISION_NULL_ARGUMENT;
    if (result2 == MAP_SUCCESS)
        return EUROVISION_SUCCESS;


    return EUROVISION_JUDGE_NOT_EXIST;
}


EurovisionResult eurovisionAddVote(Eurovision eurovision, int stateGiver,
                                   int stateTaker) {
    if (eurovision == NULL)
        return EUROVISION_NULL_ARGUMENT;
    if (stateGiver < 0 || stateTaker < 0)
        return EUROVISION_INVALID_ID;
    int tmp1 = stateGiver;
    int *tmp_giver = &tmp1;
    int tmp2 = stateTaker;
    int *tmp_taker = &tmp2;
    if (!mapContains(eurovision->states, tmp_giver)
        || !mapContains(eurovision->states, tmp_taker))
        return EUROVISION_STATE_NOT_EXIST;
    if (stateGiver == stateTaker)
        return EUROVISION_SAME_STATE;
    MAP_FOREACH(int*, iter, eurovision->states) {
        if (compareIntKeyElement(iter, tmp_giver) == 0) {
            State data_m = mapGet(eurovision->states, iter);
            if (mapContains(GetMapOfVotes(data_m), tmp_taker)) {
                int *data = mapGet(GetMapOfVotes(data_m), tmp_taker);
                if (data == NULL) {
                    return EUROVISION_NULL_ARGUMENT;
                }
                (*data)++;

                return EUROVISION_SUCCESS;

            }

        }
    }
    return EUROVISION_SUCCESS;
}


EurovisionResult eurovisionRemoveVote(Eurovision eurovision, int stateGiver
        , int stateTaker) {
    if (eurovision == NULL)
        return EUROVISION_NULL_ARGUMENT;
    if (stateGiver < 0 || stateTaker < 0)
        return EUROVISION_INVALID_ID;
    int tmp1 = stateGiver;
    int *tmp_giver = &tmp1;
    int tmp2 = stateTaker;
    int *tmp_taker = &tmp2;
    if (!mapContains(eurovision->states, tmp_giver)
        || !mapContains(eurovision->states, tmp_taker))
        return EUROVISION_STATE_NOT_EXIST;
    if (stateGiver == stateTaker)
        return EUROVISION_SAME_STATE;
    MAP_FOREACH(int*, iter, eurovision->states) {
        if (compareIntKeyElement(iter, tmp_giver) == 0) {
            State data_m = mapGet(eurovision->states, iter);
            int *data = mapGet(GetMapOfVotes(data_m), tmp_taker);
            if (data == NULL)
                return EUROVISION_NULL_ARGUMENT;
            if (*data != 0)
                (*data)--;

            return EUROVISION_SUCCESS;
        }
    }
    return EUROVISION_SUCCESS;
}


static int convertI(int i);

int convertI(int i) {
    if (i == 0)
        return 12;
    if (i == 1)
        return 10;
    if (i == 2)
        return 8;
    if (i == 3)
        return 7;
    if (i == 4)
        return 6;
    if (i == 5)
        return 5;
    if (i == 6)
        return 4;
    if (i == 7)
        return 3;
    if (i == 8)
        return 2;
    if (i == 9)
        return 1;
    return 0;

}


static void freeListElement(ListElement element) {
    free(element);

}

static ListElement copyListName(ListElement name) {
    if (name == NULL)
        return NULL;
    char *copy = malloc(strlen((char *) name) + 1);
    if (copy == NULL)
        return NULL;
    strcpy(copy, (char *) name);
    return (ListElement) copy;
}

static int compreListNames(ListElement name1, ListElement name2) {
    char *name1_temp = (char *) name1;
    char *name2_temp = (char *) name2;

    if (strcmp(name1_temp, name2_temp) == 0)return 0;
    if (strcmp(name1_temp, name2_temp) > 0)return 1;
    return -1;


}

static void free8Elements(int *a, int *b, State state1, State state2, Map map1
        , List list1, Eurovision eurovision1, char *name1) {
    free(a);
    free(b);
    DestroyState(state1);
    DestroyState(state2);
    mapDestroy(map1);
    listDestroy(list1);
    eurovisionDestroy(eurovision1);
    free(name1);
    return;
}

static void free9Elements(int *a, int *b, State state1, State state2, Map map1
        , List list1, Eurovision eurovision1, char *name1,
              char *name2) {
    free(a);
    free(b);
    DestroyState(state1);
    DestroyState(state2);
    mapDestroy(map1);
    listDestroy(list1);
    eurovisionDestroy(eurovision1);
    free(name1);
    free(name2);
    return;
}

static void free2Names(char *name1, char *name2) {
    free(name1);
    free(name2);
    return;
}

static void free2Int(int *a, int *b) {
    free(a);
    free(b);
    return;
}

static void addingNamesToList(char *name1, char *name2
        , char *final_name, List list) {
    final_name = strcpy(final_name, name1);
    strcat(final_name, " - ");
    strcat(final_name, name2);
    listInsertFirst(list, final_name);
    free(final_name);
}


static List friendlyAux(Eurovision eurovision, Map map, List list) {
    int *iterator = mapGetFirst(eurovision->states);
    while (iterator != NULL) {
        MAP_FOREACH(int*, iter, map) {
            if (compareIntKeyElement(iter, iterator) != 0) {
                State data1 = mapGet(map, iterator);
                State data2 = mapGet(map, iter);
                int *result3 = findFavoriteTen(data1);
                int *result4 = findFavoriteTen(data2);
                if (*result4 == *iterator && *result3 == *iter) {
                    char *get_name1 = StateGetName(data1);
                    char *get_name2 = StateGetName(data2);
                    char *name1 = malloc(strlen(get_name1) + 1);
                    char *name2 = malloc(strlen(get_name2) + 1);
                    if (name1 == NULL || name2 == NULL) {
                        if (name1 == NULL)
                            free8Elements(result3, result4, data1
                                    , data2, map, list, eurovision, name2);
                        else
                            free8Elements(result3, result4, data1
                                    , data2, map, list, eurovision, name1);
                        return NULL;
                    }
                    strcpy(name1, get_name1);
                    strcpy(name2, get_name2);
                    char *final_name = malloc((strlen(name1)
                                               + strlen(name2) + 4));
                    if (final_name == NULL) {
                        free9Elements(result3, result4, data1, data2
                                , map, list, eurovision, name1, name2);
                        return NULL;
                    }
                    if (strcmp(name1, name2) >= 0) {
                        addingNamesToList(name2, name1, final_name, list);
                    } else {
                        addingNamesToList(name1, name2, final_name, list);
                    }
                    free2Names(name1, name2);
                }
                free2Int(result3, result4);
            }
        }
        mapRemove(map, iterator);
        iterator = mapGetNext(eurovision->states);
    }
    mapDestroy(map);
    ListResult result = listSort(list, &compreListNames);
    if (result == LIST_OUT_OF_MEMORY) {
        listDestroy(list);
        eurovisionDestroy(eurovision);
        return NULL;
    }
    if (result == LIST_NULL_ARGUMENT) {
        listDestroy(list);
        return NULL;
    }
    return list;
}

List eurovisionRunGetFriendlyStates(Eurovision eurovision) {
    if (eurovision == NULL)
        return NULL;
    List list = listCreate(&copyListName, &freeListElement);
    if (list == NULL)
        return NULL;
    Map map = mapCopy(eurovision->states);
    if (map == NULL) {
        listDestroy(list);
        return NULL;
    }
    return friendlyAux(eurovision, map, list);

}


List eurovisionRunAudienceFavorite(Eurovision eurovision) {
    if (eurovision == NULL)
        return NULL;
    return eurovisionRunContest(eurovision, HUNDRED);
}


List eurovisionRunContest(Eurovision eurovision, int audiencePercent) {
    if (eurovision == NULL)
        return NULL;
    if (audiencePercent <= 0 || audiencePercent > HUNDRED)
        return NULL;
    List final_list = listCreate(&copyListName, &freeListElement);
    if (final_list == NULL)
        return NULL;
    Map tmp_states_map = mapCopy(eurovision->states);
    if (tmp_states_map == NULL) {
        listDestroy(final_list);
        eurovisionDestroy(eurovision);
        return NULL;
    }
    double max = -2;
    double tmp = 0;
    int tmp_id = -1;
    int size = mapGetSize(tmp_states_map);
    for (int i = 0; i < size; i++) {
        int *iterator1 = mapGetFirst(tmp_states_map);
        while (iterator1 != NULL) {
            tmp = stateFinalPoints(eurovision, iterator1, audiencePercent);
            if (tmp == -1) {
                listDestroy(final_list);
                mapDestroy(tmp_states_map);
                eurovisionDestroy(eurovision);
                return NULL;
            }
            if (tmp > max) {
                max = tmp;
                tmp_id = *iterator1;
            }
            if (tmp == max) {
                if (tmp_id > *iterator1)
                    tmp_id = *iterator1;
            }
            iterator1 = mapGetNext(tmp_states_map);
        }
        State data = mapGet(eurovision->states, &tmp_id);
        char *name = StateGetName(data);
        listInsertLast(final_list, name);
        mapRemove(tmp_states_map, &tmp_id);
        max = -2;
    }
    mapDestroy(tmp_states_map);
    return final_list;
}


/*calculates the points id_searched got from all the citizens*/
static double stateAveragePlaceByCitizens(Eurovision eurovision
        , MapKeyElement key){
    double sum = 0;
    int *tmp = (int *) key;
    int id_searched = *tmp;
    double size = mapGetSize(eurovision->states);
    size = size - 1;
    if(size==0)
        return 0;
    int *favorite = malloc(sizeof(*favorite) * TOP_TEN);
    if (favorite == NULL) {
        return -1;
    }
    MAP_FOREACH(int*, iterator, eurovision->states) {

        State data = mapGet(eurovision->states, iterator);
        int *tmp111 = findFavoriteTen(data);
        if (tmp111 == NULL) {
            free(favorite);
            free(tmp111);
            return -1;
        }

        for (int j = 0; j < TOP_TEN; j++) {
            favorite[j] = tmp111[j];

        }
        for (int i = 0; i < TOP_TEN; i++) {
            if (id_searched == favorite[i]) {
                sum += convertI(i);
            }

        }
        free(tmp111);
    }
    free(favorite);
    return sum / size;
}


static double stateAveragePlaceByJudge(Eurovision eurovision
        ,MapKeyElement key) {
    double sum = 0;
    double size_of_map = mapGetSize(eurovision->judges);
    if (size_of_map == 0)
        return 0;
    int *temp_result2 = malloc(sizeof(*temp_result2) * TOP_TEN);
    if (temp_result2 == NULL) {
        return -1;
    }
    MAP_FOREACH(int*, iterator, eurovision->judges) {
        Judge data = mapGet(eurovision->judges, iterator);
        int *arr = getJudgeVotes(data);

        if (arr == NULL) {
            free(temp_result2);
            return -1;
        }

        for (int i = 0; i < TOP_TEN; i++) {
            temp_result2[i] = arr[i];
        }
        int *key_temp = (int *) key;
        int temp_key = *key_temp;

        for (int i = 0; i < TOP_TEN; i++) {
            if (temp_result2[i] == temp_key) {
                sum += convertI(i);
            }
        }
    }
    free(temp_result2);
    return sum / size_of_map;
}


static double stateFinalPoints(Eurovision eurovision
        , MapKeyElement key, int audiencePercent) {
    double tmp1 = 0, tmp2 = 0, final = 0;


    tmp1 = stateAveragePlaceByCitizens(eurovision, key);
    if (mapGetSize(eurovision->states) >= TOP_TEN) {
        tmp2 = stateAveragePlaceByJudge(eurovision,key);
    }
    if (tmp1 == -1 || tmp2 == -1)
        return -1;
    final = tmp1 * audiencePercent + tmp2 * (HUNDRED - audiencePercent);
    return final / (double) HUNDRED;
}





