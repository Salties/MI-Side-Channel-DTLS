#!/usr/bin/python3

import serial

def SwapEndian16(u8ary):
    i = 0
    while i < len(u8ary):
        u8ary[i], u8ary[i+1] = u8ary[i+1], u8ary[i]
        i += 2

    return u8ary


def main():
    ser = serial.Serial('/dev/ttyUSB0')
    plaintext = SwapEndian16(bytes.fromhex('0123456789ABCDEF')) # Initial plaintext.

    # Wait until the device is ready.
    while True:
        line = ser.readline().decode("utf-8")
        if line != "#DEVICE READY.\n":
            print("#Invalid:"+line)
        else:
            print("#DEVICE READY")
            break

    # Start reading ciphertext.
    while True:
        #ser.write(plaintext)
        line = ser.read(8)
        for i in range(8):
            print('{:02X}'.format(line[i]), end=''),
        print('')
        plaintext = SwapEndian16(line)

    return


if __name__ =='__main__':
    main()
