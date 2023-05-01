import serial
import math
import time 

'''Make sure com0com is installed and the emulated send recieve ports are correct'''

print("***** \nserial emulator online \n*****")

ser = serial.Serial("COM5", 209700)

t_start = round(time.time(), 1)

read = []

while True:
    t = time.time() - t_start
    message = "{},{},{},{},{}".format(t,math.sin(t)+5,math.cos(t)+5,-math.sin(t)+4,-math.cos(t)+20) 
    ser.write(bytes(message,"utf"))
    print(message)
    
    # try:
    #     read = ser.readline() #.decode('ascii')
    # except:
    #     pass
    # print(read)
    
    time.sleep(1)