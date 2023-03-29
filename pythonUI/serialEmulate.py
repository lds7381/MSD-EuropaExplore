import serial
import math
import time 

print("***** \nserial emulator online \n*****")

ser = serial.Serial("COM5", 9600)

t_start = round(time.time(), 1)

read = []

while True:
    t = time.time() - t_start
    message = str(math.sin(t)) + "\n"
    ser.write(bytes(message,"utf"))

    # try:
    #     read = ser.read().decode('ascii')
    # except:
    #     pass

    # print(read)
    time.sleep(0.001)