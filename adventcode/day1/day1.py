global_count = 0
with open('day1_input.txt','r') as file:
    f = file.readlines()
    for i in range(0, len(f)-3):
        sum_a = int(f[i+1])+int(f[i+2])+int(f[i+3])
        sum_b = int(f[i])+int(f[i+1])+int(f[i+2])
        if sum_a > sum_b:
            global_count += 1
    print(f"In general, the depth was increased {global_count} times")