#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

/* Very slow seed: 686846853 */

#include "dungeon.h"

#include "pc.h"
#include "npc.h"
#include "move.h"
#include "path.h"
#include "dungeon.h"
#include "<ncurses.h>"

const char *victory =
  "\n                                       o\n"
  "                                      $\"\"$o\n"
  "                                     $\"  $$\n"
  "                                      $$$$\n"
  "                                      o \"$o\n"
  "                                     o\"  \"$\n"
  "                oo\"$$$\"  oo$\"$ooo   o$    \"$    ooo\"$oo  $$$\"o\n"
  "   o o o o    oo\"  o\"      \"o    $$o$\"     o o$\"\"  o$      \"$  "
  "\"oo   o o o o\n"
  "   \"$o   \"\"$$$\"   $$         $      \"   o   \"\"    o\"         $"
  "   \"o$$\"    o$$\n"
  "     \"\"o       o  $          $\"       $$$$$       o          $  ooo"
  "     o\"\"\n"
  "        \"o   $$$$o $o       o$        $$$$$\"       $o        \" $$$$"
  "   o\"\n"
  "         \"\"o $$$$o  oo o  o$\"         $$$$$\"        \"o o o o\"  "
  "\"$$$  $\n"
  "           \"\" \"$\"     \"\"\"\"\"            \"\"$\"            \""
  "\"\"      \"\"\" \"\n"
  "            \"oooooooooooooooooooooooooooooooooooooooooooooooooooooo$\n"
  "             \"$$$$\"$$$$\" $$$$$$$\"$$$$$$ \" \"$$$$$\"$$$$$$\"  $$$\""
  "\"$$$$\n"
  "              $$$oo$$$$   $$$$$$o$$$$$$o\" $$$$$$$$$$$$$$ o$$$$o$$$\"\n"
  "              $\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\""
  "\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"$\n"
  "              $\"                                                 \"$\n"
  "              $\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\""
  "$\"$\"$\"$\"$\"$\"$\"$\n"
  "                                   You win!\n\n";

const char *tombstone =
  "\n\n\n\n                /\"\"\"\"\"/\"\"\"\"\"\"\".\n"
  "               /     /         \\             __\n"
  "              /     /           \\            ||\n"
  "             /____ /   Rest in   \\           ||\n"
  "            |     |    Pieces     |          ||\n"
  "            |     |               |          ||\n"
  "            |     |   A. Luser    |          ||\n"
  "            |     |               |          ||\n"
  "            |     |     * *   * * |         _||_\n"
  "            |     |     *\\/* *\\/* |        | TT |\n"
  "            |     |     *_\\_  /   ...\"\"\"\"\"\"| |"
  "| |.\"\"....\"\"\"\"\"\"\"\".\"\"\n"
  "            |     |         \\/..\"\"\"\"\"...\"\"\""
  "\\ || /.\"\"\".......\"\"\"\"...\n"
  "            |     |....\"\"\"\"\"\"\"........\"\"\"\"\""
  "\"^^^^\".......\"\"\"\"\"\"\"\"..\"\n"
  "            |......\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"......"
  "..\"\"\"\"\"....\"\"\"\"\"..\"\"...\"\"\".\n\n"
  "            You're dead.  Better luck in the next life.\n\n\n";

void usage(char *name)
{
  fprintf(stderr,
          "Usage: %s [-r|--rand <seed>] [-l|--load [<file>]]\n"
          "          [-s|--save [<file>]] [-i|--image <pgm file>]\n"
          "          [-n|--nummon <count>]",
          name);

  exit(-1);
}




void PlayerCharControl(dungeon_t *d)
{
  pair_t newpos;
  newpos[dim_x] = d->pc.position[dim_x];
  newpos[dim_y] = d->pc.position[dim_y];

  int keyboardRes; 
  int inbound = 1;

  while(inbound)
  {
    keyboardRes = getch();

    switch(keyboardRes)
    {
      //7 or y  Attempt to move PC one cell to the upper left. 7 = 55, y = 89
      case 55:
        newpos[dim_x]--;
        newpos[dim_y]--;
        if(mappair(newpos) == ter_wall_immutable || mappair(newpos) == ter_wall)
        {
          printf("wrong position");
          newpos[dim_x]++;
          newpos[dim_y]++;
        }else{
          move_character(d, &d->pc, newpos);
          inbound = 0; 
          dijkstra(d);
          dijkstra_tunnel(d);
        }
        break;
      case 89:
        newpos[dim_x]--;
        newpos[dim_y]--;
        if(mappair(newpos) == ter_wall_immutable || mappair(newpos) == ter_wall)
        {
          printf("wrong position");
          newpos[dim_x]++;
          newpos[dim_y]++;
        }else{
          move_character(d, &d->pc, newpos);
          inbound = 0; 
          dijkstra(d);
          dijkstra_tunnel(d);
        }
        break;
      //8 or k Attempt to move PC one cell up. 8 = 56  , k = 75
        case 56:
        newpos[dim_y]--;
        if(mappair(newpos) == ter_wall_immutable || mappair(newpos) == ter_wall)
        {
          printf("wrong position");
          newpos[dim_y]++;
        }else{
          move_character(d, &d->pc, newpos);
          inbound = 0; 
          dijkstra(d);
          dijkstra_tunnel(d);
        }
        break;
      case 75:
        newpos[dim_y]--;
        if(mappair(newpos) == ter_wall_immutable || mappair(newpos) == ter_wall)
        {
          printf("wrong position");
          newpos[dim_y]++;
        }else{
          move_character(d, &d->pc, newpos);
          inbound = 0; 
          dijkstra(d);
          dijkstra_tunnel(d);
        }
        break;

        //9 or u Attempt to move PC one cell to the upper right. 9 = 57, u = 85
        case 57:
        newpos[dim_x]++;
        newpos[dim_y]--;
        if(mappair(newpos) == ter_wall_immutable || mappair(newpos) == ter_wall)
        {
          printf("wrong position");
          newpos[dim_x]--;
          newpos[dim_y]++;
        }else{
          move_character(d, &d->pc, newpos);
          inbound = 0; 
          dijkstra(d);
          dijkstra_tunnel(d);
        }
        break;
      case 89:
        newpos[dim_x]++;
        newpos[dim_y]--;
        if(mappair(newpos) == ter_wall_immutable || mappair(newpos) == ter_wall)
        {
          printf("wrong position");
          newpos[dim_x]--;
          newpos[dim_y]++;
        }else{
          move_character(d, &d->pc, newpos);
          inbound = 0; 
          dijkstra(d);
          dijkstra_tunnel(d);
        }
        break;

      //6 or l  Attempt to move PC one cell to the right. 6 = 54 , l = 76
       case 54:
        newpos[dim_x]++;
        if(mappair(newpos) == ter_wall_immutable || mappair(newpos) == ter_wall)
        {
          printf("wrong position");
          newpos[dim_x]--;
        }else{
          move_character(d, &d->pc, newpos);
          inbound = 0; 
          dijkstra(d);
          dijkstra_tunnel(d);
        }
        break;
      case 76:
        newpos[dim_x]++;
        if(mappair(newpos) == ter_wall_immutable || mappair(newpos) == ter_wall)
        {
          printf("wrong position");
          newpos[dim_x]--;
        }else{
          move_character(d, &d->pc, newpos);
          inbound = 0; 
          dijkstra(d);
          dijkstra_tunnel(d);
        }
        break;

        //3 or n Attempt to move PC one cell to the lower right. 3 = 51, n = 78
        case 51:
        newpos[dim_x]++;
        newpos[dim_y]++;
        if(mappair(newpos) == ter_wall_immutable || mappair(newpos) == ter_wall)
        {
          printf("wrong position");
          newpos[dim_x]--;
          newpos[dim_y]--;
        }else{
          move_character(d, &d->pc, newpos);
          inbound = 0; 
          dijkstra(d);
          dijkstra_tunnel(d);
        }
        break;
      case 78:
        newpos[dim_x]++;
        newpos[dim_y]++;
        if(mappair(newpos) == ter_wall_immutable || mappair(newpos) == ter_wall)
        {
          printf("wrong position");
          newpos[dim_x]--;
          newpos[dim_y]--;
        }else{
          move_character(d, &d->pc, newpos);
          inbound = 0; 
          dijkstra(d);
          dijkstra_tunnel(d);
        }
        break;

        //2 or j Attempt to move PC one cell down. 2 = 50, j = 74
        case 50:
        newpos[dim_y]++;
        if(mappair(newpos) == ter_wall_immutable || mappair(newpos) == ter_wall)
        {
          printf("wrong position");
          newpos[dim_y]--;
        }else{
          move_character(d, &d->pc, newpos);
          inbound = 0; 
          dijkstra(d);
          dijkstra_tunnel(d);
        }
        break;
      case 74:
        newpos[dim_y]++;
        if(mappair(newpos) == ter_wall_immutable || mappair(newpos) == ter_wall)
        {
          printf("wrong position");
          newpos[dim_y]--;
        }else{
          move_character(d, &d->pc, newpos);
          inbound = 0; 
          dijkstra(d);
          dijkstra_tunnel(d);
        }
        break;


       // 1 or b Attempt to move PC one cell to the lower left. 1 = 49, b = 66
       case 49:
        newpos[dim_x]--;
        newpos[dim_y]++;
        if(mappair(newpos) == ter_wall_immutable || mappair(newpos) == ter_wall)
        {
          printf("wrong position");
          newpos[dim_x]++;
          newpos[dim_y]--;
        }else{
          move_character(d, &d->pc, newpos);
          inbound = 0; 
          dijkstra(d);
          dijkstra_tunnel(d);
        }
        break;
      case 66:
        newpos[dim_x]--;
        newpos[dim_y]++;
        if(mappair(newpos) == ter_wall_immutable || mappair(newpos) == ter_wall)
        {
          printf("wrong position");
          newpos[dim_x]++;
          newpos[dim_y]--;
        }else{
          move_character(d, &d->pc, newpos);
          inbound = 0; 
          dijkstra(d);
          dijkstra_tunnel(d);
        }
        break;

       // 4 or h Attempt to move PC one cell to the left. 4 = 52, h = 72
        case 52:
        newpos[dim_x]--;
        if(mappair(newpos) == ter_wall_immutable || mappair(newpos) == ter_wall)
        {
          printf("wrong position");
          newpos[dim_x]++;
        }else{
          move_character(d, &d->pc, newpos);
          inbound = 0; 
          dijkstra(d);
          dijkstra_tunnel(d);
        }
        break;
      case 72:
        newpos[dim_x]--;
        if(mappair(newpos) == ter_wall_immutable || mappair(newpos) == ter_wall)
        {
          printf("wrong position");
          newpos[dim_x]++;
        }else{
          move_character(d, &d->pc, newpos);
          inbound = 0; 
          dijkstra(d);
          dijkstra_tunnel(d);
        }
        break;
      
          //Attempt to go down stairs. Works only if standing on down staircase. > = 190, Delete the old dungeon and make a new one
        case 190:
        if(mappair(newpos) == ter_stair_down)
        {
          delete_dungeon(d);
          free(d -> pc.pc);
          init_dungeon(d);
          gen_dungeon(d);
          config_pc(d);
          gen_monsters(d);
          d->inLookMood = 0;
          d->end = 0;
          inbound = 0;
        }else{
          printf("Not valid position to go down the stairs.")
        }
        break;

        //Attempt to go up stairs. Works only if standing on down staircase. < = 188, Delete the old dungeon and make a new one
        case 188:
        if(mappair(newpos) == ter_stair_up)
        {
          delete_dungeon(d);
          free(d -> pc.pc);
          init_dungeon(d);
          gen_dungeon(d);
          config_pc(d);
          gen_monsters(d);
          d->end = 0;
          inbound = 0;
        }else{
          printf("Not valid position to go up the stairs.")
        }
        break;

       // 5 or space Rest for a turn. NPCs still move. 5 = 53 , space = 32
       case 53:
        inbound = 0; 
        break; 
        case 32:
        inbound = 0; 
        break; 

       // m Display a list of monsters in the dungeon, with their symbol and position relative to the PC (e.g.: “c, 2 north and 14 west”)., m = 77
       case 77:
        printf("Still working");
        break;

      //up arrow When displaying monster list, if entire list does not ﬁt in screen and not currently at top of list, scroll list up. up = 38
      case 38:
      printf("Still working");
      break;

      //down arrow When displaying monster list, if entire list does not ﬁt in screen and not currently at bottom of list, scroll list down. down = 40
      case 40: 
      printf("still working");
      break;

      //escape When displaying monster list, return to character control. escape = 27
      case 27: 
      printf("still working");
      break;

      //Q  Quit the game. Q = 81
      case 81:
      d->end = 1;
      printf("Game Ended");
      return;
      break;

      default:
      printf("Invalid Command");
    }
  }

}



int main(int argc, char *argv[])
{
  dungeon_t d;
  time_t seed;
  struct timeval tv;
  uint32_t i;
  uint32_t do_load, do_save, do_seed, do_image, do_save_seed, do_save_image;
  uint32_t long_arg;
  char *save_file;
  char *load_file;
  char *pgm_file;

  /* Quiet a false positive from valgrind. */
  memset(&d, 0, sizeof (d));
  
  /* Default behavior: Seed with the time, generate a new dungeon, *
   * and don't write to disk.                                      */
  do_load = do_save = do_image = do_save_seed = do_save_image = 0;
  do_seed = 1;
  save_file = load_file = NULL;
  d.max_monsters = MAX_MONSTERS;

  /* The project spec requires '--load' and '--save'.  It's common  *
   * to have short and long forms of most switches (assuming you    *
   * don't run out of letters).  For now, we've got plenty.  Long   *
   * forms use whole words and take two dashes.  Short forms use an *
    * abbreviation after a single dash.  We'll add '--rand' (to     *
   * specify a random seed), which will take an argument of it's    *
   * own, and we'll add short forms for all three commands, '-l',   *
   * '-s', and '-r', respectively.  We're also going to allow an    *
   * optional argument to load to allow us to load non-default save *
   * files.  No means to save to non-default locations, however.    *
   * And the final switch, '--image', allows me to create a dungeon *
   * from a PGM image, so that I was able to create those more      *
   * interesting test dungeons for you.                             */
 
 if (argc > 1) {
    for (i = 1, long_arg = 0; i < argc; i++, long_arg = 0) {
      if (argv[i][0] == '-') { /* All switches start with a dash */
        if (argv[i][1] == '-') {
          argv[i]++;    /* Make the argument have a single dash so we can */
          long_arg = 1; /* handle long and short args at the same place.  */
        }
        switch (argv[i][1]) {
        case 'n':
          if ((!long_arg && argv[i][2]) ||
              (long_arg && strcmp(argv[i], "-nummon")) ||
              argc < ++i + 1 /* No more arguments */ ||
              !sscanf(argv[i], "%hu", &d.max_monsters)) {
            usage(argv[0]);
          }
          break;
        case 'r':
          if ((!long_arg && argv[i][2]) ||
              (long_arg && strcmp(argv[i], "-rand")) ||
              argc < ++i + 1 /* No more arguments */ ||
              !sscanf(argv[i], "%lu", &seed) /* Argument is not an integer */) {
            usage(argv[0]);
          }
          do_seed = 0;
          break;
        case 'l':
          if ((!long_arg && argv[i][2]) ||
              (long_arg && strcmp(argv[i], "-load"))) {
            usage(argv[0]);
          }
          do_load = 1;
          if ((argc > i + 1) && argv[i + 1][0] != '-') {
            /* There is another argument, and it's not a switch, so *
             * we'll treat it as a save file and try to load it.    */
            load_file = argv[++i];
          }
          break;
        case 's':
          if ((!long_arg && argv[i][2]) ||
              (long_arg && strcmp(argv[i], "-save"))) {
            usage(argv[0]);
          }
          do_save = 1;
          if ((argc > i + 1) && argv[i + 1][0] != '-') {
            /* There is another argument, and it's not a switch, so *
             * we'll save to it.  If it is "seed", we'll save to    *
	     * <the current seed>.rlg327.  If it is "image", we'll  *
	     * save to <the current image>.rlg327.                  */
	    if (!strcmp(argv[++i], "seed")) {
	      do_save_seed = 1;
	      do_save_image = 0;
	    } else if (!strcmp(argv[i], "image")) {
	      do_save_image = 1;
	      do_save_seed = 0;
	    } else {
	      save_file = argv[i];
	    }
          }
          break;
        case 'i':
          if ((!long_arg && argv[i][2]) ||
              (long_arg && strcmp(argv[i], "-image"))) {
            usage(argv[0]);
          }
          do_image = 1;
          if ((argc > i + 1) && argv[i + 1][0] != '-') {
            /* There is another argument, and it's not a switch, so *
             * we'll treat it as a save file and try to load it.    */
            pgm_file = argv[++i];
          }
          break;
        default:
          usage(argv[0]);
        }
      } else { /* No dash */
        usage(argv[0]);
      }
    }
  }

  if (do_seed) {
    /* Allows me to generate more than one dungeon *
     * per second, as opposed to time().           */
    gettimeofday(&tv, NULL);
    seed = (tv.tv_usec ^ (tv.tv_sec << 20)) & 0xffffffff;
  }

  if (!do_load && !do_image) {
    printf("Seed is %ld.\n", seed);
  }
  srand(seed);


  initscr();
  raw();
  noecho();
  keypad(stdscr, TRUE);
  curs_set(0);
  init_dungeon(&d);

  if (do_load) {
    read_dungeon(&d, load_file);
  } else if (do_image) {
    read_pgm(&d, pgm_file);
  } else {
    gen_dungeon(&d);
  }

  /* Ignoring PC position in saved dungeons.  Not a bug. */
  config_pc(&d);
  gen_monsters(&d);

  while (pc_is_alive(&d) && dungeon_has_npcs(&d)) {
    render_dungeon(&d);
    do_moves(&d);
    PlayerCharControl(&d);
    usleep(33000);
  }

  render_dungeon(&d);

  if (do_save) {
    if (do_save_seed) {
       /* 10 bytes for number, please dot, extention and null terminator. */
      save_file = malloc(18);
      sprintf(save_file, "%ld.rlg327", seed);
    }
    if (do_save_image) {
      if (!pgm_file) {
	fprintf(stderr, "No image file was loaded.  Using default.\n");
	do_save_image = 0;
      } else {
	/* Extension of 3 characters longer than image extension + null. */
	save_file = malloc(strlen(pgm_file) + 4);
	strcpy(save_file, pgm_file);
	strcpy(strchr(save_file, '.') + 1, "rlg327");
      }
    }
    write_dungeon(&d, save_file);

    if (do_save_seed || do_save_image) {
      free(save_file);
    }
  }

  printf("%s", pc_is_alive(&d) ? victory : tombstone);
  printf("You defended your life in the face of %u deadly beasts.\n"
         "You avenged the cruel and untimely murders of %u "
         "peaceful dungeon residents.\n",
         d.pc.kills[kill_direct], d.pc.kills[kill_avenged]);

  pc_delete(d.pc.pc);

  delete_dungeon(&d);

  return 0;
}
