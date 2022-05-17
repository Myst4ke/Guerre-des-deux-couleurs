#include <SDL2/SDL_surface.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct Case {
public:
  unsigned int colone;
  unsigned int ligne;
  void affiche() { cout << "[" << ligne << "," << colone << "]" << endl; }
};

class Object {
public:
  string nom;
  string nom_classe;
  Case position;
  string image_file = "";
  bool statique;
  bool vide = true;
  unsigned int deplacement;
  int equipe;
  

  

  Object() { vide = true; }

  // Déplace l'objet aux coordonées indiquée (ligne colone)
  // On vérifie que le déplacement est possible (Object statique ou non)
  // Puis on vérifie que le déplacement reste dans la limite des déplacements
  // possibles
  void deplace(int pcolone, int pligne) {
    if (statique == false) {
      if (pcolone >= position.colone - deplacement &&
          pcolone <= position.colone + deplacement &&
          pligne >= position.ligne - deplacement &&
          pligne <= position.ligne + deplacement) {
        position.colone = pcolone;
        position.ligne = pligne;
      }
      cerr << "La nouvelle position de l'unité n'est pas valable" << endl;
      throw 1;
    }
    cout << "L'unité est statique elle n'a donc pas été déplacée" << endl;
  }
};

class Ressources : public Object {
public:
  Ressources() {
    nom_classe = "Ressource";
    statique = true;
    deplacement = 0;
    vide = false;
  }
};

class Arbre : public Ressources {
public:
  Arbre() : Ressources() {
    nom = "arbre";
    image_file = "assets/arbre.bmp";
    /* actions.push_back("recoltable"); */
  }
};
class Rocher : public Ressources {
public:
  Rocher() : Ressources() {
    nom = "rocher";
    image_file = "assets/rock.bmp";
    /* actions.push_back("recoltable"); */
  }
};

class Buildings : public Object {
public:
  Buildings() {
    nom_classe = "Batiment";
    statique = true;
    deplacement = 0;
    vide = false;
  }
};
class Chateau : public Buildings {
public:
  Chateau(int pequipe) {
    nom = "Chateau";
    statique = true;
    deplacement = 0;
    vide = false;
    image_file = "assets/Chateau50.bmp";
    equipe = pequipe;
  }
};

class Scierie : public Buildings {
public:
  Scierie(int pequipe) {
    nom = "scierie";
    statique = true;
    deplacement = 0;
    vide = false;
    image_file = "assets/maison.bmp";
    equipe = pequipe;
  }
};
class Mine : public Buildings {
public:
  Mine(int pequipe) {
    nom = "mine";
    statique = true;
    deplacement = 0;
    vide = false;
    image_file = "assets/pioche.bmp";
    equipe = pequipe;
  }
};

class Units : public Object {
public:
  bool jouée;
  Units() { statique = false; }
  Units(int pdeplacement) {
    nom_classe = "unité";
    statique = false;
    deplacement = pdeplacement;
    vide = false;
  }
};

class Ouvrier : public Units {
public:
  Ouvrier(int depl, int pequipe) : Units(depl) {
    nom = "ouvrier";
    image_file = "assets/hache.bmp";
    equipe = pequipe;
  }
};

class Joueur {
public:
  int equipe;
  int bois;
  int gold;
  vector<Units> unités;
  Joueur(int eqp, int pbois, int pgold){
    equipe = eqp;
    bois = pbois;
    gold = pgold;
  }
};
