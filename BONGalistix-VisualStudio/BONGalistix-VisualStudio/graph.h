#ifndef GRAPH_H
#define GRAPH_H

// -------------------------------------------
/*
librairie graph.h
Auteur:David Marche
Date: Automne 2021
Description: Librairie de fonctions graphiques encapsulant certaines fonctions
             de graphisme de la librairie SDL. Pour le devoir, utiliser uniquement
             ces fonctions pour la gestion de l'affichage graphique
Note:        Les implémentations des fonctions se trouvent dans le fichier graph.c
*/
// -------------------------------------------

// -------------------------------------------
//      NE PAS MODIFIER CE FICHIER
// -------------------------------------------


// -------------------------------------------
// librairie SDL
// Le fichier SDL2.dll doit se trouver dans
// le répertoire de l'exécutable
// -------------------------------------------
#include <SDL.h>

// ---------------------------------------------
// variables globales de gestion du graphisme
// et des évènemensts de souris avec la librairie SDL
// (extern car déclarées et initialisées dans graph.c)
// ---------------------------------------------
extern SDL_Window* fenetre;            // Fenetre graphique
extern SDL_Renderer* renderer;         // renderer utilisé pour le graphisme
extern Uint8 r,g,b,a;                  // variables pour la gestion des couleurs
extern SDL_Event event;                // evenement souris/clavier/croix/...

// -----------------------------------------
// constantes utiles à l'affichage graphique
// -----------------------------------------
// dimensions de la fenêtre graphique
#define LARGEUR_FEN   400
#define HAUTEUR_FEN   400

// ---------------------------------------------
// Prototypes des fonctions d'encapsulation des
// graphiques de la librairie SDL
// ---------------------------------------------

void graph_select_couleur(int r, int g, int b, int a);
void graph_clear(int r, int g, int b, int a);
void graph_trace_point(int x,int y);
void graph_trace_segment(int x1,int y1,int x2,int y2);
void graph_trace_rectangle(int x1,int y1,int x2,int y2);
void graph_mise_a_jour();
void graph_get_mousexy(int *mousex,int *mousey);
void graph_delai_ms(int milisec);
void graph_titre(char* titre);
int graph_init(char* titre,int largeur, int hauteur, int r, int g, int b, int a);
void graph_fin();

#endif // GRAPH_H
