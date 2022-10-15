# file = "day14_input_test.txt"
# entry = open(file, "r").readlines()[0].strip()
# operations = open(file,'r').readlines()[2:]
# occurences = {}

# for i in range(len(operations)):
#     operations[i] = operations[i].strip()
# op = []

# for i in operations:
#     op.append(i.split(" -> "))

# max_turns = 40
# for turn in range(max_turns):
#     new_entry = entry[0]
#     for i in range(len(entry)-1):
#         temp = entry[i] + entry[i+1]
#         for j in op:
#             if temp in j:
#                 new_entry+=  j[1] + entry[i+1]
#     entry = new_entry

# for j in range(len(entry)):
#     if entry[j] not in occurences:
#         occurences[entry[j]] = 1
#     else:
#         occurences[entry[j]] += 1

# top = max(occurences, key = occurences.get)
# bot = min(occurences, key = occurences.get)
# diff = int(occurences[top]) - int(occurences[bot])
# print(diff)
# print(len(entry), occurences)
import numpy

lines = open("day14_input.txt").read().splitlines()
rules = dict([l.split(' -> ') for l in lines[2:]])
ridx = dict(zip(rules, range(len(rules))))
mat = numpy.zeros((len(rules), len(rules)), dtype=object)
for p in rules:
    mat[ridx[p[0] + rules[p]]][ridx[p]] = 1
    mat[ridx[rules[p] + p[1]]][ridx[p]] = 1
tmp = list(zip(lines[0], lines[0][1:]))
v = [ tmp.count((p[0],p[1])) for p in rules ]
for pow in [ 10, 40 ]:
    w = numpy.dot(numpy.linalg.matrix_power(mat, pow), v)
    cnt = {lines[0][0]: 1}
    for p in ridx:
        cnt[p[1]] = cnt.get(p[1],0) + w[ridx[p]]
    lmin = min(cnt, key=lambda k: cnt[k])
    lmax = max(cnt, key=lambda k: cnt[k])
    print(cnt[lmax]-cnt[lmin])