#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define XMAP 21
#define YMAP 80
#define XROOMMIN 3
#define YROOMMIN 3

typedef struct room 
{
    int x; //xstarting point
    int y; //ystarting point
    int ylength; //length of room in Y distance
    int xlength; //length of room in X distance
}room;

void printDungeon(char dungeonMap [XMAP][YMAP]);
void makeRoom();
void makeCorridors(room one, room two);

//The map.
char dungeonMap [XMAP][YMAP];

struct room ROOMS[10];

void GenerateBorder()
{
    for(int i = 0; i < YMAP; i++)
    {
        dungeonMap[0][i] = '-';
        dungeonMap[20][i] = '-';
        if (i > 0 && i < 20)
        {
            dungeonMap[i][0] = '|';
            dungeonMap[i][79] = '|';
        }
    }
}

void GenerateDungeon(int no_of_rooms)
{
    for(int i = 0; i < XMAP; i++){
        for(int j = 0; j < YMAP; j++)
        {
            if(dungeonMap[i][j] != '|' && dungeonMap[i][j] != '-')
            {
                dungeonMap[i][j] = ' ';
            }
        }
    }
    makeRoom(no_of_rooms,0);
}

int generateRandom(int min, int max)
{   
    int i; //seed; 
    //seed = time(NULL);
    int diff = max - min + 1;
    i = rand() % diff;
    if(i < 0)
    {
        i =  -1;
    }
    return min + i;
}

bool ValidRoom(int current_room)
{
    if( ROOMS[current_room].xlength > 20 || ROOMS[current_room].ylength  > 79)
    {
        return false;
    }

    for (int x = ROOMS[current_room].x; x < ROOMS[current_room].xlength; x++){
		for (int y = ROOMS[current_room].y; y < ROOMS[current_room].ylength; y++){
			if ( dungeonMap[x][y] == '.' || dungeonMap[x+1][y] == '.' || dungeonMap[x][y+1] == '.' 
            || dungeonMap[x-1][y] == '.' || dungeonMap[x][y-1] == '.' )
                {
                    return false;
                }
		}
	}
    return true;
}


void makeRoom(int no_of_rooms, int current_room)
{
    
    int xroomlength, yroomlength, xstart, ystart, i , j;
    int attempts = 0;

    while (attempts < 2000 && current_room < no_of_rooms )
    {   
        ystart = generateRandom(1, YMAP);
        xstart = generateRandom(1, XMAP);
       
        xroomlength = generateRandom(xstart, xstart+7);
        yroomlength = generateRandom(ystart, ystart+7);
        
        if (xroomlength  - xstart < XROOMMIN)
        {
            xroomlength += XROOMMIN;
        }
        
        if (yroomlength - ystart < YROOMMIN)
        {
            yroomlength += YROOMMIN;
        }

        ROOMS[current_room].x = xstart;
        ROOMS[current_room].y = ystart;
        ROOMS[current_room].xlength = xroomlength;
        ROOMS[current_room].ylength = yroomlength;

        if(ValidRoom(current_room) == true)
        {
            for(i = ROOMS[current_room].x; i < ROOMS[current_room].xlength; i++){
                for(j =  ROOMS[current_room].y; j < ROOMS[current_room].ylength; j++){
                    dungeonMap[i][j] = '.' ;
                }
            }
            current_room++; 
            attempts = 0;
            continue;
        }
        attempts++;
    }

    for(int i = 0; i < no_of_rooms - 1; i++)
    {
         makeCorridors(ROOMS[i], ROOMS[i+1]);
    }
    GenerateBorder();
    printDungeon(dungeonMap);
}

void printDungeon(char dungeonMap [XMAP][YMAP])
{
    for(int i = 0; i < XMAP; i++){
        for(int j = 0; j < YMAP; j++)
        {
            printf("%c", dungeonMap[i][j]);
        }
        printf("\n");
    }
}

void makeCorridors(room one, room two)
{
	// printf("%d", one.x);
    // printf("\n");
	// printf("%d", two.x);
    // printf("\n");
    if(one.x > two.x)
    {
        for(int i = one.x; i > two.x; i--)
        {
            if(dungeonMap[i][one.y] == ' ')
            {
                dungeonMap[i][one.y] = '#' ;
            }
        }
    }else{
        for(int i = one.x; i < two.x; i++)
        {
            if(dungeonMap[i][one.y] == ' ')
            {
                dungeonMap[i][one.y] = '#' ;
            }
        }
    }

    if(one.y > two.y)
    {
        for(int j = one.y; j > two.y; j--)
        {
            if(dungeonMap[two.x][j] == ' ')
            {
                dungeonMap[two.x][j] = '#';
            }
        }
    }else{
        for(int j = one.y; j < two.y; j++)
        {
            if(dungeonMap[two.x][j] == ' ')
            {
                dungeonMap[two.x][j] = '#';
            }
        }
    }
}


int main(int argc, char const *argv[])
{
    /* code */
    srand(time(NULL));
    int noOfRooms = generateRandom(5,10);
    GenerateDungeon(noOfRooms);
    return 0;
}