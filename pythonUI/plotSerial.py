import time
import serial
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from tkinter import * 
from matplotlib.figure import Figure
from matplotlib.backends.backend_tkagg import (FigureCanvasTkAgg, NavigationToolbar2Tk)
from PIL import ImageTk,Image
from tkinter import filedialog

# Reference for protocol interpretation: https://docs.google.com/document/d/17hC0oAoF7pTGZNYeh3ka6t4bvLvzr4SShMjbHldEPO4/edit


# Tkinter class to hold everything 
class window():

    def __init__(self, container):
        super().__init__()
        container.title("Embeded figures in window")
        container.minsize(640,400)
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

        # something about widths IDK
        container.columnconfigure(0, weight=1)
        container.columnconfigure(1, weight=1)
        container.columnconfigure(2, weight=1)

        # create each frame and add them to the root container 
        data_frame = self.create_plotting_frame(container)
        data_frame.grid(column=0,row=0)
        print("plots are added")

        pics_frame = self.create_camera_frame(container)
        pics_frame.grid(column=1,row=0)
        print("camera pictures added")
        
        cont_frame = self.create_control_frame(container)
        cont_frame.grid(column=2,row=0)
        print("rover controls added")
        
        # Now that everything is started, run the capture and animate loop
        # TODO this only works with a single figure. Convert the array of figures to subplots
        ani = animation.FuncAnimation(fig, self.animate, frames=100, fargs=(dataList, ser), interval=100)





    def create_camera_frame(self, container):
        frame = Frame(container)
        filename = filedialog.askopenfilename(title='open')
        print(filename)
        img_front = ImageTk.PhotoImage(Image.open(filename))
        panel_front = Label(frame, image = img_front)
        panel_front.pack(side = "bottom", fill = "both", expand = "yes")

        return frame 
    
    def create_plotting_frame(self, container):
        frame = Frame(container)

        fig,axs = plt.subplots(4, 1)
        
        # plot test data in figures 
        axs[0].plot([1,2,3,4,5,6,7,8], [7,6,1,3,8,9,3,5])
        axs[1].plot([1,2,3,4,5,6,7,8], [5,6,1,3,8,9,3,5])
        axs[2].plot([1,2,3,4,5,6,7,8], [3,6,1,3,8,9,3,5])
        axs[3].plot([1,2,3,4,5,6,7,8], [1,6,1,3,8,9,3,5])

        # prepare frames for figure placement 
        frame = Frame(frame, width= 10, height= 10)

        # Tie the figures with the frames 
        canvas = FigureCanvasTkAgg(fig, master = frame)
        # canvas.show()
        
        # Place the frames in a grid 
        canvas.get_tk_widget().pack()

        return frame


    # continuously recieves serial and decides on display options 
    def animate(i, dataList, ser):
        ser.write(b'g')                                     # Transmit the char 'g' to receive the Arduino data point
        data_data = ser.read()
        data_string = data_data.decode('ascii') # Decode receive Arduino data as a formatted string
        print('{0}\t{1}'.format(i,data_string))      # 'i' is a incrementing variable based upon frames = x argument

        try:
            data_float = float(data_string)   # Convert to float
            dataList.append(data_float)              # Add to the list holding the fixed number of points to animate
        
        except:                                             # Pass if data point is bad                               
            pass
        data_ID = []
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
        ax.clear()  #                                        # Clear last data frame
        ax.plot(dataList)                                   # Plot new data frame
        
        ax.set_ylim([-5, 5])                              # Set Y axis limit of plot
        ax.set_title("Arduino Data")                        # Set title of figure
        ax.set_ylabel("Value")                              # Set title of y axis 
    
    def create_control_frame(self,container):
        controlFrame = Frame(container,height=500,width=500)
        
        def stop():
            return
        def rudder_command():
            return
        def propeller_command():
            return

        Button(controlFrame, text="rudder left",    command=rudder_command,     height=5,width=5).grid(column=0, row=1)
        Button(controlFrame, text="rudder right",   command=rudder_command,     height=5,width=5).grid(column=2, row=1)
        Button(controlFrame, text="move forwards",  command=propeller_command,  height=5,width=5).grid(column=1, row=0)
        Button(controlFrame, text="move backwards", command=propeller_command,  height=5,width=5).grid(column=1, row=2)
        Button(controlFrame, text="STOP",           command=stop,               height=5,width=5).grid(column=1, row=1)
        for widget in controlFrame.winfo_children():
            widget.grid(padx=5, pady=5)

        return controlFrame

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

print("***** \nplotter online \n*****")

dataList = []                                           # Create empty list variable for later use
                                                        
fig = plt.figure()                                      # Create Matplotlib plots fig is the 'higher level' plot window
ax = fig.add_subplot(111)                               # Add subplot to main fig window

ser = serial.Serial("COM4", 9600)                       # Establish Serial object with COM port and BAUD rate to match Arduino Port/rate
time.sleep(2)                                           # Time delay for Arduino Serial initialization 

                                                        # Matplotlib Animation Fuction that takes takes care of real time plot.
                                                        # Note that 'fargs' parameter is where we pass in our dataList and Serial object. 
print("test")

# ani = animation.FuncAnimation(fig, animate, frames=100, fargs=(dataList, ser), interval=100) 
# plt.show()                                              # Keep Matplotlib plot persistent on screen until it is closed

if __name__ == '__main__':
    root = Toplevel()
    window(root)
    root.mainloop()
    

ser.close()                                             # Close Serial connection when plot is closed

ser.in_waiting
Frame()