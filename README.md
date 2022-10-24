# **Battaille des deux Couleurs**

## But du jeu :
- **Votre but** est de détruire le château adverse. Pour cela vous pouvez produire des <b>chevaliers</b> pour attaquer et des <b>ouvriers</b> pour collecter les ressources sur la carte.
- Les Joueurs ont **deux ressources** différentes à leur disposition. **L'or et le bois**.
- Les **ouvriers** peuvent se déplacer sur les cases **pierre** et **bois** pour les transformer en **Mines** et en **Scierie** afin de les exploiter et obtenir des ressources à tous les tours.
-Les **chevaliers** et les **ouvriers** coûtent des ressources 50 bois pour les ouvriers et 100 d'or pour les chevalier.

![Exemple du jeu](/projet/assets/jeu.png)

## Structure du Projet :
- **Le projet** est scindé en 6 fichier dans l'ordre suivant ```SDL++.cpp > object.cpp > struct.cpp > draw.cpp > game.cpp > main.cpp```
- `SDL++.cpp` : Fichier contenant toutes les fonctions utiles pour certains calculs & redéfinition des fonctions SDL.<br>
- `object.cpp `: Fichier contenant toutes les classes d'objets relatif au jeu, **ex :** `class Chevalier{ ... }`
- `struct.cpp` : Fichier contenant la constrution du **système de cellule** et de **plateau de jeu**.
- `draw.cpp` : Fichier contenant toutes les fonction utilisant la SDL2 pour afficher **le Plateau**, **les cellules** et ce qui s'y trouve à l'interrieur.
- `game.cpp` : Fichier contenant **les interractions** entre objets et **gestions des actions** effectuées par l'utilisateur.
- `main.cpp` : Fichier contenant toute la mise en place de la **SDL2** ainsi que **la boucle des évènements** relatif à la fenêtre de jeu.

## Comment compiler :
- **Installer la SDL2**  :`apt-get install libsdl2-dev`
- **Installer la SDL2-Image** : `apt-get install libsdl2-image-dev`
- **Compiler** : `g++ main.cpp $(sdl2-config --cflags --libs) && ./a.out`

## Les contrôles :
- **Clic gauche** : Permet de produire un chevalier quand il est effectué sur le chateau.
- **Clic droit** : Permet de produire un ouvrier quand il est effectué sur le chateau. Permet de déplacer les unités sur les cases ressources et les cases vides. Permet d'attaquer les autre unités lorsque qu'il est effectué depuis un chevalier vers une autre unité<br>
- <b>Clic mollette :</b> Permet de passer son tour quand il est effectué sur le chateau<br>
