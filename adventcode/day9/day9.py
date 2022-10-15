# grid = open("day9_input.txt","r").readlines()
# for i in range(len(grid)):
#     grid[i] = grid[i].strip()
    
# def is_low_point(val_map, index_tuple, value):
#     '''Returns true if it's adyacent values are higher
#     than the checked value.'''
    
#     row = index_tuple[0]
#     col = index_tuple[1]
    
    
#     # check up
#     if row != 0:
#         val_up = int(val_map[row-1][col])
#     else:
#         val_up = 10 # If it's a corner/boundary situation, it's compared
#                     # to 10 so that check will pass

#     # check right
#     if len(val_map[row]) != col+1:
#         val_right = int(val_map[row][col+1])
#     else:
#         val_right = 10

#     # check down
#     if len(val_map) != row + 1:
#         val_down = int(val_map[row+1][col])
#     else:
#         val_down = 10

#     # check left
#     if col != 0:
#         val_left = int(val_map[row][col-1])
#     else:
#         val_left = 10

#     ady_values = [val_up, val_right, val_down, val_left]

#     if all([ady > int(value) for ady in ady_values]):
        
#         return True
#     else:
#         return False

# low_point_sum = 0

# for idx_x, row in enumerate(grid):
#     for idx_y, col in enumerate(row):
#         if is_low_point(grid, (idx_x, idx_y), grid[idx_x][idx_y]):
#             low_point_sum += int(grid[idx_x][idx_y]) + 1

# print(f"Answer is: {low_point_sum}")
class Point:
    def __init__(self, x, y):
        self.x = x
        self.y = y


def is_lower(y, x, height_map):
    if x - 1 >= 0:
        if height_map[y][x] >= height_map[y][x - 1]:
            return False
    if x + 1 <= len(height_map[y]) - 1:
        if height_map[y][x] >= height_map[y][x + 1]:
            return False
    if y - 1 >= 0:
        if height_map[y][x] >= height_map[y - 1][x]:
            return False
    if y + 1 <= len(height_map) - 1:
        if height_map[y][x] >= height_map[y + 1][x]:
            return False
    return True


def get_low_points(height_map):
    low_points = []
    points_location = []
    for y in range(len(height_map)):
        for x in range(len(height_map[y])):
            if is_lower(y, x, height_map):
                low_points.append(height_map[y][x])
                points_location.append(Point(x, y))

    return low_points, points_location


def calculcate_basin_size(point, height_map):
    size = 0
    if (point.x >= 0
        and point.y >= 0
        and point.y < len(height_map)
        and point.x < len(height_map[point.y])):
        if height_map[point.y][point.x] != '#' and height_map[point.y][point.x] < 9:
            size = 1
            height_map[point.y][point.x] = '#'
            size += calculcate_basin_size(Point(point.x - 1, point.y),
                                          height_map)
            size += calculcate_basin_size(Point(point.x + 1, point.y),
                                          height_map)
            size += calculcate_basin_size(Point(point.x, point.y - 1),
                                          height_map)
            size += calculcate_basin_size(Point(point.x, point.y + 1),
                                          height_map)
    return size


def get_basin_size(points_location, height_map):
    first = 0
    second = 0
    third = 0
    for point in points_location:
        size = calculcate_basin_size(point, height_map)
        if size > first:
            third = second
            second = first
            first = size
        elif size > second:
            third = second
            second = size
        elif size > third:
            third = size


    return first, second, third, first * second * third


def main():
    height_map = []
    with open("day9_input.txt") as f:
        for line in f:
            height_map.append(list(map(int, list(line.replace('\n', '')))))
    low_points, points_location = get_low_points(height_map)
    print(sum(low_points) +  len(low_points))
    print(get_basin_size(points_location, height_map))

if __name__ == "__main__":
    main()