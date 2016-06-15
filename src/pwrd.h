/*
The MIT License (MIT)

Copyright (c) 2015-2016 Andreas L.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

/*------------------------------------------------------------------------*//**
 * 
 * @file pwrd.h
 * @author     Andreas L.
 * @date       2016-06-03
 * 
 * @brief      Power down module
 * 
 * This module takes care of power down of the MCU.
 *----------------------------------------------------------------------------*/

#ifndef PWRD_H_
#define PWRD_H_

typedef enum
{
    PWRD_SHORT_DEEP_SLEEP_E,
    PWRD_LONG_DEEP_SLEEP_E,
    PWRD_SLEEP_WITH_TIMER_RUNNING_E,
    PWRD_INFINITE_SLEEP_E
} tPwrd_sleepMode_E;

void Pwrd_sleep(void);

void Pwrd_wakeup(void);

void Pwrd_setSleepMode(const tPwrd_sleepMode_E mode_E);

#endif /* PWRD_H_ */
