TARGET=midtls
SRC=*.cpp
CC=g++
CPPFLAG=-g -Wall

all: ${SRC}
	${CC} ${SRC} ${CPPFLAG} -o ${TARGET}

clean:
	rm -rf *~ ${TARGET}

	
