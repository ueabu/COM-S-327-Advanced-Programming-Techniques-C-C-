#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "monster.h"
#include "dungeon.h"
#include "path.h"

//Method to compare Events
static int32_t priority_compare(const void *val1, const void *val2)
{
    if(((event_t *) val1)->next_turn - ((event_t *) val2)->next_turn == 0)
    {
	  return ((event_t *) val1)->priority - ((event_t *) val2)->priority;
    }else{
	  return ((event_t *) val1)->next_turn - ((event_t *) val2)->next_turn;
  }
}

//inserting the monsters into the dungeon
void insert_monsters(dungeon_t *d)
{
    int x, y, xlength, ylength;
    int m = 1; 
    for(int i = 0; i < d->num_monsters; i++)
    {
        xlength = d->rooms[m].size[0];
		ylength = d->rooms[m].size[1];
        x = d->rooms[m].position[0];
		y = d->rooms[m].position[1];
		
        d->monsters[i].xpos = rand() % xlength + x;
		d->monsters[i].ypos = rand() % ylength + y;
        m++;
        //reset number of rooms if we run out of rooms
        if(m == d->num_rooms)
        {
			m = 1;
		}
    }
}




void run_monsters(dungeon_t *d, int num_monsters)
{
    heap_t h;
    heap_init(&h, priority_compare, NULL);
    pc_t *pc_p = &d->pc; // Getting the player character

    d->num_monsters = num_monsters; //Setting the number of monsters in the dungeon;
    d->events = malloc(sizeof(*d->events) * (num_monsters + 1));
    

    //initiallizing the first event which is the player character event; 
    d->events[0].priority = 0; //initiallizing priority 
    d->events[0].speed = 10; //initiallizing speed
    d->events[0].e_type = 'p'; 
    d->events[0].c_type.p = pc_p;
    d->events[0].next_turn = 1000 / d->events[0].speed; //Next turn = currentl + 1000/current
    d->events[0].headnode = heap_insert(&h, &d->events[0]);	

    d->monsters = malloc(sizeof(*d->monsters) * num_monsters); //initilizing size of monsters in the dungeon 

    //initiallizing events for every monster
    srand(time(NULL));
    for(int i=0; i < num_monsters; i++)
    {
		d->monsters[i].speed = rand() % (20-5+1) + 5; //initilizing speed, Random number between 5 and 20
		d->monsters[i].priority = i+1; //Initilizing priority
        d->monsters[i].characteristics = rand() & 0x0000000f; //initilizing characteristics
        //inintilizing events informations
		d->events[i+1].c_type.m = &d->monsters[i]; 
		d->events[i+1].e_type = 'm';
		d->events[i+1].priority = d->monsters[i].priority;
		d->events[i+1].speed = d->monsters[i].speed;
		d->events[i+1].next_turn = 1000 / d->events[i+1].speed;
		d->events[i+1].headnode = heap_insert(&h, &d->events[i+1]);
	}
    //
    insert_monsters(d);
    render_dungeon(d);

    static event_t *p;
    int end = 1; 
    int random;

    while ((p = heap_remove_min(&h)) && end)
    {		
		if(p->e_type == 'p') //Move player - Render Dungeon. Creating maps everytime I move players or render dungeon
        {
			movepc(d,p);
			dijkstra(d);
			dijkstra_tunnel(d);
			render_dungeon(d);
			p->next_turn += (1000 / p->speed); //Calculating next turn for player //Next turn = currentl + 1000/current
		}else{
            //not players turn 
			if((p->c_type.m->characteristics & TELEPATHIC) && (p->c_type.m->characteristics & TUNNELING) && (p->c_type.m->characteristics & ERRATIC)  && (p->c_type.m->characteristics & SMART))
            {
			    random  = rand() % 2 + 1;				 
			    if(random == 1)
                {
                    movee(d,p);
                }else{
                    move2(d,p);
                }
			}else if((p->c_type.m->characteristics & SMART) && (p->c_type.m->characteristics & TELEPATHIC)  && (p->c_type.m->characteristics & TUNNELING))
            {
				movee(d, p);
			}else if((p->c_type.m->characteristics & SMART) && (p->c_type.m->characteristics & TELEPATHIC) && (p->c_type.m->characteristics & ERRATIC))
            {
				random = rand() % 2 + 1;
				if(random == 1)
                {
                    movec(d,p);
                }else{
                    move0(d,p);
                }
			}else if((p->c_type.m->characteristics & SMART) && (p->c_type.m->characteristics & TUNNELING) && (p->c_type.m->characteristics & ERRATIC))
            {
			    random = rand() % 2 + 1;
		        if(random == 1)
                {
                    movea(d,p);
                }else{
                    move2(d,p);
                }
			}else if((p->c_type.m->characteristics & TELEPATHIC) && (p->c_type.m->characteristics & TUNNELING) && (p->c_type.m->characteristics & ERRATIC))
            {		//7
				random = rand() % 2 + 1;
		        if(random == 1)
                {
                    move6(d,p);
                }else{
                    move2(d,p);
                }
			}else if((p->c_type.m->characteristics & SMART) && (p->c_type.m->characteristics & TELEPATHIC))
            {	
				movec(d, p);
			}else if((p->c_type.m->characteristics & SMART) && (p->c_type.m->characteristics & TUNNELING))
            {	
				movea(d,p);
			}else if((p->c_type.m->characteristics & SMART) && (p->c_type.m->characteristics & ERRATIC))
            {		//9
				random = rand() % 2 + 1;
		        if(random == 1)
                {
                    move8(d,p);
                }else{
                    move0(d,p);
                }
			}else if((p->c_type.m->characteristics & TELEPATHIC) && (p->c_type.m->characteristics & TUNNELING))
            {	//6
			    move6(d, p);
			}else if((p->c_type.m->characteristics & TELEPATHIC) && (p->c_type.m->characteristics & ERRATIC))
            {		//5
			    random = rand() % 2 + 1;
		        if(random == 1)
                {
                    move4(d,p);
                }else{
                    move2(d,p);
                }
			}else if((p->c_type.m->characteristics & TUNNELING) && (p->c_type.m->characteristics & ERRATIC))
            {		//3
				move2(d,p);
			}else if(p->c_type.m->characteristics & SMART)
            {			//8
				move8(d,p);
			}else if(p->c_type.m->characteristics & TELEPATHIC)
            {			//4
				move4(d,p);
			}else if(p->c_type.m->characteristics & TUNNELING)
            {		//2
				move2(d,p);
			}else{													//0 + 1
				move0(d,p);
			}
			
			if(d->pc_distance[p->c_type.m->ypos][p->c_type.m->xpos] == 0){
				end = 0;
				render_dungeon(d);
				printf("GAME ENDED YOU DONE BRO\n");
			}
			p->next_turn += (1000 / p->speed);
		}
		usleep(8333);
		p->headnode = heap_insert(&h, p);
	}
}

//Called to move the pc.
void movepc(dungeon_t *d, event_t *p)
{
    if(d->map[d->pc.position[1]+1][d->pc.position[0]] > ter_floor)
    {
		d->pc.position[1] = d->pc.position[1] + 1;
	}
	else if(d->map[d->pc.position[1]][d->pc.position[0]+1] > ter_floor)
    {
		d->pc.position[0] = d->pc.position[0] + 1;
	}
	else if(d->map[d->pc.position[1]][d->pc.position[0]-1] > ter_floor)
    {
		d->pc.position[0] = d->pc.position[0] - 1;
	}
	else if(d->map[d->pc.position[1]-1][d->pc.position[0]] > ter_floor)
    {
		d->pc.position[1] = d->pc.position[1] - 1;
	}
}

void move0(dungeon_t *d, event_t *p)
{
    int random = rand() % 2 + 1; //Geting a random number between 1 and 2 to decide how to move
	if(random == 1)
    {
		if(d->map[p->c_type.m->ypos+1][p->c_type.m->xpos] > ter_floor)
        {
			p->c_type.m->ypos = p->c_type.m->ypos + 1;
		}
		else if(d->map[p->c_type.m->ypos-1][p->c_type.m->xpos] > ter_floor)
        {
			p->c_type.m->ypos = p->c_type.m->ypos - 1;
		}
		else if(d->map[p->c_type.m->ypos][p->c_type.m->xpos+1] > ter_floor)
        {
			p->c_type.m->xpos = p->c_type.m->xpos + 1;
		}
		else if(d->map[p->c_type.m->ypos][p->c_type.m->xpos-1] > ter_floor)
        {
			p->c_type.m->xpos = p->c_type.m->xpos - 1;
		}
	}else if(random == 2)
    {
		if(d->map[p->c_type.m->ypos][p->c_type.m->xpos-1] > ter_floor)
        {
			p->c_type.m->xpos = p->c_type.m->xpos - 1;
		}
		else if(d->map[p->c_type.m->ypos][p->c_type.m->xpos+1] > ter_floor)
        {
			p->c_type.m->xpos = p->c_type.m->xpos + 1;
		}
		else if(d->map[p->c_type.m->ypos-1][p->c_type.m->xpos] > ter_floor)
        {
			p->c_type.m->ypos = p->c_type.m->ypos - 1;
		}
		else if(d->map[p->c_type.m->ypos+1][p->c_type.m->xpos] > ter_floor)
        {
			p->c_type.m->ypos = p->c_type.m->ypos + 1;
		}
	}

}

//For tunneling characteristics movements
void move2(dungeon_t *d, event_t *p)
{
    int direction = rand() % 4 + 1; //Random direction for to move in
	int tunnell = 85;
	
	if(direction == 1)
    {
		if(d->map[p->c_type.m->ypos + 1][p->c_type.m->xpos] != ter_wall_immutable)
        {
			if(d->hardness[p->c_type.m->ypos + 1][p->c_type.m->xpos] > 0)
            {
				if((tunnell - d->hardness[p->c_type.m->ypos+1][p->c_type.m->xpos]) > 0)
                {
					d->hardness[p->c_type.m->ypos+1][p->c_type.m->xpos] = 0;
				}else{
					d->hardness[p->c_type.m->ypos+1][p->c_type.m->xpos] -= 85;
				}
				dijkstra_tunnel(d);
			}else{
				if(d->map[p->c_type.m->ypos + 1][p->c_type.m->xpos] == ter_wall)
                {
					d->map[p->c_type.m->ypos + 1][p->c_type.m->xpos] = ter_floor_hall;
					dijkstra(d);
					dijkstra_tunnel(d);
				}
				p->c_type.m->ypos = p->c_type.m->ypos + 1;
			}
		}
	}
	else if(direction == 2)
    {
		if(d->map[p->c_type.m->ypos][p->c_type.m->xpos+1] != ter_wall_immutable)
        {
			if(d->hardness[p->c_type.m->ypos][p->c_type.m->xpos+1] > 0)
            {
				if((tunnell - d->hardness[p->c_type.m->ypos][p->c_type.m->xpos+1]) > 0)
                {
					d->hardness[p->c_type.m->ypos][p->c_type.m->xpos+1] = 0;
				}else{
					d->hardness[p->c_type.m->ypos][p->c_type.m->xpos+1] -= 85;
				}
				dijkstra_tunnel(d);
			}else{
				if(d->map[p->c_type.m->ypos][p->c_type.m->xpos+1] == ter_wall)
                {
					d->map[p->c_type.m->ypos][p->c_type.m->xpos+1] = ter_floor_hall;
					dijkstra(d);
					dijkstra_tunnel(d);
				}
				p->c_type.m->xpos = p->c_type.m->xpos + 1;
			}
		}
	}
	else if(direction == 3)
    {
		if(d->map[p->c_type.m->ypos - 1][p->c_type.m->xpos] != ter_wall_immutable)
        {
			if(d->hardness[p->c_type.m->ypos - 1][p->c_type.m->xpos] > 0)
            {
				if((tunnell - d->hardness[p->c_type.m->ypos-1][p->c_type.m->xpos]) > 0)
                {
					d->hardness[p->c_type.m->ypos-1][p->c_type.m->xpos] = 0;
				}else{
					d->hardness[p->c_type.m->ypos-1][p->c_type.m->xpos] -= 85;
				}
				dijkstra_tunnel(d);
			}else{
				d->hardness[p->c_type.m->ypos - 1][p->c_type.m->xpos] = 0;
				if(d->map[p->c_type.m->ypos - 1][p->c_type.m->xpos] == ter_wall)
                {
					d->map[p->c_type.m->ypos - 1][p->c_type.m->xpos] = ter_floor_hall;
					dijkstra(d);
					dijkstra_tunnel(d);
				}
				p->c_type.m->ypos = p->c_type.m->ypos - 1;
			}
		}
	}
	else if(direction == 4)
    {
		if(d->map[p->c_type.m->ypos][p->c_type.m->xpos-1] != ter_wall_immutable)
        {
			if(d->hardness[p->c_type.m->ypos][p->c_type.m->xpos-1] > 0)
            {
				if((tunnell - d->hardness[p->c_type.m->ypos][p->c_type.m->xpos-1]) > 0)
                {
					d->hardness[p->c_type.m->ypos][p->c_type.m->xpos-1] = 0;
				}else{
					d->hardness[p->c_type.m->ypos][p->c_type.m->xpos-1] -= 85;
				}
				dijkstra_tunnel(d);
			}else if(d->hardness[p->c_type.m->ypos][p->c_type.m->xpos-1] == 0)
            {
				if(d->map[p->c_type.m->ypos][p->c_type.m->xpos-1] == ter_wall)
                {
					d->map[p->c_type.m->ypos][p->c_type.m->xpos-1] = ter_floor_hall;
					dijkstra(d);
					dijkstra_tunnel(d);
				}
				p->c_type.m->xpos = p->c_type.m->xpos-1;
			}
		}
	}
}

//For telepathic Characteristics movements
void move4(dungeon_t *d, event_t *p)
{
    int x = 0;
    int y = 0;
	if((p->c_type.m->ypos - d->pc.position[1]) < 0)
    {
	    y=1;
	}else if((p->c_type.m->ypos - d->pc.position[1]) > 0)
    {
		y=-1;
	}
	if((p->c_type.m->xpos - d->pc.position[0]) < 0)
    {
		x=1;
	}else if((p->c_type.m->xpos - d->pc.position[0]) > 0)
    {
		x=-1;
	}
	if(d->map[p->c_type.m->ypos+y][p->c_type.m->xpos+x] > ter_floor)
    {
		p->c_type.m->ypos = p->c_type.m->ypos + y;
		p->c_type.m->xpos = p->c_type.m->xpos + x;
	}	

}

void move6(dungeon_t *d, event_t *p)
{
    int x = 0;
    int y = 0;
	int tunnell = 85;
	if((p->c_type.m->ypos - d->pc.position[1]) < 0)
    {
		y=1;
	}else if((p->c_type.m->ypos - d->pc.position[1]) > 0)
    {
		y=-1;
	}
	if((p->c_type.m->xpos - d->pc.position[0]) < 0)
    {
		x=1;
	}else if((p->c_type.m->xpos - d->pc.position[0]) > 0)
    {
		x=-1;
	}
	if(d->hardness[p->c_type.m->ypos+y][p->c_type.m->xpos+x] > 0)
    {
		if((tunnell - d->hardness[p->c_type.m->ypos+y][p->c_type.m->xpos+x]) > 0)
        {
			d->hardness[p->c_type.m->ypos+y][p->c_type.m->xpos+x] = 0;
		}else{
			d->hardness[p->c_type.m->ypos+y][p->c_type.m->xpos+x] -= 85;
		}
		dijkstra_tunnel(d);
	}else if(d->hardness[p->c_type.m->ypos+y][p->c_type.m->xpos+x] == 0)
    {
		if(d->map[p->c_type.m->ypos+y][p->c_type.m->xpos+x] == ter_wall)
        {
			d->map[p->c_type.m->ypos+y][p->c_type.m->xpos+x] = ter_floor_hall;
			dijkstra(d);
			dijkstra_tunnel(d);
		}
		p->c_type.m->ypos = p->c_type.m->ypos+y;
		p->c_type.m->xpos = p->c_type.m->xpos+x;
	}
}

//Smart MF
void move8(dungeon_t *d, event_t *p)
{
    if((p->c_type.m->ypos - d->pc.position[1]) > -20 && (p->c_type.m->xpos - d->pc.position[0]) < 20)
    {
		movec(d, p);
	}else{
		move0(d, p);
	}
}

void movea(dungeon_t *d, event_t *p)
{
    if((p->c_type.m->ypos - d->pc.position[1]) > -20 && (p->c_type.m->xpos - d->pc.position[0]) < 20){
		movee(d, p);
	}else{
		move2(d, p);
	}
}

void movec(dungeon_t *d, event_t *p)
{
	if(d->pc_distance[p->c_type.m->ypos+1][p->c_type.m->xpos] < d->pc_distance[p->c_type.m->ypos][p->c_type.m->xpos]){
		p->c_type.m->ypos = p->c_type.m->ypos + 1;
	}
	else if(d->pc_distance[p->c_type.m->ypos-1][p->c_type.m->xpos] < d->pc_distance[p->c_type.m->ypos][p->c_type.m->xpos]){
		p->c_type.m->ypos = p->c_type.m->ypos - 1;
	}
	else if(d->pc_distance[p->c_type.m->ypos][p->c_type.m->xpos+1] < d->pc_distance[p->c_type.m->ypos][p->c_type.m->xpos]){
		p->c_type.m->xpos = p->c_type.m->xpos + 1;
	}
	else if(d->pc_distance[p->c_type.m->ypos][p->c_type.m->xpos-1] < d->pc_distance[p->c_type.m->ypos][p->c_type.m->xpos]){
		p->c_type.m->xpos = p->c_type.m->xpos - 1;
	}
	else if(d->pc_distance[p->c_type.m->ypos+1][p->c_type.m->xpos+1] < d->pc_distance[p->c_type.m->ypos][p->c_type.m->xpos]){
		p->c_type.m->ypos = p->c_type.m->ypos + 1;
		p->c_type.m->xpos = p->c_type.m->xpos + 1;
	}
	else if(d->pc_distance[p->c_type.m->ypos-1][p->c_type.m->xpos+1] < d->pc_distance[p->c_type.m->ypos][p->c_type.m->xpos]){
		p->c_type.m->ypos = p->c_type.m->ypos - 1;
		p->c_type.m->xpos = p->c_type.m->xpos + 1;
	}
	else if(d->pc_distance[p->c_type.m->ypos+1][p->c_type.m->xpos-1] < d->pc_distance[p->c_type.m->ypos][p->c_type.m->xpos]){
		p->c_type.m->ypos = p->c_type.m->ypos + 1;
		p->c_type.m->xpos = p->c_type.m->xpos - 1;
	}
	else if(d->pc_distance[p->c_type.m->ypos-1][p->c_type.m->xpos-1] < d->pc_distance[p->c_type.m->ypos][p->c_type.m->xpos]){
		p->c_type.m->ypos = p->c_type.m->ypos - 1;
		p->c_type.m->xpos = p->c_type.m->xpos - 1;
	}	
}

void movee(dungeon_t *d, event_t *p)
{
    int tunnell = 85;
	if(d->pc_tunnel[p->c_type.m->ypos+1][p->c_type.m->xpos] < d->pc_tunnel[p->c_type.m->ypos][p->c_type.m->xpos])
    {
		if(d->hardness[p->c_type.m->ypos+1][p->c_type.m->xpos] > 0)
        {
			if((tunnell - d->hardness[p->c_type.m->ypos+1][p->c_type.m->xpos]) > 0)
            {
				d->hardness[p->c_type.m->ypos+1][p->c_type.m->xpos] = 0;
			}else{
				d->hardness[p->c_type.m->ypos+1][p->c_type.m->xpos] -= 85;
			}
			dijkstra_tunnel(d);
		}else if(d->hardness[p->c_type.m->ypos+1][p->c_type.m->xpos] == 0)
        {
			if(d->map[p->c_type.m->ypos+1][p->c_type.m->xpos] == ter_wall)
            {
				d->map[p->c_type.m->ypos+1][p->c_type.m->xpos] = ter_floor_hall;
				dijkstra(d);
				dijkstra_tunnel(d);
			}
			p->c_type.m->ypos = p->c_type.m->ypos+1;
		}
	}
	else if(d->pc_tunnel[p->c_type.m->ypos-1][p->c_type.m->xpos] < d->pc_tunnel[p->c_type.m->ypos][p->c_type.m->xpos])
    {
		if(d->hardness[p->c_type.m->ypos-1][p->c_type.m->xpos] > 0)
        {
			if((tunnell - d->hardness[p->c_type.m->ypos-1][p->c_type.m->xpos]) > 0)
            {
				d->hardness[p->c_type.m->ypos-1][p->c_type.m->xpos] = 0;
			}else{
				d->hardness[p->c_type.m->ypos-1][p->c_type.m->xpos] -= 85;
			}
			dijkstra_tunnel(d);
		}else if(d->hardness[p->c_type.m->ypos-1][p->c_type.m->xpos] == 0)
        {
			if(d->map[p->c_type.m->ypos-1][p->c_type.m->xpos] == ter_wall)
            {
				d->map[p->c_type.m->ypos-1][p->c_type.m->xpos] = ter_floor_hall;
				dijkstra(d);
				dijkstra_tunnel(d);
			}
			p->c_type.m->ypos = p->c_type.m->ypos-1;
		}
	}
	else if(d->pc_tunnel[p->c_type.m->ypos][p->c_type.m->xpos+1] < d->pc_tunnel[p->c_type.m->ypos][p->c_type.m->xpos])
    {
		if(d->hardness[p->c_type.m->ypos][p->c_type.m->xpos+1] > 0){
			if((tunnell - d->hardness[p->c_type.m->ypos][p->c_type.m->xpos+1]) > 0)
            {
				d->hardness[p->c_type.m->ypos][p->c_type.m->xpos+1] = 0;
			}else{
				d->hardness[p->c_type.m->ypos][p->c_type.m->xpos+1] -= 85;
			}
			dijkstra_tunnel(d);
		}else if(d->hardness[p->c_type.m->ypos][p->c_type.m->xpos+1] == 0)
        {
			if(d->map[p->c_type.m->ypos][p->c_type.m->xpos+1] == ter_wall)
            {
				d->map[p->c_type.m->ypos][p->c_type.m->xpos+1] = ter_floor_hall;
				dijkstra(d);
				dijkstra_tunnel(d);
			}
			p->c_type.m->xpos = p->c_type.m->xpos+1;
		}
	}
	else if(d->pc_tunnel[p->c_type.m->ypos][p->c_type.m->xpos-1] < d->pc_tunnel[p->c_type.m->ypos][p->c_type.m->xpos])
    {
		if(d->hardness[p->c_type.m->ypos][p->c_type.m->xpos-1] > 0)
        {
			if((tunnell - d->hardness[p->c_type.m->ypos][p->c_type.m->xpos-1]) > 0)
            {
				d->hardness[p->c_type.m->ypos][p->c_type.m->xpos-1] = 0;
			}else{
				d->hardness[p->c_type.m->ypos][p->c_type.m->xpos-1] -= 85;
			}
			dijkstra_tunnel(d);
		}else if(d->hardness[p->c_type.m->ypos][p->c_type.m->xpos-1] == 0)
        {
			if(d->map[p->c_type.m->ypos][p->c_type.m->xpos-1] == ter_wall)
            {
				d->map[p->c_type.m->ypos][p->c_type.m->xpos-1] = ter_floor_hall;
				dijkstra(d);
				dijkstra_tunnel(d);
			}
			p->c_type.m->xpos = p->c_type.m->xpos-1;
		}
	}
	else if(d->pc_tunnel[p->c_type.m->ypos+1][p->c_type.m->xpos+1] < d->pc_tunnel[p->c_type.m->ypos][p->c_type.m->xpos])
    {
		if(d->hardness[p->c_type.m->ypos+1][p->c_type.m->xpos+1] > 0)
        {
			if((tunnell - d->hardness[p->c_type.m->ypos+1][p->c_type.m->xpos+1]) > 0)
            {
				d->hardness[p->c_type.m->ypos+1][p->c_type.m->xpos+1] = 0;
			}else{
				d->hardness[p->c_type.m->ypos+1][p->c_type.m->xpos+1] -= 85;
			}
			dijkstra_tunnel(d);
		}else if(d->hardness[p->c_type.m->ypos+1][p->c_type.m->xpos+1] == 0)
        {
			if(d->map[p->c_type.m->ypos+1][p->c_type.m->xpos+1] == ter_wall)
            {
				d->map[p->c_type.m->ypos+1][p->c_type.m->xpos+1] = ter_floor_hall;
				dijkstra(d);
				dijkstra_tunnel(d);
			}
			p->c_type.m->ypos = p->c_type.m->ypos+1;
			p->c_type.m->xpos = p->c_type.m->xpos+1;
		}
	}
	else if(d->pc_tunnel[p->c_type.m->ypos-1][p->c_type.m->xpos+1] < d->pc_tunnel[p->c_type.m->ypos][p->c_type.m->xpos])
    {
		if(d->hardness[p->c_type.m->ypos-1][p->c_type.m->xpos+1] > 0)
        {
			if((tunnell - d->hardness[p->c_type.m->ypos-1][p->c_type.m->xpos+1]) > 0)
            {
				d->hardness[p->c_type.m->ypos-1][p->c_type.m->xpos+1] = 0;
			}else{
				d->hardness[p->c_type.m->ypos-1][p->c_type.m->xpos+1] -= 85;
			}
			dijkstra_tunnel(d);
		}else if(d->hardness[p->c_type.m->ypos-1][p->c_type.m->xpos+1] == 0)
        {
			if(d->map[p->c_type.m->ypos-1][p->c_type.m->xpos+1] == ter_wall)
            {
				d->map[p->c_type.m->ypos-1][p->c_type.m->xpos+1] = ter_floor_hall;
				dijkstra(d);
				dijkstra_tunnel(d);
			}
			p->c_type.m->ypos = p->c_type.m->ypos-1;
			p->c_type.m->xpos = p->c_type.m->xpos+1;
		}
	}
	else if(d->pc_tunnel[p->c_type.m->ypos+1][p->c_type.m->xpos-1] < d->pc_tunnel[p->c_type.m->ypos][p->c_type.m->xpos])
    {
		if(d->hardness[p->c_type.m->ypos+1][p->c_type.m->xpos-1] > 0)
        {
			if((tunnell - d->hardness[p->c_type.m->ypos+1][p->c_type.m->xpos-1]) > 0)
            {
				d->hardness[p->c_type.m->ypos+1][p->c_type.m->xpos-1] = 0;
			}else{
				d->hardness[p->c_type.m->ypos+1][p->c_type.m->xpos-1] -= 85;
			}
			dijkstra_tunnel(d);
		}else if(d->hardness[p->c_type.m->ypos+1][p->c_type.m->xpos-1] == 0)
        {
			if(d->map[p->c_type.m->ypos+1][p->c_type.m->xpos-1] == ter_wall)
            {
				d->map[p->c_type.m->ypos+1][p->c_type.m->xpos-1] = ter_floor_hall;
				dijkstra(d);
				dijkstra_tunnel(d);
			}
			p->c_type.m->ypos = p->c_type.m->ypos+1;
			p->c_type.m->xpos = p->c_type.m->xpos-1;
		}
	}
	else if(d->pc_tunnel[p->c_type.m->ypos-1][p->c_type.m->xpos-1] < d->pc_tunnel[p->c_type.m->ypos][p->c_type.m->xpos])
    {
		if(d->hardness[p->c_type.m->ypos-1][p->c_type.m->xpos-1] > 0)
        {
			if((tunnell - d->hardness[p->c_type.m->ypos-1][p->c_type.m->xpos-1]) > 0)
            {
				d->hardness[p->c_type.m->ypos-1][p->c_type.m->xpos-1] = 0;
			}else{
				d->hardness[p->c_type.m->ypos-1][p->c_type.m->xpos-1] -= 85;
			}
			dijkstra_tunnel(d);
		}else if(d->hardness[p->c_type.m->ypos-1][p->c_type.m->xpos-1] == 0)
        {
			if(d->map[p->c_type.m->ypos-1][p->c_type.m->xpos-1] == ter_wall)
            {
				d->map[p->c_type.m->ypos-1][p->c_type.m->xpos-1] = ter_floor_hall;
				dijkstra(d);
				dijkstra_tunnel(d);
			}
			p->c_type.m->ypos = p->c_type.m->ypos-1;
			p->c_type.m->xpos = p->c_type.m->xpos-1;
		}
	}
}



