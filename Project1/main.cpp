/**
 * Program 1
 * Kim Arre
 * Section 1
 */

#include <iostream>
#include <stdio.h>
#include <cassert>
#include "tiny_obj_loader.h"
#include "Image.h"
#include <algorithm>
#include <string>

float *depthBuffer = NULL;
int userChoice;

struct Pixel {
   color_t color;
   int x;
   int y;
}; 

struct Triangle {
   Pixel p1;
   Pixel p2;
   Pixel p3;
   float depth;
};

void resize_obj(std::vector<tinyobj::shape_t> &shapes);

float computeArea(Pixel a, Pixel b, Pixel c) {
   return ((b.x - a.x) * (c.y - a.y) - ((c.x - a.x) * (b.y - a.y))) / 2;
}

float findMin(float first, float second, float third) {
   if (first <= second && first <= third) {
      return first;
   } 
   else if (second <= first && second <= third) {
      return second;
   }
   else {
      return third;
   }
}

float findMax(float first, float second, float third) {
   if (first >= second && first >= third) {
      return first;
   } 
   else if (second >= first && second >= third) {
      return second;
   }
   else {
      return third;
   }
}

Pixel w2pLargerWidth(int w, int h, float Xw, float Yw) {
   Pixel p;
   float x, y;

   x = (w-1.0)*(h/(2.0*w)) * Xw + (w-1.0)/2.0;
   y = ((h-1.0)/2.0) * Yw + (h-1.0)/2.0;

   p.x = (int)x;
   p.y = (int)y;

   return p;
}

void setPointColor(Pixel *point, float red, float green, float blue) {
  point->color.r = red;
  point->color.g = green;
  point->color.b = blue;
}

/* Takes the entire list of vertices, as well as three indexes that make up a 
   triangle. */
void rasterize(const std::vector<float>& vertices, size_t v1, size_t v2, size_t v3, 
 Image& img) {
   Triangle tri;
   Pixel p1, p2, p3, min, max, tempPoint;
   float alpha, beta, gamma, triangleArea;

   float v1depth, v2depth, v3depth, xVal, yVal;
   
   // convert from 3d vertex to 2d pixels here - must convert from index to value
   p1 = w2pLargerWidth(img.width(), img.height(), vertices[v1*3+0], vertices[v1*3+1]);
   p2 = w2pLargerWidth(img.width(), img.height(), vertices[v2*3+0], vertices[v2*3+1]);
   p3 = w2pLargerWidth(img.width(), img.height(), vertices[v3*3+0], vertices[v3*3+1]);

   tri.p1 = p1;
   tri.p2 = p2;
   tri.p3 = p3;

   // make a color
   color_t yellow, blue, triClr;

   yellow.r = 1;
   yellow.g = .6;
   yellow.b = 0;

   blue.r = 0;
   blue.g = 0;
   blue.b = 1;

   // assign the colors to the three points/vertices
   setPointColor(&p1, 1, 0, 0);
   setPointColor(&p2, 0, 0, 1);
   setPointColor(&p3, 0, 0, 0);

   min.x = findMin(p1.x, p2.x, p3.x);
   min.y = findMin(p1.y, p2.y, p3.y);

   max.x = findMax(p1.x, p2.x, p3.x);
   max.y = findMax(p1.y, p2.y, p3.y);

   triangleArea = computeArea(p1, p3, p2);

   

   // rasterize triangle
   for (int i = min.x; i < max.x; i++) {   
      for (int j = min.y; j < max.y; j++) { 
         tempPoint.x = i;
         tempPoint.y = j;
         alpha = computeArea(p2, tempPoint, p3) / triangleArea;
         beta = computeArea(p3, tempPoint, p1) / triangleArea;
         gamma = computeArea(p1, tempPoint, p2) / triangleArea;

         if (alpha >= 0 && alpha <= 1 && beta >= 0 && beta <= 1 && gamma >= 0 && gamma <= 1) {
            
            if (userChoice == 1) {   // choice for depth
               v1depth = -1 * (vertices[v1*3+2] / 2 + .5);
               v2depth = -1 * (vertices[v2*3+2] / 2 + .5);
               v3depth = -1 * (vertices[v3*3+2] / 2 + .5);

               tri.depth = v1depth * alpha + v2depth * beta + v3depth * gamma;

               if (tri.depth < depthBuffer[img.width() * j + i]) {
                  depthBuffer[img.width() * j + i] = tri.depth;

                  triClr.r = 0;
                  triClr.g = 1 - (1 * tri.depth + 1);
                  triClr.b = 1 - (1 * tri.depth + 1);;
                  
                  img.pixel(i, j, triClr);
               }
            }
            else if (userChoice == 2) {   // choice for color ramp
               xVal = vertices[v1*3+0] * alpha + vertices[v2*3+0] * beta + vertices[v3*3+0] * gamma;
               
               triClr.r = (1 - xVal/2 + .5) * yellow.r * (xVal/2 + .5) * blue.r; 
               triClr.g = (1 - xVal/2 + .5) * yellow.g + (xVal/2 + .5) * blue.g;
               triClr.b = (1 - xVal/2 + .5) * yellow.b + (xVal/2 + .5) * blue.b;

               img.pixel(i, j, triClr);
            }

            else {
               xVal = vertices[v1*3+0] + vertices[v2*3+0] + vertices[v3*3+0] / 3;
               
               /*triClr.r = p1.x * alpha
               triClr.g = 
               triClr.b = */

               triClr.r = (1 - xVal) * yellow.r + xVal * blue.r; 
               triClr.g = (1 - xVal) * yellow.g + xVal * blue.g;
               triClr.b = (1 - xVal) * yellow.b + xVal * blue.b;

               img.pixel(i, j, triClr);
            }
         }
      }
   }
}


int main( int argc, const char* argv[] )
{
   int width, height;

   if (argc != 3) {
      printf("Inappropriate number of arguments. Enter only one .obj file and image option 1 or 2.\n");
      exit(0);
   }
   std::string inputfile = argv[1];

   userChoice = std::stoi(argv[2]);  

   std::vector<tinyobj::shape_t> shapes;
   std::vector<tinyobj::material_t> materials;

   std::string err = tinyobj::LoadObj(shapes, materials, inputfile.c_str());

   resize_obj(shapes);

   if (!err.empty()) {
      std::cerr << err << std::endl;
      exit(1);
   }

   printf("Enter the window width: ");
   scanf("%d", &width);
   printf("Enter the window height: ");
   scanf("%d", &height);

   // make and fill z-buffer with infinity
   depthBuffer = (float *)(malloc(sizeof(float) * width * height));

   for (int i = 0; i < width * height; i++) {
      depthBuffer[i] = std::numeric_limits<float>::infinity();
   }

   // make a color
   color_t clr;
   clr.r = 0.5;
   clr.g = 0.5;
   clr.b = 0.9;

   // make a 640x480 image (allocates buffer on the heap)
   Image img(width, height);

   for (size_t i = 0; i < shapes.size(); i++) {
      for (size_t f = 0; f < shapes[i].mesh.indices.size() / 3; f++) {
         // passes the entire list of vertices, and the indexes of the three points that make a triangle
         rasterize(shapes[i].mesh.positions, shapes[i].mesh.indices[3*f+0], 
            shapes[i].mesh.indices[3*f+1], shapes[i].mesh.indices[3*f+2], img);
      }
   }

   // write the targa file to disk
   // true to scale to max color, false to clamp to 1.0
   img.WriteTga((char *)"awesome.tga", true); 
}