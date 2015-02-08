/*
  CPE 471 Lab 1 
  Base code for Rasterizer
  Example code using B. Somers' image code - writes out a sample tga
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Image.h"
#include "types.h"

struct Point {
  color_t color;
  int x;
  int y;
}; 

struct Triangle {
  Point p1;
  Point p2;
  Point p3;
  Point min;
  Point max;
};

float computeArea(Point a, Point b, Point c) {
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

void setPointColor(Point *point, float red, float green, float blue) {
  point->color.r = red;
  point->color.g = green;
  point->color.b = blue;
}

int main(void) {
  Triangle tri;
  Point p1, p2, p3, min, max, tempPoint;
  float alpha, beta, gamma, triangleArea;

  int minX, minY, maxX, maxY;

  tri.p1 = p1;
  tri.p2 = p2;
  tri.p3 = p3;
  tri.min = min;
  tri.max = max;

  // make a color
  color_t clr, triClr;

  clr.r = 0.3;
  clr.g = 0.7;
  clr.b = 0.8;

  // make a 640x480 image (allocates buffer on the heap)
  Image img(640, 480);

  printf("Enter three vertices in (x,y) form: \n");
  printf("1 >> ");
  scanf("%d %d", &p1.x, &p1.y);
  printf("2 >> ");
  scanf("%d %d", &p2.x, &p2.y);
  printf("3 >> ");
  scanf("%d %d", &p3.x, &p3.y);


  // assign the colors to the three points/vertices
  setPointColor(&p1, 1, 0, 0);
  setPointColor(&p2, 0, 0, 1);
  setPointColor(&p3, 0, 0, 0);
  
  min.x = findMin(p1.x, p2.x, p3.x);
  min.y = findMin(p1.y, p2.y, p3.y);

  max.x = findMax(p1.x, p2.x, p3.x);
  max.y = findMax(p1.y, p2.y, p3.y);

  triangleArea = computeArea(p1, p3, p2);
  
  printf("\nArea computed: %f", triangleArea);

  // rasterize triangle
  for (int i = min.x; i < max.x; i++) {   
    for (int j = min.y; j < max.y; j++) { 
      tempPoint.x = i;
      tempPoint.y = j;
      alpha = computeArea(p2, tempPoint, p3) / triangleArea;
      beta = computeArea(p3, tempPoint, p1) / triangleArea;
      gamma = computeArea(p1, tempPoint, p2) / triangleArea;

      if (i == 3 && j == 2) {
        printf("At point p: alpha %f, beta %f, gamma %f\n", alpha, beta, gamma);
      }

      if (alpha >= 0 && alpha <= 1 && beta >= 0 && beta <= 1 && gamma >= 0 && gamma <= 1) {
        triClr.r = p1.color.r * alpha + p2.color.r * beta + p3.color.r * gamma;
        triClr.g = p1.color.g * alpha + p2.color.g * beta + p3.color.g * gamma;
        triClr.b = p1.color.b * alpha + p2.color.b * beta + p3.color.b * gamma;

        img.pixel(i, j, triClr);
      }
      else {
        //img.pixel(i, j, clr);
      }
    }
  }

  // write the targa file to disk
  img.WriteTga((char *)"awesome.tga", true); 
  // true to scale to max color, false to clamp to 1.0

}
