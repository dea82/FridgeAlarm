#include <avr/io.h>
#include <avr/wdt.h>

#include "conf.h"
#include "type.h"
#include "wdtc.h"

void Wdtc_SetTimer(const tWdtc_Timeout_E timeout_E, const tB interrupt_B)
{
    switch(timeout_E)
    {
        case WDTC_OFF_E:
            /* Timed sequence to turn WDT off. */
            WDTCR = _BV(WDCE) | _BV(WDE);
            WDTCR = _BV(WDCE);
            break;
        case WDTC_16MS_E:
            WDTCR = _BV(WDIF) | (_BV(WDIE) && interrupt_B) | _BV(WDE);
            break;
        case WDTC_8S_E:
            WDTCR = _BV(WDIF) | (_BV(WDIE) && interrupt_B) | _BV(WDE) | _BV(WDP3) | _BV(WDP0);
            break;
    }
}
