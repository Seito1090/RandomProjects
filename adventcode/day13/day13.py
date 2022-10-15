def setup(file):
    entries = [i.strip().split(",") for i in open(file,"r").readlines()]
    splitter = entries.index([''])
    coordinates = entries[:splitter]
    for i in range(len(coordinates)):
        coordinates[i] = (int(coordinates[i][0]),int(coordinates[i][1]))
    folds = entries[splitter+1:]
    for i in range(len(folds)):
        folds[i] = folds[i][0]
    return coordinates, folds

def borders(coordinates):
    max_x, max_y = 0,0
    for x,y in coordinates:
        if int(x) > max_x:
            max_x = int(x) 
        if int(y) > max_y:
            max_y =  int(y) 
    return (0,max_x), (0, max_y)

def place_points(grid, coordinates):
    for i in coordinates:
        grid[i[1]][i[0]] = "#"
    return grid

def replace_coords(save_grid, grid, change):
    for i in range(len(save_grid)):
        for j in range(len(save_grid[i])):
            if (change[0] == "y" and i > change[1] and save_grid[i][j] == "#"):
                new_y = 2*change[1]-i
                grid[new_y][j] = "#"
            if (change[0] == "x" and j > change[1] and save_grid[i][j] == "#"):
                new_x = 2*change[1]-j
                grid[i][new_x] = "#"
    return grid

def folding(grid, folds):
    instructions = []
    for i in folds:
        i = i.split()[-1:]
        letter, value = i[0].split("=")
        instructions.append((letter, int(value)))    

    for instruction in instructions:
        if instruction[0] == 'y':
            save_grid = grid
            grid = grid[:instruction[1]]
            grid = replace_coords(save_grid, grid, instruction)
        if instruction[0] == 'x':
            save_grid = grid
            grid = [grid[i][:instruction[1]]  for i in range(len(grid))]
            grid = replace_coords(save_grid, grid, instruction)
        if instructions.index(instruction) == 0:   #part 1 
            dots = get_number_dots(grid)
            print(dots)
    return grid

def get_number_dots(grid):
    counter = 0
    for i in grid:
        for j in i:
            if j == "#":
                counter +=1
    return counter

def show_grid(grid,final = False):
    for row in grid:
        text = ''
        for character in row:
            if final:
                if character == ".":
                    character = " "
            text+=character
        print(text)

def grid(coordinates, fold = ('x', -1)):
    border_x, border_y = borders(coordinates)
    grid = [['.' for x in range(border_x[1]+1) ] for y in range(border_y[1]+1)]
    grid = place_points(grid, coordinates)
    # show_grid(grid)
    grid = folding(grid,fold)
    return grid

if __name__ == "__main__":
    coordinates, folds = setup("day13_input.txt")
    grid = grid(coordinates, folds)
    show_grid(grid, True)
