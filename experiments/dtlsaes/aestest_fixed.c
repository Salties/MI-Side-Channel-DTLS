#include "contiki.h"
#include "sys/rtimer.h"
#include "sys/etimer.h"

#include "aes/rijndael.h"

#include <stdio.h>              /* For printf() */
#include <string.h>

#define AES_KEY_LEN 16
#define AES_BLOCK_LEN 16

#ifndef NROUND
#define NROUND 200
#endif

#ifndef NSAMPLE
#define NSAMPLE 100
#endif

static rijndael_ctx aes_ctx;

static uint8_t Aes128Key[AES_KEY_LEN] = {
    0x01, 0x23, 0x45, 0x67,
    0x89, 0xab, 0xcd, 0xef,
    0x12, 0x34, 0x56, 0x78,
    0x9a, 0xbc, 0xde, 0xf0
};

static const uint8_t fixed_data[AES_BLOCK_LEN] = {
    0xda, 0x39, 0xa3, 0xee,
    0x54, 0x6b, 0x4b, 0x0d,
    0x32, 0x55, 0xbf, 0xef,
    0x95, 0x60, 0x18, 0x90
};

static uint8_t plaintext[NROUND][AES_BLOCK_LEN] = { {0} };
static uint8_t ciphertext[NROUND][AES_BLOCK_LEN] = { {0} };

void PrintBlock(const char *prefix, const uint8_t * block,
                const char *appendix)
{
    int i;

    printf(prefix);
    for (i = 0; i < AES_KEY_LEN; i++)
        printf("%02x ", block[i]);
    printf(appendix);
    return;
}

/*---------------------------------------------------------------------------*/
PROCESS(aestest, "aestest");
AUTOSTART_PROCESSES(&aestest);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(aestest, ev, data)
{
    static int i, j;
    unsigned long start, end;
    static struct etimer periodic_timer;

    PROCESS_BEGIN();

    printf("#Fixed AES-128 implementation test for %s.\n", TARGET_NAME);
    printf("#Using OpenBSD optimised AES implementation.\n");
    printf("#Rounds in each sample: %d\n", NROUND);
    printf("#Sample size: %d\n", NSAMPLE);
    printf("#Rtimer clock ticks per second on this platform is : %lu\n",
           (unsigned long) RTIMER_SECOND);


    etimer_set(&periodic_timer, (1 * CLOCK_SECOND));

    //Initialise Data.
    for (j = 0; j < NROUND; j++)
        memcpy(plaintext[j], fixed_data, AES_BLOCK_LEN);

    //Begin test.
    for (i = 0; i < NSAMPLE; i++) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));
        etimer_reset(&periodic_timer);

#ifdef VERBOSE_AESTEST
        printf("#Sample %d/%d\n", i + 1, NSAMPLE);
        PrintBlock("#Key\t:", Aes128Key, "\n");
        PrintBlock("#Plaintext\t: ", fixed_data, "\n");
#endif
        //Set Key.
        rijndael_set_key_enc_only(&aes_ctx, Aes128Key, 8 * AES_KEY_LEN);

        //Timing.AES.
        start = RTIMER_NOW();
        for (j = 0; j < NROUND; j++) {
            rijndael_encrypt(&aes_ctx, plaintext[j], ciphertext[j]);
        }
        end = RTIMER_NOW();


#ifdef VERBOSE_AESTEST
        //Print result.
        PrintBlock("#Ciphertext\t: ", ciphertext[0], "\n");
        printf("#Round\t: %d\n", NROUND);
        printf("#Start\t: %lu\n", start);
        printf("#End\t: %lu\n", end);
        printf("#Time Elapsed\t:\n %lu\n", end - start);
#else
        printf("%lu\n", end - start);
#endif
    }

    printf("#%d tests done for %s.\n", NSAMPLE, TARGET_NAME);

    PROCESS_END();
}

/*---------------------------------------------------------------------------*/
