To start this project, I started by looking at the example the instructor gave. Also using what he said in class. The first thing I did
was to make the random number generator because I realized that each time you compile and run the program, it should create a new random 
map and to do that you need a random number generator. I create the function that takes in a min and a max number. After that I created 
the 2D character array and filled it up with rocks(Spaces). 
The next thing I did was to generate a random room by using the random number generator to generate x and y coordinates between the size of
of the map. After generating this, I realized that I would need to store the rooms in other to make connect them together so I created a struct
that stores x and y starting points for the room and also the x and y distance. I also made an array to store all the rooms. In addition, I made 
helper functions to help check if a room is valid or not (ie if it doesnt cross the bounds and also if it does not overlap with another room).
To make the cooridors, I used the advice the professor gave in class, locate the x starting point of the first room and the x starting point of the 
second room and connect the y coordinates of the rooms and do the same thing for the y starting points and connect the x points for the
rooms if they are not a room floor. Finally I tested the script several times to make sure it works. I also added restrictions and made helper 
functions to clean up the code
