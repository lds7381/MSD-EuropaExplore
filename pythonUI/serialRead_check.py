#---------Imports
from numpy import arange, sin, pi
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib.figure import Figure
import tkinter as Tk
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import serial

import ReadLine
#---------End of imports

print("***** \nserial reader output: \n*****")

ser = serial.Serial("COM4", 209700)
readlineer = ReadLine.ReadLine(ser)

try:
    # print("\ncollect data")
    # data_data = readlineer.readline() 
    data_data = ser.read_all() # .decode('ascii')
    if data_data != b'':
        print(data_data)
        pass
except Exception as e:
    print("error: ", e)
    pass

fig = plt.Figure()

x = np.arange(0, 2*np.pi, 0.01)        # x-array

time = [0]
data_DO = [0]
data_SA = [0]
data_PH = [0]
data_TE = [0]

def animate(i):
    print("recieving serial ...." , i)
    data = ser.read_all()
    if data != b'':
        dataString = data.decode('ascii')
        print(dataString)
        timestamp,DO,SA,PH,TE = dataString.split(",")
        print(timestamp,DO,SA,PH,TE)

        # UnboundLocalError: local variable 'time' referenced before assignment
        time = time.append(timestamp)
        data_DO = data_DO.append(DO)
        data_SA = data_SA.append(SA)
        data_PH = data_PH.append(PH) 
        data_TE = data_TE.append(TE)
        time = time[-50:]
        data_DO = data_DO[-50:]
        data_SA = data_SA[-50:]
        data_PH = data_PH[-50:] 
        data_TE = data_TE[-50:]

        print(data_DO)
        line.set_ydata(data_DO)  # update the data
        return line,

# Reference: https://stackoverflow.com/questions/21197728/embedding-a-matplotlib-animation-into-a-tkinter-frame
root = Tk.Tk()

label = Tk.Label(root,text="Simulation").grid(column=0, row=0)

canvas = FigureCanvasTkAgg(fig, master=root)
canvas.get_tk_widget().grid(column=0,row=1)

ax = fig.add_subplot(111)
line, = ax.plot(x, np.sin(x))
ani = animation.FuncAnimation(fig, animate, np.arange(1, 200), interval=25, blit=False)

Tk.mainloop()