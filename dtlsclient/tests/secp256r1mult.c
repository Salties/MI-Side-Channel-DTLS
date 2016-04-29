#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ecc/ecc.h"

#define KEYSIZE 32

uint32_t sk[KEYSIZE] = { 0 };
uint32_t pkx[KEYSIZE] = { 0 };
uint32_t pky[KEYSIZE] = { 0 };

void Print256(unsigned char* i256)
{
	int i;
	for(i = 0; i < (256 / 8); i++)
		printf("%02X", i256[i]);
}

void Print256_nbo(unsigned char* i256)
{
	int i;
	for(i = 256/8; i>0 ; i--)
		printf("%02X", i256[i]);
}

unsigned char ascii2int8(char c)
{
	char buf[2] = {0};
	buf[0] = c;
	return (unsigned char)strtol(buf,NULL,16);
}

void str2hex(unsigned char * hexval, char* str, size_t len)
{
    int i;

    memset(hexval, 0, len / 2 + len % 2);
    for(i=0; i < len; i++)
    {
	    if(!(i % 2))	//Higher 4 bits of a byte.
	    {
		*hexval |= ascii2int8(str[i]) << 4;
	    }
	    else		//Lower 4 bits of a byte.
	    {
		*hexval |= ascii2int8(str[i]);
		hexval++;
	    }
    } 

    return;
}

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
	    printf("Usage: secp256r1mult K\n");
	    exit(-1);
    }
    str2hex((void*)sk, argv[1], strlen(argv[1]));
    ecc_ec_mult(ecc_g_point_x, ecc_g_point_y, sk, pkx, pky);
    //Print256(sk);
    fieldModO(pkx, pkx, 8);
    //Print256((unsigned char *)pkx);
    Print256_nbo((unsigned char *)pkx);
    printf("\n");
    return 0;
}
