def getTargetCoordinates(area):
    coordx = area[0].split("..")
    coordy = area[1].split("..")
    coordx[0] = coordx[0].split("=")[1]
    coordy[0] = coordy[0].split("=")[1]
    return (int(coordx[0]), int(coordx[1])),(int(coordy[0]), int(coordy[1]))
def createArea(targetArea):
    area = [['.'for j in range(targetArea[0][1])] for i in range(abs(targetArea[1][1]))] if abs(targetArea[1][1]) > abs(targetArea[1][0]) else [['.'for j in range(targetArea[0][1])] for i in range(abs(targetArea[1][0]))]
    maxy = abs(targetArea[1][1]) if abs(targetArea[1][1]) >= abs(targetArea[1][0]) else abs(targetArea[1][0])
    miny = abs(targetArea[1][1]) if abs(targetArea[1][1]) <= abs(targetArea[1][0]) else abs(targetArea[1][0])
    maxx = abs(targetArea[0][1])
    minx = abs(targetArea[0][0])
    for y in range(maxy):
        for x in range(maxx):
            if (y > miny-1 and y < maxy+1) and (x > minx-1 and x < maxx+1): 
                area[y][x] = "T"
            if x == y == 0:
                area[y][x] = "S"
    return area
def showArea(area):
    for i in area:
        string = ""
        for j in i:
            string+=j
        print(string)
targetArea = [i.strip() for i in open("day17_input_test.txt", "r").readline().split(":")[1].split(",")]
coordinatesTarget = getTargetCoordinates(targetArea)
area = createArea(coordinatesTarget)
showArea(area)