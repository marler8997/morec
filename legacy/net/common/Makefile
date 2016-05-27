CC=gcc

TESTS=test/string_util_brute_force

CC_TEST_FLAGS=-I .

all: net.o string_util.o maclib.o ${TESTS}

net.o: net.c
	${CC} -c net.c

string_util.o: string_util.c
	${CC} -c string_util.c

maclib.o: maclib.c
	${CC} -c maclib.c


#
# Tests
#
test/string_util_brute_force: test/string_util_brute_force.c string_util.o
	${CC} ${CC_TEST_FLAGS} -o test/string_util_brute_force test/string_util_brute_force.c string_util.o



clean:
	rm -f *.o *~ ${TESTS}
