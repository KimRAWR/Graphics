Kim Arre
Project 3
CPE 471

 Status
--------
Currently, I believe everything works except the physical representation of the
light. I have the code for it still included in my main, and it kiiind of works 
except that if the g_light y or z values are too low (i.e. 1 or less - so 
basically to any size that you can see it in the frame), the shaders start 
getting really confused.


Development Environment Changes
-------------------------------
The main things I changed to get this working on my Mac was to modify 
glIncludes.h to include glew instead of glad. I think I also removed this header
from the main.cpp and included glIncludes.h before anything.
