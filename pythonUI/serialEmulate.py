import serial
import math
import time 

print("***** \nserial emulator online \n*****")

ser = serial.Serial("COM5", 209700)

t_start = round(time.time(), 1)

read = []

while True:
    t = time.time() - t_start
    message = "{},{},{},{},{}".format(t,math.sin(t),math.cos(t),-math.sin(t),-math.cos(t)) 
    ser.write(bytes(message,"utf"))

    # try:
    #     read = ser.readline() #.decode('ascii')
    # except:
    #     pass
    # print(read)
    
    time.sleep(.5)