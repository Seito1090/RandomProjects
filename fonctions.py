import lsts, os, random, subprocess, smtplib, qrcode, cv2
def file(fichier): #implemented 
    fil = open(fichier, "r")
    return fil

def info(file):#implemented 
    count = 0
    caract = 0
    for line in file.readlines():
        if len(line) != 0:
            count += 1
        for crct in line:
            caract += 1
    reponse = (count, caract)
    return reponse

def dictionary(file):#implemented 
    a_dictionary = {}
    a_file = open(file)
    for line in a_file:
        key, value = line.split(',')
        value = value.strip("\n")
        a_dictionary[key] = value
    return a_dictionary

def search(word, dico):#implemented 
    if word in dico:
        return True
    else:
        return False

def position(word, dico):#implemented 
    ligne = 0
    for lignes in dico:
        if word not in lignes:
            ligne += 1
        if word in lignes:
            ligne += 1
            return ligne

def sum(liste):#implemented 
    somme = 0
    for i in liste:
        somme += int(i)
    return somme

def dif(liste):#implemented 
    r = liste[0]
    for i in range(1, len(liste)):
        r = r - liste[i]
    return r

def avg(liste):#implemented 
    somme = 0
    for i in liste:
        somme += int(i)
    moyenne = somme / len(liste)
    return moyenne

def help():#implemented 
    for i in lsts.instructions:
        print(i)

def count_files(dir):
    return len([1 for x in list(os.scandir(dir)) if x.is_file()])

def get_size_format(b, factor=1024, suffix="B"):
    """
    Scale bytes to its proper byte format
    e.g:
        1253656 => '1.20MB'
        1253656678 => '1.17GB'
    """
    for unit in ["", "K", "M", "G", "T", "P", "E", "Z"]:
        if b < factor:
            return f"{b:.2f}{unit}{suffix}"
        b /= factor
    return f"{b:.2f}Y{suffix}"

def fact(n):#implemented 
    count = 1
    for x in range(1, n+1):
        count = count*x
    if count == 1:
        return 1
    if count > 1:
        return count

def greatest_divisor(a): #implemented
    div = []
    if a == 0 or a == 1:
        return None
    else:
        for x in range(2, a):
            if a % x == 0:
                div.append(x)
        return div[-1]

def binary_search(name, list_of_names): 
  first = 0
  last = len(list_of_names)-1
  found = False

  while first<=last and not found:
    middle = (first + last)//2
    print(middle)                   
    if list_of_names[middle] == name:
      found = True
    else:
      if name < list_of_names[middle]:
        last = middle-1
      else:
        first = middle+1
  return found

def croix(a, b):
    if b % 2 != 0:
        c = b // 2 + 1
    else:
        c = b // 2
    for i in range(1, b + 1):
        if i == c:
            print(a * b)
        else:
            print(" " * (c - 1) + a)

def merge(first_list, second_list):
    list = []

    def get_second(element):
        return element[1]

    for i in first_list:
        list.append(i)
    for j in second_list:
        list.append(j)
    srt_liste = sorted(list, key=get_second)
    return srt_liste

def positions(p, s):
    s = s.lower()
    p = p.lower()
    table = []                                 
    for i in range(len(s) + 1 - len(p)):
        sum = 0  
        for j in range(len(p)):  
            if p[j] == "?":
                sum += 1
            if s[i + j] == p[j]:
                sum += 1                      
        if sum == len(p):          
            table.append(i)                     
    return table   

def triangle_Pascal(): #implemented
    try:  
        f = True
        while f:
            a = [1]
            n = int(input("How far do we go? "))+1
            c = []
            if n <= 0:
                print("Enter a positive number! ")
            else:
                f = False
        for i in range(0,n):
            if i == 0:
                c.append(a)
            else:
                b = a.copy()
                b.insert(0, 0)
                b.append(0)
                d = []
                for j in range(0, len(b)-1):
                    d.append(b[j] + b[j+1])
                c.append(d)
                a = d
        for i in c:
            for j in i:
                print(j, end=' ')
            print()
    except ValueError:
        print("Please enter a value!")

def get_words(line):
    la_rp = []
    lettres = ["a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z"]
    line = line.lower()
    line = line.split()
    for i in line:
        for j in i:
            if j not in lettres:
                i = i.translate({ord(j): None})
        la_rp.append(i)
    return la_rp
  
def create_index(filename):
    direct = {}
    file = open(filename,"r")
    l = 0 
    for i in file:
        lst = get_words(i)
        print(lst)
        for j in lst:
            print(j)
            try:
                if j not in direct:
                    direct[j] = {l: 1}
                elif j in direct:
                    print(j)
                    x = direct.get(j)[l]
                    x +=1 
                    direct[j][l] = x
            except KeyError:
                direct[j][l] = 1
        l += 1
    return direct

def plus_frequent(chaine):
    if chaine == "":
        return None
    chaine = chaine.lower()
    base = "abcdefghijklmnopqrstuvwxyz"
    occurrences = {}
    for element in chaine:
        if element in base:
            if element in occurrences:
                occurrences[element] += 1
            else:
                occurrences[element] = 1
    if occurrences == {}:
        return None
    else:
        maxi = max(occurrences, key = occurrences.get)
    return maxi

def list_duplicates(seq):
  seen = set()
  seen_add = seen.add
  seen_twice = set( x for x in seq if x in seen or seen_add(x) )
  return list(seen_twice)

def f7(seq): 
    seen = set()
    seen_add = seen.add
    return [x for x in seq if not (x in seen or seen_add(x))]

def payement(): #implemented #TODO it does fail for some unknown reason wth
    continuer = True
    while continuer:
        try:
            f = int(input("Invoices done today: "))
            d = int(input("Quotes done today: "))
            p = int(input("Blueprints done today: "))
            if f < 0 or d < 0 or p < 0:
                print(f,d,p)
                print("Negative detected")
            else:
                continuer = False
        except ValueError:
            print("Enter a value please!")
    somme = 20*f + 5*d + 30*p
    try:
        with open(r'C:\Users\witte\Desktop\my_pay.txt', 'r') as file:
            for x in file:
                for j in x.split():
                    try:
                        i = int(j) 
                    except ValueError:
                        pass 
            file.close()
        i += somme
        with open(r'C:\Users\witte\Desktop\my_pay.txt', 'w+') as file:
            file.write("{} {}".format(str(i),"€"))
            file.close()
        print("Done!")
    except FileNotFoundError:
        print("Please check if the file exists or is stored proprely!")

def dividers(): #implemented 
    n = int(input("Enter a number: "))
    m = int(n ** (0.5))
    print("\n**If nothing is printed, it means the number is a prime**\n ")
    for i in range(2, (m + 1)):
        r = n % i
        if r == 0:
            print("{} is divisible by: {} and by {}".format(n, i, int(n/i)))


def primeto(nbr): #implemented 
    nbr_premiers=[]
    for n in range(nbr+1):
        i = 2
        while i < n and n % i != 0:
            i = i + 1
        if i == n:
            nbr_premiers.append(n)
    return nbr_premiers

def isprime(nbr): #implemented 
    i = 2 
    while i< nbr and nbr % i != 0:
        i += 1 
    if i == nbr:
        return "This is a prime number"
    else:
        return "This is not a prime number"

def grps(): #implemented
    members = ["Ilija", "Benjamin", "Julien", "Gauthier","Krystian", "Mathys"]
    grp = []

    def create_grp():
        a: str = random.choice(members)
        members.remove(a)
        b: str = random.choice(members)
        members.remove(b)
        grp.append(a)
        grp.append(b)

    def clear_grp():
        grp.clear()

    while len(members) > 2:
        create_grp()
        print(grp)
        clear_grp()
    print(members)

def startmusic(la_musique): #implemented  
    SW_HIDE = 0
    info = subprocess.STARTUPINFO()
    info.dwFlags = subprocess.STARTF_USESHOWWINDOW
    info.wShowWindow = SW_HIDE
    return subprocess.Popen([r'C:\Program Files (x86)\VideoLAN\VLC\vlc.exe', la_musique], startupinfo=info)

def plus_frequent(chaine):
    if chaine == "":
        return None
    base = "abcdefghijklmnopqrstuvwxyz"
    occurrences = [0 for i in range(26)]
    for element in chaine:
        if element in base:
            if element in occurrences:
                occurrences["element"] += 1
            else:
                occurrences["element"] = 1
    maxi = max(occurrences)
    for i in range(26):
        if occurrences[i] == maxi:
            rep = base[i]
            return rep

class NoSubjectFound(Exception):
    pass 

class NoTextFound(Exception):
    pass 

def send_mail(reciever):
    mail = input('From which mail should I send it? \n')
    try:
        server = smtplib.SMTP('smtp.gmail.com', 587)
        server.ehlo()
        server.starttls()
        server.ehlo()
        subject = input('What subject should I put for this mail? \n')
        body = input('What message to deliver? \n')
        server.login(mail, lsts.mails[mail])
        msg = f"Subject : {subject}\n\n{body}"
        if len(subject) < 1:
            raise NoSubjectFound
        if len(body) < 1:
            raise NoTextFound
        else: 
            server.sendmail(
                mail,
                reciever,
                msg
            )
            print("The message was succesfully sent. ")
            server.quit()
    except KeyError:
        print("Please try again the mail entered does not exist in this data base.")
        send_mail(reciever)
    except NoTextFound:
        print("I need something to send")
        send_mail(reciever)
    except NoSubjectFound:
        print("Please enter a subject")
        send_mail(reciever)

def to_qr(name, text): #implemented
    try:
        img = qrcode.make(text)
        img.save("{}.jpg".format(name))
        return True
    except:
        return False

def from_qr(file):
    try:
        d = cv2.QRCodeDetector()
        val, _, _ = d.detectAndDecode(cv2.imread(file))
        return val
    except FileExistsError:
        return "File not found, please try again."
    except FileNotFoundError:
        return "File not found, please try again."