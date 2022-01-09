# Projet de jeu video : TSI

ATTENTION SI IL MANQUE LES LIBRAIRIES GLUT ET GLEW, COPIER COLLER LE DOSSIER EXTERN DES TUTORIAUX DANS LE DOSSIER PRINCIPAL !

## Compilation

Pour compiler et executer à partir du CMakeLists.txt en ligne de commande (Linux et macOs)

```sh
mkdir build
cd build
cmake ..
make
cd ..
./build/projet
```

ou bien en utilisant directement `script.sh`


## TODO : 

- [x] Augmentation de vitesse de caméra ?
- [x] Limiter le déplacement vertical de la caméra en haut et en bas
- [x] Déplacement fluide
- [x] Déplacement avec plusieurs touches en même temps
- [x] Déplacement ZQSD & flèches directionnelles ?
- [x] Saut
- [x] Implémenter les mains.obj
- [x] Associer déplacement objet et caméra
- [x] Interaction avec objet
- [x] Creation décor
- [ ] Detection obstacle
- [x] Interface objet - Menus
- [x] Lumière directionnelle (lampe torche)
- [x] Gestion du texte
- [ ] Ajout de musiques
- [x] Ajout d'un coffre bleu, d'un coffre rouge et d'un coffre vert
- [x] Rendre l'échéquier selectionnable
- [x] Enigme pour la biblothèque
- [x] Compléter la bibliothèque


## Liens utiles / Sources :

https://www.lighthouse3d.com/tutorials/glut-tutorial/the-code-so-far-ii/

http://freeglut.sourceforge.net/docs/api.php
