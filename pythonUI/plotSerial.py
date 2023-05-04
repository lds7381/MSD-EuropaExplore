import datetime
import serial
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from tkinter import * 
from matplotlib.figure import Figure
from matplotlib.backends.backend_tkagg import (FigureCanvasTkAgg, NavigationToolbar2Tk)
from PIL import ImageTk,Image
from tkinter import filedialog
import glob, os
import csv
import argparse

# Reference for protocol interpretation: https://docs.google.com/document/d/17hC0oAoF7pTGZNYeh3ka6t4bvLvzr4SShMjbHldEPO4/edit


# ----------------------------------------------------------------------------------------------------------------------
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


# ----------------------------------------------------------------------------------------------------------------------
# ----------------------------------------------------------------------------------------------------------------------
# Tkinter class to hold everything 
class window():

    def __init__(self, container, csvFileName):
        super().__init__()
        container.title("Europa Explorer Interface")
        w = 1200
        h = 900
        # get screen width and height
        ws = root.winfo_screenwidth() # width of the screen
        hs = root.winfo_screenheight() # height of the screen
        # calculate x and y coordinates for the Tk root window
        x = (ws/2) - (w/2)
        y = (hs/2) - (h/2)
        container.minsize(w,h)
        container.geometry('%dx%d+%d+%d' % (w, h, x,y))
        # self.wm_iconbitmap('icon.ico')

        # Prepare data for capture 
        self.time_DO = [0] # time [s] 
        self.time_SA = [0] # time [s] 
        self.time_PH = [0] # time [s] 
        self.time_TE = [0] # time [s] 
        self.data_DO = [0] # dissolved oxygen [mg/L] 
        self.data_SA = [0] # salinity [ppm] 
        self.data_PH = [0] # ph [log scale] 
        self.data_TE = [0] # [deg C] 

        # configure column widths
        container.columnconfigure(0, weight=1)
        container.columnconfigure(1, weight=1)
        container.columnconfigure(2, weight=1)

        # User selects folders for storage 
        self.data_folder = filedialog.askdirectory(title='folder for sensor data')
        self.images_folder = filedialog.askdirectory(title='folder for camera images')
        self.csv_file_location = self.images_folder +"\\" + csvFileName
        print("Data is stored here: ", self.csv_file_location)

        # create each frame and add them to the root container 
        data_frame = self.create_plotting_frame(container)
        data_frame.pack(side=LEFT, anchor=N)
        print("plots are added")
        cont_frame = self.create_control_frame(container)
        cont_frame.pack(side=RIGHT, anchor=N)
        print("rover controls added")
        imgs_frame = self.create_middle_frame(container)
        imgs_frame.pack(side=RIGHT, anchor=N)
        print("Images added")

        # Initial servo position
        self.servo_pos = 2
        self.dir = -1 

        print("Window init complete \n")
    
    def create_middle_frame(self, container):
        frame = Frame(container)
        filename_bottom = "D:/Documents/_RIT/MSD-EuropaExplore/pythonUI/images/space_shark_by_bojustbo_da5h40c-fullview-1388306213.jpg" # filedialog.askopenfilename(title='open')
        print("Loaded bottom image: " , filename_bottom)
        img_front = ImageTk.PhotoImage(Image.open(filename_bottom).resize(size=[640,480]))
        self.panel_front = Label(frame)
        self.panel_front.image = img_front
        self.panel_front.configure(image=img_front)
        self.panel_front.pack( fill = "both", expand = "yes")
        Label(frame,text="Front Camera").pack(pady=10)
        filename_front = "D:/Documents/_RIT/MSD-EuropaExplore/pythonUI/images/th-21406574.jpg" # filedialog.askopenfilename(title='open')
        print("Loaded front image: " , filename_front)
        img_bottom = ImageTk.PhotoImage(Image.open(filename_front).resize(size=[640,480]))
        self.panel_bottom = Label(frame)
        self.panel_bottom.image = img_bottom
        self.panel_bottom.configure(image=img_bottom)
        self.panel_bottom.pack( fill = "both", expand = "yes")
        Label(frame,text="Bottom Camera").pack(pady=10)
        return frame 

    def create_control_frame(self,container):
        controlFrame = Frame(container)
        Button(controlFrame, text="rudder \nleft",  command=lambda:self.servo_command( 1),          height=5,width=10).grid(column=0, row=1)
        Button(controlFrame, text="rudder \nright", command=lambda:self.servo_command(-1),          height=5,width=10).grid(column=2, row=1)
        Button(controlFrame, text="forwards",       command=lambda:self.send_command("f",b"w"),     height=5,width=10).grid(column=1, row=0)
        Button(controlFrame, text="backwards",      command=lambda:self.send_command("b",b"s"),     height=5,width=10).grid(column=1, row=2)
        Button(controlFrame, text="STOP",           command=lambda:self.send_command("s",b"x"),     height=5,width=10).grid(column=1, row=1)
        for widget in controlFrame.winfo_children():
            widget.grid(padx=5, pady=5)
        return controlFrame
    
    def servo_command(self,direction):
        self.servo_pos = max(1, min(5, self.servo_pos + direction))
        ser.write(str(self.servo_pos).encode('UTF-8'))
        print("Sent command: ", "servo", self.servo_pos)

    def send_command(self,cmd,sig):
        ser.write(sig)
        print("Sent command: ", cmd, sig)

    def create_plotting_frame(self, container):
        self.csvfile = open(self.csv_file_location, 'w')

        self.sensor_fig = Figure(dpi=100)
        self.sensor_axs = self.sensor_fig.subplots(4, 1)
        self.sensorPlotUpdate(self.sensor_axs)
        self.sensor_fig.set_figheight(8)
        self.sensor_fig.tight_layout()
        self.sensor_fig.subplots_adjust(top=0.95)
        self.sensorDataUpdate("0,0,0,0")
        # prepare frames for figure placement 
        frame = Frame(container, width= 100, height= 600)
        # Tie the figures with the frames 
        canvas = FigureCanvasTkAgg(self.sensor_fig, master = frame)
        # Place the figures in the container
        canvas.get_tk_widget().pack(side=TOP,anchor=NW)
        return frame

    def sensorDataUpdate(self,dataString:str):
        try:
            convertedInts = dataString.split(",")
            PH,SA,DO,TE = [float(x) for x in convertedInts]
        except:
            return
        self.csvfile.writelines(dataString + '\n')
        # with open(self.csv_file_location, 'w') as csvfile:
        #     spamwriter = csv.writer(csvfile, delimiter=',', quotechar='"', quoting=csv.QUOTE_MINIMAL)
        #     spamwriter.writerow(dataString.split(","))

        # self.time_DO.append(timestamp)
        # self.time_SA.append(timestamp)
        # self.time_PH.append(timestamp)
        # self.time_TE.append(timestamp)
        self.data_DO.append(DO)
        self.data_SA.append(SA)
        self.data_PH.append(PH) 
        self.data_TE.append(TE)
        # self.time_DO = self.time_DO[-20:]
        # self.time_SA = self.time_SA[-20:]
        # self.time_PH = self.time_PH[-20:]
        # self.time_TE = self.time_TE[-20:]
        self.data_DO = self.data_DO[-20:]
        self.data_SA = self.data_SA[-20:]
        self.data_PH = self.data_PH[-20:] 
        self.data_TE = self.data_TE[-20:]

    def sensorPlotUpdate(self,axs):
        axs[0].clear()
        axs[1].clear()
        axs[2].clear()
        axs[3].clear()

        # print(axs[0])
        # print(axs[1])
        # print(axs[2])
        # print(axs[3])
        
        # axs[0].set_xdata(self.time_DO)
        # axs[1].set_xdata(self.time_SA)
        # axs[2].set_xdata(self.time_PH)
        # axs[3].set_xdata(self.time_TE)
        # axs[0].set_ydata(self.data_DO)
        # axs[1].set_ydata(self.data_SA)
        # axs[2].set_ydata(self.data_PH)
        # axs[3].set_ydata(self.data_TE)
        
        # axs[0].plot(self.time_DO,self.data_DO)
        # axs[1].plot(self.time_DO,self.data_SA)
        # axs[2].plot(self.time_DO,self.data_PH)
        # axs[3].plot(self.time_DO,self.data_TE)
        axs[0].grid(True)
        axs[1].grid(True)
        axs[2].grid(True)
        axs[3].grid(True)
        # Axes labels 
        axs[0].set_ylabel("Dissolved Oxygen [g/mL]")
        axs[1].set_ylabel("Salinity [ppt]")
        axs[2].set_ylabel("pH [-]")
        axs[3].set_ylabel("Temperature [Fahrenheit]")
        # axs[0].set_xlabel("Time [s]")
        # axs[1].set_xlabel("Time [s]")
        # axs[2].set_xlabel("Time [s]") # removed to not hog space
        axs[3].set_xlabel("Time [s]")
        # plot test data in figures 
        axs[0].plot(self.data_DO)
        axs[1].plot(self.data_SA)
        axs[2].plot(self.data_PH)
        axs[3].plot(self.data_TE)
        self.sensor_axs[1].plot()
        # y limit hard bounds 
        axs[0].set_ylim([0,20])
        axs[1].set_ylim([0,25])
        axs[2].set_ylim([1,12])
        axs[3].set_ylim([0,100])
        # Label last point 
        axs[0].annotate(text=self.data_DO[-1] ,xy=(self.time_DO[-1],self.data_DO[-1]),textcoords='data')
        axs[1].annotate(text=self.data_SA[-1] ,xy=(self.time_SA[-1],self.data_SA[-1]),textcoords='data')
        axs[2].annotate(text=self.data_PH[-1] ,xy=(self.time_PH[-1],self.data_PH[-1]),textcoords='data')
        axs[3].annotate(text=self.data_TE[-1] ,xy=(self.time_TE[-1],self.data_TE[-1]),textcoords='data')

        plt.show()
    def update_photo(self):
        list_of_files = glob.glob(self.images_folder+"\\*.jpg")
        latest_file =  max(list_of_files, key=os.path.getctime).replace("/", "\\") 
        try:
            if latest_file == self.last_photo:
                return
        except:
            pass
        print("New image: ", latest_file)
        im = Image.open(latest_file).resize(size=[640,480])
        ph = ImageTk.PhotoImage(im)
        self.panel_bottom.configure(image=ph)
        self.panel_bottom.image = ph
        self.last_photo = latest_file

    # continuously recieves serial and decides on display options 
    def animate(self,i, ser):
        try:
            data_data = ser.read_all()
            data_string = data_data.decode('ascii').replace("\r\n\x00","") #  receive data as a formatted string
        except Exception as e:
            print("error: ", e)
            pass  
        if data_string.count("Instruction Received")>0:
            print(data_string )
        if data_string.count(',')==3 :
            print("Instruction Received: ", data_string)
            self.sensorDataUpdate(data_string)
            self.sensorPlotUpdate(axs=self.sensor_axs)  # Plot new data framew
        self.update_photo()
        
        # # The following code is designed to sweep the rudder back and forth during imagineRIT:
        # if self.servo_pos >4:
        #     self.dir = -1
        # if self.servo_pos<=1:
        #     self.dir = 1
        # self.servo_pos = max(1, min(5, self.servo_pos + self.dir))
        # ser.write(str(self.servo_pos).encode('UTF-8'))
        # print("Sent command: ", "servo", self.servo_pos)

# ----------------------------------------------------------------------------------------------------------------------
# Code starts running here 


# 209700 bps  baud

if __name__ == '__main__':

    parser = argparse.ArgumentParser()
    parser.add_argument("-c", "--com", type=str, help='Pass the com port (default: COM4)')
    (args) = parser.parse_args()
    com = args.com 
    if com==None: com = "COM4"
    print("Using com port: ", com)

    root = Tk()
    csvFileName = datetime.datetime.now().strftime('%Y_%m_%d_%H%M%S.csv')
    win = window(root,csvFileName)
    print("window created")
    # Now that everything is started, run the capture and animate loop
    # TODO this only works with a single figure. Convert the array of figures to subplots
    # p = Process(target=animation.FuncAnimation, args=(win.sensor_fig, win.animate, 100, (ser,), 25))
    # p.start()
        
    try:
        ser = serial.Serial(com, 209700)         # Establish Serial object with COM port and BAUD rate to match Port/rate
        ani = animation.FuncAnimation(win.sensor_fig, win.animate, frames=100, fargs=(ser,), interval=50)
    except Exception as e:
        print("error: ", e)
        pass

    print("Entering Mainloop")
    root.mainloop()
    # p.join()
    
