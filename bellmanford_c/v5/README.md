# Readme_Groupe_M4

## Algorithme de Bellman-Ford en C

***

Le programme de ce projet implemente l'algorithme de Bellman-Ford, il nous permet de trouver le chemin le plus court / moins coûteu entre 2 noeuds d'un graphe. 
Il est basé sur un code python qui nous a ete fourni et vise à améliorer les performances de ce dernier avec une approche multi-thread.

### Comment lancer le programme

Il faut avoir les outils/librairies suivantes pour eviter tout soucis de compilation : CUnit, Valgrind, GCC 

Pour lancer le programme, il faut d'abord le compiler, voici quelques commandes utiles pour le faire :
 - **make sp** : Permet de compiler le programme de façon à ce qu'il puisse être utilisé, cela cree un executable 'main' que vous pouvez executer, pour plus d'infos sur le programme main utiliser ./main -h
 - **make tests** : Permet d'executer une serie de tests pour s'assurer que le programme fonctionne bien comme attendu, ceci compile les tests CUnit et les fait passer par valgrind pour vous assurer qu'aucune fuite de memoire est presente, ensuite le programme main est compile si ce n'est pas encore fait et fait passer main par valgrind pour verifier les fuites de memoire ainsi que la synchronisation entre threads.
 - **make clean** : Supprime tout les fichiers supplementaires qui ont etes crees lors de l'utilisation des 2 commandes precedentes 

Pour des soucis de clarete utiliser toujours **make clean** après avoir fini d'utiliser le programme, le makefile fait en sorte que peu importe l'ordre des commandes, il ne soit pas necessaire de faire autre chose pour faire fonctionner ce que vous voulez.  

### Utilisation de la fonction main 

La fonction main prend comme arguments : 
 - *-n* : Le nombre de threads que le programme va utiliser 
 - *-v* : Affichage des messages d'erreur, false par defaut
 - *-f* : Le fichier dans lequel les donnee seront sauvegardees
 - *-?* / *-h* : Affiche une fenetre avec cette explication 
 - Le nom du fichier avec le graph

### Utilisation du Makefile 

Le Makefile a été fortemment modifié afin de faire face à toutes les fonctionnalités que nous avons jugés utiles, il est donc important de savoir comment l'utiliser.

Premièrement, si à tout moments vous avez besoin d'aide, vous pouvez utiliser la commande **make help** ou bien simlpement **make** pour avoir une liste des commandes disponibles.
Celles-ci sont les suivantes :
 - **make sp** : compile le programme principal
 - **make run** : compile et execute le programme principal avec les paramètres par défaut
 - **make time** : compile et execute le programme principal avec plusieurs nombres de threads et affiche le temps d'execution pour chacun d'entre eux
 - **make myfile** : compile et execute le programme principal avec un fichier d'entrée personnalisé (voir plus bas)
 - **make mygraph** : compile et execute le programme principal avec un graphe personnalisé (voir plus bas)
 - **make tests** : compile et execute les tests unitaires
 - **make clean** : supprime les fichiers compilés et les fichiers de sortie
 - **make help** : affiche l'aide

### Utilisation de graph personnels

Ce projet vous permet de créer votre propre graphe, d'executer le programme sur ce graph et de vérifier que cette réponse est correcte. Vous avez 2 possibilites pour le faire :
 - Vous avez deja votre fichier binaire ? 
    Utilisez simplement la commande **make myfile**, celle-ci vous demandera le nom/chemin du fichier binaire que vous voulez utiliser, puis elle executera le programme sur ce fichier et enregistrera le résultat dans le fichier myGraphOut.bin qui sera stocké dans le dossier outputs. 

 - Si vous n'avez pas de fichier binaire ? 
    Mettez votre graphe dans un fichier ntf avec la bonne structure (plus d'infos dans le dossier ntf_files) et utiliser la commande **make mygraph**. ATTENTION : il est important de noter que votre fichier ntf doit se trouver dans le dossier ntf_files, si non le programme ne le trouvera pas. Cette commande vous demandera le nom de votre fichier et se chargera de tout le reste, elle crée le fichier binaire, execute le programme et enregistre le résultat dans le fichier myGraphOut.bin qui sera stocké dans le dossier outputs. 

Remarques : 
 - Vos fichiers ne seront pas supprimés en utilisant la commande **make clean** si vous les mettez dans le dossier outputs, pour se faire, spécifier le simplement quand vous passer le nom du fichier de sortie désiré en mettant le chemin.
 - Une limitation des 2 commandes (make mygraph et make myfile) est que le programme sera toujours executé avec le nombre de threads par défaut (4). Si vous voulez changer cela, vous devez executer le programme manuellement avec la commande **./main** en spécifiant tout ce qui est necessaire (voir plus haut).

