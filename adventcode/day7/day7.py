with open("day7_input_test.txt","r") as file:
    f = list(map(int,file.readlines()[0].strip().split(",")))

store_len = {}
''' 
for i in f:
    if i not in store_len:
        for j in range(max(f)):
            print(i, j, abs(i-j), sum(range(abs(i-j)+1)))             
            my_sum = sum(range(abs(i-j)+1))             
            print(store_len[i])
            store_len[j] = store_len[j] + my_sum
min_fuel = min(store_len, key = store_len.get)
print(min_fuel, store_len[min_fuel])
'''

for i in range(max(f)+1):
    for j in f:
        if i not in store_len:
            store_len[i] = sum(range(abs(i-j)+1))
        else:
            store_len[i] += sum(range(abs(i-j)+1))
min_fuel = min(store_len, key = store_len.get)
print(min_fuel, store_len[min_fuel])