import time
import serial
import matplotlib.pyplot as plt
import numpy as np
import matplotlib.animation as animation
from tkinter import * 
from matplotlib.figure import Figure
from matplotlib.backends.backend_tkagg import (FigureCanvasTkAgg, NavigationToolbar2Tk)
from PIL import ImageTk,Image
from tkinter import filedialog

# Reference for protocol interpretation: https://docs.google.com/document/d/17hC0oAoF7pTGZNYeh3ka6t4bvLvzr4SShMjbHldEPO4/edit


def resizeImage(img, newWidth, newHeight):
    oldWidth = img.width()
    oldHeight = img.height()
    newPhotoImage = PhotoImage(width=newWidth, height=newHeight)
    for x in range(newWidth):
        for y in range(newHeight):
            xOld = int(x*oldWidth/newWidth)
            yOld = int(y*oldHeight/newHeight)
            rgb = '#%02x%02x%02x' % img.get(xOld, yOld)
            newPhotoImage.put(rgb, (x, y))
    return newPhotoImage


# Tkinter class to hold everything 
class window():

    def __init__(self, container):
        super().__init__()
        container.title("Embeded figures in window")
        container.minsize(640,600)
        # self.wm_iconbitmap('icon.ico')

        # Prepare data for capture 
        self.time_DO = [] # time [s] 
        self.time_SA = [] # time [s] 
        self.time_PH = [] # time [s] 
        self.time_TE = [] # time [s] 
        self.data_DO = [] # dissolved oxygen [mg/L] 
        self.data_SA = [] # salinity [ppm] 
        self.data_PH = [] # ph [log scale] 
        self.data_TE = [] # [deg C] 

        # configure column widths
        container.columnconfigure(0, weight=1)
        container.columnconfigure(1, weight=1)
        container.columnconfigure(2, weight=1)

        # User selects folders for storage 
        self.data_folder = filedialog.askopenfilename(title='folder for sensor data')
        self.images_folder = filedialog.askopenfilename(title='folder for camera images')

        # create each frame and add them to the root container 
        data_frame = self.create_plotting_frame(container)
        data_frame.grid(column=0,row=0)
        print("plots are added")
        cont_frame = self.create_middle_frame(container)
        cont_frame.grid(column=1,row=0)
        print("rover controls added")
        


    def create_middle_frame(self, container):
        frame = Frame(container)

        filename_bottom = filedialog.askopenfilename(title='open')
        print(filename_bottom)
        img_front = ImageTk.PhotoImage(Image.open(filename_bottom).resize(size=[300,200]))
        panel_front = Label(frame)
        panel_front.image = img_front
        panel_front.configure(image=img_front)
        panel_front.pack( fill = "both", expand = "yes")
        Label(frame,text="Front Camera").pack(pady=10)

        filename_front = filedialog.askopenfilename(title='open')
        print(filename_front)
        img_bottom = ImageTk.PhotoImage(Image.open(filename_front).resize(size=[300,200]))
        panel_bottom = Label(frame)
        panel_bottom.image = img_bottom
        panel_bottom.configure(image=img_bottom)
        panel_bottom.pack( fill = "both", expand = "yes")
        Label(frame,text="Bottom Camera").pack(pady=10)

        def stop():
            pass
        def rudder_command(sig):
            ser.write(sig)
        def propeller_command(sig):
            ser.write(sig)

        controlFrame = Frame(frame)
        Button(controlFrame, text="rudder \nleft",    command=rudder_command(0b0100),     height=5,width=10).grid(column=0, row=1)
        Button(controlFrame, text="rudder \nright",   command=rudder_command(0b1000),     height=5,width=10).grid(column=2, row=1)
        Button(controlFrame, text="forwards",  command=propeller_command(0b0001),  height=5,width=10).grid(column=1, row=0)
        Button(controlFrame, text="backwards", command=propeller_command(0b0010),  height=5,width=10).grid(column=1, row=2)
        Button(controlFrame, text="STOP",      command=stop,               height=5,width=10).grid(column=1, row=1)
        for widget in controlFrame.winfo_children():
            widget.grid(padx=5, pady=5)

        controlFrame.pack(side="bottom",pady=40)
        return frame 
    

    def create_plotting_frame(self, container):
        self.sensor_fig,self.sensor_axs = plt.subplots(4, 1)
        self.sensorPlotUpdate(self.sensor_axs)

        self.sensor_fig.set_figheight(10)
        self.sensorDataUpdate("")
        # prepare frames for figure placement 
        frame = Frame(container, width= 100, height= 800)
        # Tie the figures with the frames 
        canvas = FigureCanvasTkAgg(self.sensor_fig, master = frame)
        # Place the figures in the container
        canvas.get_tk_widget().pack()
        return frame

    def sensorDataUpdate(self,dataString:str):
        timestamp,DO,SA,PH,TE = dataString.split(",")
        self.time_DO.pop(0)
        self.time_SA.pop(0)
        self.time_PH.pop(0)
        self.time_TE.pop(0)
        self.data_DO.pop(0)
        self.data_SA.pop(0)
        self.data_PH.pop(0) 
        self.data_TE.pop(0)
        self.time_DO.append(timestamp)
        self.time_SA.append(timestamp)
        self.time_PH.append(timestamp)
        self.time_TE.append(timestamp)
        self.data_DO.append(DO)
        self.data_SA.append(SA)
        self.data_PH.append(PH) 
        self.data_TE.append(TE)

    def sensorPlotUpdate(self,axs):
        # plot test data in figures 
        axs[0].plot(self.time_DO, self.data_DO)
        axs[1].plot(self.time_SA, self.data_SA)
        axs[2].plot(self.time_PH, self.data_PH)
        axs[3].plot(self.time_TE, self.data_TE)

        # Axes labels 
        axs[0].set_ylabel("Dissolved Oxygen [g/mL]")
        axs[1].set_ylabel("Salinity [ppt]")
        axs[2].set_ylabel("pH [-]")
        axs[3].set_ylabel("Temperature [Celcius]")
        # axs[0].set_xlabel("Time [s]")
        # axs[1].set_xlabel("Time [s]")
        # axs[2].set_xlabel("Time [s]")
        axs[3].set_xlabel("Time [s]")

        # y limit hard bounds 
        axs[0].set_ylim(0,20)
        axs[1].set_ylim(0,25)
        axs[2].set_ylim(1,12)
        axs[3].set_ylim(0,100)

        # Label last point 
        axs[0].annotate(text=self.data_DO[-1] ,xy=(self.time_DO[-1],self.data_DO[-1]),textcoords='data')
        axs[1].annotate(text=self.data_SA[-1] ,xy=(self.time_SA[-1],self.data_SA[-1]),textcoords='data')
        axs[2].annotate(text=self.data_PH[-1] ,xy=(self.time_PH[-1],self.data_PH[-1]),textcoords='data')
        axs[3].annotate(text=self.data_TE[-1] ,xy=(self.time_TE[-1],self.data_TE[-1]),textcoords='data')
        


    # continuously recieves serial and decides on display options 
    def animate(i, dataList, ser):
        data_data = ser.readline()
        data_string = data_data.decode('ascii') # Decode receive Arduino data as a formatted string

        data_ID = data_data[0:2]
        print(data_ID)

        if(data_ID == 0b000):
            pass #TODO add all the ID cases  
        elif(data_ID == 0b001):
            pass
        elif(data_ID == 0b010):
            pass
        elif(data_ID == 0b100):
            pass
        
        dataList = dataList[-50:]                           # Fix the list size so that the animation plot 'window' is x number of points
        # TODO flesh out the general case to work with each type of ID 
        self.axs_sensors.clear()  #                                        # Clear last data frame
        ax.plot(dataList)                                   # Plot new data frame

    # def create_control_frame(self,container):
    #     controlFrame = Frame(container,height=500,width=500)    
    #     def stop():
    #         return
    #     def rudder_command():
    #         return
    #     def propeller_command():
    #         return
    #     Button(controlFrame, text="rudder \nleft",    command=rudder_command,     height=5,width=10).grid(column=0, row=1)
    #     Button(controlFrame, text="rudder \nright",   command=rudder_command,     height=5,width=10).grid(column=2, row=1)
    #     Button(controlFrame, text="forwards",  command=propeller_command,  height=5,width=10).grid(column=1, row=0)
    #     Button(controlFrame, text="backwards", command=propeller_command,  height=5,width=10).grid(column=1, row=2)
    #     Button(controlFrame, text="STOP",           command=stop,               height=5,width=10).grid(column=1, row=1)
    #     for widget in controlFrame.winfo_children():
    #         widget.grid(padx=5, pady=5)
    #     return controlFrame

# ----------------------------------------------------------------------------------------------------------------------
# The serial readline function is slow and read only picks out a single character. We need a solution 

# https://github.com/pyserial/pyserial/issues/216#issuecomment-369414522
class ReadLine:
    def __init__(self, s):
        self.buf = bytearray()
        self.s = s
    
    def readline(self):
        i = self.buf.find(b"\n")
        if i >= 0:
            r = self.buf[:i+1]
            self.buf = self.buf[i+1:]
            return r
        while True:
            i = max(1, min(2048, self.s.in_waiting))
            data = self.s.read(i)
            i = data.find(b"\n")
            if i >= 0:
                r = self.buf + data[:i+1]
                self.buf[0:] = data[i+1:]
                return r
            else:
                self.buf.extend(data)

# ----------------------------------------------------------------------------------------------------------------------



ser = serial.Serial("COM4", 209700)                       # Establish Serial object with COM port and BAUD rate to match Arduino Port/rate


# 209700 bps  baud

if __name__ == '__main__':
    root = Toplevel()
    window(root)

    # Now that everything is started, run the capture and animate loop
    # TODO this only works with a single figure. Convert the array of figures to subplots
    ani = animation.FuncAnimation(fig, self.animate, frames=100, fargs=(dataList, ser), interval=100)

    root.mainloop()
    
