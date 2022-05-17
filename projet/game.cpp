#include "draw.cpp"
//
#include <SDL2/SDL_render.h>
#include <cmath>
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

class Game {
public:
  SDL_Renderer *rend;
  int tour_joueur = 0;
  vector<Joueur> joueurs;

  Game(SDL_Renderer *prend/* ,
       Plateau pplat */) {
    rend = prend;
    Joueur j1(1, 100, 100);
    Joueur j2(2, 100, 100);
    joueurs.push_back(j1);
    joueurs.push_back(j2);
  }

  void distrib_ressources(Plateau &plat) {
    srand(time(NULL));
    string rst;
    string rst2;
    int x_mid = plat.colone / 2;
    int y_mid = plat.ligne / 2;

    // Pourcentage de chance de différence entre une case au centre et une case
    // au bord Ici : 30+30 = 60%
    int decrementation_x = 30;
    int decrementation_y = 30;
    /* cout << "Xmid et Ymid sont : " << x_mid << ", " << y_mid << endl; */

    int pas_x = decrementation_x / x_mid;
    int pas_y = decrementation_y / y_mid;

    for (int j = 0; j < plat.ligne; j++) {
      /* cout << endl << "J : " << j << endl; */
      for (int i = 0; i < plat.colone; i++) {
        /* cout << "i : " << i << endl; */

        int rd = rand() % 99; // 100% de chance

        // Distance de la case au centre
        Cellule act = plat.tab[i + j * plat.colone];
        int dist_x = absol(act.pos.colone - x_mid);
        int dist_y = absol(act.pos.ligne - y_mid);

        // ajout des résultats dans le fichier result.txt
        rst = rst + "[" + to_string(0 + (dist_x * pas_x) + (dist_y * pas_y)) +
              "]";

        // ici 0 est la valeur la plus basse possible (la case la plus au
        // centre)
        if (rd < 0 + (dist_x * pas_x) + (dist_y * pas_y)) {
          int rd = rand() % 10;
          if (rd < 7) {
            Arbre arbre;
            plat.tab[i + plat.colone * j].add_unit(arbre);
            /* cout << "Ressource ajoutée en : [" << j << "," << i << "]" <<
             * endl; */
            rst2 += "[A]";
          } else {
            Rocher rocher;
            plat.tab[i + plat.colone * j].add_unit(rocher);
            /* cout << "Ressource ajoutée en : [" << j << "," << i << "]" <<
             * endl; */
            rst2 += "[R]";
          }
        } else {
          rst2 += "[ ]";
        }
      }
      rst += "\n";
      rst2 += "\n";
    }
    write_result(rst, "result/pourcents.txt");
    write_result(rst2, "result/grille.txt");
  }
  void affiche_ressources(){
    
  }


  // Appelle toutes les fonctions néscéssaires au lancement de la partie
  void start(Plateau &plat) {
    distrib_ressources(plat);
    add_castle(plat);
    affiche_plat(rend, plat);
    affiche_contenu(rend, plat);
    affiche_ressources();
  }

  // Génère deux chateaux positionés d'un côté et de l'autre du plateau
  void add_castle(Plateau &plat) {
    Chateau j1(1), j2(2);
    int y = plat.ligne / 2;
    plat.tab[y * plat.colone].add_unit(j1);
    plat.tab[y * plat.colone + plat.colone - 1].add_unit(j2);
  }
  void produire_ouvrier(SDL_Renderer *rend, Cellule cell, Plateau &plat) {
    if (cell.contenu.equipe == 1) {
      if (plat.tab[cell.indice + 1].contenu.vide ||
          plat.tab[cell.indice + 1].contenu.nom_classe == "Ressource") {
        Ouvrier ouvrier1(2, 1);
        plat.tab[cell.indice + 1].add_unit(ouvrier1);
        joueurs[0].unités.push_back(ouvrier1);
        print_unit(rend, plat.tab[cell.indice + 1]);
      }
    } else if (cell.contenu.equipe == 2) {
      if (plat.tab[cell.indice - 1].contenu.vide ||
          plat.tab[cell.indice - 1].contenu.nom_classe == "Ressource") {
        Ouvrier ouvrier2(2, 2);
        plat.tab[cell.indice - 1].add_unit(ouvrier2);
        joueurs[1].unités.push_back(ouvrier2);
        print_unit(rend, plat.tab[cell.indice - 1]);
      }
    }
  }
  bool right_clicked(SDL_Renderer *rend, Cellule &cell, Plateau &plat) {
    if (cell.contenu.vide) {
      affiche(rend, color_red, cell);
      cout << "La case est vide" << endl;
      SDL_Delay(100);
      affiche(rend, color_white, cell);
      return true;
    } else if (cell.contenu.nom == "Chateau") {
      produire_ouvrier(rend, cell, plat);
      return true;
    } else if (cell.contenu.statique) {
      affiche(rend, color_red, cell);
      cout << "Cet objet est statique il ne peut bouger" << endl;
      SDL_Delay(100);
      print_unit(rend, cell);
      return true;
    } else {
      /* affiche_deplacement(cell.contenu, plat, rend, color_orange); */
      print_unit(rend, cell);
      draw_bordure(rend, color_green, cell);
      return false;
    }
  }

  void double_right_cliked(SDL_Renderer *rend, Cellule &depart, Cellule &arrive,
                           Plateau &plat) {
    if (depart.contenu.statique) {
      cout << "Cet objet est statique il ne peut bouger" << endl;
      affiche(rend, color_red, depart);
      SDL_Delay(200);
      print_unit(rend, depart);
    } else if (depart.contenu.nom == "ouvrier" &&
               arrive.contenu.nom == "arbre") {
      // l'ouvrier commence à exploiter le bois on remplace donc le bois par un
      // ouvrier statique qui génère du bois passivement
      depart.transfert(arrive);
      print_unit(rend, arrive);
      affiche(rend, color_white, depart);
      arrive.contenu.statique = true;
    } else if (depart.contenu.nom == "ouvrier" &&
               arrive.contenu.nom == "rocher") {
      depart.clear_contenu();
      arrive.clear_contenu();
      Mine mine(1);
      arrive.add_unit(mine);
      print_unit(rend, arrive);
      affiche(rend, color_white, depart);
    } else if (!arrive.contenu.vide) {
      cout << "Il y a déjà un objet à l'arrivée !" << endl;
      affiche(rend, color_red, depart);
      affiche(rend, color_red, arrive);
      SDL_Delay(200);
      print_unit(rend, depart);
      print_unit(rend, arrive);
    } else if (arrive.contenu.vide) {
      moove_cell(depart, arrive);
    }
  }

  void left_clicked(Cellule cell) {
    affiche(rend, color_green, cell);
    SDL_Delay(100);
    affiche(rend, color_white, cell);
    print_unit(rend, cell);
    // liste des action dans le terminal
  }

  void moove_cell(Cellule &depart, Cellule &arrive) {
    cout << "1 : " << depart.pos.colone << endl;
    cout << "2 : " << depart.contenu.deplacement << endl;
    cout << "3 : " << arrive.pos.colone << endl;
    if (depart.pos.colone - depart.contenu.deplacement <= arrive.pos.colone &&
        depart.pos.colone + depart.contenu.deplacement >= arrive.pos.colone &&
        depart.pos.ligne - depart.contenu.deplacement <= arrive.pos.ligne &&
        depart.pos.ligne + depart.contenu.deplacement >= arrive.pos.ligne) {
      depart.transfert(arrive);
      print_unit(rend, arrive);
      affiche(rend, color_white, depart);
    } else {
      affiche(rend, color_red, depart);
      SDL_Delay(100);
      affiche(rend, color_white, depart);
      print_unit(rend, depart);
      cout << "Ce mouvement est impossible ! L'objet séléctioné n'a que "
           << depart.contenu.deplacement << " de point de déplacement" << endl;
    }
  }

  void finDeTour() {
    for (int i = 0; i < joueurs[tour_joueur].unités.size(); i++) {
      int cmpt = 0;
      if (joueurs[tour_joueur].unités[i].jouée == true) {
        cmpt++;
        if (cmpt == joueurs[tour_joueur].unités.size() - 1) {
          cout << "Fin du tour du joueur " << tour_joueur + 1
               << ". Toutes ses unités ont été déplacées."<<endl;
          if(tour_joueur == 0){
            tour_joueur++;
          }else if(tour_joueur == 1){
            tour_joueur--;
          }
        }
      }
    }
  }

  void compteur_tours() {}
};
