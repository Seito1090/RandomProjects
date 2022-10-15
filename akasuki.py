#Krystian Targonski ver 0.1 30/10/2020
import math, os, random, subprocess, webbrowser, turtle, psutil, lsts, smtplib
import speech_recognition as sr
import fonctions as fct
continuer = True
dir = r'C:\Users\witte\Music\My_music'
r = sr.Recognizer()
try:
    print("\n"+lsts.greetings[random.randrange(len(lsts.greetings))])
    print("\n"+lsts.second_greetings[random.randrange(len(lsts.second_greetings))])
except FileNotFoundError:
    print("I hope that I can be usefull")

while continuer:
    try:
        command = input("> ")
        parameters = command.split()
        if parameters[0] == "speech":
            with sr.Microphone() as source:
                r.adjust_for_ambient_noise(source)
                print("Oh ok, I'm listening then!")
                audio = r.listen(source)
                try: 
                    command = r.recognize_google(audio)
                    print("> {}".format(command))
                    parameters = command.split()
                except Exception as e: 
                    print("Some error happened", str(e))            
        if parameters[0] == "file":
            fichier = parameters[-1]
            if parameters[1] == "open":
                try:
                    x = fct.file(fichier)
                    print("\n"+"The file is now loaded")
                except FileNotFoundError:
                    print("\n"+"Please enter a valid file name and the location of the file")
            if parameters[1] == "info":
                try:
                    if type(fichier) == dict:
                        print("\n"+"You're trying to write in a dictionary, please reopen the file")
                    else:
                        l = fct.info(x)
                        lignes = l[0]
                        caracteres = l[1]
                        print("\n"+"Your file has", lignes, "lines and", caracteres, "characters")
                        x.seek(0)
                except NameError:
                    print("\n"+"Please open a file first")
                except TypeError:
                    print("\n"+"An error has occured, please reopen the file")
            if parameters[1] == "dictionary":
                try:
                    fichier = fct.dictionary(fichier)
                    print("\n"+ "You are now using the file as a dicitonary")
                except NameError:
                    print("\n"+"Please open a file first")
                except ValueError:
                    print("\n"+"Votre fichier est mal formaté ou vide, vérifiez le")
                except FileNotFoundError:
                    print("\n"+"Please select a file first")
            if parameters[1]=="search":
                try:
                    if type(fichier) != dict:
                        print("\n"+"I need to have a dicitonary to be able to tell you that")
                    else:
                        word = parameters[1]
                        in_list = fct.search(word, fichier)
                        if len(parameters) > 2:
                            print("Please check you syntax, only one word can be searched at a time")
                        else:
                            if in_list == True:
                                w_pos = fct.position(word, fichier)
                                word = word.capitalize()
                                if w_pos > 10000:
                                    print("\n"+ word, "is in the dictionary at the line n°"+ str(w_pos)+
                                    ". However it's quite far if you're going to search for it good luck :)") 
                                else:     
                                    print("\n"+ word, "is in the dictionary at the line n°"+ str(w_pos)) 
                            else:
                                print("\n"+ word, "is not in the dictionary")
                except TypeError:
                    print("\n"+"Please give me only letters not anything else")
            else:
                print("Please enter a valid file command")
        elif parameters[0] == "operations" or parameters[0] == "math":
            try:
                numbers = []
                if len(parameters) <= 2:
                    if parameters[1] == "Newton":
                        fct.triangle_Pascal()
                    elif parameters[1] == "dividers":
                        fct.dividers()
                    else:
                        print("\n"+"You have to give me numbers to use :(")
                else:
                    for i in range(2, len(parameters)):
                        try:
                            if i == 0:
                                pass
                            else:
                                numbers.append(int(parameters[i]))
                        except ValueError:
                            pass
                    if parameters[1] == "avg":
                        moyenne = fct.avg(numbers)
                        print("\n"+"The average is", moyenne)
                    if parameters[1] == "fact":
                        print(fct.fact(int(parameters[2])))
                    if parameters[1] == "prime":
                        try:
                            print(fct.isprime(int(parameters[2])))
                        except ValueError:
                            print(fct.primeto(int(parameters[3])))
                    if parameters[1] == "greatestdivisor":
                        print(fct.greatest_divisor(int(parameters[2])))
            except ValueError:
                print("\n"+"You have to give me numbers to use :(")
        elif parameters[0] == "music":
            if parameters[1] == 'play':
                if parameters[2] == "random":
                    la_musique = dir + "\\" + random.choice([x for x in os.listdir(dir) 
                    if os.path.isfile(os.path.join(dir, x))])
                    musique = fct.startmusic(la_musique) 
                elif parameters[2] == 'favourites':
                    la_musique = r'C:\Users\witte\Music\My_music\Nightcore - Pop Danthology 2010-2013.mp3'
                    musique = fct.startmusic(la_musique) 
                elif parameters[2] == "next":
                    musique.terminate()
                    la_musique = dir + "\\" + random.choice([x for x in os.listdir(dir) 
                    if os.path.isfile(os.path.join(dir, x))])
                    musique = fct.startmusic(la_musique) 
                elif parameters[2] == "file":
                    try:
                        titre = parameters[3]
                        dups = []
                        pos = 0
                        for x in os.listdir(dir):
                            if titre in x:
                                rep = (pos, x)
                                dups.append(rep)
                                pos += 1
                        if len(dups) > 1:
                            print("Which one do you want ? ")
                            for i in dups:
                                print(i)
                            choice = int(input(""))
                            msq = dups[choice][1]
                        if len(dups) == 1:
                            msq = dups[0][1]
                        la_musique = dir + "\\" + msq
                        musique = fct.startmusic(la_musique) 
                    except NameError:
                        print("Check the spelling, I couldn't find the file")
                else:
                    print("Check the syntax and spelling")
            elif parameters[1] == 'stop' :
                musique.terminate()
            else:
                print("\n"+"Wrong syntax")
        elif parameters[0] == "web":
            if parameters[1] == "open": 
                link = "{}.{}.{}".format('https://www', parameters[2] ,'com')
                print("Opening {}".format(link))
                webbrowser.open_new(link)
            elif parameters[1] == 'search' :
                link = "{}=".format('https://www.google.com/search?q')
                if len(parameters) > 3:
                    for i in range(2, len(parameters)):
                        link+= " "+parameters[i]
                else: 
                    link+=parameters[2]
                webbrowser.open_new(link)
            else:
                print("Please enter a valid syntax and URL")
        elif parameters[0] == "work":
            fct.payement()
        elif parameters[0] == "mail":
            reciever = input("Who do you want to send it to? \n")
            fct.send_mail(reciever)
        elif parameters[0]== "groups":
            fct.grps()
        elif parameters[0] == "play":                                 #here to finish 
            for i in range(len(lsts.games)):
                print("{} | {} ".format(i, lsts.games[i]))
            game = input("Which game do you want to play? \n")
        elif parameters[0] == "help":
            if  len(parameters) == 1:
                print('')
                fct.help()
                print('')
            else:
                if  parameters[1] == "python":
                    print('')
                    help()
                    print('')
        elif parameters[0] == "exit":
            try:
                musique.terminate()
            except:
                pass
            print("\n"+"Glad I could help ! See you next time !")
            continuer = False
        elif parameters[0] == "create":
            if parameters[1] == "code":
                name = parameters[2]
                txt_in = input("What should I make the code lead to ?: ")
                extra = input("Should it lead somewhere special ? Y/N")
                if extra.upper() == 'Y':
                    pass
                else:
                    txt_out = "https://www."+txt_in+".com"
                try:
                    fct.to_qr(name,txt_out)
                except:
                    print("There's been an error :(")
            elif parameters[1] == "decode":
                file = input('Which QRcode do I have to decode ?: ')
                txt_qr = fct.from_qr(file)
                print(txt_qr)
        elif parameters[0] == "hi":
            print("Hi there ! Hope I can be usefull for you, if not you can yell at my creator :)")
        else:
            print("\n"+"Please enter a valid command with a valid syntax, for more info enter > help")
    except IndexError:
        print("\n"+"Please enter a valid command with a valid syntax, for more info enter > help")
    