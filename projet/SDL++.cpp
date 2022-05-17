#include <SDL2/SDL.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <bits/types/cookie_io_functions_t.h>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
//
#include <SDL2/SDL_image.h>

using namespace std;

/*
------------------------------------------------------------


Ajout de multiples fonctionalités rendant l'utilisation de la
SDL plus facile et plus pratique en c++.


------------------------------------------------------------
*/

// Creation d'une classe rectangle avec un constructeur
// car celle de la SDL n'en propose pas
class Rectangle : public SDL_Rect {
public:
  Rectangle() {}
  Rectangle(int px, int py, int pw, int ph) : SDL_Rect() {
    x = px;
    y = py;
    w = pw;
    h = ph;
  }
};

// Creation d'une classe color avec un constructeur
// car celle de la SDL n'en propose pas
class Color : public SDL_Color {
public:
  Color() {}
  Color(int pr, int pg, int pb, int pa) : SDL_Color() {
    r = pr;
    g = pg;
    b = pb;
    a = pa;
  }
};

// Fonction Permettant de quitter le programme en cour.
// En renvoyant l'erreur associée.
void SDL_ExitWithError(const char *message) {
  SDL_Log("Erreur : %s > %s\n", message, SDL_GetError());
  SDL_Quit();
  exit(EXIT_FAILURE);
}

int absol(int x) {
  if (x < 0) {
    int tmp = x;
    x = x * x;
    x = sqrt(x);
    return x;
  } else {
    return x;
  }
}
// Ecrit un string (str) dans un fichier de nom "filename"
void write_result(string str, string filename) {
  fstream outfile;

  outfile.open(filename, std::ios_base::out);
  if (!outfile.is_open()) {
    cout << "failed to open " << filename << '\n';
  } else {
    outfile.write(str.data(), str.size());
    /* cout << "Done Writing!" << endl; */
  }
}

Color color_white(255, 255, 255, SDL_ALPHA_OPAQUE);
Color color_red(255, 0, 0, SDL_ALPHA_OPAQUE);
Color color_green(0, 153, 51, SDL_ALPHA_OPAQUE);
Color color_orange(255, 153, 51, SDL_ALPHA_OPAQUE);
Color color_brown(69, 51, 3, SDL_ALPHA_OPAQUE);
Color color_blue(16, 75, 201, SDL_ALPHA_OPAQUE);
Color color_orange_transparent(255, 153, 51, SDL_ALPHA_TRANSPARENT);