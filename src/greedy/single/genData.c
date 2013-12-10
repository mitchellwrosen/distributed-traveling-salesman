#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

#define MAX 10000.0
#define MIN 0
#define LINE_LENGTH 255

void genData(int numPoints, FILE * pFile);

int main(int argc, char** argv) {
   FILE * pFile;
   char *fileName;
   long numPoints;

   /* Read in values */
   fileName = argv[1];
   numPoints = strtol(argv[2], NULL, 10);
   pFile = fopen(fileName, "w");
   genData(numPoints, pFile);

}

//http://stackoverflow.com/questions/5289613/c-generate-random-float-between-two-floats
float randomFloat(float min, float max)
{
    double random = ((double) rand()) / (double) RAND_MAX;

    // generate (in your case) a float between 0 and (4.5-.78)
    // then add .78, giving you a float between .78 and 4.5
    double range = max - min;  
    return (random*range) + min;
}

void genData(int numPoints, FILE * pFile) {
	int pointIndex = 0;
	char * dataLine = calloc(sizeof(char), LINE_LENGTH);
	srand(time(NULL));
	for(; pointIndex < numPoints; pointIndex++) {
		sprintf(dataLine, "%d %.12lf %.12lf\n", pointIndex, randomFloat(MIN, MAX), randomFloat(MIN, MAX));
		fputs(dataLine, pFile);
	}
}