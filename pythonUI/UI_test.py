import tkinter as tk
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib.figure import Figure
import numpy as np
from PIL import Image, ImageTk
import pygame

class Interface(tk.Frame):
    def __init__(self, master=None):
        super().__init__(master)
        self.master = master
        self.pack()
        self.create_widgets()

    def create_widgets(self):
        # # Create the joystick
        # pygame.init()
        # pygame.joystick.init()
        # self.joystick = pygame.joystick.Joystick(0)
        # self.joystick.init()

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
        canvas1.draw()
        canvas1.get_tk_widget().pack(side=tk.LEFT, padx=5, pady=5)
        canvas2 = FigureCanvasTkAgg(fig2, master=self)
        canvas2.draw()
        canvas2.get_tk_widget().pack(side=tk.LEFT, padx=5, pady=5)
        canvas3 = FigureCanvasTkAgg(fig3, master=self)
        canvas3.draw()
        canvas3.get_tk_widget().pack(side=tk.LEFT, padx=5, pady=5)
        canvas4 = FigureCanvasTkAgg(fig4, master=self)
        canvas4.draw()
        canvas4.get_tk_widget().pack(side=tk.LEFT, padx=5, pady=5)


        # Create the two pictures
        image1 = Image.open("images\\th-21406574.jpg")
        image1 = image1.resize((200, 200) , Image.ANTIALIAS)
        photo1 = ImageTk.PhotoImage(image1)
        image2 = Image.open("images\\space_shark_by_bojustbo_da5h40c-fullview-1388306213.jpg")
        image2 = image2.resize((200, 200) , Image.ANTIALIAS)
        photo2 = ImageTk.PhotoImage(image2)

        # Create the labels for the pictures and pack them into the interface
        label1 = tk.Label(self, image=photo1)
        label1.image = photo1
        label1.pack(side=tk.LEFT, padx=5, pady=5)
        label2 = tk.Label(self, image=photo2)
        label2.image = photo2
        label2.pack(side=tk.LEFT, padx=5, pady=5)

        # Start the continuous updating of the figures
        self.update_figures(ax1, ax2, ax3, ax4)

    def update_figures(self, ax1, ax2, ax3, ax4):
        # Update the figures here
        x = np.linspace(0, 10, 1000)
        y1 = np.sin(x)
        y2 = np.cos(x)
        y3 = np.tan(x)
        y4 = np.exp(x)
        ax1.clear()
        ax1.plot(x, y1)
        ax2.clear()
        ax2.plot(x, y2)
        ax3.clear()
        ax3.plot(x, y3)
        ax4.clear()
        ax4.plot(x, y4)
        # self.after(100,self.update_figures(ax1, ax2, ax3, ax4))

root = tk.Tk()
interface = Interface(root)

while True:
    root.update_idletasks()
    root.update()