# Readme_Groupe_M4

## Algorithme de Bellman-Ford en C

***

Le programme de ce projet implémente l'algorithme de Bellman-Ford, il nous permet de trouver le chemin le plus court / moins coûteu entre 2 noeuds d'un graphe. 
Il est basé sur un code python qui nous a été fourni

### Comment lancer le programme

Il faut avoir les outils/librairies suivantes pour éviter tout soucis de compilation : CUnit et Python 

Pour lancer le programme, il faut d'abord le compiler, voici quelques commandes utiles pour le faire :
 - **make** / **make sp** : Permet de compiler le programme de façon à ce qu'il puisse être utilisé, cela crée un executable 'main' que vous pouvez executer, pour plus d'infos utiliser ./main -h
 - **make tests** : Permet d'executer une série de tests pour s'assurer que le programme fonctionne bien comme attendu
 - **make clean** : Supprime tout les fichiers supplémentaires qui ont étés crées lors de l'utilisation des 2 commandes précédentes 

Pour des soucis de clareté utiliser toujours **make clean** après avoir fini d'utiliser le programme 

#### Utilisation de la fonction main 

La fonction main prend comme arguments : 
 - *-n* : Le nombre de threads que le programme va utiliser 
 - *-v* : Affichage des messages d'erreur, false par défaut
 - *-f* : Le fichier dans lequel les donnée seront sauvegardées
 - *-?* / *-h* : Affiche une fenetre avec cette explication 