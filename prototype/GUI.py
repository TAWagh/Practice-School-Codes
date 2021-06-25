import serial
from tkinter import *
import tkinter.font
from PIL import ImageTk, Image

port = '/dev/ttyACM0'
ser = serial.Serial(port, baudrate = 9600, timeout = 1)

global data

def change_to_prep():
    #This function swaps from the order frame to the prep frame.
    prep_frame.pack(fill='both', expand=1)
    order_frame.forget()
    status.config(text='Your order is being processed')


def change_to_order():
  	#This function swaps from the order frame to the prep frame.
    order_frame.pack(fill='both', expand=1)
    prep_frame.forget()


def placeOrder():
    if topp.get() == 1:
      ser.write(b'1')
    if topp.get() == 0:
      ser.write(b'0')
    change_to_prep()


def updateStatus():
	if ser.inWaiting()>0:
		data = ser.readline().decode('utf-8').rstrip()
		status.config(text=data)
		if data == "end":
			change_to_order()
	status.after(1000, updateStatus)


# creating tkinter window 
root = Tk() 
root.title('Wafflr: Food Automtion System')
root.attributes('-fullscreen',True)


# creating frames (pages)
order_frame = Frame(root, bg='#F1CAA0', pady='100')
prep_frame = Frame(root,  bg='#F1CAA0')


# order frame.
title= Label(order_frame, text="Welcome to Wafflr",font='Georgia 45 bold',bg='#F1CAA0')
title.pack()


image = Image.open("images/im1.png")
img = ImageTk.PhotoImage(image)
im_label = Label(order_frame, image=img,bg='#F1CAA0')
im_label.pack()


text= Label(order_frame, text="Please place your order", font='Georgia 28', bg='#F1CAA0')
text.pack(pady=10)


topp = IntVar()
topping_checkbox = Checkbutton(order_frame, text = "Add Toppings", variable =topp, highlightthickness=0, bd=0, font='Georgia 14',bg='#F1CAA0')
topping_checkbox.pack(pady=10)


btn_On = Button(order_frame, text="Place Order", height = 2, width = 10, command=placeOrder , font='Georgia 20 bold',  bg='#290e04', fg='#F1CAA0')
btn_On.pack(pady=10)


# prep frame.
bg=PhotoImage( file = "images/im2.png")
frame_bg = Label(prep_frame, image=bg, highlightthickness=0, bd=0)
frame_bg.place(x=0, y=0 )


heading_prep = Label(prep_frame, text='We are preparing your order...', font='Georgia 40 bold' , bg='#F1CAA0' )
heading_prep.pack(pady=(300, 10))

status = Label(prep_frame, text='Your order is being processed', font='Georgia 24 italic', bg='#F1CAA0' )
status.pack(pady=20)
status.after(10, updateStatus)

order_frame.pack(fill='both', expand=1)

root.mainloop()