For the project, we were asked to write a C program to find all directed, open Knights Tour on a 5X5 chess board. We were also to assumer all
 spaces are numbered as what is givin in the description. We were told in class to use Recursion and Back tracking which is what I used. 
 I used a 5 by 5 array of -1s to store the positions of where the knight has been. I trued using booleans but realized that it would not work because 
 integers cannot replace booleans. I also had 2 arrays to designate the valid combinations for moves in the x and y axis. I used a recursive function 
 to iterate through and place move the knight and if it is a valid stop, it should replace the position where it stopped with a number. 
