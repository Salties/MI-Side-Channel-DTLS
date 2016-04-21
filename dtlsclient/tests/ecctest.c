#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ecc/ecc.h"

#define KEYSIZE 8

uint32_t sk[KEYSIZE] = { 0 };
uint32_t pkx[KEYSIZE] = { 0 };
uint32_t pky[KEYSIZE] = { 0 };

void PrintInt256(uint32_t * i256)
{
    int i;
    for (i = KEYSIZE; i > 0; i--)
        printf("%08lX ", i256[i - 1]);
    return;
}

void str2hex(char *str, uint32_t * hexval)
{
    int i;
    char *cptr = str;
    char strbuf[9] = { 0 };

    for (i = KEYSIZE; i > 0; i--) {
        //Skip spaces.
        while (*cptr == ' ')
            cptr++;
        if (*cptr == '\0' || *cptr == '\n')
            break;
        strncpy(strbuf, cptr, 8);
        hexval[i - 1] = strtoll(strbuf, NULL, 16);
        cptr += 8;
    }
    return;
}

void EccTiming(char *inputkey)
{
    //unsigned long start, end;

    str2hex((char *) inputkey, sk);
    printf("#Parsed Secret Key: \t");
    PrintInt256(sk);
    printf("\n");
    //Start timing
 //   leds_on(LEDS_ECC);
 //   start = RTIMER_NOW();
    ecc_gen_pub_key(sk, pkx, pky);
 //   end = RTIMER_NOW();
 //   leds_off(LEDS_ECC);
    //Print result
    printf("#Secret Key: \t");
    PrintInt256(sk);
    printf("\n");
    printf("#Public Key:\n");
    printf("#\tQ_x:\t");
    PrintInt256(pkx);
    printf("\n");
    printf("#\tQ_y:\t");
    PrintInt256(pky);
    printf("\n");
    //printf("#Time elapsed:\n %lu\n", end - start);
}

int main()
{
    char *input = NULL;
    size_t len = 0;
    getline(&input, &len, stdin);
    printf("#Received %s\n", input);
    EccTiming(input);
    free(input);
    return 0;
}
