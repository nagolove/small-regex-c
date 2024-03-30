/*
 * Testing various regex-patterns
 */

#define _POSIX_C_SOURCE 199309L
#if __STDC_VERSION__ >= 199901L
#define _XOPEN_SOURCE 600
#else
#define _XOPEN_SOURCE 500
#endif /* __STDC_VERSION__ */

#define MEASURE_TIME 1

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#if MEASURE_TIME == 1
#include <time.h>
#include <stdlib.h>
#endif
#include "libsmallregex/libsmallregex.h"


#define OK    ((char*) 1)
#define NOK   ((char*) 0)


char* test_vector[][3] =
{
  { OK,  "\\d",              "5"           },
  { OK,  "\\w+",             "hej"         },
  { OK,  "\\s",              "\t \n"       },
  { NOK, "\\S",              "\t \n"       },
  { OK,  "[\\s]",            "\t \n"       },
  { NOK, "[\\S]",            "\t \n"       },
  { NOK, "\\D",              "5"           },
  { NOK, "\\W+",             "hej"         },
  { OK,  "[0-9]+",           "12345"       },
  { OK,  "\\D",              "hej"         },
  { NOK, "\\d",              "hej"         },
  { OK,  "[^\\w]",           "\\"          },
  { OK,  "[\\W]",            "\\"          },
  { NOK, "[\\w]",            "\\"          },
  { OK,  "[^\\d]",           "d"           },
  { NOK, "[\\d]",            "d"           },
  { NOK, "[^\\D]",           "d"           },
  { OK,  "[\\D]",            "d"           },
  { OK,  "^.*\\\\.*$",       "c:\\\\Tools"   },
  { OK,  "^[\\+-]*[\\d]+$",  "+27"         },
  { OK,  "[abc]",            "1c2"         },
  { NOK, "[abc]",            "1C2"         },
  { OK,  "[1-5]+",           "0123456789"  },
  { OK,  "[.2]",             "1C2"         },
  { OK,  "a*$",              "Xaa"         },
  { OK,  "a*$",              "Xaa"         },
  { OK,  "[a-h]+",           "abcdefghxxx" },
  { NOK, "[a-h]+",           "ABCDEFGH"    },
  { OK,  "[A-H]+",           "ABCDEFGH"    },
  { NOK, "[A-H]+",           "abcdefgh"    },
  { OK,  "[^\\s]+",          "abc def"     },
  { OK,  "[^fc]+",           "abc def"     },
  { OK,  "[^d\\sf]+",        "abc def"     },
  { OK,  "\n",               "abc\ndef"    },
  { OK,  "b.\\s*\n",         "aa\r\nbb\r\ncc\r\n\r\n" },
  { OK,  ".*c",              "abcabc"      },
  { OK,  ".+c",              "abcabc"      },
  { OK,  "[b-z].*",          "ab"          },
  { OK,  "b[k-z]*",          "ab"          },
  { NOK, "[0-9]",            "  - "        },
  { OK,  "[^0-9]",           "  - "        },
  { OK,  "[Hh]ello [Ww]orld\\s*[!]?", "Hello world !" },
  { OK,  "[Hh]ello [Ww]orld\\s*[!]?", "hello world !" },
  { OK,  "[Hh]ello [Ww]orld\\s*[!]?", "Hello World !" },
  { OK,  "[Hh]ello [Ww]orld\\s*[!]?", "Hello world!   " },
  { OK,  "[Hh]ello [Ww]orld\\s*[!]?", "Hello world  !" },
  { OK,  "[Hh]ello [Ww]orld\\s*[!]?", "hello World    !" },
  { OK,  "^/Users/[a-zA-Z0-9_]+$", "/Users/alex"},
  {OK, "^/Library/Address Book Plug-Ins/Skype[a-zA-Z]+.bundle.*$", "/Library/Address Book Plug-Ins/SkypeAAA.bundle.1"},
  {OK, "^/Library/(A|B)$", "/Library/B"},
  {OK, "^(A|B|C)$", "B"},
  {OK, "^(A|B)/(u1|u2)$", "B/u2"},
  //{OK, "^(A|B)/(u1|(u[a-z]+|u4))$", "B/uad"},
  {OK, "^(u[a-z]+|u4)$", "uad"},
  {OK, "^(u|(b|c|(f|n)))$", "f"},
  {OK, "^(u|(b|c|(f|n)))+$", "bcf"},
  {OK, "^.+b.+b.+$", "aabd454abcg"},
  { NOK, "AB[b-z",			"ABb"			},
  { NOK,  "\\d\\d?:\\d\\d?:\\d\\d?",  "0s:00:00"  },
  { NOK,  "\\d\\d?:\\d\\d?:\\d\\d?",  "000:00"    },
  { NOK,  "\\d\\d?:\\d\\d?:\\d\\d?",  "00:0000"   },
  { OK,  "\\d\\d?:\\d\\d?:\\d\\d?",  "100:00:00" },
  { NOK,  "\\d\\d?:\\d\\d?:\\d\\d?",  "00:100:00" },
  { OK,  "\\d\\d?:\\d\\d?:\\d\\d?",  "00:00:100" },
  { OK,  "\\d\\d?:\\d\\d?:\\d\\d?",   "0:0:0"    },
{ OK,  "\\d\\d?:\\d\\d?:\\d\\d?",   "0:00:0"   },
{ OK,  "\\d\\d?:\\d\\d?:\\d\\d?",   "0:0:00"   },
{ OK,  "\\d\\d?:\\d\\d?:\\d\\d?",   "00:0:0"   },
{ OK,  "\\d\\d?:\\d\\d?:\\d\\d?",   "00:00:0"  },
{ OK,  "\\d\\d?:\\d\\d?:\\d\\d?",   "00:0:00"  },
{ OK,  "\\d\\d?:\\d\\d?:\\d\\d?",   "0:00:00"  },
{ OK,  "\\d\\d?:\\d\\d?:\\d\\d?",   "00:00:00" },
/*
  { OK,  "[^\\w][^-1-4]",     ")T"          },
  { OK,  "[^\\w][^-1-4]",     ")^"          },
  { OK,  "[^\\w][^-1-4]",     "*)"          },
  { OK,  "[^\\w][^-1-4]",     "!."          },
  { OK,  "[^\\w][^-1-4]",     " x"          },
  { OK,  "[^\\w][^-1-4]",     "$b"          },
*/
};

int 
main()
{
    char* text;
    char* pattern;
    int32_t should_fail;
    size_t ntests = sizeof(test_vector) / sizeof(*test_vector);
    size_t nfailed = 0;
    size_t i;
	
    for (i = 0; i < ntests; ++i)
    {
        pattern = test_vector[i][1];
        text = test_vector[i][2];
        should_fail = (test_vector[i][0] == NOK);
		
#if MEASURE_TIME == 1
		struct timespec start, stop;
		struct timespec startp, stopp;
		clock_gettime(CLOCK_MONOTONIC, &start);
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &startp);
#endif
		
		int32_t m = regex_match(pattern, text);
		
#if MEASURE_TIME == 1
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &stopp);
		clock_gettime(CLOCK_MONOTONIC, &stop);

		double result = ((double)stop.tv_sec + 1.0e-9*stop.tv_nsec) - 
           			((double)start.tv_sec + 1.0e-9*start.tv_nsec);
		double resultp = ((double)stopp.tv_sec + 1.0e-9*stopp.tv_nsec) - 
           			((double)startp.tv_sec + 1.0e-9*startp.tv_nsec);
 
        printf("test %lu took %.10lf S / %.10lf S MONOTONIC/PROCESS pattern: %s\r\n", i, result, resultp, pattern);
#endif
        if (should_fail)
        {
            if (m != (-1))
            {
                printf("\n");
                regex_trace(regex_compile(pattern));
                fprintf(stderr, "[%lu/%lu]: pattern '%s' matched '%s' unexpectedly. \n", (i+1), ntests, pattern, text);
                nfailed += 1;
            }
        }
        else
        {
            if (m == (-1))
            {
                printf("\n");
                regex_trace(regex_compile(pattern));
                fprintf(stderr, "[%lu/%lu]: pattern '%s' didn't match '%s' as expected. \n", (i+1), ntests, pattern, text);
                nfailed += 1;
            }
        }
    }

    // printf("\n");
    printf("%lu/%lu tests succeeded.\n", ntests - nfailed, ntests);
    printf("\n");
    printf("\n");
    printf("\n");

    return 0;
}

