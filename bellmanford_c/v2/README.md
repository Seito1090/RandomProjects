# Readme_Groupe_M4

## Algorithme de Bellman-Ford en C

***

Le programme de ce projet implemente l'algorithme de Bellman-Ford, il nous permet de trouver le chemin le plus court / moins coûteu entre 2 noeuds d'un graphe. 
Il est base sur un code python qui nous a ete fourni

### Comment lancer le programme

Il faut avoir les outils/librairies suivantes pour eviter tout soucis de compilation : CUnit et Python 

Pour lancer le programme, il faut d'abord le compiler, voici quelques commandes utiles pour le faire :
 - **make** / **make sp** : Permet de compiler le programme de façon à ce qu'il puisse être utilise, cela cree un executable 'main' que vous pouvez executer, pour plus d'infos utiliser ./main -h
 - **make tests** : Permet d'executer une serie de tests pour s'assurer que le programme fonctionne bien comme attendu, ceci compile et execute les tests CUnit et fait passer le programme par valgrind pour vous assurer qu'aucune fuite de memoire est presente
 - **make clean** : Supprime tout les fichiers supplementaires qui ont etes crees lors de l'utilisation des 2 commandes precedentes 

Pour des soucis de clarete utiliser toujours **make clean** après avoir fini d'utiliser le programme, le makefile fait en sorte que peu importe l'ordre du **make/make sp/make tests**, il ne soit pas necessaire de faire autre chose.  

### Utilisation de la fonction main 

La fonction main prend comme arguments : 
 - *-n* : Le nombre de threads que le programme va utiliser 
 - *-v* : Affichage des messages d'erreur, false par defaut
 - *-f* : Le fichier dans lequel les donnee seront sauvegardees
 - *-?* / *-h* : Affiche une fenetre avec cette explication 
