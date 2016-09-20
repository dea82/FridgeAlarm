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

#ifndef CPUL_H_
#define CPUL_H_

#include "type.h"

typedef struct
{
    char name_aC[4]; /**< Module name */
    tU08 prescaler_U08;  /**< Prescaler */
    tU08 cycles_U08;  /**< Number of cycles */
    tU08 crc_U08;  /**< CRC */
} tCpul_ResultBlock_str;
#if defined(__AVR_ATtiny85__)
/**
 * @brief      Set start point for cpu load measurement
 *
 * @param  prescalerRegister_U08  Prescaler register
 */
void Cpul_StartPoint(tU08 prescalerRegister_U08);
tU08 Cpul_StopPoint_U08(void);
#endif
tCpul_ResultBlock_str Cpul_CreateResultBlock_str(const char name_c[], const tU08 cycles_U08, const tU08 prescaler_U08);

#endif /* CPUL_H_ */
