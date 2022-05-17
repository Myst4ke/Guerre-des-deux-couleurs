#include "SDL++.cpp"
#include "object.cpp"
//
#include <SDL2/SDL.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <bits/types/cookie_io_functions_t.h>
//
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <typeinfo>
#include <vector>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

using namespace std;

/*

-------- Création du plateau de jeu --------

*/

/*
----------------------------
Class Cellule
----------------------------
*/
class Cellule {
public:
  Rectangle cellule;
  int indice;
  Case pos;

  // Contenu
  Object contenu;

  Cellule() {}
  Cellule(int px, int py, int pw, int ph /* , Object pobj */) {
    Rectangle cellule(px, py, pw, ph);
  }

  void set(int px, int py, int pw, int ph) {
    cellule.x = px;
    cellule.y = py;
    cellule.w = pw;
    cellule.h = ph;
  }
  void add_unit(Object &obj) {
    obj.position = pos;
    contenu = obj;
  }
  void clear_contenu(){
    //object vide
    Object a;
    add_unit(a);
  }

  //Transfert le contenu d'une cellule vers une autre
  void transfert(Cellule &arrive) {
    Object vide; // Objet vide
    arrive.contenu = contenu;
    arrive.contenu.vide = false;
    contenu = vide;
    contenu.vide = true;
  }
};

/*
----------------------------
Class Plateau
----------------------------
*/

class Plateau {
public:
  vector<Cellule> tab;
  unsigned int ligne;
  unsigned int colone;
  int cel_t; // taille des cellules

  // Constructeur du plateau
  // Appel a la fonction "rempli();"
  Plateau(int window_height, int window_width, int ptaille_cellule) {
    if (window_height == 0 || window_width == 0) {
      cerr << "invalid tab" << endl;
      throw 1;
    }
    ligne = window_height / ptaille_cellule;
    colone = window_width / ptaille_cellule;
    cel_t = ptaille_cellule;
    cout << "Ligne : " << ligne << ", Colone :" << colone << endl;
    rempli();
  }
  Plateau();
  // Rempli le tableau avec les cases
  void rempli() {
    Cellule a(0, 0, 0, 0);
    for (int i = 0; i < ligne; i++) {
      for (int x = 0; x < colone; x++) {
        a.set(x * cel_t, i * cel_t, cel_t, cel_t);
        a.indice = x + (i * colone);
        a.pos.colone = x;
        a.pos.ligne = i;
        tab.push_back(a);
        /* cout << "Cellule n°" << tab[x + (i * colone)].indice + 1 << " ajoutée !"
             << endl;
        cout << "(" << i * cel_t << "," << x * cel_t << ") (" << cel_t << ","
             << cel_t << ")" << endl; */
      }
    }
  }
  // Vérifie des coordonés, renvoie la cellule dans laquelle elles sont
  // comprises
  Cellule& appartient2(int px, int py){
    int x, y;
    x = px / cel_t;
    y = py / cel_t;
    return tab[x + (y * colone)];
  }
};



