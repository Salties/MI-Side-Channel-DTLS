SERVER_SRC=midtls_server.cpp
CLIENT_SRC=midtls_client.cpp
UTILS_SRC=utils.c
UTILS=utils.o
LIBS=-lpthread -lcrypto -lssl
CC=g++
CPPFLAG=-g -Wall

all: server client utils.o

${UTILS}: ${UTILS_SRC}
	${CC} -c ${UTILS_SRC} ${LIBS} ${CPPFLAG}

server: ${SERVER_SRC} ${UTILS}
	${CC} ${SERVER_SRC} ${UTILS} ${LIBS} ${CPPFLAG} -o server

client: ${CLIENT_SRC} ${UTILS}
	${CC} ${CLIENT_SRC} ${UTILS} ${LIBS} ${CPPFLAG} -o client

clean:
	rm -rf *~ *.o ${TARGET} server client
