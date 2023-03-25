from tkinter import * 
import serial

s = serial.Serial('COM7')
res = s.read()``
print(res)



root = Tk()
label = Label(root, fg="red")
label.pack()
counter_label(label)
root.mainloop()
