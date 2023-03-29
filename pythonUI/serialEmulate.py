import serial
import math
import time 

ser = serial.Serial("COM4", 9600)

t_start = round(time.time(), 1)
while True:
    t = time.time() - t_start
    message = str(math.sin(t))
    # print((math.sin(t)))
    ser.write(bytes(message,"utf"))
    time.sleep(0.001)