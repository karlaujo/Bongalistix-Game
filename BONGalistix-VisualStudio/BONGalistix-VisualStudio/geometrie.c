
// -------------------------------------------
/*
librairie geometrie.c
Auteur initial:David Marche
Compl�t� par: Karl-�ric Jolicoeur et Franck-Yannick Mbiele Miengue Fosso
Date: Automne 2021
Description: Librairie de fonctions d'analyses geometriques et autres
			 fonctions g�n�riques utiles.
			 ex: calcul du point d'intersection deux droites
Note: les prototypes des fonctions se trouvent dans le fichier geometrie.c
*/
// -------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include "geometrie.h"
#include "math.h"


// ---------------------------------------------
/* fonction: solution_systeme_2eq
	Description:    fonction qui trouve la solution d'un syst�me d'�quation � deux inconnue
					par la m�thode du d�terminant de Kramer.
					Le syst�me d'�quation est

					> a1.x + b1.y = c1 <
					et
					> a2.x + b2.y = c2 <

					3 �tapes pour appliquer cette m�thode:
					Etape 1: trouver le d�terminant du syst�me (d)
								d=a1.b2-b1.a1
							 s'il est nul, pas de solution. Sinon continuer
					�tape 2: calculer le d�terminant de x (dx) puis la valeur de x
								dx=c1.b2-b1.c2
								x=dx/d
					�tape 3: calculer le d�terminant de y (dy) puis la valeur de y
								dy=a1.c2-c1.a2
								y=dy/d
	Param�tre(s):   a1,a2,b1,b2,c1,c2
							les coefficients du syst�me d'�quations:
							a1.x+b1.y=c1 et a2.x+b2.y=c2
					x,y     la solution trouv�e par la fonction
	Retour:         0 si le syst�me n'a pas de solution 1 sinon. Les valeurs de x et y
					sont modifi�es par la fonction pour contenir la solution du syst�me
					d'�quation.
	Note:           S'il n'y a pas de solution, x et y sont fix�s � 0.
	Ex. d'utilisation:
					// solutionner le syst�me : 3x+4y=5 et 6x+7y=8 et la placer dans x et y
					solution_systeme_2eq(3,4,5,6,7,8,&x,&y);
*/
int solution_systeme_2eq(double a1, double b1, double c1, double a2, double b2, double c2, double* x, double* y) { /*fonction qui trouve la solution de deux systems d'�quation 
																												   partageant les memes inconnus*/

	double d, dx, dy; // variables des d�terminants
	int result = 0;   // retour de la fonction

	//Etape 1
	d = (a1 * b2) - (b1 * a2);

	if (d == 0)
	{
		result = 0;
	}

	else
	{
		dx = (c1 * b2) - (b1 * c2);
		dy = (a1 * c2) - (c1 * a2);
		*x = dx / d;
		*y = dy / d;
		result = 1;
	}

	return result;
}

// ---------------------------------------------
/* fonction: equation_droite
	Description:    Trouve l'�quation de la droite passant par les points
					(x1,y1) et (x2,y2)
					L'�quation de la droite est de la forme c.y=a.x+b
					ou le coefficient c est utile pour pouvoir �galement
					repr�senter les droites verticales (c=0)
					Deux cas possible:
					1- la droite est vertical (x1==x2)
						Dans ce cas, c=0, a=1 et b=-x1
					2- la droite n'est pas verticale
						Dans ce cas, c=1 et l'�quation de la droite est
						y=a.x+b
						il faut trouver a et b pour que la droite passe par les 2 points
						donn�s:
						y1=a.x1+b et y2=a.x2+b  (ou a et b sont les inconnues)
						Il faut donc r�soudre le syst�me suivant pour trouver a et b:

					  > x1.a + 1.b = y1 <
					  et
					  > x2.a + 1.b = y2 <

						pour trouver a et b

	Param�tre(s):   x1,y1   premier point de passage de la droite
					x2,y2   deuxi�me point de passage de la droite
					a,b,c   les coefficients de la droite tels que calcul�s par la fonction
	Retour:         0 si l'�quation ne peut pas �tre d�termin�e, 1 sinon
					les coefficient trouv�s sont mis dans les variables a, b et c
	Ex. d'utilisation:
					// trouver les coefficient a,b,c de la droite passant par les points
					// (2,2) et (4,3)
					// l'�quation de la droite sera donc c.y=a.x+b
					equation_droite(2,2,4,3,&a,&b,&c);
*/
int equation_droite(double x1, double y1, double x2, double y2, /*fonction qui a pour but de trouver l'equation d'une droite qui passe par deux points donn�s*/
	double* a, double* b, double* c) {
	
	// renvoyer 1 si un point d'intersection existe. 0 sinon.

	double result = 0;
	
	


	if (x1 == x2)
	{
		*c = 0;
		*a = 1;
		*b = -x1;
		result = 1;
	}
	else
	{
		*c = 1;
		solution_systeme_2eq(x1, 1, y1, x2, 1, y2, a, b);
		result = 1;
	}

	return result;
}
// ---------------------------------------------
/* fonction: intersection_droites
	Description:    intersection de droites donn�es par leur �quations.
					droite 1:
					> c1.y=a1.x+b1 <

					droite 2:
					> c2.y=a2.x+b2 <

					le point d'intersection est trouv� en r�solvant le syst�me
					d'�quation:

					> -a1.x+c1.y=b1 <

					et

					> -a2.x+c2.y=b2 <

	Param�tre(s):   a1,b1,c1   coefficient de l'�quation de la droite 1
					a2,b2,c2   coefficient de l'�quation de la droite 2
					x,y     les coordonn�es du point d'intersection entre les deux droites.
	Retour:         0 si les droites n'ont pas de point d'intersection, 1 sinon.
					Les coordonn�es du point d'intersection trouv� sont plac�es dans x et y
	Ex. d'utilisation:
					// trouver le point d'intersection entre la droite 1: y=0,5.x+2
					// et la droite 2 (verticale en x=3): 0.y=1.x-3
					// et placer la solution dans les variables x et y
					intersection_droites(0.5,2,1,1,-3,0,&x,&y);
*/

int intersection_droites(double a1, double b1, double c1,
	double a2, double b2, double c2,
	double* x, double* y) {
	double result = 0;

	/*
	a1 = -a1
	b1 = c1
	c1 = b1
	a2 = -a2
	b2 = c2
	c2 = b2
	*/

	// renvoyer 1 si un point d'intersection existe. 0 sinon.

	if (solution_systeme_2eq(-a1, c1, b1, -a2, c2, b2, x, y))
	{
		result = 1;
	}

	else
	{
		result = 0;
	}

	return result;
}


// ---------------------------------------------
/* fonction: intersection_segments
	Description:    intersection de segments donn�es par les coordonn�es de leurs
					extr�mit�s.
					segment 1   entre A1=(ax1,ay1) et B1=(bx1,by1)
					segment 2   entre A2=(ax2,ay2) et B1=(bx2,by2)
					La fonction trouve d'abord les �quation des droites portant chaque segment.
					Elle d�termine ensuite le point d'intersection de ces deux droite
					puis v�rifie finalement si ce point appartient au deux segments.
	Param�tre(s):   ax1,ay1 coordonn� d'une extremit� du segment 1
					bx1,by1 coordonn� de l'autre extremit� du segment 1
					ax2,ay2 coordonn� d'une extremit� du segment 2
					bx2,by2 coordonn� de l'autre extremit� du segment 2
					x,y     les coordonn�es du point d'intersection entre les segments.
	Retour:         0 si les segments se croisent, 0 sinon. Les coordonn�es du point
					de croisement sont mise dans x et y
	Note:           Si les droites portant les segments se croisent, en un point
					� l'ext�rieur d'un des deux segments, il n'y a pas d'intersection
					et la fonction renvoie donc 0
	Ex. d'utilisation:
					// trouver le point d'intersection entre de segment 1:
					// (1,2)-(3,4)
					// et le segment 2:
					// (5,6)-(7,8)
					// et placer la solution dans les variable x et y
					intersection_segments_param(1,2,3,4,5,6,7,8,&x,&y);
*/
int intersection_segments(double ax1, double ay1, double bx1, double by1, /*verifier quelle est l'�quation des droites de chaque segment et 
																			si ces deux droites s'intersectionnent a l'interieur des segments en question*/
	double ax2, double ay2, double bx2, double by2,
	double* x, double* y) {
	int result = 0;
	double a1, b1, c1;    // coefficients de l'�quation de la droite du segment 1
	double a2, b2, c2;    // coefficients de l'�quation de la droite du segment 2

	// renvoyer 1 si les segments se croisent, 0 sinon
	
	/*
	x1 = ax
	y1 = ay
	x2 = bx
	y2 = by
	*/

	if (equation_droite(ax1, ay1, bx1, by1, &a1, &b1, &c1) == 0 || equation_droite(ax2, ay2, bx2, by2, &a2, &b2, &c2) == 0)
	{
		result = 0;
	}

	else
	{
		intersection_droites(a1, b1, c1, a2, b2, c2, x, y);

		if (element_du_segment(ax1, ay1, bx1, by1, *x, *y) && element_du_segment(ax2, ay2, bx2, by2, *x, *y))
		{
			result = 1;
		}
	}

	return result;
}


// ---------------------------------------------
/* fonction: element_du_segment
	Description:    teste si un point appartenant � la droite d'un segment
					fait parti du segment
					Segment donn� par les extr�mite (ax,ay)-(bx,by) point donn� par (x,y)
	Param�tre(s):   ax,ay coordonn� d'une extremit� du segment
					bx,by coordonn� de l'autre extremit� du segment
					x,y     les coordonn�es du point element de la droite du segment
	Note:           Aucune v�rification sur le fait que le point appartient r�ellement
					� la droite du segment.
	Retour:         1 si le point appartient au segment, 0 sinon
	Ex. d'utilisation:
					// v�rifier si le point (0,0) appartient au segment
					// (-2,-2)-(3,3)
					if (intersection_segments(-2,-2,3,3,0,0))
	Attention:  si le segment est vertical seul les ordonn�es sont compar�s pour �viter
				des probl�me de valeur non exactes dues au repr�sentation en virgule flottante
				Dans tous les autres cas seuls les abscisses sont compar�es.
				Ex de cas probl�me qui ne fonctionnerait pas avec d'autres m�thode de comparaison:
				(2,10.0000002) appartient-il � la droite (0,10)-(4,10)
				(4.9999999999,8) appartient-il � la droite (5,0)-(5,15)
*/

int element_du_segment(double ax, double ay, double bx, double by, double x, double y) { /*verifier si un certain point faisantt parti d'une droite fait parti du segment meme
																							*/

	int result = 0;

	// si le segment est vertical, comparer les ordonn�es

	double a, b, c;

	/*
	x1 = ax
	y1 = ay
	x2 = bx
	y2 = by
	*/

	equation_droite(ax, ay, bx, by, &a, &b, &c);

	if (c == 0 && x == ax)
	{

		if (y >= ay && y <= by || y <= ay && y >= by)
		{
			result = 1;
		}
		else
		{
			result = 0;
		}
	}

	// dans tous les autres cas, comparer les abscisse
	
	else
	{
		if (x >= ax && x <= bx || x >= bx && x <= ax)
		{
			result = 1;
		}
		else
		{
			return 0;
		}

	}

	return result;
}

// ---------------------------------------------
/* fonction: signe
	Description:    calcule le signe d'un r�el
	Param�tre(s):   x   le r�el pourlequel le signe doit �tre determin�
	Retour:         -1 si le r�el est n�gatif. 1 dans tous les autre cas
	Ex. d'utilisation:
					x=signe(-10);   // renvoie -1
					x=signe(34);    // renvoie 1
					x=signe(0);     // renvoie 1
*/
int signe(double x) {
	int s = 1;
	if (x < 0) s = -1;
	return s;
}

// ---------------------------------------------
/* fonction: longueur
	Description:    calcule la longueur d'un segment connaissant les coordonn�es
					de ses extr�mit�s
	Param�tre(s):   x0,y0       coordonn�es d'une extr�mit�
					x1,y1       coordonn�es de l'autre extr�mit�
	Retour:         la longueur du segment (par la formule de pythagore)
	Ex. d'utilisation:
					x=longueur(10,10,15,20);    // calcule la longueur du segment reliant les points
												// (10,10) � (15,20)
*/
double longueur(double x0, double y0, double x1, double y1) {
	return sqrt(pow(x0 - x1, 2) + pow(y0 - y1, 2));
}


