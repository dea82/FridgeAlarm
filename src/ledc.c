/*
 * ledc.c
 *
 *  Created on: 19 feb 2015
 *      Author: andreas
 */

#include <avr/io.h>
#include <avr/pgmspace.h>

#include "conf.h"
#include "type.h"
#include "ledc.h"

void Ledc_init(void)
{
    CONF_IO(RED_LED_CFG, OUTPUT, 0);
    CONF_IO(GREEN_LED_CFG, OUTPUT, 0);
}

void Ledc_loop(void)
{
}

