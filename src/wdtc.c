#include <avr/io.h>
#include <avr/wdt.h>

#include "wdtc.h"

void Wdtc_SetWatchdog(const tWdtc_Timeout_E timeout_E)
{
    wdt_reset();

    switch (timeout_E)
    {
    case WDTC_OFF_E:
        WDTCR = 0;
        break;
    case WDTC_16MS_E:
        WDTCR = _BV(WDIF) | (_BV(WDIE) | _BV(WDCE));
        break;
    case WDTC_8S_E:
        WDTCR = _BV(WDIF) | (_BV(WDIE) | _BV(WDCE) | _BV(WDP3) | _BV(WDP0));
        break;
    default:
        break;
    }
}
