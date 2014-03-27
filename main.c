/*
 * File: main.c
 * Author: Connor Boyd
 * Project: CSCI 3753 Programming Assignment 3
 * Description: One file to rule them all
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <sched.h>

#define DEFAULT_ITERATIONS 1000000
#define RADIUS (RAND_MAX / 2)

#define MAXFILENAMELENGTH 80
#define DEFAULT_INPUTFILENAME "rwinput"
#define DEFAULT_OUTPUTFILENAMEBASE "rwoutput"
#define DEFAULT_BLOCKSIZE 1024
#define DEFAULT_TRANSFERSIZE 1024*100


	inline double dist(double x0, double y0, double x1, double y1)
	{
		return sqrt(pow((x1-x0),2) + pow((y1-y0),2));
	}

	inline double zeroDist(double x, double y)
	{
		return dist(0, 0, x, y);
	}

	void cpu(long iterations)
	{
		long i;
		double x, y;
		double inCircle = 0.0;
		double inSquare = 0.0;
		double pCircle = 0.0;
		double piCalc = 0.0;

		/* Calculate pi using statistical methode across all iterations*/
		for(i=0; i<iterations; i++)
		{
			x = (random() % (RADIUS * 2)) - RADIUS;
			y = (random() % (RADIUS * 2)) - RADIUS;
			if(zeroDist(x,y) < RADIUS)
			{
			    inCircle++;
			}
			inSquare++;
		}

		/* Finish calculation */
		pCircle = inCircle/inSquare;
		piCalc = pCircle * 4.0;
	}

	void io()
	{
		int iterations = 100;
		FILE *inFile;
		FILE *outFile;
		char* transferBuffer = NULL;
		int bufferSize = 16384;

		transferBuffer = malloc(bufferSize);

		inFile = fopen("/dev/urandom", "r"); // Open urandom, read only
		if(inFile == NULL)
		{
			printf("Error opening /dev/urandom!\n");
			exit(EXIT_FAILURE);
		}

		outFile = fopen("/dev/null", "w"); // Open urandom, read only
		if(outFile == NULL)
		{
			printf("Error opening /dev/null!\n");
			exit(EXIT_FAILURE);
		}
		int i = 0;
		while(i < iterations)
		{
			fread(transferBuffer, 1, bufferSize, inFile);
			fwrite(transferBuffer, 1, bufferSize, outFile);
			++i;
		}

		free(transferBuffer);
	}

	void mixed()
	{
		int iterations = 100;
		FILE *inFile;
		FILE *outFile;
		char* transferBuffer = NULL;
		int bufferSize = 16384;

		transferBuffer = malloc(bufferSize);

		inFile = fopen("/dev/urandom", "r"); // Open urandom, read only
		if(inFile == NULL)
		{
			printf("Error opening /dev/urandom!\n");
			exit(EXIT_FAILURE);
		}

		outFile = fopen("/dev/null", "w"); // Open urandom, read only
		if(outFile == NULL)
		{
			printf("Error opening /dev/null!\n");
			exit(EXIT_FAILURE);
		}

		int i = 0;
		int j = 0;
		while(i < iterations)
		{
			double x, y;
			double inCircle = 0.0;
			double inSquare = 0.0;
			double pCircle = 0.0;
			double piCalc = 0.0;

			j = 0;
			fread(transferBuffer, 1, bufferSize, inFile);
			while(j<DEFAULT_ITERATIONS / 40) //about same time for user/sys
			{
				x = (random() % (RADIUS * 2)) - RADIUS;
				y = (random() % (RADIUS * 2)) - RADIUS;
				if(zeroDist(x,y) < RADIUS)
				{
				    inCircle++;
				}
				inSquare++;
				++j;
			}			
			fwrite(transferBuffer, 1, bufferSize, outFile);
			
			pCircle = inCircle/inSquare;
			piCalc = pCircle * 4.0;

			++i;
		}

		free(transferBuffer);
	}




	int main(int argc, char* argv[])
	{
		mixed();
		return 0;
	}