/*
    This program tries to match a given regular expression with text given as input to stdin.
    If the text is a match for the pattern, the program returns 0.
    If the text doesn't match the pattern, the program returns -2.
    
    This program is used in random testing to test a lot of random text and regex together.
    See ./scripts/regex_test.py and the Makefile for this project for the gritty details.
*/

#define _POSIX_C_SOURCE 199309L
#if __STDC_VERSION__ >= 199901L
#define _XOPEN_SOURCE 600
#else
#define _XOPEN_SOURCE 500
#endif /* __STDC_VERSION__ */

#define MEASURE_TIME 1

#include <stdio.h>
#include <stdint.h>
#include "libsmallregex/libsmallregex.h"

#if MEASURE_TIME == 1
#include <time.h>
#endif

int main(int argc, char** argv)
{
	
  if (argc == 3)
  {
#if MEASURE_TIME == 1
		struct timespec start, stop;
        struct timespec startp, stopp;
        clock_gettime(CLOCK_MONOTONIC, &start);
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &startp);
#endif

    int32_t m = regex_match(argv[1], argv[2]);
#if MEASURE_TIME == 1
clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &stopp);
        clock_gettime(CLOCK_MONOTONIC, &stop);

        double result = ((double)stop.tv_sec + 1.0e-9*stop.tv_nsec) - 
           			((double)start.tv_sec + 1.0e-9*start.tv_nsec);
	double resultp = ((double)stopp.tv_sec + 1.0e-9*stopp.tv_nsec) - 
   			((double)startp.tv_sec + 1.0e-9*startp.tv_nsec);
 
        printf("----> took %.10lf S / %.10lf S MONOTONIC/PROCESS\r\n", result, resultp);
#endif
    if (m != -1)
    {
	
      return 0;
    }
	printf("failed regex: %s data: %s\r\n", argv[1], argv[2]); 
  }
  else
  {
    printf("\nUsage: %s <PATTERN> <TEXT> \n", argv[0]);
  }   
  return -2; 
}

