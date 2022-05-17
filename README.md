__<h2>Battaille des deux Couleurs</h2>__

####<u>**But du jeu :**</u>
- **Votre but** est de détruire le chateau adverse. Pour cela vous
pouvez produire des __chevaliers__ pour attaquer et des __ouvriers__ pour collecter les ressources sur la carte. 
- Les Joueurs ont __deux ressources__ différentes à leur disposition. __L'or et le bois__.
- Les __ouvriers__ peuvent se déplacer sur les cases __pierre__ et __bois__ pour les transformer en __Mines__ et en __Scierie__ afin de les exploiter et obtenir des ressources à tout les tours.
-Les __chevaliers__ et les __ouvriers__ coutent des ressources 50 bois pour les ouvriers et 100 d'or pour les chevalier.

####<u>**Structure :**</u>
- __Le projet__ est scindé en 6 fichier dans l'ordre suivant ``SDL++.cpp > object.cpp > struct.cpp > draw.cpp > game.cpp > main.cpp``
- __SDL++.cpp :__ Fichier contenant toutes les fonctions utiles pour certains calculs & redéfinition des fonctions SDL.
- __object.cpp :__ Fichier contenant toutes les classes d'objets relatif au jeu, __ex :__ ``class Chevalier{ ... }`` 
- __struct.cpp :__ Fichier contenant la constrution du __système de cellule__ et de __plateau de jeu__
- __draw.cpp :__ Fichier contenant toutes les fonction utilisant la SDL2 pour afficher __le Plateau__, __les cellules__ et ce qui s'y trouve à l'interrieur
- __game.cpp :__ Fichier contenant __les interractions__ entre objets et __gestions des actions__ effectuées par l'utilisateur.
- __main.cpp :__ Fichier contenant toute la mise en place de la __SDL2__ ainsi que __la boucle des évènements__ relatif à la fenêtre de jeu.

#####<u>**Comment compiler :**</u>
- __Installer la SDL2 (Linux) :__ ``apt-get install libsdl2-dev``
- __Installer la SDL2-Image (Linux) :__ ``apt-get install libsdl2-image-dev``
- __Compiler (Linux) :__ ``g++ main.cpp $(sdl2-config --cflags --libs) && ./a.out``

