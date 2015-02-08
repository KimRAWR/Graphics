
Program 1
Kim Arre
Section 1


How to Run:
   Compile using the Makefile included.

   After running the make file included, the commandline arguments are as follows:
   a.out <objFile> <coloringAlgorithm>
   
   The coloring algorithm can be either 1 for depth, or 2 for the color ramp.
   Any other value won't show anything.

   ex: a.out bunny.obj 1

   Once the program is running, it will load the mesh file and prompt you to
   enter the window width and height. 


Design Choices:
   Conceptually my z-buffer is a 2D array, however I chose to make it a global 
   pointer using an equation (offset = width * j + i) to index into it for the
   depth values.


Functionality:
   As far as I know and have tested, I believe it should be fully functional.

