def getNextFree(puzzle):
    #just getting a simple next empty value in a list of lists 
    for i in range(9):
        for j in range(9):
            if puzzle[i][j] == 0:
                return (i,j)
    return (None, None)

def isValid(puzzle, value, row, col):
    #cheing rows
    if value in puzzle[row]:
        return False
    #checking columns 
    colValues = [puzzle[i][col] for i in range(9)]
    if value in colValues:
        return False
    #checking squares 
    startingRow = (row // 3) * 3 
    startingCol = (col // 3) * 3
    for i in range(startingRow, startingRow + 3):
        for j in range(startingCol, startingCol + 3):
            if puzzle[i][j] == value:
                return False
    #if none of these are satisfied then the answer can be placed at this place 
    return True 

def solveSudoku(puzzle):
    #check if there's still place left to put something 
    row, col = getNextFree(puzzle)
    #if there is no space left on the board then we got it right 
    if row is None:
        return True 
    #if ther is space left then we should put values there :)
    for guess in range(1, 10):
        if isValid(puzzle, guess, row, col):
            puzzle[row][col] = guess
            if solveSudoku(puzzle):
                return True
        puzzle[row][col] = 0 
    #this means that we couldn't find any solutions to the puzzle that was passed 
    return False       

def print_puzzle(puzzle):
    print("   0 1 2 | 3 4 5 | 6 7 8 ")
    for x in range(len(puzzle)):
        if x % 3 == 0: 
            print("-|-------+-------+------- ")
            print(f"{x}| {puzzle[x][0]} {puzzle[x][1]} {puzzle[x][2]} | {puzzle[x][3]} {puzzle[x][4]} {puzzle[x][5]} | {puzzle[x][6]} {puzzle[x][7]} {puzzle[x][8]} ")
        else: 
            print(f"{x}| {puzzle[x][0]} {puzzle[x][1]} {puzzle[x][2]} | {puzzle[x][3]} {puzzle[x][4]} {puzzle[x][5]} | {puzzle[x][6]} {puzzle[x][7]} {puzzle[x][8]} ")

if __name__ == '__main__':
    finished = False
    sudoku = [
    [0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0]
    ]
    print_puzzle(sudoku)
    #input the puzzle to solve 
    while not finished:
        try:
            value, positions = tuple(input("\nEnter the value then the position (value positiony,positionx) \nor press enter if you're done: ").strip().split())
            positions = positions.split(",")
            sudoku[int(positions[0])][int(positions[1])] = int(value)
            print_puzzle(sudoku)
        except IndexError:
            print_puzzle(sudoku)
            check = input("Is this the puzzle you wan to solve? y/n")
            if check == "y":
                finished = True
            else: 
                pass
        except ValueError:
            print_puzzle(sudoku)
            check = input("Is this the puzzle you wan to solve? (y/n) ")
            if check == "y":
                finished = True
            else: 
                pass
    print("")
    solveSudoku(sudoku)
    print("Here you have the solved one:")
    print_puzzle(sudoku)
    x = input(" ")