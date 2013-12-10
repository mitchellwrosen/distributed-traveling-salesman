#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

struct Point{
   int index;
   double distance;
   double x;
   double y;
   struct Point *next;
};

typedef struct Point POINT;

float calcDistance (POINT * p1, POINT * p2);
POINT * readData(FILE * fIndex);
void displayResults(POINT * path);
POINT * greedy (POINT * points);
POINT * findClosest (POINT * origin, POINT * others);

int main(int argc, char** argv) {
   FILE * pFile;
   POINT *points;
   char *fileName;

   /* Read in values */
   fileName = argv[1];
   pFile = fopen(fileName, "r");
   points = readData(pFile);
   displayResults(greedy(points));

}

void displayResults(POINT * path) {
   printf("Order of vertices for path: ");
   double cost = 0.0;
   while(path) {
      printf("%d ", path->index);
      cost += path->distance;
      path = path->next;
   }
   printf("\nTotal Cost: %lf\n", cost);
}

float calcDistance (POINT * p1, POINT * p2) {
   return sqrt(pow((p2->x - p1->x),2) + pow((p2->y - p1->y),2));
}

POINT * findClosest (POINT * origin, POINT * others) {
   float shortestDist;
   float curDist;
   POINT * closest;
   int curIndex = 1;

   shortestDist = calcDistance(origin, others);
   closest = others;
   others = others->next;

   while(others) {
      curDist = calcDistance(origin, others);
      if(curDist < shortestDist || !shortestDist) {
         shortestDist = curDist;
         closest = others;
      }
      others = others->next;
   }
   closest->distance = shortestDist;
   return closest;
}

POINT * removePoint(POINT * points, POINT * toRemove) {
   if(points == NULL) {
      return NULL;
   }

   if(points == toRemove) {
      return points->next;
   }

   points->next = removePoint(points->next, toRemove);
   return points;
}

POINT * greedy (POINT * points) {
   POINT * path = NULL;
   POINT * pathHd = NULL;
   POINT * closestPoint;

   /* Start at first point */
   path = points;
   points = points->next;
   pathHd = path;

   while(points) {
      closestPoint = findClosest(path, points);
      path->next = closestPoint;
      path = path->next;
      points = removePoint(points, closestPoint);
      path->next = NULL;
   }

   return pathHd;
}



POINT * readData(FILE * fIndex) {
   POINT * points = NULL;
   int pointIndex = 0;
   double pointX = 0.0;
   double pointY = 0.0;

   while(fscanf(fIndex, "%d %lf %lf", &pointIndex, &pointX, &pointY) != EOF) {
      POINT * aPoint = malloc(sizeof(POINT));
      aPoint->x = pointX;
      aPoint->y = pointY;
      aPoint->distance = 0;
      aPoint->index = pointIndex;

      if(!points) {
         aPoint->next = NULL;
         points = aPoint;
      } else {
         aPoint->next = points;
         points = aPoint;
      }
   }
   return points;
}