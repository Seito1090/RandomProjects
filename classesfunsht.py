import random

class Player:
    def __init__(self, letter):
        self.letter = letter

    def move(self):
        pass 

class HumanPlayer(Player):
    def __init__(self, letter):
        super().__init__(letter)

    def move(self, game):
        move = tuple(input(f"where to place {game.current_player}? **line/column** ").strip().split())
        if game.check(move):
            game.board[int(move[0])-1][int(move[1])-1] = "X" if game.current_player == "X" else "O"
            game.current_player = "X" if game.current_player == "O" else "O"
        else:
            print("invalid position")

class NonHumanPlayer(Player):
    def __init__(self, letter):
        super().__init__(letter)

    def move(self, game):
        next_move = random.choice(game.available_moves())
        return next_move

class GodPlayer(Player):
    def __init__(self, letter):
        super().__init__(letter)

    def move(self, game):
        pass

    def descision(self):
        pass 
