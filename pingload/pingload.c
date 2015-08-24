#include "contiki.h"
#include "sys/etimer.h"
#include "sys/timer.h"
#include "sys/ctimer.h"
#include "sys/clock.h"
#include "watchdog.h"
#include "random.h"

#include "dev/cc2538-sensors.h"
#include "dev/als-sensor.h"

#include <stdio.h>		/* For printf() */

#define PAYLOAD	((1))
#define IDLE	((9))

#ifndef SCALE
#ifdef SECOND
#define SCALE CLOCK_SECOND
#else
#define SCALE 1
#endif
#endif

#define EXPECTED_PAYLOAD ((PAYLOAD * SCALE))
#define EXPECTED_IDLE ((IDLE * SCALE))

#define WATCHDOG_RESET_PERIOD (( CLOCK_SECOND/2 ))

//The expected payload cycle will be SCALER/GRANULARITY seconds.
//#define SCALER 1
//#define GRANULARITY 1

#ifdef LED_INDICATOR
#include "dev/leds.h"
#endif

/*---------------------------------------------------------------------------*/
PROCESS(pingload_process, "Pingload Process");
AUTOSTART_PROCESSES(&pingload_process);
/*---------------------------------------------------------------------------*/
static struct etimer idle_timer;
static struct timer payload_timer;
static struct ctimer resetwd_timer;

inline void Payload()
{
    static clock_time_t payload_start, payload_end;

    int i;
    int vdd, temperature, light;
    //Payload begin timeing.
    payload_start = clock_time();

    //Payload
    vdd = 0;
    temperature = 0;
    light = 0;
    for (i = 0; i < 30; i++)
      {
	      vdd += vdd3_sensor.value(CC2538_SENSORS_VALUE_TYPE_CONVERTED);
	      temperature += cc2538_temp_sensor.value(CC2538_SENSORS_VALUE_TYPE_CONVERTED);
	      light += als_sensor.value(0);
#ifdef DEBUG
	  printf("VDD = %d mV\n", vdd);
	  printf("Temperature = %d mC\n",temperature);
	  printf("Ambient light sensor = %d raw\n", light);
#endif
      }
    printf("VDD = %d mV\n", vdd/30);
    printf("Temperature = %d mC\n",temperature/30);
    printf("Ambient light sensor = %d raw\n", light/30);

    //Payload end timing.
    payload_end = clock_time();
    printf("%d ticks elapsed.\n", (int) (payload_end - payload_start));

    return;
}

//Return a random period which expectation is "expected" in clock ticks.
inline clock_time_t GetPeriod(unsigned int expected)
{
    clock_time_t r =
	(expected == 0 ? 0 : (random_rand() % (2 * expected + 1)));
    return r;
}

static void ResetWd(void *arg)
{
    watchdog_periodic();
    ctimer_reset(&resetwd_timer);
    return;
}

PROCESS_THREAD(pingload_process, ev, data)
{
    //static unsigned long long count;
    static clock_time_t idle_period, payload_period;

    PROCESS_BEGIN();

    printf("Hello, world\n");
    ctimer_set(&resetwd_timer, WATCHDOG_RESET_PERIOD, &ResetWd, NULL);

#ifdef LED_INDICATOR
    leds_init();
#endif

    while (1)
      {
	  //Idle period.
	  idle_period = GetPeriod(EXPECTED_IDLE);
#ifdef LED_INDICATOR
	  leds_on(LEDS_BLUE);
#endif
	  printf("Enter sleep for %d ticks(%d seconds)...",
		 (int) idle_period, (int) idle_period / CLOCK_SECOND);
	  etimer_set(&idle_timer, idle_period);
	  PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&idle_timer));
	  printf("Exit.\n");
#ifdef LED_INDICATOR
	  leds_off(LEDS_BLUE);
#endif

	  //Payload period.
#ifdef LED_INDICATOR
	  leds_on(LEDS_RED);
#endif
	  payload_period = GetPeriod(EXPECTED_PAYLOAD);
	  printf("Enter busy for %d ticks(%d seconds)...\n",
		 (int) payload_period,
		 (int) payload_period / CLOCK_SECOND);
	  timer_set(&payload_timer, payload_period);
	  //Start Payload.
	  Payload();
	  printf("Time remained:%d\n",
		 (int) timer_remaining(&payload_timer));
	  //Busy wait for the remaining time.
	  while (!timer_expired(&payload_timer))
	      watchdog_periodic();
#ifdef LED_INDICATOR
	  leds_off(LEDS_RED);
#endif
      }

    PROCESS_END();
}

/*---------------------------------------------------------------------------*/
