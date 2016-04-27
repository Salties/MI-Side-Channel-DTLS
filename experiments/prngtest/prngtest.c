#include "contiki.h"
#include "sys/rtimer.h"
#include "lib/random.h"

#include <stdio.h> /* For printf() */
/*---------------------------------------------------------------------------*/
PROCESS(prng_test_process, "PRNG test");
AUTOSTART_PROCESSES(&prng_test_process);
/*---------------------------------------------------------------------------*/

static unsigned char appearance[8192] =  {0};

unsigned short GetMark(unsigned short val)
{
	return (0xFFFF & (appearance[val/8] & (0x1 << (val % 8)) ));
}

void SetMark(unsigned short val)
{
	appearance[val/8] |= (0x1 << (val % 8));
	return;
}

PROCESS_THREAD(prng_test_process, ev, data)
{
  static unsigned short rndval;
  static unsigned int count;
  
  PROCESS_BEGIN();
  
  for(count = 1; count <= 0xFFFF; count++)
  {
	rndval = random_rand();
	printf("#%d: %X\n", count, rndval);
	if(GetMark(rndval))
	{
		printf("#Collision found at %d.\n", count);
	}
	SetMark(rndval);
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
