import tkinter as tk
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib.figure import Figure
import numpy as np
import serial
import time
from PIL import Image, ImageTk

class Interface(tk.Frame):
    def __init__(self, master=None):
        super().__init__(master)
        self.master = master
        self.pack()
        self.create_widgets()

    def create_widgets(self):
        # Create the two pictures
        image1 = Image.open("images\\th-21406574.jpg")
        image1 = image1.resize((200, 200), Image.ANTIALIAS)
        photo1 = ImageTk.PhotoImage(image1)
        image2 = Image.open("images\\space_shark_by_bojustbo_da5h40c-fullview-1388306213.jpg")
        image2 = image2.resize((200, 200), Image.ANTIALIAS)
        photo2 = ImageTk.PhotoImage(image2)

        # Create the labels for the pictures and pack them into the interface
        label1 = tk.Label(self, image=photo1)
        label1.image = photo1
        label1.pack(side=tk.LEFT, padx=5, pady=5)
        label2 = tk.Label(self, image=photo2)
        label2.image = photo2
        label2.pack(side=tk.LEFT, padx=5, pady=5)

        # Create the four pyplot figures
        fig1 = Figure(figsize=(5, 4), dpi=100)
        ax1 = fig1.add_subplot(111)
        fig2 = Figure(figsize=(5, 4), dpi=100)
        ax2 = fig2.add_subplot(111)
        fig3 = Figure(figsize=(5, 4), dpi=100)
        ax3 = fig3.add_subplot(111)
        fig4 = Figure(figsize=(5, 4), dpi=100)
        ax4 = fig4.add_subplot(111)

        # Create the canvas for the pyplot figures and pack them into the interface
        canvas1 = FigureCanvasTkAgg(fig1, master=self)
        canvas1.get_tk_widget().pack(side=tk.LEFT, padx=5, pady=5)
        canvas2 = FigureCanvasTkAgg(fig2, master=self)
        canvas2.get_tk_widget().pack(side=tk.LEFT, padx=5, pady=5)
        canvas3 = FigureCanvasTkAgg(fig3, master=self)
        canvas3.get_tk_widget().pack(side=tk.LEFT, padx=5, pady=5)
        canvas4 = FigureCanvasTkAgg(fig4, master=self)
        canvas4.get_tk_widget().pack(side=tk.LEFT, padx=5, pady=5)

        # Open the serial port
        ser = serial.Serial('COM3', 115200)
        ser.flushInput()
        # 

        # Define the update function for the animation
        def update(i):
            # Read the data from the serial port
            data = ser.readline().decode().strip()
            # Split the data into four values
            values = data.split(",")
            if len(values) == 4:
                # Update the four subplots with the new data
                ax1.plot(float(values[0]))
                ax2.plot(float(values[1]))
                ax3.plot(float(values[2]))
                ax4.plot(float(values[3]))

        # Create the animation object and start the animation
        anim1 = animation.FuncAnimation(fig1, update, interval=1)
        anim2 = animation.FuncAnimation(fig2, update, interval=1)
        anim3 = animation.FuncAnimation(fig3, update, interval=1)
        anim4 = animation.FuncAnimation(fig4, update, interval=1)

        # Start the mainloop
        self.master.mainloop()

        # Close the serial port when the program is terminated
        ser.close()
# root = tk.Tk()
# interface = Interface(root)


from tkinter import *
from matplotlib.figure import Figure
from matplotlib.backends.backend_tkagg import (FigureCanvasTkAgg, NavigationToolbar2Tk)


class plotFrames:
    def __init__(self,container) -> None:
        self.container = container
        self.ser = serial.Serial('COM3')
        self.initFigs
        self.serialRead

    def initFigs(self):
        self.figDO = Figure(figsize=(5, 4), dpi=100)
        self.figTE = Figure(figsize=(5, 4), dpi=100)
        self.figSA = Figure(figsize=(5, 4), dpi=100)
        self.figPH = Figure(figsize=(5, 4), dpi=100)
        self.container.grid()
        ax1 = fig1.add_subplot(111)
        
    def serialRead(self):
        res = s.read().split('\t')

        self.container.after(100,self.serialRead)

    # plot function is created for
    # plotting the graph in
    # tkinter window
    def plot(self,dataID):
        # the figure that will contain the plot
        fig = Figure(figsize = (5, 5), dpi = 100)

        # list of squares
        y = [i**2 for i in range(101)]

        # adding the subplot
        plot1 = fig.add_subplot(111)

        # plotting the graph
        plot1.plot(y)

        # creating the Tkinter canvas
        # containing the Matplotlib figure
        canvas = FigureCanvasTkAgg(fig,	master = self.container)
        canvas.draw()

        # placing the canvas on the Tkinter window
        canvas.get_tk_widget().pack()
     

# the main Tkinter window
window = Tk()

# dimensions of the main window
window.geometry("500x500")

# run the gui
while True:
    plotFrames(window)

    window.update_idletasks()
    window.update()

