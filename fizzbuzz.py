n = int(input("Max number? "))
for x in range(1, n+1):
    txt = ""
    if x % 3 == 0:
        txt+= "Fizz"
    if x % 5 == 0:
        txt += "Buzz"
    if txt == "":
        txt = x
    print(txt)
