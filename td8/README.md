# Description
L'archive td8-contents.tar contient les fichiers suivants :
- le fichier README.md, que vous êtes en train de lire ;
- le fichier lottery.cfg, fichier de configuration exemple que vous pouvez
  donner au serveur pour lancer une lotterie, sa description est donnée
  plus bas ;
- le fichier scenario-test.sh (exécutable), script de test effectuant un
  scénario simple.

# Utilisation
Pour utiliser directement le script de test, veuillez le lancer dans le même
répertoire que votre application, appelée 'million' :
```
$ ./scenario-test.sh
```

Sinon, vous pouvez changer la valeur de la variable EXEC, pour cibler votre
exécutable. **ATTENTION, les scénarii cibleront un exécutable nommé 'million'.**

# Configuration
Le fichier de configuration est détaillé ici :
```
4                   # nombre de numéros
6 25 18 39          # numéros gagnants
4 9865              # gain pour 4 chiffres
3 345               # gain pour 3 chiffres
2 19                # gain pour 2 chiffres
1 2                 # gain pour 1 chiffre
```

# @autor LE DENMAT Mickael
  @version 04/04/2020
  Groupe TD4: Salle de TD:
  Jungle - Descart, Mardi matin 9h40 12h50

# Description
This archive LE_DENMAT_Mickeal.tar contains the following files :

- the README file you are reading;
- the se_fichier.c file, library source that is use in the program;
- the se_fichier.h file, library header, describing the functions used in the program;
- the million.c file, where are wrote the functions to solve the program;
- the makefile, where are the commands to compile the program and delete the executable.

To start the program properly do a "make".
