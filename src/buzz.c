/*
 * buzz.c
 *
 *  Created on: 18 feb 2015
 *      Author: andreas
 */

#include <avr/io.h>

void Buzz_init(void)
{
    DDRB |= DDB4;
}

void Buzz_loop(void)
{

}
