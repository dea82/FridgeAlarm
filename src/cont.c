/*
 * cont.c
 *
 *  Created on: 31 maj 2015
 *      Author: andreas
 */

#include "butt.h"
#include "buzz.h"
#include "conf.h"
#include "cont.h"
#include "ledc.h"
#include "dsen.h"
#include "pwrd.h"
#include "type.h"

typedef enum
{
    NO_CALIBRATION_E, CALIBRATION_SUCCESS_E, CALIBRATION_FAIL_E
} tCalibrationState_E;

static tCalibrationState_E getCalibrationState_E(void);
static void normalModeStateMachine(void);

inline void Cont_init(void)
{
    /* Nothing */
}

inline void Cont_loop(void)
{
    tCalibrationState_E calibrationState_E = getCalibrationState_E();

    /* Sets it default. */
    Pwrd_setSleepMode(PWRD_SHORT_DEEP_SLEEP_E);

    /* Check if there has been other then power on reset, i.e BOD reset */
    if (!(MCUSR & _BV(PORF)))
    {
        /* Make sure to clear it,
         * otherwise we will not be able to reset with button */
        MCUSR = 0;
        Buzz_setSound(BUZZ_OFF_E);
        Ledc_setState(LEDC_ORANGE_E);
        Pwrd_setSleepMode(PWRD_INFINITE_SLEEP_E);
    }
    else if (calibrationState_E == CALIBRATION_SUCCESS_E)
    {
        Buzz_setSound(BUZZ_OFF_E);
        Ledc_setState(LEDC_GREEN_BLINK_E);
    }
    else if (calibrationState_E == CALIBRATION_FAIL_E)
    {
        Buzz_setSound(BUZZ_ON_E);
        Ledc_setState(LEDC_RED_BLINK_E);
        Pwrd_setSleepMode(PWRD_SLEEP_WITH_TIMER_RUNNING_E);
    }
    else
    {
        normalModeStateMachine();
    }
}

static tCalibrationState_E getCalibrationState_E(void)
{
    static tCalibrationState_E calibrationState_E;
    tButt_State_str buttState_str = Butt_getState_str();

    /* Calibration state */
    if ((buttState_str.state_E == BUTT_PRESSED_E
            && buttState_str.tickInState_U08 * TICK
                    > CALIBRATION_TIME_BUTTON)
            && calibrationState_E == NO_CALIBRATION_E)
    {
        /* Enters calibration mode */
        if (Dsen_storeClosedPos_B() == TRUE)
        {
            calibrationState_E = CALIBRATION_SUCCESS_E;
        }
        else
        {
            calibrationState_E = CALIBRATION_FAIL_E;
        }
    }
    else
    {
        /* Leaves calibration mode */
        if (buttState_str.state_E == BUTT_RELEASED_E)
        {
            calibrationState_E = NO_CALIBRATION_E;
        }
    }

    return calibrationState_E;
}

static void normalModeStateMachine(void)
{
    static tB inhibitAlarm_B;
    static tU08 counter_U08;
    tButt_State_str buttState_str = Butt_getState_str();
    tDsen_doorState_str doorState_str = Dsen_getDoorState_str();

    if (doorState_str.doorState_E == DSEN_CLOSED_E)
    {
        Buzz_setSound(BUZZ_OFF_E);
        if ((doorState_str.timeInState_U16 > LIGHTS_ON_DOOR_CLOSED / TICK)
            && (counter_U08 > MIN_TIME_AWAKE / TICK)
            && (buttState_str.state_E == BUTT_RELEASED_E))
        {
            /* When door has been closed for a long time and no activity
             * on button it's OK to go to deep sleep. */
            counter_U08 = 0;
            Ledc_setState(LEDC_OFF_E);
            Pwrd_setSleepMode(PWRD_LONG_DEEP_SLEEP_E);
        }
        else
        {
            /* Door has not been closed long enough to go to deep sleep. */
            INC_U08(counter_U08);
            Ledc_setState(LEDC_GREEN_E);
        }
        /* Reset */
        inhibitAlarm_B = FALSE;

    }
    else /* DSEN_OPEN_E */
    {
        counter_U08 = 0;

        if (doorState_str.timeInState_U16 > ALARM_OPEN / TICK)
        {
            /* Door has been open too long, sound the alarm if button has not inhibit it from going off. */
            if (buttState_str.state_E == BUTT_PRESSED_E)
            {
                inhibitAlarm_B = TRUE;
            }
            if (inhibitAlarm_B == TRUE)
            {
                Buzz_setSound(BUZZ_OFF_E);
            }
            else
            {
                Buzz_setSound(BUZZ_ALARM_E);
                /* Necessary to have the timer running due to the buzzer. */
                Pwrd_setSleepMode(PWRD_SLEEP_WITH_TIMER_RUNNING_E);
            }
            Ledc_setState(LEDC_RED_BLINK_E);
        }
        else
        {
            /* Door open but not long enough to sound the alarm. */
            Ledc_setState(LEDC_RED_E);
            Buzz_setSound(BUZZ_OFF_E);
        }
    }
}
