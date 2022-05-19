#include "draw.cpp"
//
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
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
  int tours = 1;
  SDL_Renderer *rend;
  int tour_joueur = 0;
  Joueur joueurs[2];

  Game(SDL_Renderer *prend) {
    rend = prend;
    Joueur j1(1, 100, 100);
    Joueur j2(2, 100, 100);
    joueurs[0] = j1;
    joueurs[1] = j2;
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

        int rd = rand() % 100; // 100% de chance

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
  void affiche_ressources() {
    cout << endl
         << "Joueur " << tour_joueur + 1
         << " Vous avez : " << joueurs[tour_joueur].bois << " bois et "
         << joueurs[tour_joueur].gold << " or." << endl;
  }

  // Appelle toutes les fonctions néscéssaires au lancement de la partie
  void start(Plateau &plat) {
    distrib_ressources(plat);
    add_castle(plat);
    affiche_plat(rend, plat);
    affiche_contenu(rend, plat);
    cout << "La partie commence !" << endl
         << "Votre but est de détruire le chateau adverse. \nPour cela vous "
            "pouvez produire des chevalier pour attaquer et des ouvrier pour "
            "collecter les ressources sur la carte.\n Ouvrier(50 bois) "
            "Chevalier(100 or)"
         << endl
         << "Tour n°" << tours << endl;
    affiche_ressources();
  }

  // Génère deux chateaux positionés d'un côté et de l'autre du plateau
  void add_castle(Plateau &plat) {
    Chateau j1(1), j2(2);
    int y = plat.ligne / 2;
    plat.tab[y * plat.colone].add_unit(j1);
    plat.tab[y * plat.colone + plat.colone - 1].add_unit(j2);
  }
  // Ajoute un Ouvrier devant le chateau
  void produire_ouvrier(SDL_Renderer *rend, Cellule cell, Plateau &plat) {
    if (cell.contenu.equipe == 1) {
      if (plat.tab[cell.indice + 1].contenu.vide ||
          plat.tab[cell.indice + 1].contenu.nom_classe == "Ressource") {
        if (joueurs[0].bois >= 50) {
          Ouvrier ouvrier1(3, 1);
          plat.tab[cell.indice + 1].add_unit(ouvrier1);
          joueurs[0].unités.push_back(ouvrier1);
          print_unit(rend, plat.tab[cell.indice + 1]);
          joueurs[0].bois -= 50;
        } else {
          cout << "Vous n'avez pas assez de bois pour produire un ouvrier !"
               << endl;
        }
      } else {
        cout << "L'entrée du chateau est obstruée l'unité ne peut pas sortir"
             << endl;
      }
    } else if (cell.contenu.equipe == 2) {
      if (plat.tab[cell.indice - 1].contenu.vide ||
          plat.tab[cell.indice - 1].contenu.nom_classe == "Ressource") {
        if (joueurs[1].bois >= 50) {
          Ouvrier ouvrier2(3, 2);
          plat.tab[cell.indice - 1].add_unit(ouvrier2);
          joueurs[1].unités.push_back(ouvrier2);
          print_unit(rend, plat.tab[cell.indice - 1]);
          joueurs[1].bois -= 50;
        } else {
          cout << "Vous n'avez pas assez de bois pour produire un ouvrier !"
               << endl;
        }
      } else {
        cout << "L'entrée du chateau est obstruée l'unité ne peut pas sortir"
             << endl;
      }
    }
  }
  // Ajoute un Chevalier devant le chateau
  void produire_chevalier(SDL_Renderer *rend, Cellule cell, Plateau &plat) {
    if (cell.contenu.equipe == 1) {
      if (plat.tab[cell.indice + 1].contenu.vide ||
          plat.tab[cell.indice + 1].contenu.nom_classe == "Ressource") {
        if (joueurs[0].gold >= 100) {
          Chevalier chevalier1(2, 1);
          plat.tab[cell.indice + 1].add_unit(chevalier1);
          joueurs[0].unités.push_back(chevalier1);
          print_unit(rend, plat.tab[cell.indice + 1]);
          joueurs[0].gold -= 100;
        } else {
          cout << "Vous n'avez pas assez d'o pour produire un chevalier !"
               << endl;
        }
      } else {
        cout << "L'entrée du chateau est obstruée l'unité ne peut pas sortir"
             << endl;
      }
    } else if (cell.contenu.equipe == 2) {
      if (plat.tab[cell.indice - 1].contenu.vide ||
          plat.tab[cell.indice - 1].contenu.nom_classe == "Ressource") {
        if (joueurs[1].gold >= 100) {
          Chevalier chevalier2(2, 2);
          plat.tab[cell.indice - 1].add_unit(chevalier2);
          joueurs[1].unités.push_back(chevalier2);
          print_unit(rend, plat.tab[cell.indice - 1]);
          joueurs[1].gold -= 100;
        } else {
          cout << "Vous n'avez pas assez d'or pour produire un chevalier !"
               << endl;
        }
      } else {
        cout << "L'entrée du chateau est obstruée l'unité ne peut pas sortir"
             << endl;
      }
    }
  }
  /*




  */
  bool right_clicked(SDL_Renderer *rend, Cellule &cell, Plateau &plat) {
    if (cell.contenu.vide) {
      affiche(rend, color_red, cell);
      cout << "La case est vide" << endl;
      SDL_Delay(100);
      affiche(rend, color_white, cell);
      return true;
    } else if (cell.contenu.equipe == tour_joueur + 1) {
      if (cell.contenu.nom == "Chateau") {
        produire_ouvrier(rend, cell, plat);
        affiche_ressources();
        return true;
      } else {
        /* affiche_deplacement(cell.contenu, plat, rend, color_orange); */
        print_unit(rend, cell);
        draw_bordure(rend, color_green, cell);
        return false;
      }
    } else if (cell.contenu.equipe == tour_joueur + 2 ||
               cell.contenu.equipe == tour_joueur) {
      affiche(rend, color_red, cell);
      cout << "Cette unité n'appartient pas a votre équipe" << endl;
      SDL_Delay(100);
      print_unit(rend, cell);
      return true;
    } else if (cell.contenu.statique) {
      affiche(rend, color_red, cell);
      cout << "Cet objet est statique il ne peut bouger" << endl;
      SDL_Delay(100);
      print_unit(rend, cell);
      return true;
    }
    return true;
  }
  /*




  */
  void double_right_cliked(SDL_Renderer *rend, Cellule &depart, Cellule &arrive,
                           Plateau &plat, SDL_bool &launched) {
    if (depart.contenu.statique) {
      cout << "Cet objet est statique il ne peut bouger" << endl;
      affiche(rend, color_red, depart);
      SDL_Delay(200);
      print_unit(rend, depart);
      // check de l'équipe
    } else if (depart.contenu.jouee == false) {
      if (depart.contenu.nom == "ouvrier" && arrive.contenu.nom == "arbre") {
        // l'ouvrier commence à exploiter le bois on remplace donc le bois par
        // un ouvrier statique qui génère du bois passivement
        depart.clear_contenu();
        arrive.clear_contenu();
        Scierie scierie(tour_joueur + 1);
        arrive.add_unit(scierie);
        print_unit(rend, arrive);
        affiche(rend, color_white, depart);
        joueurs[tour_joueur].bonus_bois += 25;
      } else if (depart.contenu.nom == "ouvrier" &&
                 arrive.contenu.nom == "rocher") {
        depart.clear_contenu();
        arrive.clear_contenu();
        Mine mine(tour_joueur + 1);
        arrive.add_unit(mine);
        print_unit(rend, arrive);
        affiche(rend, color_white, depart);
        joueurs[tour_joueur].bonus_gold += 50;
      } else if (depart.contenu.nom == "chevalier" &&
                 (arrive.contenu.nom == "chevalier" ||
                  arrive.contenu.nom == "Chateau" ||
                  arrive.contenu.nom == "ouvrier")) {
        // On vérifie que le chevalier est a distance d'attaque
        if (depart.pos.colone - 1 <= arrive.pos.colone &&
            depart.pos.colone + 1 >= arrive.pos.colone &&
            depart.pos.ligne - 1 <= arrive.pos.ligne &&
            depart.pos.ligne + 1 >= arrive.pos.ligne) {
          arrive.contenu.pv -= depart.contenu.attaque;
          affiche(rend, color_red, arrive);
          SDL_Delay(200);
          print_unit(rend, arrive);
          print_unit(rend, depart);
          depart.contenu.jouee = true;
          if (arrive.contenu.pv <= 0) {
            if (arrive.contenu.nom == "Chateau") {
              cout << "Le chateau est tombé !" << endl
                   << "Le joueur " << tour_joueur + 1 << " gagne la partie !"
                   << endl;
              arrive.clear_contenu();
              affiche(rend, color_red, arrive);
              SDL_Delay(200);
              affiche(rend, color_white, arrive);
              launched = SDL_FALSE;
            } else {
              cout << "Lunité est morte !" << endl;
              arrive.clear_contenu();
              affiche(rend, color_red, arrive);
              SDL_Delay(200);
              affiche(rend, color_white, arrive);
            }
          }
        } else {
          cout << endl
               << "Vous êtes trop loin pour attaquer cette unité ! Rapprochez "
                  "vous d'abord !"
               << endl;
          affiche(rend, color_red, depart);
          SDL_Delay(100);
          print_unit(rend, depart);
        }
      } else if (!arrive.contenu.vide) {
        cout << "Il y a déjà un objet à l'arrivée !" << endl;
        affiche(rend, color_red, depart);
        affiche(rend, color_red, arrive);
        SDL_Delay(200);
        print_unit(rend, depart);
        print_unit(rend, arrive);
        // Déplacement si la case est vide
      } else if (arrive.contenu.vide) {
        if (moove_cell(depart, arrive)) {
          // Si le déplacement est réussi on change le status de l'unité
          // qui est désormais sur la case d'arrivée
          arrive.contenu.jouee = true;
        } else {
          depart.contenu.jouee = false;
        }
      }
    } else {
      cout << "Cette unité a déjà été déplacée pendant ce tour !" << endl;
      print_unit(rend, depart);
    }
  }

  void left_clicked(Cellule cell, Plateau &plat) {
    if (cell.contenu.equipe == tour_joueur + 1) {
      if (cell.contenu.nom == "Chateau") {
        produire_chevalier(rend, cell, plat);
        affiche_ressources();
      } else {
        affiche(rend, color_red, cell);
        SDL_Delay(100);
        affiche(rend, color_white, cell);
        print_unit(rend, cell);
      }
    } else if (cell.contenu.equipe == tour_joueur + 2 ||
               cell.contenu.equipe == tour_joueur) {
      affiche(rend, color_red, cell);
      cout << "Cette unité n'appartient pas a votre équipe" << endl;
      SDL_Delay(100);
      print_unit(rend, cell);
    } else {
      affiche(rend, color_green, cell);
      SDL_Delay(100);
      affiche(rend, color_white, cell);
      print_unit(rend, cell);
    }
  }

  bool moove_cell(Cellule &depart, Cellule &arrive) {
    cout << "moove cell" << endl;
    if (depart.pos.colone - depart.contenu.deplacement <= arrive.pos.colone &&
        depart.pos.colone + depart.contenu.deplacement >= arrive.pos.colone &&
        depart.pos.ligne - depart.contenu.deplacement <= arrive.pos.ligne &&
        depart.pos.ligne + depart.contenu.deplacement >= arrive.pos.ligne) {
      depart.transfert(arrive);
      print_unit(rend, arrive);
      affiche(rend, color_white, depart);
      return true;
    } else {
      affiche(rend, color_red, depart);
      SDL_Delay(100);
      affiche(rend, color_white, depart);
      print_unit(rend, depart);
      cout << "Ce mouvement est impossible ! L'objet séléctioné n'a que "
           << depart.contenu.deplacement << " de point de déplacement" << endl;
      return false;
    }
  }

  void finDeTour(Cellule &cell, Plateau &plat) {
    if (cell.contenu.nom == "Chateau") {
      cout << "Fin du tour du joueur " << tour_joueur + 1 << endl;
      if (tour_joueur == 0) {
        tour_joueur++;
        tours++;
      } else if (tour_joueur == 1) {
        tour_joueur--;
        tours++;
      }
      // On remet toutes les unités du prochain joueur au status "déplaçable"
      for (int i = 0; i < plat.tab.size(); i++) {
        if (plat.tab[i].contenu.equipe == tour_joueur + 1 &&
            plat.tab[i].contenu.jouee == true) {
          plat.tab[i].contenu.jouee = false;
        }
      }
      joueurs[tour_joueur].bois += joueurs[tour_joueur].bonus_bois;
      joueurs[tour_joueur].gold += joueurs[tour_joueur].bonus_gold;
      cout << endl
           << "Tour n°" << tours << endl
           << "Joueur " << tour_joueur + 1 << ". Votre tour commence !" << endl;
      affiche_ressources();
    } else {
      cout << "help : Vous devez effectuer un clique molette sur le chateau "
              "pour terminer "
              "votre tour."
           << endl;
    }
  }
};
