SERVER_SRC=midtls_server.cpp
CLIENT_SRC=midtls_client.cpp
SRC=*.cpp
LIBS=-lpthread -lcrypto -lssl
CC=g++
CPPFLAG=-g -Wall

all: server client

server: ${SERVER_SRC}
	${CC} ${SERVER_SRC} ${LIBS} ${CPPFLAG} -o server

client: ${CLIENT_SRC}
	${CC} ${CLIENT_SRC} ${LIBS} ${CPPFLAG} -o client

clean:
	rm -rf *~ ${TARGET}

	
