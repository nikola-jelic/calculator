INCLUDE = include
SRC = src
BIN = bin
TEST = test

CFLAGS = -Wall -Wextra -pedantic -O3 -I${INCLUDE}
TEST_CFLAGS = -Wall -Wextra -pedantic -g3 -I${INCLUDE}
LDFLAGS = -lm
TEST_LDFLAGS = ${LDFLAGS} -lcunit

TARGET = ${BIN}/calc
SUITE = ${TEST}/suite

all: ${TARGET} ${SUITE}

${SRC}/calc_element.o: ${SRC}/calc_element.c ${INCLUDE}/calc_element.h
${SRC}/lexer.o: ${SRC}/lexer.c ${INCLUDE}/lexer.h ${INCLUDE}/symbols.h
${SRC}/parser.o: ${SRC}/parser.c ${INCLUDE}/parser.h ${INCLUDE}/lexer.h ${INCLUDE}/symbols.h ${INCLUDE}/calc_element.h
${SRC}/main.o: ${SRC}/main.c ${INCLUDE}/parser.h ${INCLUDE}/lexer.h ${INCLUDE}/symbols.h ${INCLUDE}/calc_element.h

${TEST}/calc_element.o: ${SRC}/calc_element.c ${INCLUDE}/calc_element.h
	${CC} ${TEST_CFLAGS}   -c -o $@ ${SRC}/calc_element.c
${TEST}/lexer.o: ${SRC}/lexer.c ${INCLUDE}/lexer.h ${INCLUDE}/symbols.h
	${CC} ${TEST_CFLAGS}   -c -o $@ ${SRC}/lexer.c
${TEST}/parser.o: ${SRC}/parser.c ${INCLUDE}/parser.h ${INCLUDE}/lexer.h ${INCLUDE}/symbols.h ${INCLUDE}/calc_element.h
	${CC} ${TEST_CFLAGS}   -c -o $@ ${SRC}/parser.c
${TEST}/suite.o: ${TEST}/suite.c ${INCLUDE}/parser.h ${INCLUDE}/lexer.h ${INCLUDE}/symbols.h ${INCLUDE}/calc_element.h
	${CC} ${TEST_CFLAGS}   -c -o $@ ${TEST}/suite.c

${TARGET}: ${SRC}/calc_element.o ${SRC}/lexer.o ${SRC}/parser.o ${SRC}/main.o
	${CC} ${LDFLAGS} -o $@ ${SRC}/calc_element.o ${SRC}/lexer.o ${SRC}/parser.o ${SRC}/main.o
	strip $@

${SUITE}: ${TEST}/calc_element.o ${TEST}/lexer.o ${TEST}/parser.o ${TEST}/suite.o
	${CC} ${TEST_LDFLAGS} -o $@ ${TEST}/calc_element.o ${TEST}/lexer.o ${TEST}/parser.o ${TEST}/suite.o

clean:
	rm -f ${SRC}/*.o
	rm -f ${TEST}/*.o
	rm -f ${TARGET}
	rm -f ${SUITE}

valgrind: ${SUITE}
	valgrind ./${SUITE}

