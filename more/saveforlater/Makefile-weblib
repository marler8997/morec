CC=gcc -std=c99
SQL_FLAGS=-I/usr/include/mysql -DBIG_JOINS=1 -fno-strict-aliasing -DUNIV_LINUX -DUNIV_LINUX -rdynamic -L/usr/lib/mysql -lmysqlclient

BUILD_DIR=build

STATIC_LIBS=${BUILD_DIR}/obj/url.o ${BUILD_DIR}/obj/sha1.o ${BUILD_DIR}/obj/sid.o \
		${BUILD_DIR}/obj/base64.o ${BUILD_DIR}/obj/string_util.o ${BUILD_DIR}/obj/request.o
DEBUG_TOOLS=${BUILD_DIR}/debug/getenv ${BUILD_DIR}/debug/setcookie
TOOLS=${BUILD_DIR}/tools/urldec ${BUILD_DIR}/tools/sha1 ${BUILD_DIR}/tools/base64
TESTS=${BUILD_DIR}/test/url_test ${BUILD_DIR}/test/string_util_test ${BUILD_DIR}/test/sha_test

all: static_libs debug_tools tools tests
static_libs: ${STATIC_LIBS}
debug_tools: ${DEBUG_TOOLS}
tools: ${TOOLS}
tests: ${TESTS}

check: ${BUILD_DIR}/check_compile
	${BUILD_DIR}/check_compile

${BUILD_DIR}/check_compile: check_compile.c
	${CC} -o ${BUILD_DIR}/check_compile check_compile.c

#
# Static Libraries
#
${BUILD_DIR}/obj/url.o: url.c url.h
	${CC} -c -o ${BUILD_DIR}/obj/url.o url.c

${BUILD_DIR}/obj/sha1.o: sha1.c sha1.h
	${CC} -c -o ${BUILD_DIR}/obj/sha1.o sha1.c

${BUILD_DIR}/obj/sid.o: sid.c sid.h
	${CC} -c -o ${BUILD_DIR}/obj/sid.o sid.c

${BUILD_DIR}/obj/base64.o: base64.c base64.h
	${CC} -c -o ${BUILD_DIR}/obj/base64.o base64.c

${BUILD_DIR}/obj/request.o: request.c request.h
	${CC} -c -o ${BUILD_DIR}/obj/request.o request.c

${BUILD_DIR}/obj/string_util.o: string_util.c string_util.h
	${CC} -c -o ${BUILD_DIR}/obj/string_util.o string_util.c

#
# Debug Tools
#
${BUILD_DIR}/debug/getenv: getenv.c
	${CC} -o ${BUILD_DIR}/debug/getenv getenv.c

${BUILD_DIR}/debug/setcookie: setcookie.c
	${CC} -o ${BUILD_DIR}/debug/setcookie setcookie.c

#
# Tools
#
${BUILD_DIR}/tools/urldec: urldec.c ${BUILD_DIR}/obj/url.o
	${CC} -o ${BUILD_DIR}/tools/urldec urldec.c ${BUILD_DIR}/obj/url.o

${BUILD_DIR}/tools/sha1: sha1main.c ${BUILD_DIR}/obj/sha1.o
	${CC} -o ${BUILD_DIR}/tools/sha1 sha1main.c ${BUILD_DIR}/obj/sha1.o

${BUILD_DIR}/tools/base64: base64main.c ${BUILD_DIR}/obj/base64.o
	${CC} -o ${BUILD_DIR}/tools/base64 base64main.c ${BUILD_DIR}/obj/base64.o

#
# Tests
#

${BUILD_DIR}/test/url_test: url_test.c ${BUILD_DIR}/obj/url.o
	${CC} -o ${BUILD_DIR}/test/url_test url_test.c ${BUILD_DIR}/obj/url.o

${BUILD_DIR}/test/sha_test: sha_test.c ${BUILD_DIR}/obj/sha1.o
	${CC} -o ${BUILD_DIR}/test/sha_test sha_test.c ${BUILD_DIR}/obj/sha1.o

${BUILD_DIR}/test/string_util_test: string_util_test.c ${BUILD_DIR}/obj/string_util.o
	${CC} -o ${BUILD_DIR}/test/string_util_test string_util_test.c ${BUILD_DIR}/obj/string_util.o

#
# Clean
#
clean:
	rm -f *.o *~
	rm -rf ${BUILD_DIR}
	mkdir ${BUILD_DIR} ${BUILD_DIR}/obj ${BUILD_DIR}/tools ${BUILD_DIR}/debug ${BUILD_DIR}/test