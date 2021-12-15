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
- [ ] Implémenter les mains.obj
- [ ] Associer déplacement objet et caméra
- [ ] Interaction avec objet
- [x] Creation décor
- [ ] Detection obstacle
- [ ] Interface objet ?
- [x] Lumière directionnelle (lampe torche)
- [x] Lumière ambiante RVB


## Liens utiles / Sources :

https://www.lighthouse3d.com/tutorials/glut-tutorial/the-code-so-far-ii/

http://freeglut.sourceforge.net/docs/api.php
