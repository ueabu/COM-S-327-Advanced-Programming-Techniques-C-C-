#include <stdlib.h>

#include "string.h"

#include "dungeon.h"
#include "pc.h"
#include "utils.h"
#include "move.h"
#include "path.h"
#include "ncurses.h"


void pc_delete(pc_t *pc)
{
  if (pc) {
    free(pc);
  }
}

uint32_t pc_is_alive(dungeon_t *d)
{
  return d->pc.alive;
}

void place_pc(dungeon_t *d)
{
  d->pc.position[dim_y] = rand_range(d->rooms->position[dim_y],
                                     (d->rooms->position[dim_y] +
                                      d->rooms->size[dim_y] - 1));
  d->pc.position[dim_x] = rand_range(d->rooms->position[dim_x],
                                     (d->rooms->position[dim_x] +
                                      d->rooms->size[dim_x] - 1));
}

void config_pc(dungeon_t *d)
{
  memset(&d->pc, 0, sizeof (d->pc));
  d->pc.symbol = '@';

  place_pc(d);

  d->pc.speed = PC_SPEED;
  d->pc.alive = 1;
  d->pc.sequence_number = 0;
  d->pc.pc = calloc(1, sizeof (*d->pc.pc));
  d->pc.npc = NULL;
  d->pc.kills[kill_direct] = d->pc.kills[kill_avenged] = 0;

  d->character[d->pc.position[dim_y]][d->pc.position[dim_x]] = &d->pc;
  d->stairs = malloc(sizeof (*d->stairs) * 3);

  int i; 
  int j;
  uint32_t num;

  for(i = 0; i < 3; i++)
  {
    num = rand_range(1, d->num_rooms - 1);

    d->stairs[i].position[dim_y] = rand_range(d->rooms[num].position[dim_y], (d->rooms[num].position[dim_y] + d->rooms[num].size[dim_y] - 1));
    d->stairs[i].position[dim_x] = rand_range(d->rooms[num].position[dim_x], (d->rooms[num].position[dim_x] + d->rooms[num].size[dim_x] - 1));
    j = rand() % 2 + 1;
    if(j == 1)
    {
      d->map[d->stairs[i].position[dim_y]][d->stairs[i].position[dim_x]] = ter_stair_up;
      d->stairs[i].character = '<';
    }else{
      d->map[d->stairs[i].position[dim_y]][d->stairs[i].position[dim_x]] = ter_stair_down;
      d->stairs[i].character = '>';
    }
  }

  dijkstra(d);
  dijkstra_tunnel(d);

}

uint32_t pc_next_pos(dungeon_t *d, pair_t dir)
{
  static uint32_t have_seen_corner = 0;
  static uint32_t count = 0;

  dir[dim_y] = dir[dim_x] = 0;

  if (in_corner(d, &d->pc)) {
    if (!count) {
      count = 1;
    }
    have_seen_corner = 1;
  }

  /* First, eat anybody standing next to us. */
  if (charxy(d->pc.position[dim_x] - 1, d->pc.position[dim_y] - 1)) {
    dir[dim_y] = -1;
    dir[dim_x] = -1;
  } else if (charxy(d->pc.position[dim_x], d->pc.position[dim_y] - 1)) {
    dir[dim_y] = -1;
  } else if (charxy(d->pc.position[dim_x] + 1, d->pc.position[dim_y] - 1)) {
    dir[dim_y] = -1;
    dir[dim_x] = 1;
  } else if (charxy(d->pc.position[dim_x] - 1, d->pc.position[dim_y])) {
    dir[dim_x] = -1;
  } else if (charxy(d->pc.position[dim_x] + 1, d->pc.position[dim_y])) {
    dir[dim_x] = 1;
  } else if (charxy(d->pc.position[dim_x] - 1, d->pc.position[dim_y] + 1)) {
    dir[dim_y] = 1;
    dir[dim_x] = -1;
  } else if (charxy(d->pc.position[dim_x], d->pc.position[dim_y] + 1)) {
    dir[dim_y] = 1;
  } else if (charxy(d->pc.position[dim_x] + 1, d->pc.position[dim_y] + 1)) {
    dir[dim_y] = 1;
    dir[dim_x] = 1;
  } else if (!have_seen_corner || count < 250) {
    /* Head to a corner and let most of the NPCs kill each other off */
    if (count) {
      count++;
    }
    if (!against_wall(d, &d->pc) && ((rand() & 0x111) == 0x111)) {
      dir[dim_x] = (rand() % 3) - 1;
      dir[dim_y] = (rand() % 3) - 1;
    } else {
      dir_nearest_wall(d, &d->pc, dir);
    }
  }else {
    /* And after we've been there, let's head toward the center of the map. */
    if (!against_wall(d, &d->pc) && ((rand() & 0x111) == 0x111)) {
      dir[dim_x] = (rand() % 3) - 1;
      dir[dim_y] = (rand() % 3) - 1;
    } else {
      dir[dim_x] = ((d->pc.position[dim_x] > DUNGEON_X / 2) ? -1 : 1);
      dir[dim_y] = ((d->pc.position[dim_y] > DUNGEON_Y / 2) ? -1 : 1);
    }
  }

  /* Don't move to an unoccupied location if that places us next to a monster */
  if (!charxy(d->pc.position[dim_x] + dir[dim_x],
              d->pc.position[dim_y] + dir[dim_y]) &&
      ((charxy(d->pc.position[dim_x] + dir[dim_x] - 1,
               d->pc.position[dim_y] + dir[dim_y] - 1) &&
        (charxy(d->pc.position[dim_x] + dir[dim_x] - 1,
                d->pc.position[dim_y] + dir[dim_y] - 1) != &d->pc)) ||
       (charxy(d->pc.position[dim_x] + dir[dim_x] - 1,
               d->pc.position[dim_y] + dir[dim_y]) &&
        (charxy(d->pc.position[dim_x] + dir[dim_x] - 1,
                d->pc.position[dim_y] + dir[dim_y]) != &d->pc)) ||
       (charxy(d->pc.position[dim_x] + dir[dim_x] - 1,
               d->pc.position[dim_y] + dir[dim_y] + 1) &&
        (charxy(d->pc.position[dim_x] + dir[dim_x] - 1,
                d->pc.position[dim_y] + dir[dim_y] + 1) != &d->pc)) ||
       (charxy(d->pc.position[dim_x] + dir[dim_x],
               d->pc.position[dim_y] + dir[dim_y] - 1) &&
        (charxy(d->pc.position[dim_x] + dir[dim_x],
                d->pc.position[dim_y] + dir[dim_y] - 1) != &d->pc)) ||
       (charxy(d->pc.position[dim_x] + dir[dim_x],
               d->pc.position[dim_y] + dir[dim_y] + 1) &&
        (charxy(d->pc.position[dim_x] + dir[dim_x],
                d->pc.position[dim_y] + dir[dim_y] + 1) != &d->pc)) ||
       (charxy(d->pc.position[dim_x] + dir[dim_x] + 1,
               d->pc.position[dim_y] + dir[dim_y] - 1) &&
        (charxy(d->pc.position[dim_x] + dir[dim_x] + 1,
                d->pc.position[dim_y] + dir[dim_y] - 1) != &d->pc)) ||
       (charxy(d->pc.position[dim_x] + dir[dim_x] + 1,
               d->pc.position[dim_y] + dir[dim_y]) &&
        (charxy(d->pc.position[dim_x] + dir[dim_x] + 1,
                d->pc.position[dim_y] + dir[dim_y]) != &d->pc)) ||
       (charxy(d->pc.position[dim_x] + dir[dim_x] + 1,
               d->pc.position[dim_y] + dir[dim_y] + 1) &&
        (charxy(d->pc.position[dim_x] + dir[dim_x] + 1,
                d->pc.position[dim_y] + dir[dim_y] + 1) != &d->pc)))) {
    dir[dim_x] = dir[dim_y] = 0;
  }

  int keyboardRes; 
  keyboardRes = getch();

  switch(keyboardRes)
    {
      //7 or y  Attempt to move PC one cell to the upper left. 7 = 55,
      case 55:
      case 121:
      dir[dim_x] = -1;
      dir[dim_y] = -1;
      break;
     
      //8 or k Attempt to move PC one cell up. 8 = 56  , k = 75
        case 56:
        case 107:
        dir[dim_x] = 0;
        dir[dim_y] = -1;
        break;
      
        //9 or u Attempt to move PC one cell to the upper right. 9 = 57, u = 85
        case 57:
        case 117:
        dir[dim_x] = 1;
        dir[dim_y] = -1;
        break;
    

      //6 or l  Attempt to move PC one cell to the right. 6 = 54 , l = 76
       case 54:
       case 108:
       dir[dim_x] = 1;
       dir[dim_y] = 0;
       break;

        //3 or n Attempt to move PC one cell to the lower right. 3 = 51, n = 78
        case 51:
        case 110:
        dir[dim_x] = 1;
        dir[dim_y] = 1;
        break;


        //2 or j Attempt to move PC one cell down. 2 = 50, 
        case 50:
        case 106:
        dir[dim_y] = 1;
        dir[dim_x] = 0;
        break;


       // 1 or b Attempt to move PC one cell to the lower left. 1 = 49, b = 66
       case 49:
       case 98:
        dir[dim_x] = -1;
        dir[dim_y] = 1;
        break;

       // 4 or h Attempt to move PC one cell to the left. 4 = 52,
        case 52:
        case 104:
        dir[dim_x] = -1;
        dir[dim_y] = 0;
        break;

      
          //Attempt to go down stairs. Works only if standing on down staircase. > = 190, Delete the old dungeon and make a new one
      case 62:
      if(d->map[d->pc.position[dim_y]][d->pc.position[dim_x]] == ter_stair_down){
        d->mode = '>';
      }
			break;

      //Attempt to go up stairs. Works only if standing on down staircase. < = 188, Delete the old dungeon and make a new one

		 case 60:           
      if(d->map[d->pc.position[dim_y]][d->pc.position[dim_x]] == ter_stair_up){
        d->mode = '<';
      }
			break;
	        
     //Q  Quit the game. Q = 81
      case 113:	
      d->mode = 'Q';
      break;

    
      // 5 or space Rest for a turn. NPCs still move. 5 = 53 , space = 32
      case 53:
      case 32:
      dir[dim_x] = 0;
      dir[dim_y] = 0;
      break; 
      

      // m Display a list of monsters in the dungeon, with their symbol and position relative to the PC (e.g.: “c, 2 north and 14 west”)., m = 77
      case 109:
      d->mode = 'm';
      break;

      default:
      printf("Invalid Command");
    }
  return 0;
 }


uint32_t pc_in_room(dungeon_t *d, uint32_t room)
{
  if ((room < d->num_rooms)                                     &&
      (d->pc.position[dim_x] >= d->rooms[room].position[dim_x]) &&
      (d->pc.position[dim_x] < (d->rooms[room].position[dim_x] +
                                d->rooms[room].size[dim_x]))    &&
      (d->pc.position[dim_y] >= d->rooms[room].position[dim_y]) &&
      (d->pc.position[dim_y] < (d->rooms[room].position[dim_y] +
                                d->rooms[room].size[dim_y]))) {
    return 1;
  }

  return 0;
}
