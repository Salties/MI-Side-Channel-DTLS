#ifdef CONTIKI_TARGET_CC2538DK

#include "contiki.h"
#include "sys/rtimer.h"
#include "lib/random.h"
#include "dev/serial-line.h"
#include "lpm.h"
#include "dev/leds.h"

#include "dev/rfcore.h"
#include "dev/cc2538-rf.h"
#include "dev/soc-adc.h"
#include "dev/sys-ctrl.h"
#include "reg.h"
#include "net/netstack.h"

#include <stdio.h>              /* For printf() */
#include <stdlib.h>
/*---------------------------------------------------------------------------*/
PROCESS(prng_test_process, "PRNG test");
AUTOSTART_PROCESSES(&prng_test_process);
/*---------------------------------------------------------------------------*/

#define VERBOSE 1
#define WAIT_TIME ((CLOCK_SECOND * 0.1))
#define SEEDLEN ((16))        //Seed length in bytes.

static struct etimer et;

unsigned short ReadRnd()
{
    return REG(SOC_ADC_RNDL) | (REG(SOC_ADC_RNDH) << 8);
}

void printseed(unsigned char* seedval, size_t seedlen)
{
	size_t i;

	if(seedlen <= 0)
		return;

	for(i = 0; i < seedlen; i++)
		printf("%02X", seedval[i]);
	return;
}

void RfRnd(unsigned char *seed, size_t seedlen)
 {
    int i = 0, j = 0;
    unsigned char s = 0;
    int overheated = 0;
#if VERBOSE
    int rssi = 0, freqest = 0;
#endif

    memset(seed, 0, seedlen);

    /* Make sure the RNG is on */
    REG(SOC_ADC_ADCCON1) &=
        ~(SOC_ADC_ADCCON1_RCTRL1 | SOC_ADC_ADCCON1_RCTRL0);

    /* Enable clock for the RF Core */
    REG(SYS_CTRL_RCGCRFC) = 1;

    /* Wait for the clock ungating to take effect */
    while (REG(SYS_CTRL_RCGCRFC) != 1);

    /* Infinite RX - FRMCTRL0[3:2] = 10
     *    * This will mess with radio operation - see note above */
    REG(RFCORE_XREG_FRMCTRL0) = 0x00000008;

    /* Turn RF on */
    CC2538_RF_CSP_ISRXON();
#if 0
    //This is something evil...
    //Turn on PD_OVERRIDE
    REG(RFCORE_XREG_PTEST1) = 0x08;
    //Turn off LNA and mixer.
    REG(RFCORE_XREG_PTEST0) = 0x04;
#endif

    /*
     *    * Wait until "the chip has been in RX long enough for the transients to
     *       * have died out. A convenient way to do this is to wait for the RSSI-valid
     *          * signal to go high."
     *             */
    while (!(REG(RFCORE_XREG_RSSISTAT) & RFCORE_XREG_RSSISTAT_RSSI_VALID));
     /*
     *    * Form the seed by concatenating bits from IF_ADC in the RF receive path.
     *       * Keep sampling until we have read at least 16 bits AND the seed is valid
     *          *
     *             * Invalid seeds are 0x0000 and 0x8003 and should not be used.
     *                */
    for (i = 0; i < seedlen; i++) {
	//Read 8 bits from RFRND.
        for (j = 0; j < 8; j++) {
            if (!((REG(RFCORE_XREG_RSSISTAT) & RFCORE_XREG_RSSISTAT_RSSI_VALID))) 
	    {
		//RF core is invalidated. Stop reading.
		printf("#RFRND invalidated.\n");
                overheated = 1;
                break;
            }
            s <<= 1;
            s |= (REG(RFCORE_XREG_RFRND) & RFCORE_XREG_RFRND_IRND);
        }
        if (overheated)
	{
            break;
	}
        seed[i] = s;
	s = 0;
    }

#if VERBOSE
    //Read out RSSI and FREQEST.
    if(128 <= (rssi = REG(RFCORE_XREG_RSSI)))
	    rssi -= 256;
    if(128 <= (freqest = REG(RFCORE_XREG_FREQEST)))
	    freqest -= 256;
    //Print bits read, RSSI and frequency offset.
    printf("#(NBIT,RSSI,FREQEST) %d %d %d\n", i * 8, rssi - 73, (freqest * 7800 - 15000) / 1000 );
#else
    printf("#(NBIT) %d\n", i * 8);
#endif

    /* RF Off. NETSTACK_RADIO.init() will sort out normal RF operation */
    CC2538_RF_CSP_ISRFOFF();

    printseed(seed, i);
    printf("\n");

    return;
}

PROCESS_THREAD(prng_test_process, ev, data)
{
    static unsigned char seed[SEEDLEN];

    PROCESS_BEGIN();

    lpm_set_max_pm(LPM_PM0);

    etimer_set(&et, WAIT_TIME);
    for (;;) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
        etimer_restart(&et);
        leds_on(LEDS_ALL);
        RfRnd(seed, sizeof(seed));
        leds_off(LEDS_ALL);
    }

    PROCESS_END();
}

/*---------------------------------------------------------------------------*/
#endif
