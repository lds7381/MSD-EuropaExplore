from tkinter import * 
import serial
import re 
import numpy as np
from matplotlib import pyplot as plt
from matplotlib.figure import Figure
from matplotlib.backends.backend_tkagg import (FigureCanvasTkAgg, NavigationToolbar2Tk)
import csv 

# INIT
# start the interface elements 
root = Tk()
root.title = "Europa Explorer Interface"
root.resizable(0,0)
root.wm_attributes("-topmost", 1)

# start the csv writer 
# csv_writer = csv.writer() TODO

# Init data 
data_timestamps = [] # [seconds]
data_DO = [] # dissolved oxygen [mg/L]
data_SA = [] # salinity [ppm]
data_PH = [] # ph [log scale]
data_TE = [] # [deg C]

# Init serial interface 
# ser = serial.Serial('COM3', baudrate=9600, timeout=1) TODO

# ------------------------------------------------------------

# controls 
def create_control_frame(container):
    controlFrame = Frame(container,height=500,width=500)

    Button(controlFrame, text="rudder left",command=rudder_command,height=50,width=50).grid(column=0, row=1)
    Button(controlFrame, text="rudder right",command=rudder_command,height=50,width=50).grid(column=2, row=1)
    Button(controlFrame, text="move forwards",command=propeller_command,height=50,width=50).grid(column=1, row=0)
    Button(controlFrame, text="move backwards",command=propeller_command,height=50,width=50).grid(column=1, row=2)
    Button(controlFrame, text="STOP",command=stop,height=50,width=50).grid(column=1, row=1)
    for widget in controlFrame.winfo_children():
        widget.grid(padx=5, pady=5)
    # return controlFrame

# ------------------------------------------------------------

# captures data in csv and updates plot 
# continuously updating plots https://stackoverflow.com/questions/47970163/continuously-updating-graph-in-tkinter
# integrate plots with tk https://www.geeksforgeeks.org/how-to-embed-matplotlib-charts-in-tkinter-gui/
def create_plots_frame(container):
    plt.ion()
    new_line = 'ads\tfser' #.readline().decode('ascii')
    if new_line == '':
        pass
    else:   
        line_arr = new_line.split('\n')
        val_time,val_DO,val_SA,val_PH,val_TE = line_arr 
        data_timestamps.append(val_time)
        data_DO.append(val_DO)
        data_SA.append(val_SA)
        data_PH.append(val_PH)
        data_TE.append(val_TE)

        # csv_writer.writerow(line_arr) TODO

        plt.plot(x, y, 'r-')
        plt.show()
        plt.pause(0.0001)

    # TODO: bastardized version of two sources that doesn't work yet
    # creating the Tkinter canvas
    # containing the Matplotlib figure
    canvas = FigureCanvasTkAgg(fig, master = container)  
    canvas.draw()
    # placing the canvas on the Tkinter container
    canvas.get_tk_widget().pack()
    return root.after(100, create_plots_frame)

# ------------------------------------------------------------

def stop():
    return
def rudder_command():
    return
def propeller_command():
    return


# ------------------------------------------------------------
canvas = Canvas(root, width=500, height=400, bd=0, highlightthickness=0)
canvas.pack()

class control:
    def __init__(self, canvas):
        self.canvas = canvas
        self.id = canvas.b
        self.canvas.move(self.id, 245, 100)

        self.canvas.bind("<Button-1>", self.canvas_onclick1)
        self.canvas.bind("<Button-3>", self.canvas_onclick2)
        self.text_id = self.canvas.create_text(300, 200, anchor='se')
        self.canvas.itemconfig(self.text_id, text='hello')

        self.acc = 9.81
        self.vel = -5; 
        
    def canvas_onclick1(self, event):
        print(canvas.coords(self.id))
        self.vel = self.vel  - 10
    def canvas_onclick2(self, event):
        print(canvas.coords(self.id))
        self.vel = self.vel  + 10
    
    def draw(self):
        self.canvas.move(self.id, 0, round(self.vel,0))
        self.canvas.after(10, self.draw)
        self.vel = self.vel + self.acc * 0.01



create_control_frame(root)
# create_plots_frame(root)

root.mainloop()

# ------------------------------------------------------------
# root = Tk()
# s = serial.Serial('COM3')
# res = s.read().split('\t')
# print(res)


# label_DO = Label(root, text="DOVAL")
# label_PH = Label(root, text="PHVAL")
# label_SA = Label(root, text="SAVAL")
# label_TE = Label(root, text="TEVAL")

# analog gauges https://stackoverflow.com/questions/46789053/python3-tkinter-analog-gauge


# label = Label(root, fg="red")
# label.pack()
# counter_label(label)


# # https://stackoverflow.com/questions/29158220/tkinter-understanding-mainloop/
# while True:
#     root.update_idletasks()
#     root.update()
