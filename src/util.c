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

#include "util.h"

#include <inttypes.h>

void Util_safeIncrementU08(tU08 * const val_pU08)
{
    if (*val_pU08 != UINT8_MAX)
    {
        (*val_pU08)++;
    }
}

void Util_safeIncrementU16(tU16 * const val_pU16)
{
    if (*val_pU16 != UINT16_MAX)
    {
        (*val_pU16)++;
    }
}

tU08 Util_loByteU16_U08(const tU16 val_U16)
{
  return (tU08)(val_U16 & (tU16)(0x00FF));
}

tU08 Util_hiByteU16_U08(const tU16 val_U16)
{
  return (tU08)(val_U16 >> 8u);
}
