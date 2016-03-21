/*
 * Copyright (c) 2006, Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 */

/**
 * \file
 *         A very simple Contiki application showing how Contiki programs look
 * \author
 *         Adam Dunkels <adam@sics.se>
 */

#include "contiki.h"
#include "lib/aes-128.h"
#include "sys/rtimer.h"
#include "sys/etimer.h"

#include <stdio.h>              /* For printf() */
#include <string.h>

#define AES_KEY_LEN 16
#define AES_BLOCK_LEN 16

#ifndef NROUND
#define NROUND 200
#endif

#ifndef NSAMPLE
#define NSAMPLE 200
#endif

static uint8_t Aes128Key[AES_KEY_LEN] = {
    0x01, 0x23, 0x45, 0x67,
    0x89, 0xab, 0xcd, 0xef,
    0x12, 0x34, 0x56, 0x78,
    0x9a, 0xbc, 0xde, 0xf0
};

static const uint8_t fixed_data[AES_BLOCK_LEN] ={
	0xda, 0x39, 0xa3, 0xee,
	0x54, 0x6b, 0x4b, 0x0d,
	0x32, 0x55, 0xbf, 0xef,
	0x95, 0x60, 0x18, 0x90
};

static uint8_t datablock[NROUND][AES_BLOCK_LEN] = { {0} };

void PrintBlock(const char *prefix, const uint8_t * block, const char *appendix)
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

    printf("Testing AES-128 implementation for %s.\n", TARGET_NAME);

    etimer_set(&periodic_timer, (1 * CLOCK_SECOND));

    for( i = 0; i < NSAMPLE; i++)
    {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));
        etimer_reset(&periodic_timer);

#ifndef AES_128_CONF
        printf("Testing Contiki Software AES implementation, sample %d/%d\n", i+1, NSAMPLE);
#else
        printf("Testing Harware AES processor, sample %d/%d\n", i+1, NSAMPLE);
#endif

        PrintBlock("Key\t:", Aes128Key, "\n");
        AES_128.set_key(Aes128Key);

        PrintBlock("Plaintext\t: ", fixed_data, "\n");

	//Initialise Data
	for( j = 0; j < NROUND; j++)
		memcpy(datablock[j], fixed_data, AES_BLOCK_LEN);

	//Start timing.
        start = RTIMER_NOW();
        for (j = 0; j < NROUND; j++) 
	{
            AES_128.encrypt(datablock[j]);
         }
        end = RTIMER_NOW();

	//Print result.
        PrintBlock("Ciphertext\t: ", datablock[0], "\n");
        printf("Round\t: %d\n", NROUND);
        printf("Start\t: %lu\n", start);
        printf("ENd\t: %lu\n", end);
        printf("Time Elapsed\t: %lu\n", end - start);
    }

    printf("%d tests done for %s.\n", NSAMPLE, TARGET_NAME);

    PROCESS_END();
}

/*---------------------------------------------------------------------------*/
