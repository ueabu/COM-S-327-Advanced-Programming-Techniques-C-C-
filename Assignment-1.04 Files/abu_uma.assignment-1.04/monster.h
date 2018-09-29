#ifndef MONSTERS_H
# define MONSTERS_H
# include "macros.h"

#define TUNNELING 0x00000003
#define SMART 0x00000001
#define ERRATIC 0x00000004
#define TELEPATHIC 0x00000002

typedef struct dungeon dungeon_t;
typedef struct event event_t;

typedef struct monster {
    int characteristics;
    int speed;
    int priority; 
    int xpos;
    int ypos;
}monster_t;


void insert_monsters(dungeon_t *d);
void run_monsters(dungeon_t *d, int num_monsters);
void movea(dungeon_t *d, event_t *p);
void movee(dungeon_t *d, event_t *p);
void movec(dungeon_t *d, event_t *p);
void move0(dungeon_t *d, event_t *p);
void move2(dungeon_t *d, event_t *p);
void move4(dungeon_t *d, event_t *p);
void move6(dungeon_t *d, event_t *p);
void move8(dungeon_t *d, event_t *p);
void movepc(dungeon_t *d, event_t *p);

#endif