#!/bin/bash
#The MIT License (MIT)
#
#Copyright (c) 2015-2016 Andreas L.
#
#Permission is hereby granted, free of charge, to any person obtaining a copy
#of this software and associated documentation files (the "Software"), to deal
#in the Software without restriction, including without limitation the rights
#to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#copies of the Software, and to permit persons to whom the Software is
#furnished to do so, subject to the following conditions:
#
#The above copyright notice and this permission notice shall be included in
#all copies or substantial portions of the Software.
#
#THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
#THE SOFTWARE.

# Parsing of input arguments -f (mcu frequency) and -b (desired baudrate)
while [[ $# -gt 1 ]]
do
key="$1"

case $key in
    -f|--f_cpu)
    F_CPU="$2"
    shift # past argument
    ;;
    -b|--baudrate)
    BAUDRATE="$2"
    shift # past argument
    ;;
    *)
            # unknown option
    ;;
esac
shift # past argument or value
done

# Number of clockcycles per bit on uart
CYCLES=$(((F_CPU + (BAUDRATE / 2)) / BAUDRATE))

# Actual baudrate achived (calculate backwards)
ACTUAL_BAUDRATE=$((2 * F_CPU / (2 * CYCLES - 1)))

# Baud rate error, integer part
BAUDRATE_ERROR=$((ACTUAL_BAUDRATE * 100 / BAUDRATE - 100))

# Baud rate error, decimal part
BAUDRATE_ERROR_TENTHS=$((ACTUAL_BAUDRATE * 1000 / BAUDRATE - 1000 - BAUDRATE_ERROR * 10))

# Printouts
echo Desired Baud Rate: "${BAUDRATE}" bps
echo Actual Baud Rate :  "${ACTUAL_BAUDRATE}" bps
echo ERROR: "${BAUDRATE_ERROR}"."${BAUDRATE_ERROR_TENTHS}"%
if [ "$BAUDRATE_ERROR" -gt "1" ]
then
    echo =======================================================
    echo WARNING! BAUD RATE ERROR ABOVE RECOMMENDED LIMIT OF 2%.
    echo =======================================================
fi
