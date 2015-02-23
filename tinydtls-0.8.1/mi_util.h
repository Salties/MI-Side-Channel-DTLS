#ifndef MI_UTIL_H
#define MI_UTIL_H

#include <stdlib.h>
#include <stdint.h>

//For general use
unsigned int mi_rand();

//For Single_Or_Even Toy
void SingleOrEven_C(void *buf, size_t *len);
int SingleOrEven_S(void *inbuf, size_t inlen, void* outbuf, size_t *outlen);

//For Leaky Coffee Toy
int OrderCoffee_C(uint8_t *buf, size_t *len);
int LeakyCoffee_C(uint8_t *buf, size_t *len);
int LeakyCoffee_S(uint8_t *inbuf, size_t inlen, uint8_t* outbuf, size_t *outlen);

#endif
