<h2><b>Battaille des deux Couleurs</b></h2>

<h1><u><b>But du jeu :</b></u></h1>
- <b>Votre but</b> est de détruire le chateau adverse. Pour cela vous
pouvez produire des <b>chevaliers</b> pour attaquer et des <b>ouvriers</b> pour collecter les ressources sur la carte. 
- Les Joueurs ont <b>deux ressources</b> différentes à leur disposition. <b>L'or et le bois</b>.
- Les <b>ouvriers</b> peuvent se déplacer sur les cases <b>pierre</b> et <b>bois</b> pour les transformer en <b>Mines</b> et en <b>Scierie</b> afin de les exploiter et obtenir des ressources à tout les tours.
-Les <b>chevaliers</b> et les <b>ouvriers</b> coutent des ressources 50 bois pour les ouvriers et 100 d'or pour les chevalier.

<h1><u>__Structure :__</u></h1>
- <b>Le projet</b> est scindé en 6 fichier dans l'ordre suivant ``SDL++.cpp > object.cpp > struct.cpp > draw.cpp > game.cpp > main.cpp``
- <b>SDL++.cpp :</b> Fichier contenant toutes les fonctions utiles pour certains calculs & redéfinition des fonctions SDL.
- <b>object.cpp :</b> Fichier contenant toutes les classes d'objets relatif au jeu, <b>ex :</b> ``class Chevalier{ ... }`` 
- <b>struct.cpp :</b> Fichier contenant la constrution du <b>système de cellule</b> et de <b>plateau de jeu</b>
- <b>draw.cpp :</b> Fichier contenant toutes les fonction utilisant la SDL2 pour afficher <b>le Plateau</b>, <b>les cellules</b> et ce qui s'y trouve à l'interrieur
- <b>game.cpp :</b> Fichier contenant <b>les interractions</b> entre objets et <b>gestions des actions</b> effectuées par l'utilisateur.
- <b>main.cpp :</b> Fichier contenant toute la mise en place de la <b>SDL2</b> ainsi que <b>la boucle des évènements</b> relatif à la fenêtre de jeu.

<h1><u>__Comment compiler :__</u></h1>
- <b>Installer la SDL2 (Linux) :</b>``apt-get install libsdl2-dev``
- <b>Installer la SDL2-Image (Linux) :</b> ``apt-get install libsdl2-image-dev``
- <b>Compiler (Linux) :</b> ``g++ main.cpp $(sdl2-config --cflags --libs) && ./a.out``

