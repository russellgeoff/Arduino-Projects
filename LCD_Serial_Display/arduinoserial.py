import serial
import time

ser = serial.Serial(4, 9600)

print ser.name

while ("I got a time!" not in ser.readline()):
    timestamp = int(time.time())
    messageStr = "T" + str(timestamp) + "000"
    print messageStr
    ser.write(messageStr)
    time.sleep(1)
    # output = ser.readline()
    # print(output)
    # print("I got a time!" not in output)

ser.close()
