import serial

val = b''

ser = serial.Serial("/dev/tty.usbmodem14101", 9600)
while val != b'>':
    val = ser.read()
    print(val)


ser.write(b'u')
ser.write(b'\n')

val = ser.read()
while val != b'>':
    val = ser.read()
    print(val)

ser.write(0x01)
ser.write(0x55)
ser.write(0x02)
ser.write(0x55)
ser.write(0x1F)

val = ser.read()
while val != b'F':
    val = ser.read()
    print(val)

close(ser)
