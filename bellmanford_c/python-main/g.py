import matplotlib.pyplot as plt
import numpy as np
#sans affichage du résultat
python = [0.000441,0.230789,1.545829,24.265549,202.015765]
c = [0.000131,0.003378,0.007286,0.030500,0.109232]
node_values = [10,100,200,500,1000]
f = [0,0,0,0,0]

plot = plt.figure()
plot.suptitle('Comparaison temps d\'execution du code C et Python sans affichage du résultat')
plt.plot(node_values, python,label='Python')
plt.plot(node_values, c, label='C')
plt.xticks(node_values)
plt.xlabel('Nombre de noeuds')
plt.ylabel('Temps d\'execution [s]')
plt.legend()
plt.show()