from tinyspline import *
from tkinter import *


spline = BSpline(7)
ctrlp = spline.control_points
ctrlp[0]  = 120 # x0
ctrlp[1]  = 100 # y0
ctrlp[2]  = 270 # x1
ctrlp[3]  = 40  # y1
ctrlp[4]  = 370 # x2
ctrlp[5]  = 490 # y2
ctrlp[6]  = 590 # x3
ctrlp[7]  = 40  # y3
ctrlp[8]  = 570 # x4
ctrlp[9]  = 490 # y4
ctrlp[10] = 420 # x5
ctrlp[11] = 480 # y5
ctrlp[12] = 220 # x6
ctrlp[13] = 500 # y6
spline.control_points = ctrlp


tk = Tk()
canvas = Canvas(tk, width=800, height=600)
canvas.config(background="white")
canvas.pack()


num = 0
def draw():
	canvas.delete("all")
	lines = spline.sample(num)
	for i in range(int(len(lines) / spline.dimension) - 1):
		xf = lines[(i)*spline.dimension]
		yf = lines[(i)*spline.dimension + 1]
		xt = lines[(i+1)*spline.dimension]
		yt = lines[(i+1)*spline.dimension + 1]
		canvas.create_line(xf, yf, xt, yt)


def update(event):
	global num
	num = int(event)
	draw()


scale = Scale(tk, from_=0, to=50, orient=HORIZONTAL, command=update)
scale.pack()
draw()
mainloop()
