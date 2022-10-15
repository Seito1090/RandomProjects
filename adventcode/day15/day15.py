import os
from collections import defaultdict

Grid = []
with open("day15_input.txt", "r") as data:
    for t in data:
        FirstRead = list(map(int, t.strip()))
        Grid.append(FirstRead)

#print(Grid)
GridDistances = defaultdict()
Height = len(Grid)
Width = len(Grid[0])
NewHeight = Height*5
NewWidth = Width*5
ImperialHoldings = set()
ImperialFrontier = [(0, (0,0))]
Directions = [(1, 0), (-1, 0), (0, 1), (0, -1)]
Expand = True
Cycles = 0
while Expand:
    Cycles += 1
    if not(ImperialFrontier):
        break
    NextExpansion = ImperialFrontier.pop(0)
    CurDistance = NextExpansion[0]
    Y = NextExpansion[1][0]
    X = NextExpansion[1][1]
    ImperialHoldings.add((Y, X))
    GridDistances[Y, X] = CurDistance
    if Y == NewHeight - 1 and X == NewWidth - 1:
        Expand = False
        break

    for t in Directions:
        NY = Y + t[0]
        NX = X + t[1]
        if 0 <= NY < NewHeight and 0 <= NX < NewWidth and (NY, NX) not in ImperialHoldings:
            GY = NY % Height
            GX = NX % Width
            Repeat = NY//Height + NX//Width
            NewDistance = Grid[GY][GX] + Repeat
            NewDistance = (NewDistance - 1) % 9 + 1
            NewTotalDist = NewDistance + CurDistance
            ImperialFrontier.append((NewTotalDist, (NY, NX)))
            ImperialHoldings.add((NY, NX))

    ImperialFrontier = sorted(ImperialFrontier)
    if Cycles % 5000 == 0:
        print(f"{Cycles = }")


print(GridDistances[Height-1, Width-1])
print(GridDistances[NewHeight-1, NewWidth-1])