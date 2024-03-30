/*
    This program prints out a verbose explanation of a given regular expression.
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
#if MEASURE_TIME == 1
#include <time.h>
#endif
#include "libsmallregex/libsmallregex.h"


int main(int argc, char** argv)
{
  if (argc == 2)
  {
	#if MEASURE_TIME == 1
		struct timespec tstart={0,0}, tend={0,0};
		clock_gettime(CLOCK_MONOTONIC, &tstart);
	#endif
	struct small_regex *sr = regex_compile(argv[1])
	
	#if MEASURE_TIME == 1
		clock_gettime(CLOCK_MONOTONIC, &tend);
		printf("-->took about %.10f seconds\r\n\r\n",
           ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) - 
           ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec));
	#endif

	re_print(sr);
	free((void*) sr);
  }
  else
  {
    printf("\nUsage: %s <PATTERN> \n", argv[0]);
  }   
  return -2; 
}

