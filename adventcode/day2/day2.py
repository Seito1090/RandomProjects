pos_x = 0
pos_y = 0
aim = 0
with open('day2_input.txt', 'r') as file:
    for i in file:
        i = i.split()
        if i[0] == 'up':
            aim -= int(i[1])
        elif i[0] == 'down':
            aim += int(i[1])
        else:
            pos_x += int(i[1])
            pos_y += aim*int(i[1])
    print(f"You've arrived at a depth of {pos_y}m and wend forward {pos_x}m, if we multiply it we get {pos_x*pos_y}")
        