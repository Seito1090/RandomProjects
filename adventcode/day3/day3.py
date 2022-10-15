gamma_rate = ''
epsilon_rate = ''
with open('day3_input.txt', 'r') as file:
    f = file.readlines()
    loop = len(f[0].strip())
    for i in range(loop):
        nbr_0 = 0 
        nbr_1 = 0
        for bin in f:
            bin = bin.strip()
            if bin[i] == '0':
                nbr_0 += 1
            if bin[i] == '1':
                nbr_1 += 1
        if nbr_0 > nbr_1:
            gamma_rate += '0'
            epsilon_rate += '1'
        else:
            gamma_rate += '1'
            epsilon_rate += '0'
    g_r = int(gamma_rate,2)
    e_r = int(epsilon_rate,2)
    print(f"gamma rate in decimal = {g_r} and in binary = {gamma_rate}, epsilon_rate in decimal = {e_r} and in binary = {epsilon_rate}")
    power = e_r * g_r
    print(f"The total power is {power}")
    #now for part 2 with the O2 and CO2 ratings
    O2_candidates = [bin.strip() for bin in f if bin[0] == gamma_rate[0]]
    CO2_candidates = [bin.strip() for bin in f if bin[0] == epsilon_rate[0]]
    for i in range(1,loop):
        winner_O2_2 = []
        nbr_0 = 0 
        nbr_1 = 0
        for candidate in O2_candidates:
            if candidate[i] == '0':
                nbr_0 += 1
            if candidate[i] == '1':
                nbr_1 += 1
        top_nbr = '0' if nbr_0 > nbr_1 else '1'
        for candidate in O2_candidates:
            if candidate[i] == top_nbr:
                winner_O2_2.append(candidate)
        O2_candidates = winner_O2_2
    for i in range(1,loop):
        winner_CO2_2 = []
        nbr_0 = 0
        nbr_1 = 0
        for candidate in CO2_candidates:
            if candidate[i] == '0':
                nbr_0 += 1
            if candidate[i] == '1':
                nbr_1 += 1
        if nbr_0 == nbr_1:
            pass
        else:
            top_nbr = '0' if nbr_0 < nbr_1 else '1'
            for candidate in CO2_candidates:
                if candidate[i] == top_nbr:
                    winner_CO2_2.append(candidate)
            CO2_candidates = winner_CO2_2
            if len(CO2_candidates) == 1:
                break
    O2_r = int(winner_O2_2[0],2)
    CO2_r = int(winner_CO2_2[0],2)
    print(f"O2 generator rating in decimal = {O2_r} and in binary = {winner_O2_2[0]}, CO2 scrubber rating in decimal = {CO2_r} and in binary = {winner_CO2_2[0]}")
    life_support = O2_r * CO2_r
    print(f"The total life support is {life_support}")
# with open("day3_input.txt", "r") as file:
#     numbers = [row.rstrip() for row in file]

# # Part 1
# gamma_rate_binary = ""
# binary_columns = zip(*numbers)
# for column in binary_columns:
#     if column.count("1") > column.count("0"):
#         gamma_rate_binary += "1"
#     else:
#         gamma_rate_binary += "0"
# gamma_rate = int(gamma_rate_binary, 2)

# num_bits = int(len(numbers[0]))
# max_rate = 2 ** num_bits
# epsilon_rate = max_rate - gamma_rate - 1
# power_consumption = gamma_rate * epsilon_rate
# print(f"Power consumption: {power_consumption}")

# # Part 2
# def get_binary_rating(is_reversed=False):
#     remaining_numbers = numbers.copy()
#     num_bits = int(len(numbers[0]))
#     for i in range(num_bits):
#         if len(remaining_numbers) == 1:
#             break
#         binary_columns = list(zip(*remaining_numbers))
#         first_column = binary_columns[i]
#         key_number = "0"
#         if is_reversed:
#             if first_column.count("0") > first_column.count("1"):
#                 key_number = "1"
#         else:
#             if first_column.count("1") >= first_column.count("0"):
#                 key_number = "1"
#         new_remaining_numbers = []
#         for number in remaining_numbers:
#             if number[i] == key_number:
#                 new_remaining_numbers.append(number)
#         remaining_numbers = new_remaining_numbers
#     return remaining_numbers[0]

# oxygen_gen_rating_binary = get_binary_rating()
# co2_scubber_rating_binary = get_binary_rating(is_reversed=True)
# print(co2_scubber_rating_binary)
# oxygen_gen_rating = int(oxygen_gen_rating_binary, 2)
# co2_scrubber_rating = int(co2_scubber_rating_binary, 2)
# life_support_rating = oxygen_gen_rating * co2_scrubber_rating
# print(f"Life support rating: {life_support_rating}")