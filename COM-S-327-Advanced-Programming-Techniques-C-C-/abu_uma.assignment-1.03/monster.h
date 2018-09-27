#ifndef MONSTER_H
# define MONSTER_H
# include "macros.h"

//Defining the type of monsters characteristics of the monsters

#define ERRATIC 0x00000004
#define TUNNELING 0x00000003
#define INTELLEGENT 0x00000001
#define TELEPATHIC  0x00000002

//Making structs for the monsters
typedef struct monster{
    int characteristics;
    int priority;
    int speed;
    int x-pos;
    int y-pos;
} monster_t;

