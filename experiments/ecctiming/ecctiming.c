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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include "contiki.h"
#include "dev/serial-line.h"
#include "dev/leds.h"

#include "ecc/ecc.h"

#define KEYSIZE 8

uint32_t sk[KEYSIZE] = { 0 };
uint32_t pkx[KEYSIZE] = { 0 };
uint32_t pky[KEYSIZE] = { 0 };

void PrintInt256(uint32_t * i256)
{
    int i;
    printf("#");
    for (i = KEYSIZE; i > 0; i--)
        printf("%08lX ", i256[i - 1]);
    printf("\n");
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
        hexval[i - 1] = strtol(strbuf, NULL, 16);
        cptr += 8;
    }
    return;
}

void EccTiming(char* inputkey)
{
        unsigned long start, end;
	
	str2hex((char*)inputkey, sk);
	//Start timing
        start = RTIMER_NOW();
        //ecc_gen_pub_key(sk, pkx, pky);
        end = RTIMER_NOW();
	//Print result
	printf("#Secret Key:\n");
	PrintInt256(sk);
	printf("#Public Key:\n");
	printf("#Q_x: ");
	PrintInt256(pkx);
	printf("#Q_y: ");
	PrintInt256(pky);
        printf("#Time elapsed:\n %lu\n", end - start);
}

/*---------------------------------------------------------------------------*/
PROCESS(ecc_timing_process, "ECC timing process");
AUTOSTART_PROCESSES(&ecc_timing_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(ecc_timing_process, ev, data)
{


    PROCESS_BEGIN();
    
    serial_line_init();
    leds_init();
    printf("#Hello, world\n");
    leds_on(LEDS_ALL);
    
    while(1) {
	PROCESS_WAIT_EVENT_UNTIL(ev == serial_line_event_message && data != NULL);
	leds_toggle(LEDS_ALL);
	printf("#Received line:%s\n", (char*) data);
	EccTiming(data);

    }

    PROCESS_END();
}

/*---------------------------------------------------------------------------*/
