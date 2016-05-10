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

#include <stdio.h>              /* For printf() */
#include <stdlib.h>
/*---------------------------------------------------------------------------*/
PROCESS(prng_test_process, "PRNG test");
AUTOSTART_PROCESSES(&prng_test_process);
/*---------------------------------------------------------------------------*/

static struct etimer et;

unsigned short ReadRnd()
{
    return REG(SOC_ADC_RNDL) | (REG(SOC_ADC_RNDH) << 8);
}

unsigned long long RfRnd()
{
    int i;
    unsigned long long s = 0;

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
    for (i = 0; i < sizeof(s) * 8; i++) {
        s <<= 1;
        s |= (REG(RFCORE_XREG_RFRND) & RFCORE_XREG_RFRND_IRND);
    }

    /* RF Off. NETSTACK_RADIO.init() will sort out normal RF operation */
    CC2538_RF_CSP_ISRFOFF();
    return s;
}

PROCESS_THREAD(prng_test_process, ev, data)
{
    PROCESS_BEGIN();

    lpm_set_max_pm(LPM_PM0);

    etimer_set(&et, CLOCK_SECOND / 4);
    for (;;) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
        etimer_restart(&et);
        leds_on(LEDS_ALL);
        random_init(0);
        leds_off(LEDS_ALL);
        //printf("%04X\n", ReadRnd());
        printf("%016llX\n", RfRnd());
    }

    PROCESS_END();
}

/*---------------------------------------------------------------------------*/
#endif
