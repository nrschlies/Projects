import tkinter as tk
import math
import random
from termcolor import colored

class monte_approx:
    def __init__(self):
        self.window = tk.Tk()
        self.window.title("Monte Carlo Pi Approximation")
        
        self.real_pi = 3.1415926535897
 
        self.canvas_width = 400
        self.canvas_height = 430

        self.canvas = tk.Canvas(self.window,
            width=self.canvas_width,
            height=self.canvas_height)
        self.canvas.grid(row=1,column=0)

        self.canvas.create_oval(
            4,4,self.canvas_width,self.canvas_width
        )
        
        self.option_frame = tk.Frame(self.window)
        self.option_frame.grid(row=2)

        self.approx = tk.StringVar()
        self.approximation = tk.Entry(self.option_frame,textvariable=self.approx)
        self.approximation.grid(row=1,column=1)

        self.enter_button = tk.Button(self.option_frame, text="Enter", command=self.enter)
        self.enter_button.grid(row=1,column=2)

        self.clear_button = tk.Button(self.option_frame, text="Clear", command=self.clear)
        self.clear_button.grid(row=1,column=3)
        self.window.mainloop()
    def enter(self):
        self.circle_hits = 0
        self.total_hits = 0
        self.pi = 0
        self.hits_message = colored("Shooting {} darts at the target...".format(int(self.approx.get())),"red")
        print(self.hits_message)
        for i in range(int(self.approx.get())):
            x = random.uniform(-1*self.canvas_width, self.canvas_width)
            y = random.uniform(-1*self.canvas_width, self.canvas_width)
            origin = x**2 + y**2
            self.dot = self.canvas.create_rectangle(
                x,y,x+1,y+1, tags="a"
            )
            if origin <= self.canvas_width**2:
                self.circle_hits += 1
            self.total_hits += 1
            self.pi = 4*self.circle_hits/self.total_hits
        self.uncertainty = (1-abs((self.pi-self.real_pi)/self.real_pi))*100
        self.offset_message = "You were % {:.4f} correct!".format(self.uncertainty)
        print(colored(self.offset_message, 'green'))
        print("With a value of: {:.7f}".format(self.pi))
    def clear(self):
        self.canvas.delete("a")
        self.approx.set("")
if __name__ == "__main__":
    monte_approx()