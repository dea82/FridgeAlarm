#!/usr/bin/python

import serial
import time

# configure the serial connections (the parameters differs on the device you are connecting to)
ser = serial.Serial(
    port='/dev/tty.usbserial-A90JRLTH',
    baudrate=57600,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS
)

resultBlockSize = 7

ser.isOpen()

def main():
    print "Syncronizing..."
    while 1 :
        time.sleep(1)
        out = ''
        while ser.inWaiting() > 0:
            out += ser.read(1)

        if out != '':
            print ">>" + out

if __name__ == "__main__":
    main()
