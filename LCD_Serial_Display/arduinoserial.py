import serial
import time

ser = serial.Serial(4, 9600)

print ser.name
time.sleep(.5)
timestamp = int(time.time())
messageStr = "T" + str(timestamp) + "000"
print messageStr
ser.write(messageStr)

ser.close()
