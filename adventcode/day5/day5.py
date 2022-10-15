def get_coord(line):
    line = line.split(" -> ")
    x1,y1 = tuple(map(int, line[0].strip().split(",")))
    x2,y2 = tuple(map(int, line[1].strip().split(",")))
    if x1 < x2 or y1 < y2:
        return x1,y1,x2,y2
    else:
        return x2,y2,x1,y1

def diagonal_emulator(x1,y1,x2,y2):
    current_x = x1
    current_y = y1

    #check if we have to do up/down and left/right
    delta_x = delta_y = 1
    if x1 > x2:
        delta_x = -1
    if y1 > y2:
        delta_y = -1
    
    while current_x != x2:
        # Doesn't matter whether we check x or y, they should be the same amount
        yield current_x, current_y
        current_x += delta_x
        current_y += delta_y

    # Need to yield last result, because we need it
    yield current_x, current_y 

with open('day5_input.txt','r') as file:
    f = file.readlines()

points = {}
for line in f:
    x1, y1, x2, y2 = get_coord(line)

    #not a straight line 
    if not (x1 == x2 or y1 == y2):
        for x,y in diagonal_emulator(x1,y1,x2,y2):
            if (x,y) in points:
                points[(x,y)] += 1
            else:
                points[(x,y)] = 1

    #straight line 
    if (x1 == x2 or y1 == y2):
        for x in range(x1, x2+1):
            for y in range(y1, y2+1):
                if (x,y) in points:
                    points[(x,y)] += 1
                else:
                    points[(x,y)] = 1
    
#count the coordinates with more than 1 as their value
count = 0
for x,y in points:
    if points[(x,y)] > 1:
        count += 1
    
print(count)


    