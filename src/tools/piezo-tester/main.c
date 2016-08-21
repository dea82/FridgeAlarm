#include <avr/io.h>

/* Declaring main as OS_main saves some register pushing to stack. */
int main(void) __attribute__((OS_main));
int main(void)
{
    for(;;)
    {
        PORTB = 1;
    }
}
