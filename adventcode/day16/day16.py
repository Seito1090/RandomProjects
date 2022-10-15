hex_convertion = {'0' : '0000','1' : '0001','2' : '0010','3' : '0011','4' : '0100','5' : '0101','6' : '0110','7' : '0111','8' : '1000','9' : '1001','A' : '1010','B' : '1011','C' : '1100','D' : '1101','E' : '1110','F' : '1111',}

def binPartitions(bin):
    decimal = ''
    bits = 4
    step = 0
    max_setps = len(bin) // bits
    to_pass = ''
    while step != max_setps:
        index_val = [i for i in range(bits)]
        for i in index_val:
            to_pass += bin[i+step*4]
        step += 1 
    decimal = int(to_pass,2)
    return decimal

def hexToBinary(entry):
    binary_entry = ''
    for i in entry:
        binary_entry += hex_convertion[i]
    return binary_entry
 
# def decode(entry, position = 0):
#     bin_entry = hexToBinary(entry)
#     version = int(bin_entry[:3], 2)
#     type_ID = int(bin_entry[3:6], 2)
#     position += 6
#     packet_value = ''
#     if type_ID == 4:
#         binLeft = bin_entry[6:]
#         last = False
#         while not last:
#             last = False if binLeft[0] == '1' else True
#             packet_value += binLeft[1:5]
#             binLeft = binLeft[5:]
#             position += 5
#         packet_value = binPartitions(packet_value) 
#     else: 
#         lengthTypeID = bin_entry[6]
#         binLeft = bin_entry[7:]
#         position += 1
#         if lengthTypeID == '0':
#             totalLength = int(binLeft[:15], 2)
#             binLeft = binLeft[15:]
#             position += 15
#             subPosition = 0
#             values = []
#             while subPosition != totalLength:
#                 binLeft, subPosition, value = decode(binLeft, subPosition)
#                 values.append(value)
#             position += subPosition
#         elif lengthTypeID == '1':
#             totalCount = int(binLeft[:11],2)
#             binLeft = binLeft[11:]
#             position += 11
#             count = 0
#             subPosition = 0
#             values = []
#             while count != totalCount:
#                 binLeft, subPosition, value = decode(binLeft, subPosition)
#                 values.append(value)
#                 count += 1
#             position += subPosition
#         if type_ID == 0:
#             ans = sum(values)
#         elif type_ID == 1:
#             ans = 1
#             for item in values:
#                 ans *= item
#         elif type_ID == 2:
#             ans = min(values)
#         elif type_ID == 3:
#             ans = max(values)
#         elif type_ID == 5:
#             ans = 1 if values[0]>values[1] else 0
#         elif type_ID== 6:
#             ans = 1 if values[0]<values[1] else 0
#         elif type_ID == 7:
#             ans = 1 if values[0]==values[1] else 0
            
#         # print(packet_type, values, 'ans=', ans)  
#         packet_value = ans
#     return binLeft, position, packet_value 
bs = bin(int('1'+open("day16_input.txt").read(),16))[3:]

def ps1(startbit):
    i = startbit # index into bs
    tv = int(bs[i:i+3],2) # total version
    ID = int(bs[i+3:i+6],2) # packet type ID
    i += 6
    if ID == 4: #literal value
        while True:
            i += 5
            if bs[i-5] == '0': #last value packet
                break
    else:
        if bs[i] == '0':
            endi = i + 16 + int(bs[i+1:i+16],2)
            i += 16
            while i < endi:
                i,v = ps1(i)
                tv += v
        else:
            np = int(bs[i+1:i+12],2)
            i += 12
            for _ in range(np):
                i,v = ps1(i)
                tv += v

    return i,tv

print("Total of version numbers:",ps1(0)[1])


### PART 2 ###

import math

op = [sum, math.prod, min, max,
      lambda ls: ls[0], # literal
      lambda ls: 1 if ls[0] > ls[1] else 0,  # gt
      lambda ls: 1 if ls[0] < ls[1] else 0,  # lt
      lambda ls: 1 if ls[0] == ls[1] else 0] # eq

def ps2(startbit):
    i = startbit # index into bs
    ID = int(bs[i+3:i+6],2) # packet type ID
    i += 6
    if ID == 4: #literal value
        vals = [0]
        while True:
            vals[0] = 16*vals[0] + int(bs[i+1:i+5],2)
            i += 5
            if bs[i-5] == '0': #last value packet
                break
    else:
        vals = []
        if bs[i] == '0': # subpacket length in bits
            endi = i + 16 + int(bs[i+1:i+16],2)
            i += 16
            while i < endi:
                i,v = ps2(i)
                vals.append(v)
        else:
            np = int(bs[i+1:i+12],2) # number of subpackets
            i += 12
            for _ in range(np):
                i,v = ps2(i)
                vals.append(v)

    return i,op[ID](vals)

print('Total value:',ps2(0)[1])
