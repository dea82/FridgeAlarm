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

#define CALIBRATION_TIME_BUTTON 4000
#define DEEP_SLEEP_TIME 8000
#define ALARM_OPEN 3000

typedef enum
{
    NO_CALIBRATION_E, CALIBRATION_SUCCESS_E, CALIBRATION_FAIL_E
} tCalibrationState_E;

static tB deepSleepOk_B = FALSE;

void Cont_loop(void)
{
    static tCalibrationState_E calibrationState_E;
    static tB inhibitAlarm_B;
    static tU08 counter_U08;
    tDsen_doorState_str doorState_str = Dsen_getDoorState_str();

    /* Calibration */
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

    if (calibrationState_E == CALIBRATION_SUCCESS_E)
    {
        Buzz_setSound(BUZZ_OFF_E);
        Ledc_setState(LEDC_GREEN_BLINK_E);
    }
    else if (calibrationState_E == CALIBRATION_FAIL_E)
    {
        Ledc_setState(LEDC_RED_BLINK_E);
        Buzz_setSound(BUZZ_ON_E);
    }
    else
    {
        switch (doorState_str.doorState_E)
        {
        case DSEN_CLOSED_E:
            Buzz_setSound(BUZZ_OFF_E);
            if ((doorState_str.timeInState_U16 > DEEP_SLEEP_TIME / TICK)
                    && counter_U08 > 100)
            {
                deepSleepOk_B = TRUE;
                counter_U08 = 0;
                Ledc_setState(LEDC_OFF_E);
            }
            else
            {
                INC_U08(counter_U08);
                deepSleepOk_B = FALSE;
                Ledc_setState(LEDC_GREEN_E);
            }
            inhibitAlarm_B = FALSE;

            break;
        case DSEN_OPEN_E:
            counter_U08 = 0;
            deepSleepOk_B = FALSE;
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
                    Buzz_setSound(BUZZ_ON_E);
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
}

tB Cont_deepSleepOk_B(void)
{
    return deepSleepOk_B;
}
