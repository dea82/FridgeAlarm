/*
 * dswm.c
 *
 *  Created on: 20 feb 2015
 *      Author: andreas
 */

#include <avr/io.h>

#include "conf.h"
#include "dswm.h"

#define DOOR_OPEN   (DSWM_PIN & DSWM_OPEN_BIT)
#define DOOR_CLOSED (DSWM_PIN & DSWM_CLOSED_BIT)

/* Initalize switches */
void Dswm_init(void)
{
    DSWM_PORT |= (1 << DSWM_OPEN_BIT | 1 << DSWM_CLOSED_BIT);
}

/* Global door state */
static tDoorState_str doorState_str;

void Dswm_loop(void)
{
    if ((DOOR_OPEN && DOOR_CLOSED) || (!DOOR_OPEN && !DOOR_CLOSED))
    {
        doorState_str.state_E = UNKNOWN_E;
        doorState_str.timeInState_U16 = 0;
    }
    else
    {
        switch (doorState_str.state_E)
        {
        case OPEN_VALIDATING_E:
            /* Door open */
            if (DOOR_OPEN)
            {
                doorState_str.timeInState_U16++;
                if (doorState_str.timeInState_U16 >= 10)
                {
                    doorState_str.state_E = OPEN_CONFIRMED_E;
                    doorState_str.timeInState_U16 = 0;
                }
            }
            else if (DOOR_CLOSED)
            {
                doorState_str.state_E = CLOSED_VALIDATING_E;
                doorState_str.timeInState_U16 = 0;
            }
            break;
        case OPEN_CONFIRMED_E:
            if (DOOR_CLOSED)
            {
                doorState_str.state_E = CLOSED_VALIDATING_E;
                doorState_str.timeInState_U16 = 0;
            }
            break;
        case CLOSED_VALIDATING_E:
            if (DOOR_OPEN)
            {
                doorState_str.state_E = OPEN_VALIDATING_E;
                doorState_str.timeInState_U16 = 0;
            }
            else if (DOOR_CLOSED)
            {
                doorState_str.timeInState_U16++;
                if (doorState_str.timeInState_U16 >= 10)
                {
                    doorState_str.state_E = OPEN_CONFIRMED_E;
                    doorState_str.timeInState_U16 = 0;
                }
            }
            break;
        case CLOSED_CONFIRMED_E:
            if (DOOR_OPEN)
            {
                doorState_str.state_E = OPEN_VALIDATING_E;
                doorState_str.timeInState_U16 = 0;
            }
            break;

        default:
            /* Catches unknown state */
            if (DOOR_OPEN)
            {
                doorState_str.state_E = OPEN_VALIDATING_E;
            }
            else
            {
                doorState_str.state_E = CLOSED_VALIDATING_E;
            }
            doorState_str.timeInState_U16 = 0;
            break;
        }

    }
}

tDoorState_E Dswm_getDoorState(void)
{
    return doorState_str.state_E;
}
