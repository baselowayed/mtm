
#include "map.h"
#include <stdlib.h>
#include <stdbool.h>

#define NULLARGUMENT -1





void DestroyAux(Map map);
typedef struct node {
    MapDataElement Data;
    MapKeyElement Key;
    struct node* next;
} *Node;


struct Map_t
{ Node head;
    Node iterator;
    Node map_index;
    copyMapDataElements copyDataElement;
    copyMapKeyElements copyKeyElement;
    freeMapDataElements freeDataElement;
    freeMapKeyElements freeKeyElement;
    compareMapKeyElements compareKeyElements;
};




static Node CreateNewNode(Map map,MapKeyElement Key,MapDataElement Data){
    if(map==NULL || Data==NULL || Key== NULL)
        return NULL;
    Node new_node=malloc(sizeof(*new_node));
    if(new_node==NULL)
        return NULL;

    new_node->Data=map->copyDataElement(Data);
    new_node->Key=map->copyKeyElement(Key);
    new_node->next=NULL;
    return new_node;

}

static Node FindLastNode(Map map,Node head,MapKeyElement keyElement)
{ Node temp1=head;
 Node temp2=temp1->next;
 int size=mapGetSize(map);
 for(int i=0;i<size;i++) {
     if(temp2==NULL)
     {
        break;
     }
     if (map->compareKeyElements(temp2->Key, keyElement) >= 0)
         return temp1;
     temp1 = temp2;
     temp2 = temp1->next;
 }
    return  temp1;
}



void DestroyNewNode(Map map,Node node)
{ if(map==NULL || node==NULL)
        return;
    while(node)
    { Node destroy_node= node;
        if(destroy_node->Data!=NULL)
            map->freeDataElement(destroy_node->Data);
        if(destroy_node->Key!=NULL)
            map->freeKeyElement(destroy_node->Key);
        node=node->next;
        free(destroy_node);


    }


}


Map mapCreate(copyMapDataElements copyDataElement,
              copyMapKeyElements copyKeyElement,
              freeMapDataElements freeDataElement,
              freeMapKeyElements freeKeyElement,
              compareMapKeyElements compareKeyElements)
{
    if(copyDataElement==NULL || copyKeyElement == NULL || freeDataElement==NULL || freeKeyElement==NULL ||
       compareKeyElements== NULL)
        return NULL;
    Map map= malloc(sizeof(*map));
    if(map==NULL)
        return NULL;
    map->head=NULL;
    map->iterator=map->head;
    map->map_index=map->head;
    map->copyDataElement=copyDataElement;
    map->copyKeyElement=copyKeyElement;
    map->freeDataElement=freeDataElement;
    map->freeKeyElement=freeKeyElement;
    map->compareKeyElements=compareKeyElements;

    return map;

}

Map mapCopy(Map map) {
    if (map == NULL)
        return NULL;

    Map copy_map = mapCreate(map->copyDataElement,
                             map->copyKeyElement,
                             map->freeDataElement,
                             map->freeKeyElement,
                             map->compareKeyElements);
    if (copy_map == NULL)
        return NULL;
    if(map->head==NULL)
        return copy_map;
    copy_map->head = CreateNewNode(map, map->head->Key, map->head->Data);
   if(copy_map->head==NULL)
       return NULL;
    copy_map->map_index=copy_map->head;
    map->map_index=map->head->next;
while(map->map_index!=NULL)
{
    copy_map->map_index->next = CreateNewNode(map, map->map_index->Key, map->map_index->Data);
    if (copy_map->map_index->next == NULL) {
        mapDestroy(copy_map);
        return NULL;
    }
    map->map_index= map->map_index->next;
    copy_map->map_index= copy_map->map_index->next;


}

    return copy_map;
}


int mapGetSize(Map map){
    if(map==NULL)
        return NULLARGUMENT;
    int size_of_map=0;
    MAP_FOREACH(MapKeyElement ,iterator,map)
    {
        size_of_map++;
    }
    return size_of_map;
}
bool mapContains(Map map, MapKeyElement element)
{ if (map==NULL || element==NULL)
        return false;
    MAP_FOREACH(MapKeyElement,iter,map)
    {if(map->compareKeyElements(iter,element)==0) {

            return true;
        }
    }

    return false;
}


MapDataElement mapGet(Map map, MapKeyElement keyElement)
{
    if(map == NULL || keyElement == NULL)
        return NULL;
    if(!mapContains(map,keyElement))
        return NULL;

    map->map_index=map->head;
    while (map->map_index!=NULL)
    {
        if(map->compareKeyElements(map->map_index->Key,keyElement)==0)
            return map->map_index->Data;
        map->map_index=map->map_index->next;
    }
    return NULL;
}


MapResult mapPut(Map map, MapKeyElement keyElement, MapDataElement dataElement){
    if(map==NULL || keyElement==NULL || dataElement==NULL)
        return MAP_NULL_ARGUMENT;
    if(mapContains(map,keyElement)){
        map->map_index=map->head;
        while(map->map_index!=NULL){
            if(map->compareKeyElements(map->map_index->Key,keyElement)==0){
                map->freeDataElement(map->map_index->Data);
                map->map_index->Data=map->copyDataElement(dataElement);
                map->iterator=NULL;
                return MAP_SUCCESS;
            }
            map->map_index=map->map_index->next;
        }
    }

    if(map->head==NULL){
        Node new_node_1=CreateNewNode(map,keyElement,dataElement);
        map->head=new_node_1;
        map->iterator=NULL;
        return MAP_SUCCESS;
    }
    if(map->head->next==NULL){
        Node new_node_1=CreateNewNode(map,keyElement,dataElement);
        if(map->compareKeyElements(map->head->Key,keyElement)>0) {

            new_node_1->next=map->head;
            map->head=new_node_1;
            map->iterator=NULL;
            return MAP_SUCCESS;
        } else
        {
            map->head->next=new_node_1;
            map->iterator=NULL;
            return MAP_SUCCESS;
        }

    }

    Node new_node=FindLastNode(map,map->head,keyElement);

    Node new_node_2=CreateNewNode(map,keyElement,dataElement);
    if(new_node_2==NULL)
        return MAP_OUT_OF_MEMORY;
    Node temp_node=new_node->next;
    new_node->next=new_node_2;
    new_node_2->next=temp_node;
    map->iterator=NULL;
    return MAP_SUCCESS;



}





MapResult mapRemove(Map map, MapKeyElement keyElement)
{ if(map==NULL || keyElement==NULL)
        return MAP_NULL_ARGUMENT;
    map->map_index=map->head;
    MAP_FOREACH(MapKeyElement,temp_iterator,map)
    { if(map->compareKeyElements(temp_iterator,keyElement)==0)
        { if(map->iterator == map->head){
                map->freeDataElement(map->head->Data);
                map->head->Data=NULL;
                map->freeKeyElement(map->head->Key);
                map->head->Key=NULL;
                map->head=map->iterator->next;
                free(map->map_index);
                map->iterator=NULL;
                return MAP_SUCCESS;
            }
            else { map->map_index->next=map->iterator->next;
                map->freeKeyElement(map->iterator->Key);
                map->iterator->Key=NULL;
                map->freeDataElement(map->iterator->Data);
                map->iterator->Data=NULL;
                free(map->iterator);
                map->iterator=NULL;
                return MAP_SUCCESS;

            }
        }
        map->map_index=map->iterator;


    }


    map->iterator=NULL;
    return  MAP_ITEM_DOES_NOT_EXIST;





}


MapKeyElement mapGetFirst(Map map){
    if(map== NULL)
        return NULL;
    if(map->head==NULL)
        return NULL;
    map->iterator=map->head;

    return map->head->Key;
}

MapKeyElement mapGetNext(Map map)
{
    if(map== NULL)
        return NULL;

    if(map->iterator==NULL)
        return NULL;
    if(map->iterator->next==NULL)
        return NULL;
    map->iterator=map->iterator->next;

    return map->iterator->Key;
}

MapResult mapClear(Map map)
{
    if(map== NULL)
        return MAP_NULL_ARGUMENT;
    DestroyAux(map);
    return MAP_SUCCESS;



}

void DestroyAux(Map map)
{Node temp=map->head;
    Node next=NULL;
    while(temp)
    { next=temp->next;
        map->freeDataElement(temp->Data);
        map->freeKeyElement(temp->Key);
        free(temp);
        temp=next;
    }
    map->head=NULL;
}
void mapDestroy(Map map){
    if(map==NULL)
        return;
    DestroyAux(map);
    free(map);
}



