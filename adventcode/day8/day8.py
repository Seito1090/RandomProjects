#!/usr/bin/env python

itxt = open("day8_input.txt", mode='r').read().strip().splitlines()
itxt = [i.split(' ') for i in itxt]

u = [[s for s in e[11:15] if len(s) in [2,4,3,7]] for e in itxt]
f = [j for sub in u for j in sub]
    
print(len(f))
#!/usr/bin/env python

"""
  0:      1:      2:      3:      4:     5:      6:      7:      8:      9:
 aaaa    ....    aaaa    aaaa    ....    aaaa    aaaa    aaaa    aaaa    aaaa
b    c  .    c  .    c  .    c  b    c  b    .  b    .  .    c  b    c  b    c
b    c  .    c  .    c  .    c  b    c  b    .  b    .  .    c  b    c  b    c
 ....    ....    dddd    dddd    dddd    dddd    dddd    ....    dddd    dddd
e    f  .    f  e    .  .    f  .    f  .    f  e    f  .    f  e    f  .    f
e    f  .    f  e    .  .    f  .    f  .    f  e    f  .    f  e    f  .    f
 gggg    ....    gggg    gggg    ....   gggg     gggg    ....    gggg    gggg
"""
#! 0:6 1:2 2:5 3:5 4:4 5:5 6:6 7:3 8:7 9:6
#! 1, 4, 7, 8 [2,4,3,7]

#haystack len
def getsl(e: list, l: int) -> set:
    return set([s for s in e if len(s) == l])

itxt = open("day8_input.txt", mode='r').read().strip().splitlines()
itxt = [i.split(' ') for i in itxt]

outp = list()

for e in itxt:
    mapn = dict({0: None, 1: None, 2: None, 3: None, 4: None, 5: None, 6: None, 7: None, 8: None, 9: None})

    for d in e[0:10]:
        match len(d):
            case 2: mapn.update({ 1: set(d) }) #! 1
            case 4: mapn.update({ 4: set(d) }) #! 4
            case 3: mapn.update({ 7: set(d) }) #! 7
            case 7: mapn.update({ 8: set(d) }) #! 8

    for d in getsl(e, 6): 
        if len(set(d).difference(mapn.get(1))) == 5:
            mapn.update({ 6: set(d) })  #! 6
        elif set(d).issuperset(mapn.get(4)):
            mapn.update({ 9: set(d) })  #! 9
        else:
            mapn.update({ 0: set(d) })  #! 0
            
    for d in getsl(e, 5):
        if len(set(d).difference(mapn.get(4))) == 3:
            mapn.update({ 2: set(d) })  #! 2
        else:
            if set(d).issuperset(mapn.get(1)):
                mapn.update({ 3: set(d) })  #! 3
            else: 
                mapn.update({ 5: set(d) }) #! 5
    
    print(mapn)
    outp.append(int(''.join([[str(k) for k,v in mapn.items() if v == set(d)][0] for d in e[11:]])))

print(outp)
print(sum(outp))