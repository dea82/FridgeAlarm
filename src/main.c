/*
 * main.c
 *
 *  Created on: 18 feb 2015
 *      Author: andreas
 */

#include <avr/interrupt.h>
#include <avr/power.h>
#include <avr/sleep.h>

#include "buzz.h"
#include "dswm.h"
#include "ledc.h"

typedef enum
{
    DOOR_OPEN_E, ALARM_E, DOOR_CLOSED_E, SLEEP_E
} tMainState_E;

typedef struct
{
    tMainState_E state_E;
    unsigned int timeInState_U16;
} tMainState_str;

static tMainState_str mainState_str;

static void initTimer(void);
static void startTimer(void);
static void stopTimer(void);
static void powerDown(void);
static void initWakeUpInterrupt(void);
static void enableWakeUpInterrupt(void);
static void disableWakeUpInterrupt(void);

/* Wake-up routine */
void INT0_vect(void)
{
    mainState_str.state_E = DOOR_OPEN_E;
    mainState_str.timeInState_U16 = 0;
    disableWakeUpInterrupt();
    startTimer();
}

/* Called every tick 0.1 s */
void TIM0_COMPA_vect(void)
{
    /* Make sure door state is updated first.*/
    Dswm_loop();
    tDoorState_E doorState_E = Dswm_getDoorState();

    switch (mainState_str.state_E)
    {
    case DOOR_OPEN_E:
        if (doorState_E == CLOSED_CONFIRMED_E)
        {
            mainState_str.state_E = DOOR_CLOSED_E;
            mainState_str.timeInState_U16 = 0;
        }
        else if (doorState_E == OPEN_CONFIRMED_E)
        {
            mainState_str.timeInState_U16++;
            if (mainState_str.timeInState_U16 >= 300)
            {
                mainState_str.state_E = ALARM_E;
                mainState_str.timeInState_U16 = 0;
            }
        }
        break;
    case ALARM_E:
        if (doorState_E == CLOSED_CONFIRMED_E)
        {
            mainState_str.state_E = DOOR_CLOSED_E;
            mainState_str.timeInState_U16 = 0;
        }
        break;
    case DOOR_CLOSED_E:
        if (doorState_E == OPEN_CONFIRMED_E)
        {
            mainState_str.state_E = DOOR_OPEN_E;
            mainState_str.timeInState_U16 = 0;
        }
        else if (doorState_E == CLOSED_CONFIRMED_E)
        {
            mainState_str.timeInState_U16++;
            if (mainState_str.timeInState_U16 >= 50)
            {
                mainState_str.state_E = SLEEP_E;
                mainState_str.timeInState_U16 = 0;
            }
        }
        break;
    default:
        break;
    }
}

int main(void)
{
    Buzz_init();
    Dswm_init();
    Ledc_init();

    initTimer();
    initWakeUpInterrupt();
    power_adc_disable();
    sei();

    for (;;)
    {
        if (mainState_str.state_E == SLEEP_E)
        {
            powerDown();
        }
    }
    return 0;
}

static void initTimer(void)
{
    TCCR0A = (1 << WGM01); //CTC
    TCCR0B = (1 << CS02); //Pre-scaler 256
    OCR0A = 249;
    TIMSK0 = (1 << OCIE0A); //interrupt
}

static void startTimer(void)
{
    TCCR0B = (1 << CS02); //Pre-scaler 256
}

static void stopTimer(void)
{
    TCCR0B = 0; //stop timer
    TCNT0 = 0; // reset counter
}

static void powerDown(void)
{
    stopTimer();
    enableWakeUpInterrupt();
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_mode();
}

static void initWakeUpInterrupt(void)
{
    /* Nothing */
}

static void enableWakeUpInterrupt(void)
{
    GIMSK = (1 << INT0);
}

static void disableWakeUpInterrupt(void)
{
    GIMSK = 0;
}
