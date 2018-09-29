#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "dungeon.h"
#include "path.h"

//Method to compare Events
static int32_t priority_compare(const void *val1, const void *val2)
{
    if(((event_t *) val1)->nextTurn - ((event_t *) val2)->nextTurn == 0)
    {
	  return ((event_t *) val1)->priority - ((event_t *) val2)->priority;
    }else{
	  return ((event_t *) val1)->nextTurn - ((event_t *) val2)->nextTurn;
  }
}

void run_monsters(dungeon_t *d, int num_monsters)
{
    heap_t h;
    heap_init(&h, priority_compare, NULL);
}