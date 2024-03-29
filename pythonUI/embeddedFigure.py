from tkinter import * 
from matplotlib.figure import Figure
from matplotlib.backends.backend_tkagg import (FigureCanvasTkAgg, NavigationToolbar2Tk)
from tkinter import filedialog
from PIL import ImageTk,Image



class Root(Tk):
    def __init__(self):
        super().__init__()
        self.title("Embeded figures in window")
        self.minsize(640,400)
        # self.wm_iconbitmap('icon.ico')

        self.matplotCanvas()

    def matplotCanvas(self):
        f = Figure(figsize=(5,5), dpi=100)
        a = f.add_subplot(111)
        a.plot([1,2,3,4,5,6,7,8], [5,6,1,3,8,9,3,5])

        canvas = FigureCanvasTkAgg(f,self)
        # canvas.show()
        canvas.get_tk_widget().pack(side = BOTTOM, fill= BOTH, expand= True)

        toolbar = NavigationToolbar2Tk(canvas, self)
        canvas.get_tk_widget().pack(side = BOTTOM, fill= BOTH, expand= True)
        


        filename = filedialog.askopenfilename(title='open')
        print(filename)

        img_front = ImageTk.PhotoImage(Image.open(filename))
        panel_front = Label()
        panel_front.image = img_front
        panel_front.configure(image=img_front)
        panel_front.pack( side = "bottom", fill = "both", expand = "yes")


    




if __name__ == '__main__':
    root = Root()
    root.mainloop()