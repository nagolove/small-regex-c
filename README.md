# small-regex-c
[Original source](https://repo.4neko.org/4NEKO/small-regex-c.git) by Aleksandr M.
# A reimplementation of the small and portable library written in C.  
This implementation avoids using recursion during matching/compiling process in order to fit in small stack machins. Also it is using different method of the the structuring compiled data.

### Description
About [Regular Expressions](https://en.wikipedia.org/wiki/Regular_expression) (regex). 

---

This code can also be used in kernel space (i.e it is ussing heap instead of the static variables or storages allocated on the stack).

When using in kernel space of the OS, there is no warranty that the code does not contain any errors which can cause kernel panic or there is no proof that the code is not vulnerable. At the moment, according to the tests, it seems stable.

When compiling small-regex against kernel, there is no need to edit any preprocessor values. The code for kernel mode will be attached automatically by _KERNEL proprocessor definition. The same thing with userspace.

[About this library at NiXD](link todo)

### Current status
- All regex operations stated below are supported.
- Code is ready to be introduced into your project (including FreeBSD kernel or any other BSD kernel or kernel module)

**Patches are welcomed.**

### Design goals
To introduce regex evaluation to the kernel space for the project. And also use it on microcontrollers.
One thing is that, the code was optimised to work on Intel/AMD x86 x64 processors. However, it is required to adjust the variable sizes accrding to the bit capacity of the architecture. Also, even AVR8 has heap memory allocator. The library is working with data stored in heap. You can modify it to work with data on stack and read pre-compiled regex from EEPROM. All you need to do is to remove all malloc/calloc/free and create a static variable for states table and make sure that the program will not went out of allocated memory bounds.

### Design
The initial design was taken from (https://github.com/kokke/tiny-regex-c) but at the moment there are only 15% of original code was left.

The main idea is not to use a recursion when matching and compiling. Instead, use the linear matching with the path selection depending on results of the the regex node evaluation.

The structure of the regex node is:
- type - the node type i.e BEGIN END BRANCH
- trueoffset - the offset to the next node when the evaluation of the node resulted true
- falseoffset - the offset to the next node when the evaluation of the node resulted false
- data - i.e character or offset to the string of the class

Below is the example of matching:
```
regex string: ^ABC.$
compiled regex:
0000 BEGIN 0001 0005
0001 CHAR  0002 0005 ;A
0002 CHAR  0003 0005 ;B
0003 CHAR  0004 0005 ;C
0004 DOT   0005 0005 ;.
0005 END   0000 0000 ; EXIT
0006 UNUSED 0000 0000 ; INDICATES THE END OF THE REGEX BINARY

trace:
truepath: when matching against "ABCD"
    BEGIN--TURE-->CHAR[A]--TRUE-->CHAR[B]--TRUE-->CHAR[C]--TRUE-->DOT--TRUE-->END[TRUE]
                    A==A            B==B            C==C        D==.
false pathes: when matching "ACBD"
    BEGIN--TRUE-->CHAR[A]--TRUE-->CHAR[B]--FALSE-goto 0005 false-->END[FALSE] 
                    A==A            B!=C
```

Thorough testing : [exrex](https://github.com/asciimoo/exrex) is used to randomly generate test-cases from regex patterns

When running in kernel space of FreeBSD the malloced zone is defined in the MALLOC_DECLARE section which is in .h file:
```C
MALLOC_DECLARE(MC_RE);
```

The default values are defined in .h file and can be override:
```C
#ifndef DEFAULT_TEMP_LEN
    //the initial buffer allocated for the initial offset calculations (in compiler)
    #define DEFAULT_TEMP_LEN 40UL
#endif

#ifndef MAX_TEMP_REALLOCS
    //maximum stack depth i.e 10 will be 10*DEFAULT_TEMP_LEN(40)*sizeof(struct re_temp i.e 8) = 3.2kB
    #define MAX_TEMP_REALLOCS 10UL
#endif

#ifndef DEFAULT_STATES_DEP
    //the initial stack length
    #define DEFAULT_STATES_DEP 40UL
#endif

#ifndef MAX_STATE_REALLOCS
    //maximum stack depth i.e 10 will be 10*DEFAULT_STACK_DEP(40)*sizeof(struct stack i.e 16) = 6.4kB
    #define MAX_STATE_REALLOCS 10UL
#endif
```


### API

```

### Supported regex-operators
The following features / regex-operators are supported by this library.


 *   '.'        Dot, matches any character
 *   '^'        Start anchor, matches beginning of string
 *   '$'        End anchor, matches end of string
 *   '*'        Asterisk, match zero or more (greedy)
 *   '+'        Plus, match one or more (greedy)
 *   '?'        Question, match zero or one (non-greedy)
 *   '[abc]'    Character class, match if one of {'a', 'b', 'c'}
 *   '[^abc]'   Inverted class, match if NOT one of {'a', 'b', 'c'} -- NOTE: feature is currently broken!
 *   '[a-zA-Z]' Character ranges, the character set of the ranges { a-z | A-Z }
 *   '\s'       Whitespace, \t \f \r \n \v and spaces
 *   '\S'       Non-whitespace
 *   '\w'       Alphanumeric, [a-zA-Z0-9_]
 *   '\W'       Non-alphanumeric
 *   '\d'       Digits, [0-9]
 *   '\D'       Non-digits
 *   '('        Group start
 *   ')'        Group ends
 *   '|'        Branch
```

### Usage
Compile a regex from ASCII-string (char-array) to a custom pattern structure using `regex_compile()`.

Search a text-string for a regex and get an index into the string, using `regex_match()` or `regex_matchp()`.

The returned index points to the first place in the string, where the regex pattern matches.

If the regular expression doesn't match, the matching function returns an index of -1 to indicate failure.

### Examples
Example of usage:
```C
//compile regex
struct small_regex * re = regex_compile("^/Users/[a-zA-Z0-9_]+/Library/Preferences$");

//printing out to the stdout
regex_print(re);

//mathing, if returned 0 then matched, -1 not matched
int32_t res = regex_matchp(re, "/Users/abc/Library/Preferences");
```

For the test, please see directory tests

### TODO
Common:
- Test implementation of the inverted class
- Further tests
- Add validation of the offset values in order to make sure that binary regex offsets to the correct place

Security:
- Perform a code review and security tests

### FAQ
- *Q: How different this library from the rest of the C regex implementations?*

  A: It is not using stack (recursion) during matching. It is using a true and false paths depending on the result of the matching operation.

- *Q: Is it a kernel module and will it run with Linux Kernel Codebase?*

  A: No this is not a kernel module, but a code which is prepared to work in the kernel space. It will not compile with Linux kernel because included header files are different with Linux Kernel.

### License
2 clause BSD License

### Performance
Test was performed on Linux 4.13.0-21-generic AMD A10-5800K APU with Radeon(tm) HD Graphics 3793.099MHz 2048 KB cache

#### Test1
```
test 0 took 0.0000366430 S / 0.0000354750 S MONOTONIC/PROCESS pattern: \d
test 1 took 0.0000015500 S / 0.0000011780 S MONOTONIC/PROCESS pattern: \w+
test 2 took 0.0000010060 S / 0.0000006790 S MONOTONIC/PROCESS pattern: \s
test 3 took 0.0000012530 S / 0.0000009420 S MONOTONIC/PROCESS pattern: \S
test 4 took 0.0000011370 S / 0.0000008200 S MONOTONIC/PROCESS pattern: [\s]
test 5 took 0.0000033030 S / 0.0000029770 S MONOTONIC/PROCESS pattern: [\S]
test 6 took 0.0000011400 S / 0.0000008360 S MONOTONIC/PROCESS pattern: \D
test 7 took 0.0000015950 S / 0.0000013010 S MONOTONIC/PROCESS pattern: \W+
test 8 took 0.0000017510 S / 0.0000014360 S MONOTONIC/PROCESS pattern: [0-9]+
test 9 took 0.0000011160 S / 0.0000007920 S MONOTONIC/PROCESS pattern: \D
test 10 took 0.0000012370 S / 0.0000009300 S MONOTONIC/PROCESS pattern: \d
test 11 took 0.0000010820 S / 0.0000007740 S MONOTONIC/PROCESS pattern: [^\w]
test 12 took 0.0000010360 S / 0.0000007230 S MONOTONIC/PROCESS pattern: [\W]
test 13 took 0.0000014680 S / 0.0000010360 S MONOTONIC/PROCESS pattern: [\w]
test 14 took 0.0000012890 S / 0.0000009510 S MONOTONIC/PROCESS pattern: [^\d]
test 15 took 0.0000011530 S / 0.0000008330 S MONOTONIC/PROCESS pattern: [\d]
test 16 took 0.0000011230 S / 0.0000007880 S MONOTONIC/PROCESS pattern: [^\D]
test 17 took 0.0000010790 S / 0.0000007580 S MONOTONIC/PROCESS pattern: [\D]
test 18 took 0.0000017040 S / 0.0000013650 S MONOTONIC/PROCESS pattern: ^.*\\.*$
test 19 took 0.0000014970 S / 0.0000011510 S MONOTONIC/PROCESS pattern: ^[\+-]*[\d]+$
test 20 took 0.0000014380 S / 0.0000009470 S MONOTONIC/PROCESS pattern: [abc]
test 21 took 0.0000014170 S / 0.0000010030 S MONOTONIC/PROCESS pattern: [abc]
test 22 took 0.0000018210 S / 0.0000014170 S MONOTONIC/PROCESS pattern: [1-5]+
test 23 took 0.0000020610 S / 0.0000016410 S MONOTONIC/PROCESS pattern: [.2]
test 24 took 0.0000022350 S / 0.0000016310 S MONOTONIC/PROCESS pattern: a*$
test 25 took 0.0000020660 S / 0.0000016520 S MONOTONIC/PROCESS pattern: a*$
test 26 took 0.0000018380 S / 0.0000013710 S MONOTONIC/PROCESS pattern: [a-h]+
test 27 took 0.0000030230 S / 0.0000023120 S MONOTONIC/PROCESS pattern: [a-h]+
test 28 took 0.0000018960 S / 0.0000013980 S MONOTONIC/PROCESS pattern: [A-H]+
test 29 took 0.0000023630 S / 0.0000019030 S MONOTONIC/PROCESS pattern: [A-H]+
test 30 took 0.0000018610 S / 0.0000013920 S MONOTONIC/PROCESS pattern: [^\s]+
test 31 took 0.0000018880 S / 0.0000014080 S MONOTONIC/PROCESS pattern: [^fc]+
test 32 took 0.0000016560 S / 0.0000012110 S MONOTONIC/PROCESS pattern: [^d\sf]+
test 33 took 0.0000018450 S / 0.0000012810 S MONOTONIC/PROCESS pattern: 

test 34 took 0.0000020830 S / 0.0000017390 S MONOTONIC/PROCESS pattern: b.\s*

test 35 took 0.0000014640 S / 0.0000010830 S MONOTONIC/PROCESS pattern: .*c
test 36 took 0.0000015110 S / 0.0000010900 S MONOTONIC/PROCESS pattern: .+c
test 37 took 0.0000019160 S / 0.0000014350 S MONOTONIC/PROCESS pattern: [b-z].*
test 38 took 0.0000017160 S / 0.0000012250 S MONOTONIC/PROCESS pattern: b[k-z]*
test 39 took 0.0000021440 S / 0.0000017040 S MONOTONIC/PROCESS pattern: [0-9]
test 40 took 0.0000016250 S / 0.0000011350 S MONOTONIC/PROCESS pattern: [^0-9]
test 41 took 0.0000023750 S / 0.0000019250 S MONOTONIC/PROCESS pattern: [Hh]ello [Ww]orld\s*[!]?
test 42 took 0.0000020120 S / 0.0000016210 S MONOTONIC/PROCESS pattern: [Hh]ello [Ww]orld\s*[!]?
test 43 took 0.0000021450 S / 0.0000016980 S MONOTONIC/PROCESS pattern: [Hh]ello [Ww]orld\s*[!]?
test 44 took 0.0000042950 S / 0.0000038710 S MONOTONIC/PROCESS pattern: [Hh]ello [Ww]orld\s*[!]?
test 45 took 0.0000017930 S / 0.0000013790 S MONOTONIC/PROCESS pattern: [Hh]ello [Ww]orld\s*[!]?
test 46 took 0.0000021990 S / 0.0000017260 S MONOTONIC/PROCESS pattern: [Hh]ello [Ww]orld\s*[!]?
test 47 took 0.0000020680 S / 0.0000016550 S MONOTONIC/PROCESS pattern: ^/Users/[a-zA-Z0-9_]+$
test 48 took 0.0000032020 S / 0.0000027920 S MONOTONIC/PROCESS pattern: ^/Library/Address Book Plug-Ins/Skype[a-zA-Z]+.bundle.*$
test 49 took 0.0000020040 S / 0.0000013860 S MONOTONIC/PROCESS pattern: ^/Library/(A|B)$
test 50 took 0.0000016780 S / 0.0000012210 S MONOTONIC/PROCESS pattern: ^(A|B|C)$
test 51 took 0.0000020160 S / 0.0000016280 S MONOTONIC/PROCESS pattern: ^(A|B)/(u1|u2)$
test 52 took 0.0000019170 S / 0.0000013400 S MONOTONIC/PROCESS pattern: ^(u[a-z]+|u4)$
test 53 took 0.0000020260 S / 0.0000014680 S MONOTONIC/PROCESS pattern: ^(u|(b|c|(f|n)))$
test 54 took 0.0000022440 S / 0.0000018270 S MONOTONIC/PROCESS pattern: ^(u|(b|c|(f|n)))+$
test 55 took 0.0000020930 S / 0.0000017060 S MONOTONIC/PROCESS pattern: ^.+b.+b.+$
the next element in pattern [] is NULL before reaching ] symbol
--> i=6 value=b-z at __regex_compile_count:367
failed at regex_match:229
test 56 took 0.0000143580 S / 0.0000139020 S MONOTONIC/PROCESS pattern: AB[b-z
57/57 tests succeeded.

```

#### Test 2
```
Testing pathological pattern '.+nonexisting.+' to force worst-case asymptotic performance: 
compile took 0.0000028490 S / 0.0000017640 S MONOTONIC/PROCESS
  matching on 64 bytes of test input: -1 
test 0 took 0.0000420340 S / 0.0000416500 S MONOTONIC/PROCESS
  matching on 128 bytes of test input: -1 
test 1 took 0.0001373310 S / 0.0001369550 S MONOTONIC/PROCESS
  matching on 256 bytes of test input: -1 
test 2 took 0.0005075310 S / 0.0005071340 S MONOTONIC/PROCESS
  matching on 512 bytes of test input: -1 
test 3 took 0.0020644220 S / 0.0020639100 S MONOTONIC/PROCESS
  matching on 1024 bytes of test input: -1 
test 4 took 0.0078064600 S / 0.0078060770 S MONOTONIC/PROCESS
  matching on 2048 bytes of test input: -1 
test 5 took 0.0314271400 S / 0.0314182610 S MONOTONIC/PROCESS
  matching on 4096 bytes of test input: -1 
test 6 took 0.1239317370 S / 0.1239329220 S MONOTONIC/PROCESS
  matching on 8192 bytes of test input: -1 
test 7 took 0.4963413850 S / 0.4963483590 S MONOTONIC/PROCESS
  matching on 16384 bytes of test input: -1 
test 8 took 1.9739927620 S / 1.9739974500 S MONOTONIC/PROCESS
  matching on 32768 bytes of test input: -1 
test 9 took 7.9121831500 S / 7.9122624800 S MONOTONIC/PROCESS
```
