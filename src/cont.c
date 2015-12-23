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
#include "type.h"

typedef enum
{
    NO_CALIBRATION_E, CALIBRATION_SUCCESS_E, CALIBRATION_FAIL_E
} tCalibrationState_E;

static tSleepMode_E sleepMode_E = CONT_SHORT_DEEP_SLEEP_E;

inline void Cont_init(void)
{
    /* Nothing */
}

inline void Cont_loop(void)
{
#if 1
    static tCalibrationState_E calibrationState_E;
    static tB inhibitAlarm_B;
    static tU08 counter_U08;
    tDsen_doorState_str doorState_str = Dsen_getDoorState_str();

    /* Calibration state */
    if ((Butt_getState_str().state_E == BUTT_PRESSED_E
            && Butt_getState_str().tickInState_U08 * TICK
                    > CALIBRATION_TIME_BUTTON)
            && calibrationState_E == NO_CALIBRATION_E)
    {
        if (Dsen_storeClosedPos())
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
        if (Butt_getState_str().state_E == BUTT_RELEASED_E)
        {
            calibrationState_E = NO_CALIBRATION_E;
        }

    }

    sleepMode_E = CONT_SHORT_DEEP_SLEEP_E;

    if (calibrationState_E == CALIBRATION_SUCCESS_E)
    {
        Buzz_setSound(BUZZ_OFF_E);
        Ledc_setState(LEDC_GREEN_BLINK_E);

    }
    else if (calibrationState_E == CALIBRATION_FAIL_E)
    {
        sleepMode_E = CONT_SLEEP_WITH_TIMER_RUNNING_E;
        Ledc_setState(LEDC_RED_BLINK_E);
        Buzz_setSound(BUZZ_ON_E);
    }
    else
    {
        switch (doorState_str.doorState_E)
        {
        case DSEN_CLOSED_E:
            Buzz_setSound(BUZZ_OFF_E);
            if ((doorState_str.timeInState_U16 > LIGHTS_ON_DOOR_CLOSED / TICK)
                    && counter_U08 > MIN_TIME_AWAKE / TICK
                    && Butt_getState_str().state_E == BUTT_RELEASED_E)
            {
                counter_U08 = 0;
                Ledc_setState(LEDC_OFF_E);
                sleepMode_E = CONT_LONG_DEEP_SLEEP_E;
            }
            else
            {
                INC_U08(counter_U08);
                Ledc_setState(LEDC_GREEN_E);
            }
            inhibitAlarm_B = FALSE;

            break;
        case DSEN_OPEN_E:
            counter_U08 = 0;

            if (doorState_str.timeInState_U16 > ALARM_OPEN / TICK)
            {
                if (Butt_getState_str().state_E == BUTT_PRESSED_E)
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
                    sleepMode_E = CONT_SLEEP_WITH_TIMER_RUNNING_E;
                }
                Ledc_setState(LEDC_RED_BLINK_E);
            }
            else
            {
                Ledc_setState(LEDC_RED_E);
                Buzz_setSound(BUZZ_OFF_E);
            }
            break;
        }
    }
#endif
}

tSleepMode_E Cont_sleepMode_E(void)
{
    return sleepMode_E;
}
