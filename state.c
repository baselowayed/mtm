
#include "state.h"
#define TOP_TEN 10

struct state_t{
    char* state_name;
    char* state_song;
    Map  citizen_votes;
};

static bool ValidLetter(char symbol)
{
    if((symbol>='a' && symbol<='z') ||symbol==' ')
        return true;
    return false;
}

static bool ValidName(char* name)
{ char* str=name;
    bool name_valid=false;
    while(*str)
    {
        if(ValidLetter(*str))
            name_valid=true;
        str++;
    }
    return name_valid;

}

int compareIntKeyElement(MapKeyElement Key1,MapKeyElement Key2)
{int* key1=(int*)Key1;
int* key2=(int*)Key2;

    if(*key1==*key2)return 0;
    if(*key1>*key2)return 1;
    return -1;

}
MapKeyElement copyIntElement(MapKeyElement stateKeyElement) {
    if (stateKeyElement == NULL)
        return NULL;
    int *new_element = malloc(sizeof(*new_element));
    if (new_element == NULL) return NULL;
    *new_element = *(int *) stateKeyElement;
    return (MapKeyElement) new_element;

}
MapDataElement copyIntDataElement(MapDataElement  stateKeyElement)
{
    if (stateKeyElement == NULL)
        return NULL;
    int *new_element = malloc(sizeof(*new_element));
    if (new_element == NULL)return NULL;
    *new_element = *(int *) stateKeyElement;
    return (MapDataElement) new_element;



}

void freeIntDataElement(MapDataElement stateKeyElement)
{
    free(stateKeyElement);


}
void freeIntElement(MapKeyElement stateKeyElement)
{
    free(stateKeyElement);
}



void DestroyState(State state)
{ if(state==NULL)
        return;
    free(state->state_name);
    free(state->state_song);
    mapDestroy(state->citizen_votes);
    free(state);
    return;
}


void DestroyStateElemnt(MapDataElement state)
{ if(state==NULL)
        return;
    free(((State)state)->state_name);
    free(((State)state)->state_song);
    mapDestroy(((State)state)->citizen_votes);
    ((State)state)->state_name=NULL;
    ((State)state)->state_song=NULL;
    ((State)state)->citizen_votes=NULL;
    free((State)state);
    return;
}

State StateCreate(char *state_name,char *state_song,StateResult *result)
{ if(state_name==NULL || state_song==NULL || result== NULL)
    {if(result) *result=STATE_NULL_ARGUMENT;
        return NULL;
    }
    State new_state=malloc(sizeof(*new_state));
    if(new_state==NULL) {
        if(result) *result = STATE_OUT_OF_MEMORY;
        return NULL;
    }
    if(!(ValidName(state_name)) || !(ValidName(state_song)))
    {  if(result) *result=STATE_INVALID_NAME;
    free(new_state);
            return NULL;
        }
    new_state->state_song=malloc(sizeof(*new_state->state_song)
            *strlen(state_song)+1);
    if(new_state->state_song==NULL) {
        if(result)  *result = STATE_OUT_OF_MEMORY;
        free(new_state);
        return NULL;

    }
    strcpy(new_state->state_song,state_song);
    new_state->state_name=malloc(sizeof(*new_state->state_name)
            *strlen(state_name)+1);
    if(new_state->state_name==NULL) {
        if (result) *result = STATE_OUT_OF_MEMORY;
        free(new_state->state_song);
        free(new_state);
        return NULL;
    }
    strcpy(new_state->state_name,state_name);


    new_state->citizen_votes=mapCreate(copyIntDataElement,copyIntElement
            ,freeIntDataElement,freeIntElement,compareIntKeyElement);
    if(new_state->citizen_votes==NULL) {
        if (result) {
            *result = STATE_OUT_OF_MEMORY;
        }
        free(new_state->state_name);
        free(new_state->state_song);
        free(new_state);
        return NULL;
    }

    *result=STATE_SUCSESS;
    return new_state;

}



int* findFavoriteTen(State state) {
    int *favorite = malloc(sizeof(*favorite) * TOP_TEN);
    if(favorite==NULL)
        return NULL;
    int* favorite_votes = malloc(sizeof(*favorite_votes) * TOP_TEN);
    if(favorite_votes==NULL) {
        free(favorite);
        return NULL;
    }

   Map citizens_votes = mapCopy(state->citizen_votes);


    if(citizens_votes==NULL)
    {

        free(favorite);
        free(favorite_votes);
        return NULL;
    }

    int max = -1, max_id =-1;

    for (int i = 0; i < TOP_TEN; i++) {

        int* iterator=mapGetFirst(citizens_votes);
        while(iterator!=NULL){

    int* tmp_iterator_data = mapGet(citizens_votes, iterator);
    int* tmp = tmp_iterator_data;
    int iterator_data = *tmp;

    int* tmp1 = iterator;
    int iterator_id = *tmp1;

    if (iterator_data > max || max_id == -1) {
        max = iterator_data;
        max_id = iterator_id;
    }
   if(iterator_data==max){
       if(iterator_id<max_id)
       max_id=iterator_id;
   }

    iterator=mapGetNext(citizens_votes);
}

        if(max>0) {
            favorite[i] = max_id;
            favorite_votes[i] = max;
        }
        else {
            favorite[i] = -1;
            favorite_votes[i] = -1;
        }
        MapResult status = MAP_SUCCESS;

        status = mapRemove(citizens_votes, &max_id);
         max=-1;
         max_id=-1;

        if (status == MAP_NULL_ARGUMENT) {
            free(favorite);
            free(favorite_votes);
            mapDestroy(citizens_votes);
            return NULL;
        }

    }

    for(int i=0 ; i<TOP_TEN ; i++)
        for(int j=0 ; j<TOP_TEN ; j++) {
            if (i != j) {
                if (favorite_votes[i] == favorite_votes[j]) {
                    if (favorite[i] <  favorite[j]) {
                        int tmp = favorite[i];
                        favorite[i]=favorite[j];
                        favorite[j]= tmp;
                    }
                }
            }
        }

    mapDestroy(citizens_votes);
    free(favorite_votes);
    return favorite;
}






MapDataElement StateCopy(MapDataElement state)
{ if(!state)
        return NULL;

 State orginal_state=(State)state;
   State copy_state=malloc(sizeof(*copy_state));
    if( copy_state==NULL) {
        free(copy_state);
        return NULL;
    }
copy_state->state_name=malloc(sizeof(*copy_state->state_name)
        *strlen(orginal_state->state_name)+1);
    if(copy_state->state_name==NULL){
        free(copy_state);
    return NULL;}


    strcpy(copy_state->state_name,(orginal_state->state_name));
   copy_state->state_song=malloc(sizeof(*copy_state->state_name)
           *strlen(orginal_state->state_song)+1);
    if(copy_state->state_song==NULL){
        free(copy_state->state_name);
        free(copy_state);

        return NULL;}


    strcpy(copy_state->state_song,(orginal_state->state_song));
        copy_state->citizen_votes = mapCopy(orginal_state->citizen_votes);
        if(copy_state->citizen_votes==NULL){
            free(copy_state->state_name);
            free(copy_state->state_song);
            free(copy_state);
        }
    return (MapDataElement )copy_state;

}

Map GetMapOfVotes(State state)
{
    if(state==NULL)
        return NULL;

    return state->citizen_votes;

}

char* StateGetName(State state)
{ if(state==NULL)
        return NULL;
    return state->state_name;
}

