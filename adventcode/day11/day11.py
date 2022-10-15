# puzzle = [i.strip() for i in open("day11_input_test.txt","r").readlines()]
# correct = [i.strip() for i in open("correct.txt","r").readlines()]

# def show_puzzle(puzzle, step):
#     print(f'After step {step}:')
#     for i in puzzle:
#         line = ''
#         for j in i:
#             line+= str(j)
#         print(line)
#     print('')

# def  flash_react(y, x, puzzle, max_x, max_y):
#     rows = [y+1, y, y-1]
#     columns= [x+1, x, x-1]
#     centre = (columns[1], rows[1])
#     for x in columns:
#         for y in rows:
#             if (x < 0 or x >= max_x
#             or y < 0 or y >= max_y
#             or puzzle[y][x] == 0
#             or (y,x) == centre):
#                 pass
#             else:
#                 puzzle[y][x] += 1
#                 if puzzle[y][x] >= 10:
#                     puzzle = flash_react(y,x, puzzle, max_x, max_y)
#                     puzzle[y][x] = 0
#     return puzzle

# def adjacent_flash(puzzle):
#     max_x = len(puzzle[0])
#     max_y = len(puzzle)
#     for y in range(max_y): #rows
#         for x in range(max_x): #columns 
#             if puzzle[y][x] == 0:   
#                 puzzle = flash_react(y, x, puzzle, max_x, max_y)
#     return puzzle

# def simulate_step(puzzle,step):
#     new_puzzle = []
#     for i in puzzle: #adds 1 to each flash or places a 0 in case it does flash 
#         new_line = []
#         for j in i:
#             j = int(j) + 1 
#             if j <= 9: 
#                 new_line.append(j)
#             else:
#                 new_line.append(0)
#         new_puzzle.append(new_line)

#     new_puzzle = adjacent_flash(new_puzzle) #adds 1 to adjascent lines if it did flash

#     if step%10 != 0:
#         show_puzzle(new_puzzle, step)
#     return new_puzzle 

# print("Initial puzzle:")
# for i in puzzle:
#     print(i)
# print('')
# for i in range(2):
#     puzzle = simulate_step(puzzle,i+1)
    
# corr = []
# for i in correct:
#     line = [int(j) for j in i]
#     corr.append(line)
# print(corr == puzzle)
    
import numpy as np


with open("day11_input.txt") as file:
   octopus_map = file.read().splitlines()
octopus_map = np.array([[int(i) for i in x] for x in octopus_map],dtype=int)
def get_adjacents(octopus_map, position):
    i, j = position
    i_max, j_max = octopus_map.shape
    adjacents = set()
    
    if i != 0: 
        adjacents.add((i-1,j)) #above
    if i != i_max - 1:
        adjacents.add((i+1,j)) #below
    if j != 0:
        adjacents.add((i,j-1)) #left
    if j != j_max - 1:
        adjacents.add((i,j+1)) #right
    if i != 0 and j != 0:             #above-left
        adjacents.add((i-1,j-1))       #above-left
    if i != 0 and j != j_max - 1:     #above-right
        adjacents.add((i-1,j+1))       #above-right
    if i != i_max - 1 and j != 0:     #below-left
        adjacents.add((i+1,j-1))       #below-left
    if i != i_max - 1 and j != j_max - 1:  #below-right
        adjacents.add((i+1,j+1))       #below-right
    return adjacents


class Octopus:
    def __init__(self, octopus_map):
        self.map = octopus_map.copy()
        self.total_flashes = 0
        self.epoch = 0
        self.i_max, self.j_max = octopus_map.shape
    
    def increase_epoch(self, n_epochs):
        for t in range(n_epochs):
            flashed = set()
            self.map += np.ones((10,10), dtype=int)
            self.epoch += 1
            add_flashes = True if np.count_nonzero(self.map > 9) > 0 else False
            self.total_flashes += np.count_nonzero(self.map > 9)
            self.map = np.where(self.map > 9, 0, self.map)
            while add_flashes == True:
                add_flashes = False
                for i in range(self.i_max):
                    for j in range(self.j_max):
                        adjacents = get_adjacents(self.map, (i,j))
                        for adj in adjacents - flashed:
                            if self.map[i,j] != 0 and self.map[adj] == 0:
                                self.map[i,j] += 1
                        # Make flashes and count extra flashes
                if np.count_nonzero(self.map > 9) > 0:
                    add_flashes = True
                self.total_flashes += np.count_nonzero(self.map > 9)
                flashed.update([x for x in zip(np.where(self.map == 0)[0], np.where(self.map == 0)[1])])
                self.map = np.where(self.map > 9, 0, self.map)
                
    def find_synchonisation(self):
        while self.map.sum() != 0:
            self.increase_epoch(1)
        return(self.epoch)


oct_1 = Octopus(octopus_map)
oct_1.increase_epoch(100)

print("Answer to part 1 is: {}".format(oct_1.total_flashes))

oct_2 = Octopus(octopus_map)
total_epochs = oct_2.find_synchonisation()


print("Answer to part 2 is: {}".format(total_epochs))