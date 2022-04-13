from tkinter import *
from Low import runCode
from Medium import runCode2
from High import runCode3
from PIL import Image, ImageTk
import lidar_lite

root = Tk()
root.wm_title("GUI")
root.configure(bg="#E5E5E5")

screen_width = root.winfo_screenwidth()
screen_height = root.winfo_screenheight()

root.attributes("-fullscreen", True)

def LowRes():
    runCode()
    
def btnExit():
        root.destroy()
        
def end_fullscreen(event):
        root.attributes("-fullscreen", False) 
        
def MedRes():
    runCode2()
    
def HighRes():
    runCode3()

load= Image.open("jayhawk.png")
load = load.resize((round(screen_width*0.12),round(screen_width*0.12)), Image.ANTIALIAS)
render = ImageTk.PhotoImage(load)
img1 = Label(root, image=render, bg="#E5E5E5")

load2= Image.open("MATCimage.jpg")
load2 = load2.resize((round(screen_width*0.2025),round(screen_width*0.108)), Image.ANTIALIAS)
render2 = ImageTk.PhotoImage(load2)
img2 = Label(root, image=render2)



label_1 = Label(root, text="KU Mechanical Engineering\n 3rd Generation Lidar System", font="Verdana 20 bold",
                        fg="#000",
                        bg="#E5E5E5",
                        pady = round(screen_width*0.01),
                        padx = round(screen_height*0.01), justify='center')

lowButton = Button(root, text="Low Accuracy\n \n Smallest amount of data points taken\n (2700 total data points)\n \n Scan Time ~7 minutes", background = "#1E90FF",
                command=LowRes, height = round(screen_height*0.015), width=round(screen_width*0.022), font = "Arial 12 bold", justify='center', wraplength=180)

mediumButton = Button(root, text="Medium Accuracy\n \n Four times as many data points as the Low Accuracy option\n \n Scan Time ~11 minutes", background = "#1E90FF",
               command=MedRes, height = round(screen_height*0.015), width=round(screen_width*0.022), font = "Arial 12 bold", justify='center', wraplength=180)

highButton = Button(root, text="High Accuracy\n \n Eight times as many data points as the Low Accuracy option\n \n Scan Time ~23 minutes", background = "#1E90FF",
                command=HighRes, height = round(screen_height*0.015), width=round(screen_width*0.022), font = "Arial 12 bold", justify='center', wraplength=180)

exitButton = Button(root, text="Exit", background = "#DC143C",
                command=btnExit, height = round(screen_height*0.0125), width=round(screen_width*0.022), font = "Arial 12 bold", justify='center', wraplength=300)


img1.place(x=round(screen_width*0.07), y=round(screen_height*0.02))
img2.place(x=round(screen_width*0.78), y=round(screen_height*0.03))
label_1.place(x=round(screen_width*0.20), y=round(screen_height*0.03))
lowButton.place(x=round(screen_width*0.074), y=round(screen_height*0.33))
mediumButton.place(x=round(screen_width*0.384), y=round(screen_height*0.33))
highButton.place(x=round(screen_width*0.694), y=round(screen_height*0.33))
exitButton.place(x=round(screen_width*0.384), y=round(screen_height*0.7))


root.bind("<Escape>", end_fullscreen)
root.mainloop()


