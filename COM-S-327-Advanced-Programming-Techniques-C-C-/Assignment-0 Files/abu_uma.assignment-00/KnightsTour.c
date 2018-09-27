#include <stdio.h>
#include <stdbool.h>
#define S 5

void PrintKnightsTour(int validXmoves[8], int validYmoves[8], int x, int y, int trackarr[5][5], int trackNo);

void ResetBoard(int trackarr[5][5])
{
  int i,j;
  for(i = 0; i < 5; i++)
    for(j = 0; j < 5; j++)
      trackarr[i][j] = -1;
}


void knights_tour()
{
  int validXmoves[8] = {1,-2,-2,-1,1,2,2,-1};
  int validYmoves[8] = {2,1,-1,-2,-2,-1,1,2};

  int trackArr [5][5];
  ResetBoard(trackArr); 



  for(int m = 0; m < 5; m++){
    for(int n = 0; n < 5; n++){
        PrintKnightsTour(validXmoves, validYmoves, m,n, trackArr,1);
        ResetBoard(trackArr); 
    }
  }

}

bool validEndPosition(int x, int y,int trackArr[S][S])
{
  if(y >= 0 && y < 5 && x >= 0 && x < 5 && trackArr[x][y] == -1){
    return true;
  }
  return false;
}

void PrintKnightsTour(int validXmoves[8], int validYmoves[8], int x, int y, int trackArr[S][S], int trackNo)
{
  int newX, newY, i;
  
  trackArr[x][y] = trackNo;

  if (trackNo >= 25){
    for(int l = 0; l < S; l++)
		   for(int m = 0; m < S; m++)
		     printf("%2d ",trackArr[l][m]);
    printf("\n");
          trackArr[x][y] = -1;
    return;
  }

  for(i = 0; i < 8; i++)
  {
     newX = x + validXmoves[i];
     newY = y + validYmoves[i];

    if(validEndPosition(newX, newY, trackArr) == true){
      PrintKnightsTour(validXmoves, validYmoves,  newX, newY, trackArr, trackNo+1);
    }
  }
  trackArr[x][y] = -1;
}

int main(int argc, char *argv[])
{
  knights_tour();
  return 0;
}
