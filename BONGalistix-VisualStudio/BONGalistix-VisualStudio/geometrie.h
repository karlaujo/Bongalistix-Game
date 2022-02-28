#ifndef GEOMETRIE_H
#define GEOMETRIE_H

// -------------------------------------------
/*
librairie geometrie.h
Auteur:David Marche
Date: Automne 2021
Description: Librairie de fonctions d'analyses geometriques et autres
             fonctions génériques utiles.
             ex: calcul du point d'intersection deux droites
Note: les prototypes des fonctions se trouvent dans le fichier geometrie.c
*/
// -------------------------------------------

// ---------------------------------------------
// Prototypes des fonctions de la librairie
// ---------------------------------------------

int solution_systeme_2eq(double a1, double b1,double c1,
                         double a2, double b2, double c2,
                         double* x,double* y);

int equation_droite(double x1,double y1, double x2, double y2,
                    double *a, double *b, double *c);

int intersection_droites(double a1, double b1, double c1,
                         double a2, double b2, double c2,
                         double *x, double *y);

int intersection_segments(double ax1, double ay1, double bx1, double by1,
                          double ax2,double ay2,double bx2, double by2,
                          double* x, double *y);

int element_du_segment(double ax,double ay,double bx,double by,
                       double x,double y);

int signe(double x);

double longueur(double x0, double y0, double x1, double y1);


#endif // GEOMETRIE_H
