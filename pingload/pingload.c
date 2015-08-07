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
#include "sys/etimer.h"
#include "sys/timer.h"
#include "sys/clock.h"
#include "dev/leds.h"
#include "watchdog.h"
#include "random.h"

#include "dev/cc2538-sensors.h"

#include <stdio.h>		/* For printf() */

#define EXPECTED_IDLE (1000 - EXPECTED_PAYLOAD)
#define EXPECTED_PAYLOAD (800)	//Processor payload (in 1/1000 ).
//#define PAYLOAD_SCALER 1
#define GRANULARITY 10000
#define MAGIC_PRIME 63793

/*---------------------------------------------------------------------------*/
PROCESS(pingload_process, "Pingload Process");
AUTOSTART_PROCESSES(&pingload_process);
/*---------------------------------------------------------------------------*/

unsigned short GetRandom(unsigned int space)
{
    static int init = 0;

    if (!init)
      {
	  random_init(clock_time() % 0xffff);
	  init = 1;
      }

    return random_rand() % space;
}

inline unsigned int GetPeriod(unsigned int expected)
{
    //Return a random period which expectation is "expected".
    return (expected == 0 ? 0 : GetRandom(2 * expected + 1));
}

PROCESS_THREAD(pingload_process, ev, data)
{
    static struct etimer et;
    static struct timer t;
    //static unsigned short i;
    static unsigned long long count;
    static unsigned int sleep_period, busy_period;

    PROCESS_BEGIN();

    printf("Hello, world\n");

    leds_init();

    while (1)
      {
	  //Sleep period.
	  leds_on(LEDS_BLUE);
	  sleep_period = GetPeriod(EXPECTED_IDLE);
	  printf("Enter sleep for %d/%d seconds...", sleep_period,
		 GRANULARITY);
	  etimer_set(&et, CLOCK_SECOND * sleep_period / GRANULARITY);
	  PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
	  printf("Exit.\n");
	  leds_off(LEDS_BLUE);

	  //Busy period.
	  leds_on(LEDS_RED);
	  busy_period = GetPeriod(EXPECTED_PAYLOAD);
	  printf("Enter busy for %d/%d seconds...\n", busy_period,
		 GRANULARITY);
	  timer_set(&t, CLOCK_SECOND * busy_period / GRANULARITY);
	  for (count = 0; !timer_expired(&t); watchdog_periodic(), count++)
	    {
		//Place the work load here
		clock_wait(1);
 	    }
	  leds_off(LEDS_RED);
	  printf("Exit.(count=%lld)\n", count);
      }

    PROCESS_END();
}

/*---------------------------------------------------------------------------*/
