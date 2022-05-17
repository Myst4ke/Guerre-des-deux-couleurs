#include "game.cpp"
//
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
//
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <string>
using namespace std;

int main(int argc, char **argv) {
  /* -------- Variables -------- */
  SDL_Window *window = NULL;
  SDL_Window *window2 = NULL; // Inutilisé
  SDL_Renderer *renderer = NULL;
  SDL_Renderer *renderer2 = NULL; // Inutilisé

  // Variable qui boucle dans un while qui défini
  // l'arret de la boucle quand le programme doit s'arrêter.
  SDL_bool program_launched = SDL_TRUE;

  /* ------------------------ */

  /* -------- Setup -------- */

  // Initialisation de la SDL (partie vidéo)
  // "SDL_ExitWithError()" Ref : "SDL++.cpp"
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    SDL_ExitWithError("Initialisation de la SDL");
  }

  // Création d'une nouvelle fenêtre
  window =
      SDL_CreateWindow("Main Window", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

  if (window == NULL) {
    SDL_ExitWithError("Creation de la fenêtre échouée");
  }

  // Création d'un rendu pour la fenêtre que l'on vient de créer
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
  if (renderer == NULL) {
    SDL_ExitWithError("Creation du renderer échouée");
  }
  /* -------------------------- */

  /* --- Static Draw --- */
  /*
  Dessin statique affiché par défaut au lancement du proggramme.
  On y affiche le plateau ainsi que la configuration initiale.
  */

  // Définition du Plateau avec la taille de la fenêtre (hauteur largeur)
  Plateau plat(WINDOW_HEIGHT, WINDOW_WIDTH, 50);
  // Les coordonées maximales de la fenêtre sont rentrée a l'envers puisque
  // le tableau de case est sous la forme [y,x] ou [ligne, colone]

  // Affichage du plateau
  Game game(renderer);
  game.start(plat);
  affiche_plat(renderer, plat);

  /* Units unit(0);
  plat.tab[0].add_unit(unit); */
  affiche_contenu(renderer, plat);
  /* affiche_deplacement(unit, a, unit.position.x, unit.position.y, renderer,
                      color_orange); */

  /* ----------------------- */

  /* --- Event Loop --- */
  /*
  Boucle des évenements qui gère tout les évènements relatifs à la
  fenêtre (Clic, position de la souris, fermeture de la fenêtre, etc ..)
  */

  // Lancement du programme
  // Boucle à l'infini tant que aucun évènement ne nécessite de l'arrêter.
  while (program_launched) {

    // Définition d'une variable event pour récuperer tout les évènements
    SDL_Event event;
    // Boucle en captant tout les évènements
    while (SDL_PollEvent(&event)) {
      // Test des évènements suivant :
      switch (event.type) {
      // Clic sur un bouton de la souris
      case SDL_MOUSEBUTTONDOWN: // gestion des clics
        // Clic Molette
        if (event.button.button == SDL_BUTTON_MIDDLE) {
          Cellule &actuelle = plat.appartient2(event.button.x, event.button.y);

          game.finDeTour(actuelle, plat);
          // Clic Gauche
        } else if (event.button.button == SDL_BUTTON_LEFT) {
          Cellule &actuelle = plat.appartient2(event.button.x, event.button.y);

          /* cout << "Clic gauche sur la case : ";
          actuelle.pos.affiche(); */

          game.left_clicked(actuelle, plat);

          // Clic Droit
        } else if (event.button.button == SDL_BUTTON_RIGHT) {
          Cellule &actuelle = plat.appartient2(event.button.x, event.button.y);

          /* cout << "Clic droit sur la case : ";
          actuelle.pos.affiche(); */

          if (game.right_clicked(renderer, actuelle, plat)) {
          } else {

            print_unit(renderer, actuelle);
            draw_bordure(renderer, color_green, actuelle);

            /* affiche_deplacement(actuelle.contenu, plat, renderer,
             * color_green); */

            while (SDL_WaitEvent(&event)) {
              switch (event.type) {
              case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                  Cellule &prochaine =
                      plat.appartient2(event.button.x, event.button.y);

                  game.right_left_clicked(renderer, actuelle, prochaine, plat, program_launched);
                  goto end;
                } else if (event.button.button == SDL_BUTTON_RIGHT) {
                  Cellule &prochaine =
                      plat.appartient2(event.button.x, event.button.y);

                  game.double_right_cliked(renderer, actuelle, prochaine, plat);

                  goto end;
                }
                break;

              default:
                break;
              }
            }
          end:;
          }
        }
        break;
        // event.button.clicks -> nb de clics
        /* event.button.button : -> le clic utilisé
        SDL_BUTTON_LEFT
        SDL_BUTTON_MIDDLE
        SDL_BUTTON_RIGHT
        */

        /* case SDL_WINDOWEVENT: //
          if (event.window.event == SDL_WINDOWEVENT_LEAVE) {
            cout << "La souris est sortie de la fenêtre" << endl;
          } else if (event.window.event == SDL_WINDOWEVENT_ENTER) {
            cout << "La souris est dans la fenêtre" << endl;
          }
          break; */

      case SDL_QUIT: // Clic sur la croix de la fenêtre
        program_launched = SDL_FALSE;
        break;

      default:
        SDL_RenderPresent(renderer);
        break;
      }
    }
  }
  /* ---------------------- */

  /* --- End --- */

  // Clear le rendu
  if (SDL_RenderClear(renderer) != 0) {
    SDL_ExitWithError("Echec du clear du renderer");
  }

  // Détruit tout les pointeurs alloués pour les fenêtres
  //  et les rendus
  SDL_DestroyRenderer(renderer);
  SDL_DestroyRenderer(renderer2);
  SDL_DestroyWindow(window);
  SDL_DestroyWindow(window2);
  // Quitte la SDL
  SDL_Quit();

  return EXIT_SUCCESS;
}