# with open("day6_input_test.txt", 'r') as file:
#     f = file.readlines()[0]

# start = list(map(int, f.split(',')))
# days = int(input("What's the max days you wanna get ? "))
# for i in range(days):
#     for j in range(len(start)):
#         if start[j] != 0:
#             start[j] -= 1
#         else:
#             start.append(8)
#             start[j] = 6
#     print(f"Day {i}")

# print(len(start))
def get_data(filename):
    with open(filename) as file:
        return [int(x) for x in file.read().split(",")]


def simulate(fish_ages):
    count = [fish_ages.count(i) for i in range(9)]
    result_at_day_80 = 0
    for day in range(1, 256+1):
        zeros = count[0]
        count[:-1] = count[1:]
        count[6] += zeros
        count[8] = zeros
        if day == 80:
            result_at_day_80 = sum(count)
    return result_at_day_80, sum(count)


sample_data = get_data("day6_input_test.txt")
challenge_data = get_data("day6_input.txt")

if __name__ == "__main__":
    sample_part_1, sample_part_2 = simulate(sample_data)
    assert sample_part_1 == 5934
    assert sample_part_2 == 26984457539

    challenge_part_1, challenge_part_2 = simulate(challenge_data)
    print(challenge_part_1)  # 372300
    print(challenge_part_2)  # 1675781200288