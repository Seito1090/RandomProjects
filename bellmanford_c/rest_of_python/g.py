import matplotlib.pyplot as plt
import numpy as np
#sans affichage du résultat
python = [451329,691532,1446172,192154021]
c1 = [14280,130760,492360,12025160]
c2 = [19788,136268,497868,12030668]
c4 = [20532,137012,498612,12031412]
c10 = [22764,139244,500844,12033644]
c100 = [27444,156724,534324,12037124]
c200 = [32644,161924,539524,12104324]
node_values = [10,50,100,500]

plot = plt.figure()
plot.suptitle('Comparaison de la mémoire utilisée par le code en Python et C')
plt.plot(node_values, python,label='Python')
plt.plot(node_values, c1, label='C single thread')
plt.plot(node_values, c2, label='C 2 threads')
plt.plot(node_values, c4, label='C 4 threads')
plt.plot(node_values, c10, label='C 10 threads')
plt.plot(node_values, c100, label='C 100 threads')
plt.plot(node_values, c200, label='C 200 threads')
plt.yscale('log')
""" plt.xscale('log') """
plt.xticks(node_values)
plt.ylabel('Mémoire utilisée [bytes]')
plt.xlabel('Nombre de noeuds')
plt.legend()
plt.show()