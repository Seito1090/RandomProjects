import matplotlib.pyplot as plt
import numpy as np
#sans affichage du résultat
""" python = [451329,691532,1446172,192154021]
c1 = [14280,130760,492360,12025160]
c2 = [19788,136268,497868,12030668]
c4 = [20532,137012,498612,12031412]
c10 = [22764,139244,500844,12033644]
c100 = [27444,156724,534324,12037124]
c200 = [32644,161924,539524,12104324]
node_values = [10,50,100,500]
""" 
""" python = [0.171,18.11,293.329,2988,48569.26]
c1 = [0.005,0.034,0.226,28.328,30.26]
c2 = [0.006,0.022,0.123,13.684,14.749]
c4 = [0.006,0.016,0.071,6.958,7.323]
c10 = [0.007,0.016,0.073,7.338,7.517]
c100 = [0.007,0.02,0.085,7.192,7.434]
c200 = [0.007,0.02,0.080,7.252,7.435]
node_values = [10,50,100,500,1000] """
python = [7.125*10**-3,7.545*10**-1,0.012*10**3,0.59*10**3]
c1 = [1.944*10**-4,1.322*10**-3,8.789*10**-3,9.44*10**-1]
c2 = [3.667*10**-4,1.344*10**-3,7.517*10**-3,8.58*10**-1]
c4 = [6.333*10**-4,1.69*10**-3,7.5*10**-3,7.377*10**-1]
c10 = [7.389*10**-4,1.69*10**-3,7.7*10**-3,7.713*10**-1]
c100 = [7.389*10**-4,2.111*10**-3,8.97*10**-3,7.687*10**-1]
c200 = [7.389*10**-4,2.111*10**-3,8.44*10**-3,7.6*10**-1]
node_values = [10,50,100,500]

plot = plt.figure()
plot.suptitle("Comparaison de l'énergie consommée en fonction du nombre de noeuds")
plt.plot(node_values, python,label='Python')
plt.plot(node_values, c1, label='C single thread')
plt.plot(node_values, c2, label='C 2 threads')
plt.plot(node_values, c4, label='C 4 threads')
plt.plot(node_values, c10, label='C 10 threads')
plt.plot(node_values, c100, label='C 100 threads')
plt.plot(node_values, c200, label='C 200 threads')
# plt.xscale('log')
plt.yscale('log')
""" plt.xscale('log') """
plt.xticks(node_values)
plt.ylabel("Energie consommée [mWh]")
plt.xlabel('Nombre de noeuds')
plt.legend()
plt.show() 
""" """
""" import matplotlib.pyplot as plt
import numpy as np
#sans affichage du résultat
python = [0.000441,0.230789,1.545829,24.265549,202.015765]
c = [28.328,13.684,6.958,7.338,7.192,7.252]
node_values = [10,100,200,500,1000]
threads_values = [1,2,4,10,100,200]
f = [0,0,0,0,0,0]

plot = plt.figure()
plot.suptitle("Comparaison du temps d'execution du code en C en fonction du nombre de threads")
plt.plot(node_values, python,label='Python')
plt.plot(threads_values, c, '-ro', color="b", label='Graph de 500 noeuds et 250 000 liens')
plt.xticks(node_values)
plt.xscale('log')
plt.xlabel('Nombre de threads')
plt.ylabel('Temps d\'execution [s]')
plt.legend()
plt.show() """