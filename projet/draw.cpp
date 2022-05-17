#include "struct.cpp"
//
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
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

using namespace std;

/*
-----------------------
Affichage Cellule
-----------------------
*/

// Affichage d'une Cellule
void affiche(SDL_Renderer *rend, Color &color, Cellule cell) {
  /* cout << "dessin de la cellule : (" << cell.cellule.x << "," << cell.cellule.y
       << ")" << endl; */
  // Dessin de la cellule
  if (SDL_SetRenderDrawColor(rend, color.r, color.g, color.b,
                             SDL_ALPHA_OPAQUE) != 0) {
    SDL_ExitWithError("Impossible de changer la couleur du rendu");
  }
  if (SDL_RenderFillRect(rend, &cell.cellule) != 0) {
    SDL_ExitWithError("Impossible d'ajouter un rectangle");
  }

  // Dessin de la bordure de la cellule
  if (SDL_SetRenderDrawColor(rend, 0, 0, 0, SDL_ALPHA_OPAQUE) != 0) {
    SDL_ExitWithError("Impossible de changer la couleur du rendu");
  }
  if (SDL_RenderDrawRect(rend, &cell.cellule) != 0) {
    SDL_ExitWithError("Impossible d'ajouter un rectangle");
  }
  SDL_RenderPresent(rend);
}

// Affichage de bordure épaisses pour mettre en valeur la cellule
void draw_bordure(SDL_Renderer *rend, Color &color, Cellule cell) {
  for (int i = 0; i < 4; i++) {
    Rectangle b(cell.cellule.x + i, cell.cellule.y + i, cell.cellule.w - i - i,
                cell.cellule.h - i - i);
    if (SDL_SetRenderDrawColor(rend, color.r, color.g, color.b,
                               SDL_ALPHA_OPAQUE) != 0) {
      SDL_ExitWithError("Impossible de changer la couleur du rendu");
    }
    if (SDL_RenderDrawRect(rend, &b) != 0) {
      SDL_ExitWithError("Impossible d'ajouter un rectangle");
    }
    SDL_RenderPresent(rend);
  }
}

void print_unit(SDL_Renderer *rend, Cellule cell) {
  // Temporaire
  if (!cell.contenu.vide) {
    if (cell.contenu.image_file != "") {
      SDL_Surface *image;
      SDL_Texture *texture;

      image = SDL_LoadBMP(cell.contenu.image_file.c_str());
      // Check de l'erreur
      if (image == NULL) {
        SDL_ExitWithError("Impossible de créer l'image");
      }
      texture = SDL_CreateTextureFromSurface(rend, image);
      SDL_FreeSurface(image);

      if (texture == NULL) {
        SDL_ExitWithError("Impossible de charger la texture");
      }

      if (SDL_QueryTexture(texture, NULL, NULL, &cell.cellule.w,
                           &cell.cellule.h) != 0) {
        SDL_ExitWithError("Impossible d'appliquer la texture");
      }

      if (SDL_RenderCopy(rend, texture, NULL, &cell.cellule) != 0) {
        SDL_ExitWithError("Impossible d'afficher la texture");
      }
      if(cell.contenu.equipe == 1){
        draw_bordure(rend, color_red, cell);
      }else if(cell.contenu.equipe == 2){
        draw_bordure(rend, color_blue, cell);
      }
      SDL_RenderPresent(rend);
    } else {
      Rectangle rect(cell.cellule.x + 10, cell.cellule.y + 10,
                     cell.cellule.w - 20, cell.cellule.h - 20);
      if (SDL_SetRenderDrawColor(rend, 255, 0, 0, SDL_ALPHA_OPAQUE) != 0) {
        SDL_ExitWithError("Impossible de changer la couleur du rendu");
      }
      if (SDL_RenderFillRect(rend, &rect) != 0) {
        SDL_ExitWithError("Impossible d'ajouter un rectangle");
      }
      SDL_RenderPresent(rend);
    }
  }
}

/*
-----------------------
Affichage Plateau
-----------------------
*/

// Affichage d'un Plateau blanc
void affiche_plat(SDL_Renderer *rend /* Color &color */, Plateau plat) {
  Rectangle *rst = new Rectangle[plat.colone * plat.ligne];
  int n = 0;
  for (int i = 0; i < plat.ligne; i++) {
    for (int x = 0; x < plat.colone; x++) {
      rst[n] = plat.tab[x + (i * plat.colone)].cellule;
      n++;
    }
    if (SDL_SetRenderDrawColor(rend, 255, 255, 255, SDL_ALPHA_OPAQUE) != 0) {
      SDL_ExitWithError("Impossible de changer la couleur du rendu");
    }
    if (SDL_RenderFillRects(rend, rst, n) != 0) {
      SDL_ExitWithError("Impossible d'ajouter un rectangle");
    }

    if (SDL_SetRenderDrawColor(rend, 0, 0, 0, SDL_ALPHA_OPAQUE) != 0) {
      SDL_ExitWithError("Impossible de changer la couleur du rendu");
    }
    if (SDL_RenderDrawRects(rend, rst, n) != 0) {
      SDL_ExitWithError("Impossible d'ajouter un rectangle");
    }
    SDL_RenderPresent(rend);
  }
}

// Affichage des cases du Plateau contenant un objet
void affiche_contenu(SDL_Renderer *rend, Plateau plat) {
  for (int i = 0; i < plat.ligne; i++) {
    for (int x = 0; x < plat.colone; x++) {
      if (plat.tab[x + (i * plat.colone)].contenu.nom != "") {
        print_unit(rend, plat.tab[x + (i * plat.colone)]);
      }
    }
    SDL_RenderPresent(rend);
  }
}

// Affiche la zone de déplacement d'une unité
void affiche_deplacement(Object &unit, Plateau &plat, SDL_Renderer *rend,
                         Color color) {
  cout << "l'unité est en : ";
  unit.position.affiche();
  Cellule cell = plat.tab[(unit.position.ligne * unit.position.colone) +
                          unit.position.colone];
  cout << "accessed" << endl;
  if (unit.deplacement == 0) {
    draw_bordure(rend, color_red, cell);
    cout << "This unit is unmovable !" << endl;
  } else {
    // Création d'un rectangle partant de la cellule d'indice
    // [x-deplacement, y-deplacement] et allant jusqu'en [x+deplacement,
    // y+deplacement] ici 'plat.cel_t' correspont à la taille des cellules en px
    Rectangle a(cell.cellule.x - (unit.deplacement * plat.cel_t),
                cell.cellule.y - (unit.deplacement * plat.cel_t),
                ((unit.deplacement * 2 + 1) * plat.cel_t),
                ((unit.deplacement * 2 + 1) * plat.cel_t));

    if (SDL_SetRenderDrawColor(rend, color.r, color.g, color.b,
                               SDL_ALPHA_OPAQUE) != 0) {
      SDL_ExitWithError("Impossible de changer la couleur du rendu");
    }
    if (SDL_RenderDrawRect(rend, &a) != 0) {
      SDL_ExitWithError("Impossible d'ajouter une ligne");
    }
  }
}
