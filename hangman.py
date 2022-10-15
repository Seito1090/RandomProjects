import random
words = []
with open("words.txt", 'r') as file:
    for i in file:
        words.append(i.strip().lower())
secretWord = random.choice(words)
print("J'ai choisi le mot à toi de le deviner! \n")
guessed = "_ "*len(secretWord)
point = 6
wrong = []
while "_ " in guessed and point > 0:
    print(f"{guessed} \nMauvaises lettres déjà utilisées {wrong}")
    guessing = input("Quelle lettre vous choisisez? \n")
    if guessing.lower() not in secretWord:
        point -= 1
        print(f"Raté! il ne te reste plus que {point} points")
        wrong.append(guessing.lower())
    if guessing.lower() in secretWord:
        print("Vous avez trouvé une des lettres! ")
        positions = []
        for j in range(len(secretWord)):
            if guessing.lower() == secretWord[j]:
                positions.append(j)
        old = [x for x in guessed if x!= " "]
        new = old
        for x in positions:
            new[x] = guessing.lower()
        guessed = ""
        for i in new:
            guessed += i+" "
    if guessing.lower() in wrong or guessing.lower() in guessed:
        print("Vous avez déjà mis cette lettre")
if point == 0:
    print(f"Vous avez perdu! Le mot était {secretWord}")
else:
    print(f"Bien joué vous avez gagné ! \n{secretWord}")