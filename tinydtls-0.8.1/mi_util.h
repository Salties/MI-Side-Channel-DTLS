#ifndef MI_UTIL_H
#define MI_UTIL_H

#include <stdlib.h>

unsigned int mi_rand();
void SingleOrEven_C(void *buf, size_t *len);
int SingleOrEven_S(void *inbuf, size_t inlen, void* outbuf, size_t *outlen);

#endif
