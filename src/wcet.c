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
#if defined(__AVR_ATtiny85__)
#include <avr/pgmspace.h>

#include "wcet.h"
#include "uart.h"

static tU08 crc8_U08 (const tU08 inCrc_U08, const  tU08 inData_U08);

FORCE_INLINE void Wcet_StopMeasurement(void)
{
    TCCR1 = 0;
}

void Wcet_OutputResult(const char * taskName_c, const tU08 prescalerRegister_U08)
{
    tU08 cycles_U08;

    if (TIFR & _BV(TOV1))
    {
        cycles_U08 = 0xFF;
    }
    else
    {
        cycles_U08 = TCNT1;
    }

    tWcet_ResultBlock_str resultBlock_str =
        Wcet_CreateResultBlock_str(taskName_c, cycles_U08, prescalerRegister_U08);
    Uart_TransmitBlock((tU08*)&resultBlock_str, sizeof(resultBlock_str));
}

tWcet_ResultBlock_str Wcet_CreateResultBlock_str(const char * name_c, const tU08 cycles_U08, const tU08 prescalerRegister_U08)
{
  tWcet_ResultBlock_str resultBlock_str;
  strcpy_P(resultBlock_str.name_aC, (PGM_P)name_c);
  resultBlock_str.cycles_U08 = cycles_U08;
  resultBlock_str.prescaler_U08 = prescalerRegister_U08;

  /* Temporary pointer */
  tU08 * data_pU08 = (tU08*)&resultBlock_str;

  for (tU08 byte_U08 = sizeof(tWcet_ResultBlock_str); byte_U08 > sizeof(((tWcet_ResultBlock_str*)0)->crc_U08); byte_U08--)
  {
    resultBlock_str.crc_U08 = crc8_U08(resultBlock_str.crc_U08, *data_pU08++);
  }

  return resultBlock_str;
}

static tU08 crc8_U08 (const tU08 inCrc_U08, const tU08 inData_U08)
{
    tU08 i_U08;
    tU08 data_U08;

    data_U08 = inCrc_U08 ^ inData_U08;

    for ( i_U08 = 0; i_U08 < 8; i_U08++ )
    {
        if (( data_U08 & 0x80 ) != 0 )
        {
            data_U08 <<= 1;
            data_U08 ^= 0x07;
        }
        else
        {
            data_U08 <<= 1;
        }
    }
    return data_U08;
}
#endif
