#include "utils.h"

#include <stdlib.h>

void initsock(struct sockaddr_in *addr)
{
	memset(addr, 0, sizeof(struct sockaddr_in));
	return;
}
