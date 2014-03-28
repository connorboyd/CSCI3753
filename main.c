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
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

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

	void cpu()
	{
		long i;
		double x, y;
		double inCircle = 0.0;
		double inSquare = 0.0;
		double pCircle = 0.0;
		double piCalc = 0.0;

		/* Calculate pi using statistical methode across all iterations*/
		for(i=0; i < DEFAULT_ITERATIONS; i++)
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



	//argv[1] = scheduler; argv[2] = test type; argv[3] = load
	int main(int argc, char* argv[])
	{
		int policy; // Scheduling policy
		struct sched_param param;
		int numProcesses;
		int processType;

		if(argc != 4)
		{
			printf("argc isn't 4!\n");
			exit(EXIT_FAILURE);
		}

		if(!strcmp(argv[1], "SCHED_OTHER"))
		    policy = SCHED_OTHER;
		else if(!strcmp(argv[1], "SCHED_FIFO"))
		    policy = SCHED_FIFO;
		else if(!strcmp(argv[1], "SCHED_RR"))
		    policy = SCHED_RR;

		/* Set process to max prioty for given scheduler */
		param.sched_priority = sched_get_priority_max(policy);
		sched_setscheduler(0, policy, &param);

		// Get process type from argv
		if(!strcmp(argv[2], "CPU"))
		    processType = 1;
		else if(!strcmp(argv[2], "IO"))
		    processType = 2;
		else if(!strcmp(argv[2], "MIXED"))
		    processType = 3;

		// Get number of forks from argv
		if(!strcmp(argv[3], "LOW"))
		    numProcesses = 10;
		else if(!strcmp(argv[3], "MEDIUM"))
		    numProcesses = 70;
		    numProcesses = 300;

		int pid;

		int i = 0;
		while(i < numProcesses)	//fork a whole bunch
		{
			++i;
			pid = fork();
			if(pid == 0)	//break if this is a child process
				break;

		}

		if(pid == 0)	//only child processes
		{
			if(processType == 1)
				cpu();
			else if(processType == 2)
				io();
			else if(processType == 3)
				mixed();
		}

		// Wait for child processes
		while(pid = waitpid(-1, NULL, 0) )
		{
			if(errno == ECHILD)
				break;
		}




		return 0;
	}