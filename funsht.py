from classesfunsht import HumanPlayer, NonHumanPlayer

class Game:
    def __init__(self, x_player = "X", o_player = "O"):
        self.board = [[" " for x in range(3)] for i in range(3)]
        self.winner = False 
        self.current_player = x_player

    def print_board(self):
        print("")
        print(" | 1 | 2 | 3 ")
        print("-+-----------")
        for i in range(len(self.board)):
            print("{}| {} | {} | {} ".format(i+1, self.board[i][0],
             self.board[i][1], self.board[i][2]))
            if i != len(self.board)-1 :
                print("-|-----------")
        print("") 

    def change_letter(self):
        self.current_player = "X" if self.current_player == "O" else "O"

    def check(self, move):
        return self.board[int(move[0])-1][int(move[1])-1] == " "
    
    def available_moves(self):
        return [(i,j) for i in range(len(self.board)) for j \
            in range(len(self.board[i])) if self.board[i][j] == " "]
    
    def check_win(self, move):
        coord = [int(move[0])-1, int(move[1])-1]
        letter = "X" if self.current_player == "O" else "O"
        wrong_one = self.current_player
        # lines to check 
        if "." not in self.board[coord[0]]:
            if wrong_one not in self.board[coord[0]]:
                return True 
        # columns to check
        column = coord[1]
        if self.board[0][column] == letter and self.board[1][column] == letter \
            and self.board[2][column] == letter:
            return True
        # diagonals to check 
        if self.board[0][0] == letter and self.board[1][1] == letter \
            and self.board[2][2] == letter:
            return True
        if self.board[0][2] == letter and self.board[1][1] == letter \
            and self.board[2][0] == letter:
            return True
        return False
    
    def breaker(self):
        for i in self.board:
            if " " in i:
                return False
        return True

def play():
    game = Game()
    stop = False
    while not stop:
        game.print_board()
        game.make_move(move)
        if game.check_win(move):
            stop = True
            game.print_board()
            print(f"{'X' if game.player == 'O' else 'O'} won!")
        else:    
            if game.breaker():
                stop = True
                print("tied")
        
play()