# Compiler to use - can be replaced by gcc for instance
CC := clang

# Number of random text expressions to generate, for random testing
NRAND_TESTS := 1000


# Flags to pass to compiler
CFLAGS := -O2 -Wall -Wextra -std=c99 -I.

all:
	@$(CC) $(CFLAGS) libsmallregex/libsmallregex.c tests/test1.c     -o tests/test1
	@$(CC) $(CFLAGS) libsmallregex/libsmallregex.c tests/test2.c     -o tests/test2
	@$(CC) $(CFLAGS) libsmallregex/libsmallregex.c tests/test3.c     -o tests/test3
	@$(CC) $(CFLAGS) libsmallregex/libsmallregex.c tests/test_rand.c -o tests/test_rand

clean:
	@rm -f tests/test1 tests/test2 tests/test3 tests/test_rand
	@#@$(foreach test_bin,$(TEST_BINS), rm -f $(test_bin) ; )
	@rm -f a.out
	@rm -f *.o


test: all
	@echo
	@echo Testing hand-picked regex\'s:
	@./tests/test1
	@echo Testing patterns against $(NRAND_TESTS) random strings matching the Python implementation and comparing:
	@echo
	@python ./scripts/regex_test.py \\d+\\w?\\D\\d         $(NRAND_TESTS)
	@python ./scripts/regex_test.py \\s+[a-zA-Z0-9?]*      $(NRAND_TESTS)
	@python ./scripts/regex_test.py \\w*\\d?\\w\\?         $(NRAND_TESTS)
	@#python ./scripts/regex_test.py [^\\d]+\\\\?\\s        $(NRAND_TESTS)
	@#python ./scripts/regex_test.py [^\\w][^-1-4]          $(NRAND_TESTS)
	@#python ./scripts/regex_test.py [^\\w]                 $(NRAND_TESTS)
	@#python ./scripts/regex_test.py [^1-4]                 $(NRAND_TESTS)
	@#python ./scripts/regex_test.py [^-1-4]                $(NRAND_TESTS)
	@#python ./scripts/regex_test.py [^\\d]+\\s?[\\w]*      $(NRAND_TESTS)
	@python ./scripts/regex_test.py a+b*[ac]*.+.*.[\\.].   $(NRAND_TESTS)
	@python ./scripts/regex_test.py a?b[ac*]*.?[\\]+[?]?   $(NRAND_TESTS)
	@#python ./scripts/regex_test.py [1-5-]+[-1-2]-[-]     $(NRAND_TESTS)
	@python ./scripts/regex_test.py [-1-3]-[-]+            $(NRAND_TESTS)
	@python ./scripts/regex_test.py [1-5]+[-1-2]-[\\-]     $(NRAND_TESTS)
	@python ./scripts/regex_test.py [-1-2]*                $(NRAND_TESTS)
	@python ./scripts/regex_test.py \\s?[a-fKL098]+-?      $(NRAND_TESTS)
	@python ./scripts/regex_test.py [\\-]*                 $(NRAND_TESTS)
	@python ./scripts/regex_test.py [\\\\]+                $(NRAND_TESTS)
	@python ./scripts/regex_test.py [0-9a-fA-F]+           $(NRAND_TESTS)
	@python ./scripts/regex_test.py [1379][2468][abcdef]   $(NRAND_TESTS)
	@python ./scripts/regex_test.py [012345-9]?[0123-789]  $(NRAND_TESTS)
	@python ./scripts/regex_test.py [012345-9]             $(NRAND_TESTS)
	@python ./scripts/regex_test.py [0-56789]              $(NRAND_TESTS)
	@python ./scripts/regex_test.py [abc-zABC-Z]           $(NRAND_TESTS)
	@python ./scripts/regex_test.py [a\d]?1234             $(NRAND_TESTS)
	@python ./scripts/regex_test.py .*123faerdig           $(NRAND_TESTS)
	@python ./scripts/regex_test.py .?\\w+jsj$             $(NRAND_TESTS)
	@python ./scripts/regex_test.py [?to][+to][?ta][*ta]   $(NRAND_TESTS)
	@#python ./scripts/regex_test.py [^-1-4]                $(NRAND_TESTS)
	@echo
	@echo
	@./tests/test2
	@echo
	@echo
	
