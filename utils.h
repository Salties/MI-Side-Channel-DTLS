#ifndef UTILS_H
#define UTILS_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void initsock(struct sockaddr_in *addr);

#endif
