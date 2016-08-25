#include <avr/io.h>
#include <util/delay.h>

/* Declaring main as OS_main saves some register pushing to stack. */
int main(void) __attribute__((OS_main));
int main(void)
{
    
#if F_CPU == 9600000
    OCR0A = 43;
#else
#error "Correct compare value to CPU speed."
#endif

DDRB = _BV(PB0);

    for(;;)
    {
        /* Set to 'CTC' mode, toggle on match */
        TCCR0A = _BV(COM0A0) | _BV(WGM01);
        /* Start clock - prescaler 64 */
        TCCR0B = _BV(CS01) | _BV(CS00);
        _delay_ms(1000);

        TCCR0B = 0;
        TCCR0A = 0;
        _delay_ms(1000);
    }
}
