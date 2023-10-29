import tkinter as tk
from tkinter import ttk

root = tk.Tk()
root.geometry("400x200")
root.title("Unit Converter")

# Top Bar possibly to delete later

topbar = ttk.Frame(root, padding=2)
topbar.grid()
topbar.columnconfigure(0, weight=1)
topbar.columnconfigure(1, weight=1)
topbar.columnconfigure(2, weight=1)
topbar.columnconfigure(3, weight=5)
topbar.columnconfigure(4, weight=0)


btn0 = ttk.Menubutton(topbar, text="Help")
btn0.grid(column=1, row=0, sticky="EW")

btn1 = ttk.Button(topbar, text="Settings")
btn1.grid(column=2, row=0, sticky="EW")

btn3 = ttk.Menubutton(topbar, text="File")
btn3.grid(column=0, row=0, sticky="EW")

exitbtn = ttk.Button(topbar, text="Exit", command=root.destroy)
exitbtn.grid(column=4, row=0, sticky="EW")

topbar.pack(fill="x")

# Main Content

main = ttk.Frame(root, padding=2)
main.columnconfigure(0, weight=1)
main.columnconfigure(1, weight=5)
main.columnconfigure(2, weight=5)

text0 = ttk.Label(main, text="Starting units : ", font=("Arial", 12), padding=12)
text0.grid(column=0, row=0, sticky="EW")

units0 = ttk.Menubutton(main, width=20, text="Select Units")
units0.grid(column=1, row=0, sticky="EW")
units0.menu = tk.Menu(units0, tearoff=0)
units0["menu"] = units0.menu
units0.menu.add_command(label="Celsius")
units0.menu.add_command(label="Kelvin")
units0.menu.add_command(label="Fahrenheit")
units0.menu.add_command(label="m")
units0.menu.add_command(label="km")
units0.menu.add_command(label="cm")
units0.menu.add_command(label="mm")
units0.menu.add_command(label="in")
units0.menu.add_command(label="ft")
units0.menu.add_command(label="yd")
units0.menu.add_command(label="mi")
units0.menu.add_command(label="l")
units0.menu.add_command(label="ml")
units0.menu.add_command(label="gal")
units0.menu.add_command(label="oz")
units0.menu.add_command(label="lb")
units0.menu.add_command(label="kg")
units0.menu.add_command(label="g")
units0.menu.add_command(label="mg")
units0.menu.add_command(label="ton")
units0.menu.add_command(label="tonne")
units0.menu.add_command(label="N")
units0.menu.add_command(label="kN")
units0.menu.add_command(label="lbf")
units0.menu.add_command(label="kip")
units0.menu.add_command(label="Pa")
units0.menu.add_command(label="kPa")
units0.menu.add_command(label="MPa")
units0.menu.add_command(label="GPa")
units0.menu.add_command(label="psi")
units0.menu.add_command(label="ksi")
units0.menu.add_command(label="atm")
units0.menu.add_command(label="bar")
units0.menu.add_command(label="mmHg")
units0.menu.add_command(label="inHg")

text1 = ttk.Label(main, text=" Desired units : ", font=("Arial", 12), padding=12)
text1.grid(column=0, row=1, sticky="EW")

input0 = ttk.Entry(main, width=20)
input0.grid(column=2, row=0, sticky="EW")

output0 = ttk.Label(main, text=" Value ", font=("Arial", 12), padding=12)
output0.grid(column=2, row=1, sticky="EW")

units1 = ttk.Menubutton(main, width=20, text="Select Units")
units1.grid(column=1, row=1, sticky="EW")
units1.menu = tk.Menu(units1, tearoff=0)
units1["menu"] = units1.menu
units1.menu.add_command(label="Fahrenheit")


main.pack(expand=True, fill="both")

root.mainloop()