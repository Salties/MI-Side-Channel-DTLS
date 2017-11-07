#!/usr/bin/python3

import serial
import random
import platform


def SwapEndian16(u8ary):
    i = 0
    while i < len(u8ary):
        u8ary[i], u8ary[i + 1] = u8ary[i + 1], u8ary[i]
        i += 2

    return u8ary


def PrintArm16Data(data, logfile=None):
    pdata = bytearray(data)
    rwdstr = SwapEndian16(pdata).hex().upper()

    # Insert spaces for between each 16-bits.
    dstr = ''
    i = 0
    while i in range(len(rwdstr)):
        dstr += rwdstr[i:i + 4] + ' '
        i += 4

    print(dstr)
    if logfile != None:
        logfile.write(dstr + '\r\n')
        logfile.flush()

    return


def main():
    # Detect running OS.
    os = platform.system()
    print('#OS: {}'.format(os))

    if platform.system() == 'Linux':
        # Initialisation for Linux.
        ser = serial.Serial('/dev/ttyUSB0')
    elif platform.system() == 'Windows':
        # Initialisation for Windows.
        ser = serial.Serial('COM1')
    else:
        print('Unsupported OS.')
        exit(-1)

    random.seed()

    # plaintext = SwapEndian16(bytearray(0x0123456789ABCDEF.to_bytes(8, byteorder='big'))) # Initial plaintext.
    logfd = open('pt.dat', 'w+')

    # Start reading ciphertext.
    while True:
        plaintext = bytearray(random.getrandbits(64).to_bytes(8, 'big'))
        # Send plaintext to the device.
        #print('P:', end='')
        PrintArm16Data(plaintext, logfd)
        ser.write(plaintext)

        # Read out ciphertext.
        ciphertext = bytearray(ser.read(8))
        #print('C:', end='')
        # PrintArm16Data(ciphertext)

        # Update plaintext for the next round.
        #plaintext = SwapEndian16(ciphertext)
        plaintext = ciphertext

    ser.close()
    logfd.close()

    return


if __name__ == '__main__':
    main()
