I used the professors codes to start this homework. I edited a few methods and data structures to add stairs to the dungeon. I was 
having issues with the player / monsters getting on the stairs but I asked on piazza and I got help. Most of the code for this assignment is in
pc.c under player movements where I added use cases with getch to move the players accordingly. I also made alot of changes to rgl.c adding
cases for if the user quit the game and if the user wanted to see the monsterlist. 

If the user quit the game, then the game mode is 'Q' and the game ends. else if the press m, then a new window opens and calls a 
method print monsters that uses an array of strcts that I made and store all the monsters in the dungeon and print starting from 
a specific window index. To find the position of the monsters, I subtracted the player position in the x and y coordinate 
 from the monster position in the x and y coordinate and based on if the results were positive or negative i would print North or 
 South or East or West and the absolute value of the distance. I got srolling to work where if the user presses the up or down arrow key, 
 the number of monsters shown on the page is moved one up or one down and the list is printed again.

 **Note, the escape works if you are on the monster page. It may take like 1 - 2 seconds but it works. 