
OBJ=hex.o BinaryConversion.o
TESTS=WaitEventTest

INCLUDE=-I test

CE=gcc $(INCLUDE) -std=c99 -o $@
CC=$(CE) -c

all: obj $(TESTS)


obj: ${OBJ}


hex.o: hex.c hex.h
	${CC} -c hex.c

BinaryConversion.o: BinaryConversion.c
	$(CC) $^

WaitEventTest: WaitEventTest.c
	$(CE) WaitEventTest.c



clean:
	rm -f *~ *.o $(TESTS)