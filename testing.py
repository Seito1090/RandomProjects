# import subprocess
# nbr_musique = fct.count_files("/Users/witte/Music/My_music")

# import os
# import subprocess

# import tkinter
# import tkinter.messagebox as tkMessageBox
# import os

# top = tkinter.Tk()

# def helloCallBack():
#     def musical():
#         la_musique = r'C:\Users\witte\Music\My_music\Nightcore - Pop Danthology 2010-2013.mp3'
#         musique = subprocess.Popen([r'C:\Program Files (x86)\VideoLAN\VLC\vlc.exe', la_musique])
#         while True:
#             x = input('yeet ? ')         
#             if x == 'yeet indeed':
#                 musique.terminate()

# B = tkinter.Button(top, text ="Hello", command = helloCallBack)
# # C = tkinter.Button(top, text ="Music", command = musical, pady = 1)

# B.pack()
# # C.pack()
# top.mainloop()

# os.startfile("le_doc.txt", "print")
# import speech_recognition as sr
#  for index, name in enumerate(sr.Microphone.list_microphone_names()):
#      print("Microphone with name \"{1}\" found for `Microphone(device_index={0})`".format(index, name))

# def main():
#     r = sr.Recognizer()
    
#     with sr.Microphone() as source:
#         r.adjust_for_ambient_noise(source)
#         print("Alright say smth")
#         audio = r.listen(source)
#         try: 
#             print("You've just said: \n"+r.recognize_google(audio))
#         except: 
#             print("Some error happened")

# if __name__ == "__main__":
#     main()


# lst = [0,1,2,3,4,5,6,7,8,9,10,"a"]

# for i in range(len(lst)):
#     lst.remove(lst[0])

# print(lst)
# import requests
# supportedSites = {}
# item = input("Please enter the product you want to check ")
# site = input("Where do I have to search ? ")
# user_agent = 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/88.0.4324.104 Safari/537.36'
# if site not in supportedSites:
#     print("Sorry I can't go to that site, chose another")

# def price_check(item):
#     pass 

# def product_check(item):
#     pass 

# print(price_check())

# import smtplib

# mails = {"sktpiko@gmail.com": "qckjtmtmedowqmfn", "siphano150@gmail.com": "tbsjroghgvlfdfyi"}

# class NoSubjectFound(Exception):
#     pass 

# class NoTextFound(Exception):
#     pass 

# def send_mail(reciever):
#     try:
#         server = smtplib.SMTP('smtp.gmail.com', 587)
#         server.ehlo()
#         server.starttls()
#         server.ehlo()
#         subject = input('What subject should I put for this mail? ')
#         body = input('What message to deliver? ')
#         mail = input('From which mail should I send it? ')
#         server.login(mail, mails[mail])
#         msg = f"Subject : {subject}\n\n{body}"
#         if len(subject) < 1:
#             raise NoSubjectFound
#         if len(body) < 1:
#             raise NoTextFound
#         else: 
#             server.sendmail(
#                 mail,
#                 reciever,
#                 msg
#             )
#             print("The message was succesfully sent. ")
#             server.quit()
#     except KeyError:
#         print("Please try again the mail entered does not exist in this data base.")
#         send_mail(reciever)
#     except NoTextFound:
#         print("I need something to send")
#         send_mail(reciever)
#     except NoSubjectFound:
#         print("Please enter a subject")
#                                             # possible errors: 
#                                             # mail not found, just ask again and make sure to tell the user how dum he is :)
#                                             # no message entered, same 

# send_mail("pauline.hanssens@student.uclouvain.be")
# # send_mail("andrzejtargan@op.pl")
# # import random 
# # list = ["Krystian","Benjamin", "Célia", "Mariya", "Grégoire" ]
# # while list != []:
# #     nom = random.choice(list)
# #     list.remove(nom)
# #     print(nom)
def files_are_equal(file1, file2):
    with open(file1, 'r') as f1, open(file2, 'r') as f2:
        contents1 = f1.read()
        contents2 = f2.read()
        if contents1 == contents2:
            return True
        else:
            return False
        
def read_binary_file(file_path1, file_path2):
    with open(file_path1, 'rb') as f1, open(file_path2, 'rb') as f2:
        while True:
            # Read 4 bytes in big-endian byte order
            data1 = f1.read(4)
            data2 = f2.read(4)
            if not data1 or not data2:
                break
            # Unpack the 4 bytes as a big-endian integer
            value1 = int.from_bytes(data1, byteorder='big')
            value2 = int.from_bytes(data2, byteorder='big')
            # Print the integer value
            print("val py : ", value1 ," val c : " , value2)

read_binary_file("bellmanford_c/v3/tests/output_tests/default_c.bin","bellmanford_c/v3/multithreadtest.bin")
print(files_are_equal("bellmanford_c/v3/tests/output_tests/default_c.bin","bellmanford_c/v3/multithreadtest.bin"))