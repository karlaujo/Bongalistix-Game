/*
programme de ballistique avec rebond
Auteur initial:David Marche
Modification: Abdelmoumène Toudeft
Complété par: Karl-Éric Jolicoeur et Franck-Yannick Mbiele Miengue Fosso
Date: Automne 2021
Description: Jeu de tir pour atteindre une cible. L'objet lancé est sujet à la gravité et
            rebondit sur les murs qu'il rencontre. Le tir est réussi si la cible est atteinte
            avant que le projectile n'atteigne une vitesse quasi-nulle. Le niveau suivant est
            alors chargé et le jeu continue. Il n'y a pas de limite de nombre de tirs pour réussir
            un niveau. Quand le dernier niveau est réussi, le jeu recommence au premier niveau.
*/

// -------------------------------------------
// librairie standard d'entrée/sortie
// et de mathématique
// -------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// -------------------------------------------
// librairie graphique
// -------------------------------------------
#include "graph.h"

// -------------------------------------------
// librairie géométrique et autres fonctions utiles
// -------------------------------------------
#include "geometrie.h"

// -------------------------------------------
// constantes utiles aux calculs mathématiques
// -------------------------------------------
#define PI 3.14159

// ------------------------------------------
// constantes utiles à la simulation numérique
// et la modélisation du projectile
// ------------------------------------------
#define MASSE           2       // masse de l'objet lancé (kg)
#define RAYON           0.05    // rayon du projectile (m)
#define CX              0.4     // coef. de résistance aérodynamique d'une sphère
#define RHO             1.2     // masse volumique de l'air
#define GRAVITE         9.8     // accélération gravitationnelle m2/s
#define C_RESTITUTION   0.95    // coefficient de restitution de vitesse de la paroi lors des rebonds
#define C_FROTTEMENT    (0.5*CX*RHO*PI*RAYON*RAYON) // coefficient de frottement de l'air pour une sphère
#define V_INITIALE_MAX  120     // vitesse initiale maximum (m/s)
#define T_IMMOBILE_MAX  2       // temps d'immobilité maximum du projectile avant de déclarer la fin d'un lancer (s)
#define DELTAT          0.1     // incrément de temps maximum lors de la simulation numérique du vol du projectile (s)
#define DECALAGE_REBOND 0.05    // distance à la paroi à laquelle le projectile est replacé lors des rebonds

// -----------------------------------------
// contantes utiles au chargement des différents niveaux
// tous les niveaux doivent se trouver dans un dossier au même endroit que l'exécutable
// le nom des fichiers de niveau est toujours le même, seul l'index varie
// ex: "Niveaux\niveau1.text" ... "Niveaux\niveau10.text"
// Le premier niveau est le plus facile.
// -----------------------------------------

#define DOSSIER_NIVEAU              "Niveaux" // dossier dans lequel se trouvent les fichiers de niveaux
#define DEBUT_NOM_FICHIER_NIVEAU    "niveau"  // préfixe du nom de ficher de niveau (ex:niveau4.txt)
#define FIN_NOM_FICHIER_NIVEAU      ".txt"    // extension du nom de fichier de niveau (ex:niveau4.txt)
#define PREMIER_NIVEAU              1         // index du premier niveau (ex: "Niveaux\niveau1.text")
#define DERNIER_NIVEAU              18        // index du dernier niveau (ex: "Niveaux\niveau17.text")

// -----------------------------------------
// constantes utiles à l'affichage graphique
// -----------------------------------------
// couleur RGBA
// valeur comprise entre 0 et 255
// couleur de fond de la fenêtre graphique (RGBA)
#define COULEUR_BG_R   0
#define COULEUR_BG_G   0
#define COULEUR_BG_B   0
#define COULEUR_BG_A   255  // A est la transparence. 255=pas transparent, 0=invisible
// couleur de dessin pour les murs (RGBA)
#define COULEUR_MUR_R  255
#define COULEUR_MUR_G  255
#define COULEUR_MUR_B  255
#define COULEUR_MUR_A  255
// couleur de dessin pour le lanceur
#define COULEUR_LAN_R  255
#define COULEUR_LAN_G  0
#define COULEUR_LAN_B  0
#define COULEUR_LAN_A  255
// couleur du projectile
#define COULEUR_PRO_R  0
#define COULEUR_PRO_G  255
#define COULEUR_PRO_B  0
#define COULEUR_PRO_A  255
// couleur de la cible
#define COULEUR_CIB_R  0
#define COULEUR_CIB_G  255
#define COULEUR_CIB_B  255
#define COULEUR_CIB_A  255

// taille de la représentation graphique du projectile (carré)
#define TAILLE_PRO 2    // nb pixels ajoutés de chaque côté de son centre.

// ---------------------------------------------
// dimensions du tableau des segments de murs du niveau en cours
// ---------------------------------------------
#define DIMENSION_POINT     2       // nb de dimension pour les coordonnées des points 2 pour 2D, 3 pour 3D
#define POINT_PAR_SEGMENT   2       // nb de points pour définir un segment (mur)
#define MAXSEGMENT          100     // nb maximum de segments (murs)

#define COL_TABSEG  (DIMENSION_POINT*POINT_PAR_SEGMENT)     // nb de col du tableau de segments de murs
#define COL_TABCIB  (DIMENSION_POINT*POINT_PAR_SEGMENT)     // nb de col du tableau d'info sur la cible
#define COL_TABLAN  (DIMENSION_POINT*POINT_PAR_SEGMENT)     // nb de col du tableau d'info sur le lanceur
#define COL_TABPRO  (DIMENSION_POINT*POINT_PAR_SEGMENT)     // nb de col du tableau d'info sur le projectile
#define LIG_TABPRO  2                                       // nb de ligne du tableau d'info sur le projectile

// indices de colonne d'accès aux élément des tableau
#define X0                  0
#define Y0                  1
#define X1                  2
#define Y1                  3

// indices de ligne d'accès aux éléments du tableau de projectile
#define POSITION            0
#define VITESSE             1

// ---------------------------------------------
// limite de taille pour les chaines de caractères
// ---------------------------------------------
#define MAX_CHAR    100

// ---------------------------------------------
// différents modes (états) dans lequel le jeu peut-être
// ---------------------------------------------

#define MODE_DEBUT                  0   // Mode initial du programme
#define MODE_QUITTER                1   // L'utilisateur veux quitter le programme
#define MODE_CHARGEMENT_NIVEAU      2   // Chargement et affichage du prochain fichier de niveau
#define MODE_AJUSTEMENT_LANCER      3   // Choix de la direction et force d'un lancé
#define MODE_DEPART_LANCER          4   // application de la position et vitesse initiale au projectile
#define MODE_SIMULATION_VOL         5   // Simulation numérique de la trajectoire du projectile
#define MODE_TOUCHER_CIBLE          6   // Affichage indiquant que le niveau réussi
#define MODE_RATER_CIBLE            7   // Affichage indiquant que la balle s'est immobilisée sans toucher la cible

// ---------------------------------------------
// différents éléments graphique pouvant être visible ou non
// ---------------------------------------------
#define MURS_VISIBLE        8  //(0b1000)      // valeur à utiliser pour rendre les murs visible lors de l'affichage
#define PROJECTILE_VISIBLE  4  //(0b0100)      // valeur à utiliser pour rendre le projectile visible lors de l'affichage
#define LANCEUR_VISIBLE     2  //(0b0010)      // valeur à utiliser pour rendre le lanceur visible lors de l'affichage
#define CIBLE_VISIBLE       1  //(0b0001)      // valeur à utiliser pour rendre la cible visible lors de l'affichage
#define TOUS_VISIBLE        15 //(0b1111)      // valeur à utiliser pour rendre tous les éléments visible lors de l'affichage
#define TOUS_INVISIBLE      0  //(0b0000)      // valeur à utiliser pour rendre tous les éléments invisible lors de l'affichage

// ---------------------------------------------
// paramètres de clignotement de la cible ou des murs lors de la fin d'un lancer (succes ou échec)
// ---------------------------------------------
#define PERIODE_CLIGNOTEMENT    200     // période de clignotement des murs ou de la cible lors de la fin d'un lancer
#define NB_CLIGNOTEMENT         4       // nombre de clignotement des murs ou de la cible lors de la fin d'un lancer

// ---------------------------------------------
// prototypes des fonctions
// ---------------------------------------------

void afficher_elements(int elements_visibles,
                       int tab_murs[][COL_TABSEG],int nb_murs,
                       int tab_cible[],
                       int tab_lanceur[],
                       double tab_projectile[][COL_TABPRO]);
int afficher_murs(int tab_segments[][COL_TABSEG],int nb_segments,int max_segments);
void afficher_projectile(double tab_projectile[][COL_TABPRO]);
void afficher_cible(int tab_cible[]);
void afficher_lanceur(int tab_lanceur[]);

void calcule_lanceur(int mousex,int mousey,int vmax,int tab_lanceur[]);

void mise_a_jour_ballistique(double tab_projectile[][COL_TABPRO],double *t,double dt);
void mise_a_jour_rebond(int tab_segments[][COL_TABSEG],int nb_segments,
                        double tab_projectile[][COL_TABPRO],
                        double *t,double *dt);

int cible_atteinte(double tab_projectile[][COL_TABCIB],int tab_cible[]);
int projectile_immobile(double tab_projectile[][COL_TABPRO]);

void affiche_tab2D(int tab_segments[][COL_TABSEG],int nb_segments);
int lire_fichier_niveau(int niveau, char* dossier, char* txt_debut_fichier, char* txt_fin_fichier,
                        int tab_seg[][COL_TABSEG],int tab_lanceur[], int tab_cible[]);


// ---------------------------------------------
// programme principal
// ---------------------------------------------

int main( int argc, char* args[] ){

    // variables:

    // éléments du jeu
    int tab_segments[MAXSEGMENT][COL_TABSEG]={{0}};   // tableau de segments
    int tab_lanceur[COL_TABLAN]={0};        // coordonnées de la position et du bout du lanceur
    int tab_cible[COL_TABCIB]={0};          // coordonnées de la cible (coin inférieur gauche et coin supérieur droit)
    double tab_projectile[LIG_TABPRO][COL_TABCIB]={{0}}; // position/vitesse actuelle et précédente du projectile

    // état du jeu
    char titre_fenetre[MAX_CHAR];           // titre de la fenetre pour afficher le numéro du niveau
    int mode=MODE_DEBUT;                    // mode du jeu
    int niveau=PREMIER_NIVEAU;              // index du niveau
    int nb_segments=0;                      // nb de segments de murs valides dans le tableau de segments
    double t=0;                             // temps écoulé lors de la simulation du vol du projectile
    double dt=DELTAT;                       // incrément de temps de la simulation du vol
    double chrono_immobile=0;               // chronomètre pour compter le temps d'immobilité du projectile
    int elements_visibles=TOUS_INVISIBLE;   // définition des éléments graphiques visibles

    // interaction souris
    int mousex=0,mousey=0;                      // position de la souris

    // autres
    int i;                                  // itérateur pour le décompte de clignotement


    // initialiser le graphisme et ouvrir la fenêtre d'affichage du jeu
    if (!graph_init("BONGallistix: Balistique et rebonds",LARGEUR_FEN,HAUTEUR_FEN,
                    COULEUR_BG_R,COULEUR_BG_G,COULEUR_BG_B,COULEUR_BG_A)){
        printf("[ERR] - Creation de la fenetre graphique impossible \n");
        mode=MODE_QUITTER;
    }
    else{
        printf("[ OK] - Creation de la fenetre graphique \n");
        // la fenêtre graphique à pu être créée, passer en mode chargement de niveau
        mode=MODE_CHARGEMENT_NIVEAU;

        // tant que le mode quitter n'est pas atteint, continuer le jeu
        while (mode!=MODE_QUITTER){
            // gérer les évènements de souris affectant la fenêtre graphique
            while(SDL_PollEvent(&event))
                {
                    switch(event.type){
                        case SDL_QUIT:
                            mode = MODE_QUITTER;
                            printf("[ OK] - Mode QUITTER\n");
                            break;
                        case SDL_MOUSEMOTION:
                                graph_get_mousexy(&mousex,&mousey);
                                break;
                        case SDL_MOUSEBUTTONUP:
                            switch(mode){
                                case MODE_AJUSTEMENT_LANCER:
                                    mode=MODE_DEPART_LANCER;
                                    break;
                                case MODE_SIMULATION_VOL:
                                    mode=MODE_CHARGEMENT_NIVEAU;
                                    break;
                            }
                            break;
                    }
                }

            // selon le mode mettre à jour le lanceur, projectile, ... et les éléments qui doivent être visibles à l'écran
			switch (mode) {

			case MODE_CHARGEMENT_NIVEAU:
				// Charger le niveau en mémoire
				nb_segments = lire_fichier_niveau(niveau, DOSSIER_NIVEAU, DEBUT_NOM_FICHIER_NIVEAU, FIN_NOM_FICHIER_NIVEAU,
					tab_segments, tab_lanceur, tab_cible);

				// si le chargement à fonctionné, l'afficher et passer au lancer
				if (nb_segments < 0) {
					printf("[ERR] - Mode CHARGEMENT_NIVEAU (niveau %d)\n", niveau);
					mode = MODE_QUITTER;
				}
				else {
					printf("[ OK] - Mode CHARGEMENT_NIVEAU (niveau %d)\n", niveau);
					// ajuster le titre de la fenetre en fonction du niveau chargé
					sprintf(titre_fenetre, "BONGallistix - Niveau %d", niveau);
					graph_titre(titre_fenetre);
					// passer au mode d'ajustement de lancer
					mode = MODE_AJUSTEMENT_LANCER;
					// définir les élément visible aprèes le chargement de niveau pour le prochain affichage
					elements_visibles = MURS_VISIBLE | CIBLE_VISIBLE | LANCEUR_VISIBLE;
					// afficher un petit message dans la console
					printf("[ OK] - Mode AJUSTEMENT_LANCER\n");
				}
				break;

			case MODE_AJUSTEMENT_LANCER:
				// calculer les paramètres du lanceur en fonction de la position de la souris
				calcule_lanceur(mousex, mousey, V_INITIALE_MAX, tab_lanceur);
				// définir les éléments qui sont visibles lors de l'ajustement du lancer pour le prochain affichage
				elements_visibles = MURS_VISIBLE | CIBLE_VISIBLE | LANCEUR_VISIBLE;
				break;

				mode = MODE_DEPART_LANCER;

			case MODE_DEPART_LANCER:
				printf("[ OK] - Mode DEPART_LANCER\n");
				// initialiser la position (x1,y1) du projectile selon le point d'origine du lanceur,
							
				tab_projectile[POSITION][X1] = tab_lanceur[X0];
				tab_projectile[POSITION][Y1] = tab_lanceur[Y0];

				// la vitesse (vx1,vy1) du projectile selon la longueur du lanceur

					tab_projectile[VITESSE][X1] = tab_lanceur[X1] - tab_lanceur[X0];
					tab_projectile[VITESSE][Y1] = tab_lanceur[Y1] - tab_lanceur[Y0];

				// remise du temps a 0
			
				t = 0; 

                // passer au mode de simulation de trajectoire
                 
				mode = MODE_SIMULATION_VOL;
                    // définir les éléments visibles pour le prochain affichage
                    elements_visibles = MURS_VISIBLE | CIBLE_VISIBLE | LANCEUR_VISIBLE | PROJECTILE_VISIBLE;

                    // afficher un petit message dans la console
				mode = MODE_SIMULATION_VOL;
                    printf("[ OK] - Mode SIMULATION_VOL\n");
					break;

                case MODE_SIMULATION_VOL:
                    // ajuster le titre de la fenetre en fonction du niveau chargé
                    sprintf(titre_fenetre,"BONGallistix - Niveau %d - temps: %5.2lf",niveau,t);
                    graph_titre(titre_fenetre);

                    // recalculer la position du lanceur pour qu'il suive la souris pendant la simulation de vol
                	
					calcule_lanceur(mousex, mousey,V_INITIALE_MAX, tab_lanceur);

                    // mise à jour ballistique de la position/vitesse (ignorer les rebonds pour le moment)
                   
					mise_a_jour_ballistique(tab_projectile, &t, dt);

                    // corriger la nouvelle position/vitesse pour tenir compte des rebonds
                    
					mise_a_jour_rebond(tab_segments, nb_segments,
						tab_projectile, &t, &dt);

                    // appliquer un délai à la simulation pour que l'écoulement du temps soit réaliste
                    // Note: le delai est appliqué directement en secondes pour que la simulation
                    // soit 1000 fois plus rapide que la réalité et ajoute du dynamisme au jeu
                    graph_delai_ms(dt);

                    // vérifier si la cible est atteinte et éventuellement changer le mode en conséquence

                    if (cible_atteinte(tab_projectile,tab_cible))
					{
                       mode = MODE_TOUCHER_CIBLE;
                    }
                    else{ // si la balle n'a pas bougé d'un pixel,
                          // chronométrer le temps d'immobilité du projectile
                        if (projectile_immobile(tab_projectile)){
                            // si la balle n'a pas bougé d'un pixel mettre à jour
                            // le chronomètre d'immobilitée
                            chrono_immobile+=dt;
                            // vérifier que le temps max d'immobilité n'est pas atteint
                            if (chrono_immobile>=T_IMMOBILE_MAX){
                                // la cible est raté passer au mode correspondant
                                ///*** à compléter ***///
								mode = MODE_RATER_CIBLE;

                                // remettre le chronomètre à 0
                                ///*** à compléter ***///
								chrono_immobile = 0;

                                //printf("vx=%lf vy=%lf vx0=%lf vy0=%lf diff=%lf dt=%lf\n",vx,vy,vx0,vy0,sqrt(pow(vx-vx0,2)+pow(vy-vy0,2)),dt);

                            }
                        }
                        else chrono_immobile=0; // si la balle bouge encore, remettre le chrono d'immobilité à 0
                    }

                    // remettre l'increment de temps à sa valeur par défaut
                    dt=DELTAT;

                    // définir les éléments qui sont visibles lors de la simulation de vol pour le prochain affichage
                    ///*** à compléter ***///

					elements_visibles = TOUS_VISIBLE;

                    break;

                case MODE_TOUCHER_CIBLE:
                    printf("[ OK] - Mode TOUCHER_CIBLE\n");
                    // en cas de réussite, faire clignoter la cible selon
                    // les paramètres NB_CLIGNOTEMENT et PERIODE_CLIGNOTEMENT
                    					
					if (cible_atteinte(tab_projectile, tab_cible))
					{
						for (i = 0; i < NB_CLIGNOTEMENT; i++)
						{
							elements_visibles = TOUS_VISIBLE;

							afficher_elements(elements_visibles,
								tab_segments, nb_segments,
								tab_cible,
								tab_lanceur,
								tab_projectile);

							graph_delai_ms(PERIODE_CLIGNOTEMENT);
							
							elements_visibles = MURS_VISIBLE | LANCEUR_VISIBLE | PROJECTILE_VISIBLE;
							
							afficher_elements(elements_visibles,
								tab_segments, nb_segments,
								tab_cible,
								tab_lanceur,
								tab_projectile);

							graph_delai_ms(PERIODE_CLIGNOTEMENT);
						}
					}
					
					

					printf("mode toucher cible");

                    // passer au niveau suivant ou revenir au premier si fin des niveaux
                    
					if (niveau == DERNIER_NIVEAU)
					{
						niveau = PREMIER_NIVEAU;
					}

					else 
					{
						niveau += PREMIER_NIVEAU;
					}

					printf("Niveau = %d", niveau);

                    //revenir au mode de chargement de niveau
                    
					
					mode = MODE_CHARGEMENT_NIVEAU;

                    break;

                case MODE_RATER_CIBLE:
                    printf("[ OK] - Mode RATER_CIBLE\n");
                    // en cas d'échec du lancer, faire clignoter les murs selon
                    // les paramètres NB_CLIGNOTEMENT et PERIODE_CLIGNOTEMENT
                    
					
					if (projectile_immobile(tab_projectile))
					{
						for (i = 0; i < NB_CLIGNOTEMENT; i++)
						{

							elements_visibles = TOUS_VISIBLE;

							afficher_elements(elements_visibles,
								tab_segments, nb_segments,
								tab_cible,
								tab_lanceur,
								tab_projectile);

							graph_delai_ms(PERIODE_CLIGNOTEMENT);

							elements_visibles =  LANCEUR_VISIBLE | PROJECTILE_VISIBLE | CIBLE_VISIBLE;

							afficher_elements(elements_visibles,
								tab_segments, nb_segments,
								tab_cible,
								tab_lanceur,
								tab_projectile);

							graph_delai_ms(PERIODE_CLIGNOTEMENT);

						}

						mode = MODE_CHARGEMENT_NIVEAU;
					}
					break;                                                                                    // TODO

            }

            // afficher/mettre à jour les éléments qui doivent être visibles
            afficher_elements(  elements_visibles,
                                tab_segments,nb_segments,
                                tab_cible,
                                tab_lanceur,
                                tab_projectile);
        }
    }


    // fin du graphisme
    graph_fin();

    return EXIT_SUCCESS;
}
// ---------------------------------------------
/* fonction: projectile_immobile
    Description:    Teste si le projectile est immobile en comparant l'ancienne et
                    la nouvelle position
    Note:           Le projectile est visuellement immobile s'il ne change pas de pixel,
                    donc si la partie entière de ses coordonnées n'ont pas changées
    Paramètre(s):   tab_projectile      Le tableau d'infos du projectile
    Retour:         1 si le projectile est immobile 0 sinon
    Ex. d'utilisation:
                    // teste si le projectile est immobile
                    if (projectile_immobile(tab_projectile))

*/
int projectile_immobile(double tab_projectile[][COL_TABPRO]){   //test si le projectile est immobile
    return ((floor(tab_projectile[POSITION][X1])-floor(tab_projectile[POSITION][X0])==0)&&
            (floor(tab_projectile[POSITION][Y1])-floor(tab_projectile[POSITION][Y0])==0));
}

// ---------------------------------------------
/* fonction: afficher_elements
    Description:    Trace les segments et rectangle de tous les éléments qui doivent être visible
                    Les consignes de visibilité pour tous les éléments sont données une seule variable
    Note:           Les constantes MURS_VISIBLE,CIBLE_VISIBLE,LANCEUR_VISIBLE et PROJECTILE_VISIBLE
                    sont définie pour construire la consigne de visibilité.
                    Par exemple, si element_visible=MURS_VISIBLE|CIBLE_VISIBLE seuls les murs et la cible
                    seront affiché. Si element_visible=MURS_VISIBLE|CIBLE_VISIBLE|LANCEUR_VISIBLE , les murs,
                    la cible et le lanceur seront affichés.
    Paramètre(s):   elements_visibles     la consigne de visibilité pour tous les éléments
                    tab_murs            Le tableau d'infos des segments de murs
                    nb_murs             Le nombre de murs à tracer
                    tab_cible           Le tableau d'infos sur la cible
                    tab_lanceur         Le tableau d'infos sur le lanceur
                    tab_projectile      Le tableau d'infos sur le projectile
    Retour:         aucun
    Ex. d'utilisation:
                    // afficher 10 segments de murs et le projectile
                    afficher_elements(MURS_VISIBLE|PROJECTILE_VISIBLE,tab_murs,10,
                                      tab_cible,tab_lanceur,tab_projectile);

*/
void afficher_elements(int elements_visibles,
                       int tab_murs[][COL_TABSEG],int nb_murs,
                       int tab_cible[],
                       int tab_lanceur[],
                       double tab_projectile[][COL_TABPRO])
{
    graph_clear(COULEUR_BG_R,COULEUR_BG_G,COULEUR_BG_B,COULEUR_BG_A);
    if (elements_visibles&MURS_VISIBLE) afficher_murs(tab_murs,nb_murs,MAXSEGMENT);
    if (elements_visibles&CIBLE_VISIBLE) afficher_cible(tab_cible);
    if (elements_visibles&LANCEUR_VISIBLE) afficher_lanceur(tab_lanceur);
    if (elements_visibles&PROJECTILE_VISIBLE) afficher_projectile(tab_projectile);
    graph_mise_a_jour();
}

// ---------------------------------------------
/* fonction: afficher_cible
    Description:    trace le rectangle de la cible dans la couleur prévue pour la cible
    Note:           les constantes COULEUR_CIB_R,COULEUR_CIB_G,COULEUR_CIB_B,COULEUR_CIB_A
                    définissent la couleur (RGBA) prévue pour la cible
                    Comme pour toutes les fonctions de dessin graphique, le dessin ne sera visible
                    qu'après un appel à la fonction graph_mise_a_jour();
    Paramètre(s):   tab_cible     le tableau d'info sur la cible
    Retour:         aucun
    Ex. d'utilisation:
                    afficher_cible(tab_cible);   // affiche le rectangle du projectile d
                                                 //écrit dans tab_cible

*/
void afficher_cible(int tab_cible[]){
    graph_select_couleur(COULEUR_CIB_R,COULEUR_CIB_G,COULEUR_CIB_B,COULEUR_CIB_A);
    graph_trace_rectangle(tab_cible[X0],tab_cible[Y0],tab_cible[X1],tab_cible[Y1]);
}

// ---------------------------------------------
/* fonction: afficher_projectile
    Description:    trace le rectangle du projectile dans la couleur prévue pour le projectile
    Note:           les constantes COULEUR_PRO_R,COULEUR_PRO_G,COULEUR_PRO_B,COULEUR_PRO_A
                    définissent la couleur (RGBA) prévue pour le projectile
                    Comme pour toutes les fonctions de dessin graphique, le dessin ne sera visible
                    qu'après un appel à la fonction graph_mise_a_jour();
    Paramètre(s):   tab_projectile     le tableau d'info sur le projectile
    Retour:         aucun
    Ex. d'utilisation:
                    afficher_projectile(tab_projectile);   // affiche le rectangle du projectile d
                                                           //écrit dans tab_projectile

*/
void afficher_projectile(double tab_projectile[][COL_TABPRO]){
    graph_select_couleur(COULEUR_PRO_R,COULEUR_PRO_G,COULEUR_PRO_B,COULEUR_PRO_A);
    graph_trace_rectangle(tab_projectile[POSITION][X1]-TAILLE_PRO,tab_projectile[POSITION][Y1]-TAILLE_PRO,
                          tab_projectile[POSITION][X1]+TAILLE_PRO,tab_projectile[POSITION][Y1]+TAILLE_PRO);

}

// ---------------------------------------------
/* fonction: afficher_lanceur
    Description:    trace le segment du lanceur dans la couleur prévue pour le lanceur
    Note:           les constantes COULEUR_LAN_R,COULEUR_LAN_G,COULEUR_LAN_B,COULEUR_LAN_A
                    définissent la couleur (RGBA) prévue pour le lanceur
                    Comme pour toutes les fonctions de dessin graphique, le dessin ne sera visible
                    qu'après un appel à la fonction graph_mise_a_jour();
    Paramètre(s):   tab_lanceur     le tableau d'info sur le lanceur
    Retour:         aucun
    Ex. d'utilisation:
                    afficher_lanceur(tab_lanceur);   // affiche le segment du lanceur décrit dans tab_lanceur

*/
void afficher_lanceur(int tab_lanceur[] ){
    graph_select_couleur(COULEUR_LAN_R,COULEUR_LAN_G,COULEUR_LAN_B,COULEUR_LAN_A);
    graph_trace_segment(tab_lanceur[X0],tab_lanceur[Y0],tab_lanceur[X1],tab_lanceur[Y1]);

}

// ---------------------------------------------
/* fonction: afficher_murs
    Description:    trace tous les segments de murs décrits dans le tableau de segments
                    en utilisant la couleur prévue pour les murs.
    Note:           les constantes COULEUR_MUR_R,COULEUR_MUR_G,COULEUR_MUR_B,COULEUR_MUR_A
                    définissent la couleur (RGBA) prévue pour les murs
                    Comme pour toutes les fonctions de dessin graphique, le dessin ne sera visible
                    qu'après un appel à la fonction graph_mise_a_jour();
    Paramètre(s):   tab_segments    le tableau d'info sur les segments de murs
                    nb_segments     le nombre de segments à tracer
                    max_segments    la nombre de segments maximum que peut contenir le tableau
    Retour:         le nombre de segments tracés ou -1 en cas d'erreur
    Ex. d'utilisation:
                    afficher_mur(tab_segments,10,100);   // affiche les 10 premier segments d'un tableau
                                                        // qui peut en contenir jusqu'a 100
*/
int afficher_murs(int tab_segments[][COL_TABSEG],int nb_segments,int max_segments){

    int i;          // itérateur pour la boucle de parcours du tableau de segments
    int retour=0;   // valeur de retour

    // sélectionner la bonne couleur pour le dessin des murs
	graph_select_couleur(COULEUR_MUR_R, COULEUR_MUR_G, COULEUR_MUR_B, COULEUR_MUR_A);

    

	if (nb_segments > max_segments) // si le nombre de segments à dessiner dépasse le maximum permis la valeur de retour est -1
	{
		retour = -1;
	}

	else						//sinon tracer les segments l'un après l'autre  et la valeur de retour est le nombre de segments.
	{
		for (i = 0; i < nb_segments; i++)
		{
			int j = 0;
			for (j = 0; j < COL_TABSEG; j++)
			{
				graph_trace_segment(tab_segments[i][X0], tab_segments[i][Y0], tab_segments[i][X1], tab_segments[i][Y1]);
				
			}
		}

		retour = nb_segments;
	}
    // renvoyer le nombre de segments tracés ou -1 en cas d'erreur
    

	return retour;
}

// ---------------------------------------------
/* fonction: cible_atteinte
    Description:    Teste si la cible est atteinte par le projectile
    Paramètre(s):   tab_projectile  tableau d'information du projectile
                    tab_cible       tableau d'info sur la cible
    Retour:         1 si la cible est touchée, 0 sinon
    Ex. d'utilisation:
                    cible_atteinte(tabp,tabc);  // teste si la cible décrite par le tableau tabc est
                                                // touchée par le projectile décrit par le tableau tabp
*/
int cible_atteinte(double tab_projectile[][COL_TABPRO],int tab_cible[]){
    
	//si le projectile atteint la cible retourner 1, sinon retour 0.
	if (tab_projectile[POSITION][X1] >= tab_cible[X0] && tab_projectile[POSITION][X1] <= tab_cible[X1] && tab_projectile[POSITION][Y1] <= tab_cible[Y1] && tab_projectile[POSITION][Y1] >= tab_cible[Y0])
	{
		return 1;
	}

	else
	{
		return 0;
	}
}


// ---------------------------------------------
/* fonction: mise_a_jour_rebond
    Description:    Mets-à-jour la  position et vitesse du projectile en tenant selon des
                    éventuels rebonds.
                    Analyse le dernier segment de trajectoire correspondant au déplacement depuis
                    l'ancienne position (x0,y0) jusqu'à la nouvelle (x1,y1) en 4 étapes:
                    1-Déterminer le premier mur croisé par le segment de trajectoire
                    2-Calculer la distance du dernier segment et celle jusqu'au rebond (d,drf)
                    3-Fixer la nouvelle position au point d'impact (x1,y1)
                    4-Déterminer le temps, et la vitesse au moment de l'impact (t,dt,vx1,vy1)
                    5.1-Inverser la vitesse horizontale ou verticale selon l'angle du mur touché
                    5.2-Replacer le projectile légèrement à côté de la paroi (du bon côté)
                    6-Réduire la vitesse pour modéliser la perte d'énergie due à l'impact
    Note:           Pour simplifier l'analyse et la mise à jour des rebonds,
                    la fonction suppose:
                    1-que seuls des murs horizontaux ou verticaux existent (pas de diagonale) De plus,
                    2-que la taille du projectile est nulle. Le rebond à donc lieu quand
                        le milieu du projectile touche la paroi. Cette simplification
                        enlève un peu de réalisme à la simulation puisque le projectile entre
                        un peu dans la paroi avant de rebondir.
    Paramètre(s):   tab_segments    tableau d'informations des segments de murs
                    nb_segments     nombre de segments de mur
                    tab_projectile  tableau d'information du projectile
                    t               adresse de la variable de temps pour pouvoir la modifier
                    dt              deltat, increment de temps de la simulation
    Retour:         Aucun. Les nouvelles valeurs calculées sont placées dans le tableau
                    tab_projectile et le temps (t) et son incrément (dt) peuvent
                    être modifiés par la fonction
    Ex. d'utilisation:
                    // mettre a jour la position du projectile en fonction d'un éventuel rebond
                    mise_a_jour_rebond(tab_segments,nb_segments,
                                       tab_projectile,&t,&dt);
*/
void mise_a_jour_rebond(int tab_segments[][COL_TABSEG],int nb_segments,
                        double tab_projectile[][COL_TABPRO],
                        double *t,double *dt)
{
    double xr,yr,xrf,yrf; // point d'intersection (impact) intermédiaire, et final
    double d,dr,drf;    // distance parcourue, distance au rebond, distance au rebond final
    int segf=-1;        // segment ou le rebond a finalement lieu
    int i;
	double a, b, c;

    // 1-déterminer le premier segment croisé pendant le dernier segment de trajectoire
    // mémoriser son index (segf),
    // la position de l'impact qui est le point d'intersection (xrf,yrf),
    // et la distance entre l'impact et la position précédente (drf)
    

	for (i = 0; i < nb_segments && segf < 0; i++)
	{
		/*intersection_segments(double ax1, double ay1, double bx1, double by1, double ax2, double ay2, double bx2, double by2, double* x, double* y*/
		/*equation_droite(double x1, double y1, double x2, double y2, double* a, double* b, double* c)*/

		if (intersection_segments(tab_projectile[POSITION][X0], tab_projectile[POSITION][Y0], tab_projectile[POSITION][X1], tab_projectile[POSITION][Y1], tab_segments[i][X0], tab_segments[i][Y0], tab_segments[i][X1], tab_segments[i][Y1], &xrf, &yrf))
		{
			segf = i;
			equation_droite(tab_segments[i][X0], tab_segments[i][Y0], tab_segments[i][X1], tab_segments[i][Y1], &a, &b, &c);
			drf = longueur(tab_projectile[POSITION][X0], tab_projectile[POSITION][Y0], xrf, yrf);
			printf("TOUCHER");
		}
	}



    // s'il y a rebond, ajuster la position, la vitesse du projectile, et le temps
    // pour correspondre aux valeurs qu'elle aurait du prendre suite à l'impact
    if(segf>=0)
	{

        // 2-calculer la distance parcourue (d) entre la position précédente
        // et la position actuelle du projectile (en ignorant le rebond)
        
		d = longueur(tab_projectile[POSITION][X0], tab_projectile[POSITION][Y0], tab_projectile[POSITION][X1], tab_projectile[POSITION][Y1]);


        // 3-fixer la nouvelle position du projectile au point d'impact du rebond (xrf,yrf)
        

		tab_projectile[POSITION][X1] = xrf;
		tab_projectile[POSITION][Y1] = yrf;

        // 4.1-corriger le temps pour qu'il corresponde au moment de l'impact en utilisant le rapport de distance
        // entre la longueur du segment sans rebond (d) et celle avec rebond (drf)
        
		*t = *t - *dt + (drf / d ) * *dt;

        // 4.2-ajuster le pas de temps pour qu'il corresponde à l'incrément qui mène à l'impact
        // en utilisant le rapport de distance
        // entre la longueur du segment sans rebond (d) et celle avec rebond (drf)
        // (cet ajustement est utile uniquement pour le réalisme de la simulation avec les delais)
        
		*dt *= (drf / d);

        // 4.3-recalculer la vitesse au moment de l'impact par interpolation
        // en utilisant le rapport de distance
        // entre la longueur du segment sans rebond (d) et celle avec rebond (drf)
        		
		tab_projectile[VITESSE][X1] = (tab_projectile[VITESSE][X0]) + (tab_projectile[VITESSE][X1] - tab_projectile[VITESSE][X0]) * (drf / d);
		tab_projectile[VITESSE][Y1] = (tab_projectile[VITESSE][Y0]) + (tab_projectile[VITESSE][Y1] - tab_projectile[VITESSE][Y0]) * (drf / d);

        // 5.1-inverser la vitesse horizontale/verticale selon la paroi
        // rencontrée (vertical/horizontale)
        
		if (c == 0)
		{
			tab_projectile[VITESSE][X1] *= -1;
		}

		if (c == 1)
		{
			tab_projectile[VITESSE][Y1] *= -1;
		}


        // 5.2-replacer le projectile légèrement du bon côté de la paroi (DECALAGE_REBOND)
        
		if (c == 0)
		{
			if (tab_projectile[POSITION][X0] <= xrf)
			{
				tab_projectile[POSITION][X1] -= DECALAGE_REBOND;
			}
			 if (tab_projectile[POSITION][X0] >= xrf)
			{
				tab_projectile[POSITION][X1] += DECALAGE_REBOND;
			}
		}

		if (c == 1)
		{
			if (tab_projectile[POSITION][Y0] <= yrf)
			{
				tab_projectile[POSITION][Y1] -= DECALAGE_REBOND;
			}
			else if (tab_projectile[POSITION][Y0] >= yrf)
			{
				tab_projectile[POSITION][Y1] += DECALAGE_REBOND;
			}
		}

        // 6-réduire la vitesse après le rebond selon le coefficient de
        // restitution de la paroi (C_RESTITUTION)
        
		tab_projectile[VITESSE][X1] *= C_RESTITUTION;

		tab_projectile[VITESSE][Y1] *= C_RESTITUTION;

        //printf(" rebond x=%lf y=%lf vx=%lf vy=%lf dt=%lf\n",*x,*y,*vx,*vy, *dt);
    }
}
// ---------------------------------------------
/* fonction: mise_a_jour_ballistique
    Description:    À partir de la position et de la vitesse précédente du projectile, calcule
                    la nouvelle position et la nouvelle vitesse du projectile en 4 étapes:
                    1-sauvegarder l'ancienne position/vitesse dans x0,y0,vx0,vy0
                    2-effet du frottement de l'air: la vitesse horizontale et la vitesse verticale
                        diminue à cause du frottement de l'air: vx1=vx0-vx0^2*cf/masse*deltat
                                                                vy1=vy0-vy0^2*cf/masse*deltat
                        où cf est le coefficient de frottement de l'air.
                    3-effet de la gravité: la vitesse horizontale n'est pas affectée mais la vitesse
                        verticale est modifiée: vy1=vy1-g*deltat
                    4-la nouvelle position est obtenue avec la nouvelle vitesse et l'ancienne position
                        x1=x0+vx*deltat
                        y1=y0+vy*deltat
    Note:           Les murs sont complètement ignorés par cette fonction. Attention,
                    l'effet du frottement est toujours opposé à la vitesse, il est donc positif pour
                    une vitesse négative et négatif quand la vitesse est positive.
    Paramètre(s):   tab_projectile  tableau d'information du projectile
                    t               adresse de la variable de temps pour pouvoir la modifier
                    dt              deltat, increment de temps de la simulation
    Retour:         Aucun, les nouvelles valeurs calculées sont placées dans le tableau tab_projectile
                    et le temps est modifié par la fonction
    Ex. d'utilisation:
                    calcule_lanceur(100,100,15,tab_lanceur);    // calcule l'extremité du lanceur
                                                                // pour une position de souris de 100,100
                                                                // et une vitesse max permise de 15 (m/s)
*/
void mise_a_jour_ballistique(double tab_projectile[][COL_TABPRO],double *t,double dt){

    //avant calculer la nouvelle position/vitesse, sauvegarder l'ancienne position dans x0,y0,vx0,vy0
    
	tab_projectile[POSITION][X0] = tab_projectile[POSITION][X1];
	tab_projectile[POSITION][Y0] = tab_projectile[POSITION][Y1];

	tab_projectile[VITESSE][X0] = tab_projectile[VITESSE][X1];
	tab_projectile[VITESSE][Y0] = tab_projectile[VITESSE][Y1];

    // calcul de la nouvelle vitesse et nouvelle position sans rebond
	// 1-effet du frottement de l'air sur la vitesse
    
	tab_projectile[VITESSE][X1] = (tab_projectile[VITESSE][X0]) - (((C_FROTTEMENT * pow(tab_projectile[VITESSE][X0], 2))) / MASSE) * (dt);
	tab_projectile[VITESSE][Y1] = (tab_projectile[VITESSE][Y0]) - (((C_FROTTEMENT * pow(tab_projectile[VITESSE][Y0], 2))) / MASSE) * (dt);
	

	// 2-effet de la gravité sur la vitesse
    
	tab_projectile[VITESSE][Y1] = tab_projectile[VITESSE][Y1] - (GRAVITE * (dt));

    // mise à jour de la position
    
	tab_projectile[POSITION][X1] = tab_projectile[POSITION][X0] + (tab_projectile[VITESSE][X1] * (dt));
	tab_projectile[POSITION][Y1] = tab_projectile[POSITION][Y0] + (tab_projectile[VITESSE][Y1] * (dt));

    // mise à jour du temps de la simulation: ajouter dt au temps
    	
	*t += dt;

    //printf("info projectile: %lf %lf %lf %lf\n",tab_projectile[POSITION][X1],tab_projectile[POSITION][Y1],tab_projectile[VITESSE][X1],tab_projectile[VITESSE][Y1]);

}

// ---------------------------------------------
/* fonction: calcule_lanceur
    Description:    calcule les coordonnées de l'extremité mobile du lanceur
                    en fonction des coordonnées de la souris et en prenant soin
                    de limiter la longueur du lanceur selon la vitesse maximale
                    spécifiée.
    Note:           La longueur du segment du lanceur correspond directement à
                    la vitesse initiale qui sera donnée au projectile. Une vitesse
                    initiale de 10 est représentée par un segment de 10 pixels de longueur
    Paramètre(s):   mousex      coordonnée en x de la souris
                    mousey      coordonnée en y de la souris
                    vmax        vitesse max permise ou longueur max du lanceur
                    tab_lanceur tableau d'info du lanceur (coordonnées des deux extrémités)
    Retour:         Aucun, les nouvelles valeurs calculées sont placées dans le tableau tab_lanceur
    Ex. d'utilisation:
                    calcule_lanceur(100,100,15,tab_lanceur);    // calcule l'extremité du lanceur
                                                                // pour une position de souris de 100,100
                                                                // et une vitesse max permise de 15 (m/s)
*/
void calcule_lanceur(int mousex,int mousey,int vmax,int tab_lanceur[])
{

    double l;   // longueur du segment du lanceur

    // placer la fin du lanceur à la position de la souris.
	tab_lanceur[X1] = mousex;
	tab_lanceur[Y1] = mousey;

    // calculer la longueur du segment du lanceur (l)

	l = longueur(tab_lanceur[X0], tab_lanceur[Y0], tab_lanceur[X1], tab_lanceur[Y1]);


    // si cette longueur dépasse la vitesse maximale (vmax) ajuster
    // l'extrémité (x1,y1) en utilisant le facteur (vmax/longueur)
	if (l > vmax)
	{
		tab_lanceur[X1] = ((vmax * (mousex - tab_lanceur[X0])) / l) + tab_lanceur[X0];
		tab_lanceur[Y1] = ((vmax * (mousey - tab_lanceur[Y0])) / l) + tab_lanceur[Y0];
	}
}

// ---------------------------------------------
/* fonction: affiche_tab2D
    Description:    affiche le contenu du tableau de segments (tableau d'entier 2D) dans la console
    Paramètre(s):   tab_segments    le tableau d'info sur les segments de murs
                    nb_segments     le nombre de segments à tracer à afficher
    Retour:         aucun
    Ex. d'utilisation:
                    affiche_tab2D(tab_segments,10);   // affiche les 10 premier segments de tab_segments
*/

void affiche_tab2D(int tab_segments[][COL_TABSEG],int nb_segments){
    int i,j;
    printf("\n");
    for (i=0; i<nb_segments; i++){
        for(j=0;j<COL_TABSEG;j++){
            printf("%5d",tab_segments[i][j]);
        }
        printf("\n");
    }
}


// ---------------------------------------------
/* fonction: lire_fichier_niveau
    Description:    lit un fichier de description de niveau et rempli les tableaux
                    d'information de segments de mur, de lanceur et de cible
    Paramètre(s):   niveau      # du niveau
                    dossier     nom du dossier dans lequel se trouve le fichier de niveau
                                ex: "dossierNiveaux"
                    txt_debut_fichier
                                préfixe du fichier de niveau (ex: "niveau" si le fichier
                                s'appelle "niveau3.txt"
                    txt_fin_fichier
                                suffixe/extension du fichier de niveau (ex: ".txt" si le fichier
                                s'appelle "niveau3.txt"
                    tab_seg     tableau à remplir avec les infos de segments de murs
                    tab_lanceur tableau à remplir avec les infos de position du lanceur
                    tab_cible   tableau à remplir avec les infos de la cibles
    Retour:         le nombre de segments de murs lu, ou -1 en cas d'erreur de lecture du fichier
    Ex. d'utilisation:
                    // charge le fichier "level/level3.niv" en mettant les information trouvées
                    // dans les tableaux tab_seg,tab_lan,tab_cible
                    nb=lire_fichier_niveau(3,"niv","level",".niv",tab_seg,tab_lan,tab_cible);

*/
int lire_fichier_niveau(int niveau, char* dossier, char* txt_debut_fichier, char* txt_fin_fichier,
                        int tab_seg[][COL_TABSEG],int tab_lanceur[], int tab_cible[])
{

    int nb_segments=0;          // nombre de segments trouvés
    char ligne[MAX_CHAR];       // chaine de caractère contenant une ligne du fichier
    FILE* fp;                   // pointeur au fichier à lire
    int ns,nsv,nsh;             // nb de segments à lire dans le fichier (vert./hor.)
    int x1,y1,x2,y2;            // coord. des points lus dans le fichier
    int idxseg=0;               // index de ligne du tableau de segments
    char nom_fichier[MAX_CHAR]; // nom du fichier de description du niveau


    // creation du nom de fichier
    sprintf(nom_fichier,"%s/%s%d%s",dossier,DEBUT_NOM_FICHIER_NIVEAU,niveau,FIN_NOM_FICHIER_NIVEAU);

    // ouverture du fichier et vérification du succès de l'ouverture
    fp = fopen(nom_fichier,"r"); // en mode lecture seulement
    if( fp == NULL ){
        nb_segments=-1;
    }
    else{

        // ------------- En tête ---------------------------
        // sauter les deux lignes d'en-tête
        if (fgets(ligne,MAX_CHAR-1,fp)==NULL) nb_segments=-1;
        if (fgets(ligne,MAX_CHAR-1,fp)==NULL) nb_segments=-1;
        // ------------- Dimension de la zone ---------------
        // obtenir les dimensions de la zone limite du niveau
        // et creer les segments du tour de la zone
        if (fgets(ligne,MAX_CHAR-1,fp)==NULL) nb_segments=-1;
        else{
            // lecture d'un point et identification des 4 champs attendus:
            //  x1, y1, x2, y2
            if (sscanf(ligne,"%d,%d,%d,%d",&x1,&y1,&x2,&y2)==4){
                // ajout des 4 segments de limite de zone dans le tableau des segments
                tab_seg[idxseg][X0]=x1;tab_seg[idxseg][Y0]=y1;
                tab_seg[idxseg][X1]=x1;tab_seg[idxseg][Y1]=y2;
                idxseg++;
                tab_seg[idxseg][X0]=x1;tab_seg[idxseg][Y0]=y2;
                tab_seg[idxseg][X1]=x2;tab_seg[idxseg][Y1]=y2;
                idxseg++;
                tab_seg[idxseg][X0]=x2;tab_seg[idxseg][Y0]=y2;
                tab_seg[idxseg][X1]=x2;tab_seg[idxseg][Y1]=y1;
                idxseg++;
                tab_seg[idxseg][X0]=x2;tab_seg[idxseg][Y0]=y1;
                tab_seg[idxseg][X1]=x1;tab_seg[idxseg][Y1]=y1;
                idxseg++;
            }
            else
                nb_segments=-1;
        }

        // ------------- Segments verticaux --------------
        // sauter l'en tête des nb de segments verticaux
        if (fgets(ligne,MAX_CHAR-1,fp)==NULL) nb_segments=-1;
        // obtenir le nombre de void graph_select_couleur(int r, int g, int b, int a)segments verticaux
        if (fgets(ligne,MAX_CHAR-1,fp)==NULL) nb_segments=-1;
        else{
            // lecture du nombre:
            if (sscanf(ligne,"%d",&nsv)!=1) {
                nsv=-1;
                nb_segments=-1;
            }
        }
        // sauter l'en tête des coordonnées de segments verticaux
        if (fgets(ligne,MAX_CHAR-1,fp)==NULL) nb_segments=-1;
        // boucle pour obtenir les coordonnées des segments verticaux (un par ligne)
        ns=0;
        while((ns<nsv)){
            // lecture d'un segment et identification des 4 champs attendus:
            //  x1, y1, x2, y2
            if (fgets(ligne,MAX_CHAR-1,fp)!=NULL)
                if (sscanf(ligne,"%d,%d,%d,%d",&x1,&y1,&x2,&y2)==4){
                    // ajout du segment dans le tableau des segments
                    tab_seg[idxseg][X0]=x1;tab_seg[idxseg][Y0]=y1;
                    tab_seg[idxseg][X1]=x2;tab_seg[idxseg][Y1]=y2;
                    idxseg++;
                    ns++;
                }
                else nb_segments=-1;
            else nb_segments=-1;

        }

        // ------------- Segments horizontaux --------------
        // sauter l'en tête des nb de segments horizontaux
        if (fgets(ligne,MAX_CHAR-1,fp)==NULL) nb_segments=-1;
        // obtenir le nombre de segments horizontaux
        if (fgets(ligne,MAX_CHAR-1,fp)==NULL) nb_segments=-1;
        else{
            // lecture du nombre:
            if (sscanf(ligne,"%d",&nsh)!=1) {
                nsh=-1;
                nb_segments=-1;
            }
        }
        // sauter l'en tête des coordonnées de segments horizontaux
        if (fgets(ligne,MAX_CHAR-1,fp)==NULL) nb_segments=-1;
        // boucle pour obtenir les coordonnées des segments horizontaux (un par ligne)
        ns=0;
        while((ns<nsh)){
            // lecture d'un segment et identification des 4 champs attendus:
            //  x1, y1, x2, y2
            if (fgets(ligne,MAX_CHAR-1,fp)!=NULL)
                if (sscanf(ligne,"%d,%d,%d,%d",&x1,&y1,&x2,&y2)==4){
                    // ajout du segment dans le tableau des segments
                    tab_seg[idxseg][X0]=x1;tab_seg[idxseg][Y0]=y1;
                    tab_seg[idxseg][X1]=x2;tab_seg[idxseg][Y1]=y2;
                    idxseg++;
                    ns++;
                }
                else nb_segments=-1;
            else nb_segments=-1;

        }

        // ------------- Position du lanceur --------------
        // sauter l'en tête de position du lanceur
        if (fgets(ligne,MAX_CHAR-1,fp)==NULL) nb_segments=-1;
        // obtenir la position du lanceur
        if (fgets(ligne,MAX_CHAR-1,fp)==NULL) nb_segments=-1;
        else{
            // lecture du nombre:
            if (sscanf(ligne,"%d,%d",&tab_lanceur[X0],&tab_lanceur[Y0])!=2) {
                nb_segments=-1;
            }
            else{
                tab_lanceur[X1]=tab_lanceur[X0];
                tab_lanceur[Y1]=tab_lanceur[Y0];
            }
        }

        // ------------- Position de la cible --------------
        // sauter l'en tête de position de la cible
        if (fgets(ligne,MAX_CHAR-1,fp)==NULL) nb_segments=-1;
        // obtenir la position de la cible (point inferieur gauche)
        if (fgets(ligne,MAX_CHAR-1,fp)==NULL) nb_segments=-1;
        else{
            // lecture du nombre:
            if (sscanf(ligne,"%d,%d",&tab_cible[X0],&tab_cible[Y0])!=2) {
                nb_segments=-1;
            }
            else{
                tab_cible[X1]=tab_cible[X0];
                tab_cible[Y1]=tab_cible[Y0];
            }
        }

        // ------------- Taille de la cible --------------
        // sauter l'en tête de la taille de la cible
        if (fgets(ligne,MAX_CHAR-1,fp)==NULL) nb_segments=-1;
        // obtenir la taille de la cible (carré)
        if (fgets(ligne,MAX_CHAR-1,fp)==NULL) nb_segments=-1;
        else{
            // lecture du nombre:
            if (sscanf(ligne,"%d",&x1)!=1) {
                nb_segments=-1;
            }
            else{
                tab_cible[X1]=tab_cible[X0]+x1;
                tab_cible[Y1]=tab_cible[Y0]+x1;
            }
        }

        // fermeture du fichier
        fclose(fp);
    }
    // si la lecture du fichier s'est bien passée,
    // il faut renvoyer le nombre de segments mis en mémoire
    if (nb_segments!=-1)
        nb_segments=idxseg;

    //renvoi du nb de points trouvés
    return nb_segments;
}






