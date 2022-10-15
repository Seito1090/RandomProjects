lines = [i.strip() for i in open("day10_input.txt","r")]
opening_signs = ["(","[","{","<"]
illegal_syntax = {")" : 3, "]" : 57, "}" : 1197, ">" : 25137}
auto_complete_syntax = {")" : 1, "]" : 2, "}" : 3, ">" : 4}
correct_binome = {"(" : ")", "[" : "]", "{" : "}", "<" : ">" }

def validity_check(string):
    if string[-1] == correct_binome[string[-2]]:
        string = string[:-2]
    else:
        return False, string, illegal_syntax[string[-1]]
    return True, string 

def analyse(string):
    """
    Pass a string to analyse it
    Return False if corrupted 
    Return True if not corrupted
    """
    to_check = ""
    for i in string:
        to_check += i
        if i not in opening_signs:
            test = validity_check(to_check)
            if not test[0]:
                return False,test[2]
            to_check = test[1]
    return True, to_check         

def auto_complete(string):
    """
    Pass in an incomplete line
    Returns the score to complete it 
    """
    points = 0
    symbols = [i for i in string]
    symbols.reverse()
    for j in symbols:
        points *= 5 
        points += auto_complete_syntax[correct_binome[j]]
    return points 

syntax_error_score = 0 
auto_complete_scores = []
for i in lines:
    score = analyse(i)
    if not score[0]:            #here it is corrupted
        syntax_error_score += score[1]
    if score[0]:
        auto_complete_scores.append(int(auto_complete(score[1])))
print(syntax_error_score)
auto_complete_scores.sort()
middle = (len(auto_complete_scores)-1)//2
print(auto_complete_scores[middle])