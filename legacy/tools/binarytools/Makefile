COMMON=../../common
CC=gcc -std=c99 -I${COMMON}

TOOLS= binaryreplace

all: tools tests
tools:${TOOLS}

binaryreplace: binaryreplace.c binarycommon.o ${COMMON}/hex.o
	${CC} -o binaryreplace binaryreplace.c binarycommon.o ${COMMON}/hex.o

binarycommon.o: binarycommon.c ${COMMON}/hex.o
	${CC} -c binarycommon.c ${COMMON}/hex.o

clean:
	rm -f *~ *.o ${TOOLS} ${TESTS}