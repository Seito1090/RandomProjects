import tkinter as tk
from tkinter import ttk
# standard units used as follows :
# distance : m
# volume : l
# mass : g
# pressure : bar 

# empirical units used as follows :
# distance : in
# volume : gal
# mass : lb
# pressure : psi

# standard units to empirical units conversions : 
m_to_in = lambda x : x * 39.3701
l_to_gal = lambda x : x * 0.2641720524
g_to_lb = lambda x : x / 453.6
bar_to_psi = lambda x : x * 14.5038

#for later implementations : 
# R = 8.314 J/(mol*K)
# 1 atm = 101325 Pa

#dictionaries :
SIunits = {"distance" : ["km", "m", "cm", "mm"], "volume" : ["l", "ml"], "mass" : ["kg", "g", "mg", "tonne"], "pressure" : ["Pa", "kPa", "MPa", "GPa"]}
EMunits = {"distance" : ["mi", "yd", "ft", "in"], "volume" : ["gal", "oz"], "mass" : ["lb", "oz"], "pressure" : ["psi", "ksi", "atm", "bar", "mmHg", "inHg"]}

# STOPPPPP___________________________________________________ JUST DO A TABLE WHERE YOU STOCK THE CONVERSION FACTORS, IT ENABLES YOU TO STORE IT IN A COMPACT WAY AND IT S NOT ROCKET SCIENCE

print(SIunits["distance"])

def convert_units():
    # Implement the conversion logic here
    initial_unit = units_var.get()
    final_unit = result_var.get()
    value = value_entry.get()
    # Check if the input is valid #TODO
    # Separate distance, volume, mass, force, pressure
    if initial_unit in SIunits["distance"] or initial_unit in EMunits["distance"]: 
        # get back to standard unit (m)
        pass
    return g_to_lb(float(value))

def update_units(*args):
    # Implement the update logic here
    new_val = convert_units()
    result_text.config(text=str(new_val))
    return 0

def delete_units(*args):
    # Implement the delete logic here
    pass

root = tk.Tk()
root.title("Unit Converter")

# Top bar frame
top_frame = tk.Frame(root)
top_frame.pack(side=tk.TOP, padx=10, pady=5, fill=tk.X)
top_frame.columnconfigure(0, weight=1)
top_frame.columnconfigure(1, weight=1)
top_frame.columnconfigure(2, weight=1)

# First row
from_label = tk.Label(top_frame, text="From:")
from_label.grid(row=0, column=0)

units_var = tk.StringVar()
units_var.set("Select Units")
units_menu = tk.OptionMenu(top_frame, units_var, "Unit 1", "Unit 2")  # Add more units as needed
units_menu.grid(row=0, column=1)

value_entry = tk.Entry(top_frame)
value_entry.grid(row=0, column=2)

# Second row
to_label = tk.Label(top_frame, text="To:")
to_label.grid(row=1, column=0)

result_var = tk.StringVar()
result_var.set("Select Units")
result_menu = tk.OptionMenu(top_frame, result_var, "Unit A", "Unit B")  # Add more units as needed
result_menu.grid(row=1, column=1)

result_menu["menu"].configure(font=("Arial", 10))

result_text = tk.Label(top_frame)
result_text.grid(row=1, column=2)

convert_button = tk.Button(root, text="Convert", command=update_units)
convert_button.pack(fill=tk.X, padx=10, pady=5)

root.mainloop()