# Readme_Groupe_M4

## Algorithme de Bellman-Ford en C

***

Le programme de ce projet implemente l'algorithme de Bellman-Ford, il nous permet de trouver le chemin le plus court / moins coûteu entre 2 noeuds d'un graphe. 
Il est base sur un code python qui nous a ete fourni

### Comment lancer le programme

Il faut avoir les outils/librairies suivantes pour eviter tout soucis de compilation : CUnit, Valgrind, GCC 

Pour lancer le programme, il faut d'abord le compiler, voici quelques commandes utiles pour le faire :
 - **make** / **make sp** : Permet de compiler le programme de façon à ce qu'il puisse être utilise, cela cree un executable 'main' que vous pouvez executer, pour plus d'infos utiliser ./main -h
 - **make tests** : Permet d'executer une serie de tests pour s'assurer que le programme fonctionne bien comme attendu, ceci compile les tests CUnit et les fait passer par valgrind pour vous assurer qu'aucune fuite de memoire est presente, ensuite le programme main est compile si ce n'est pas encore fait et fait passer main par valgrind pour verifier les fuites de memoire.
 - **make clean** : Supprime tout les fichiers supplementaires qui ont etes crees lors de l'utilisation des 2 commandes precedentes 

Pour des soucis de clarete utiliser toujours **make clean** après avoir fini d'utiliser le programme, le makefile fait en sorte que peu importe l'ordre du **make/make sp/make tests**, il ne soit pas necessaire de faire autre chose.  

### Utilisation de la fonction main 

La fonction main prend comme arguments : 
 - *-n* : Le nombre de threads que le programme va utiliser 
 - *-v* : Affichage des messages d'erreur, false par defaut
 - *-f* : Le fichier dans lequel les donnee seront sauvegardees
 - *-?* / *-h* : Affiche une fenetre avec cette explication 
 - Le nom du fichier avec le graph

### Utilisation de graph personnels

Ce projet vous permet de créer votre propre graphe, d'executer le programme sur ce graph et de vérifier que cette réponse est correcte. Vous avez 2 possibilites pour le faire :
 - Vous avez deja votre fichier binaire ? Pas de soucis vous n'avez qu'a executer le programme main avec le chemin vers votre fichier comme parametre, ensuite une fois fait, utiliser la commande : python tests/necessary_python/check_graph.py *chemin vers votre fichier binaire*, pour finir lancez le programme cunit_tests avec les 2 outputs comme parametres et il vous confirmera si les 2 fichiers outputs sont identiques ou non.
 - Si vous n'avez pas de fichier binaire ? Vous pouvez utiliser le code python qui a ete fourni pour en creer un ! Vous autrez besoin de rentrer votre graph dans un fichier ntf avec une structure : *node_from node_to cost* (voir les exemples dans le dossier tests/ntf_files) et ensuite executer le code python avec la ligne suivante : python tests/necessary_python/create_graph.py *chemin vers fichier*. Une fois fait executer le programme main normalement et suiver les instructions du point precedent.

Vos fichiers ne seront par contre pas supprimes si vous utilisez la commande **make clean**.