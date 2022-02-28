// -------------------------------------------
/*
librairie graph.c
Auteur:David Marche
Date: Automne 2021
Description: Librairie de fonctions graphiques encapsulant certaines fonctions
             de graphisme de la librairie SDL. Pour le devoir, utiliser uniquement
             ces fonctions pour la gestion de l'affichage graphique
Note:        Les prototypes des fonctions se trouvent dans le fichier graph.h
*/
// -------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>

// fichier de prototypes des fonctions graphiques
#include "graph.h"

// ---------------------------------------------
// variables globales de gestion du graphisme
// et des évènemensts de souris avec la librairie
// SDL
// ---------------------------------------------
SDL_Window* fenetre = NULL;     // Fenetre graphique
SDL_Renderer* renderer;         // renderer utilisé pour le graphisme
Uint8 r,g,b,a;                  // variables pour la gestion des couleurs
SDL_Event event;                // evenement souris/clavier/croix/...


// ---------------------------------------------
// Fonctions graphique et de gestion des évènements
// de souris
// ---------------------------------------------


// ---------------------------------------------
/* fonction: graph_select_couleur
    Description:    fonction qui permet de choisir la couleur (RGB) à utiliser pour les
                    prochains tracés.
    Paramètre(s):   r       Quantité de rouge (0-255)
                    g       Quantité de vert (0-255)
                    b       Quantité de bleu (0-255)
                    a       transparence (0-255) 0=invisible, 255=aucune transparence
    Retour:         Aucun mais la couleur active est modifiée
    Ex. d'utilisation:
                    graph_select_couleur(0,255,0,255);  // activer la couleur verte
                                                        // sans transparence
*/
void graph_select_couleur(int r, int g, int b, int a){
    //fixe la couleur a utiliser
    SDL_SetRenderDrawColor(renderer,r,g,b,a);
}

// ---------------------------------------------
/* fonction: graph_clear
    Description:    fonction qui permet d'effacer completement la zone de dessin et
                    de la remplir avec la couleur de fond.
    Note:           Comme pour toute les fonctions de dessin, l'effet ne sera visible
                    à l'écran qu'après un appel de la fonction graph_mise_a_jour.
    Paramètre(s):   mousex  adresse de l'entier où la position en x doit être écrite
                    mousey  adresse de l'entier où la position en y doit être écrite
    Retour:         Aucun, les résultats de position se retrouvent dans les par
    Ex. d'utilisation:
                    graph_get_mousexy(&a,&b);   // obtenir la position x,y de la souris
                                                // dans les variables a,b
*/
void graph_clear(int r, int g, int b, int a){
    graph_select_couleur(r,g,b,a);
    SDL_RenderClear(renderer);
}

// ---------------------------------------------
/* fonction: graph_trace_point
    Description:    trace un point à la position x,y de la fenêtre graphique.
    Note:           Comme pour toute les fonctions de dessin, l'effet ne sera visible
                    à l'écran qu'après un appel de la fonction graph_mise_a_jour.
                    Comme pour toute les fonctions de dessin, l'origine des coordonnées
                    est dans le coin inférieur gauche de la fenêtre graphique
    Paramètre(s):   x       abscisse du point à tracer
                    y       ordonnée du point à tracer
    Retour:         Aucun
    Ex. d'utilisation:
                    graph_trace_point(10,15); // tracer un point à x=10 et y=15
*/
void graph_trace_point(int x,int y){
    SDL_RenderDrawPoint(renderer,x,LARGEUR_FEN-y-1);
}

// ---------------------------------------------
/* fonction: graph_trace_segment
    Description:    trace un segment entres les points (x1,y1) et (x2,y2).
    Note:           Comme pour toute les fonctions de dessin, l'effet ne sera visible
                    à l'écran qu'après un appel de la fonction graph_mise_a_jour.
                    Comme pour toute les fonctions de dessin, l'origine des coordonnées
                    est dans le coin inférieur gauche de la fenêtre graphique
    Paramètre(s):   x1      abscisse de la première extrémité du segment
                    y1      ordonnée de la première extrémité du segment
                    x2      abscisse de la deuxième extrémité du segment
                    y2      ordonnée de la deuxième extrémité du segment
    Retour:         Aucun
    Ex. d'utilisation:
                    graph_trace_segment(10,15,100,200); // tracer un segment entre les points
                                                        // (10,15) et (100,200)
*/
void graph_trace_segment(int x1,int y1,int x2,int y2){
    SDL_RenderDrawLine(renderer,x1,LARGEUR_FEN-y1-1,x2,LARGEUR_FEN-y2-1);
}

// ---------------------------------------------
/* fonction: graph_trace_rectangle
    Description:    fonction de dessin de rectangle donné par son coin inférieur
                    gauche et supérieur droit.
    Note:           Comme pour toute les fonctions de dessin, l'effet ne sera visible
                    à l'écran qu'après un appel de la fonction graph_mise_a_jour.
                    Comme pour toute les fonctions de dessin, l'origine des coordonnées
                    est dans le coin inférieur gauche de la fenêtre graphique
    Paramètre(s):   x1      abscisse du coin inférieur droit
                    y1      ordonnée du coin inférieur gauche
                    x2      abscisse du coin supérieur droit
                    y2      ordonnée du coin supérieur gauche
    Retour:         Aucun
    Ex. d'utilisation:
                    graph_trace_rectangle(10,15,100,50); // tracer un rect. de coins (10,15) et (100,50)
*/
void graph_trace_rectangle(int x1,int y1,int x2,int y2){
    SDL_Rect rect = {x1,HAUTEUR_FEN-y1-1-abs(y2-y1),abs(x2-x1),abs(y2-y1)};
    SDL_RenderDrawRect(renderer,&rect);

}

// ---------------------------------------------
/* fonction: graph_mise_a_jour
    Description:    mise a jour de la fenetre graphique pour faire apparaitre tous les tracés
                    effectués depuis le dernier appel de cette même fonction
    Note:           Pour toute les fonctions de dessin, l'effet n'est visible à l'écran
                    qu'après l'appel de cette fonction. En attendant, les tracés se
                    font en mémoire et peuvent être transférer tous ensembles au moment
                    voulu par un appel de graph_mise_a_jour.
    Paramètre(s):   Aucun
    Retour:         Aucun
    Ex. d'utilisation:
                    graph_mise_a_jour();    // transférer les tracés en mémoire dans la
                                            // fenêtre graphique
*/
void graph_mise_a_jour(){
    SDL_RenderPresent(renderer);
}


// ---------------------------------------------
/* fonction: graph_get_mousexy
    Description:    fonction qui permet d'obtenir la position de la souris. Les
                    variables de positions (mousex et mousey) sont modifiées par
                    la fonctions. Après l'exécution de la fonction, elles contiennent
                    la position de la souris (passage des paramètres par adresse/référence).
    Paramètre(s):   mousex  adresse de l'entier où la position en x doit être écrite
                    mousey  adresse de l'entier où la position en y doit être écrite
    Retour:         Aucun, les résultats de position se retrouvent dans les par
    Ex. d'utilisation:
                    graph_get_mousexy(&a,&b);   // obtenir la position x,y de la souris
                                                // dans les variables a,b
*/
void graph_get_mousexy(int *mousex,int *mousey){
    *mousex=event.motion.x;
    *mousey=HAUTEUR_FEN-event.motion.y-1;
}

// ---------------------------------------------
/* fonction: graph_delai_ms
    Description:    fonction de delai (attente).
    Paramètre(s):   milisec valeur du délai en milisecondes
    Retour:         Aucun
    Ex. d'utilisation:
                    graph_delai_ms(10); // attendre 10ms avant de continuer le programme
*/
void graph_delai_ms(int milisec){
    SDL_Delay(milisec);
}

// ---------------------------------------------
/* fonction: graph_titre
    Description:    ajustement du titre de la fenêtre graphique
    Paramètre(s):   titre  chaine de caractère contenant le titre à utiliser
    Retour:         Aucun
    Ex. d'utilisation:
                    graph_titre("Ma fenetre graphique");
*/
void graph_titre(char* titre){
    SDL_SetWindowTitle(fenetre, titre);
}

// ---------------------------------------------
/* fonction: graph_init
    Description:    fonction d'initialisation du graphisme et de création
                    de la fenêtre graphique. Utilise les variables globales
                    fenetre et renderer. Encapsulation des fonctions d'initialisation
                    graphique de la librairie SDL.
    Paramètre(s):   titre   chaîne de caractères pour définir le titre à afficher en
                            haut de la fenetre
                    largeur taille de la fenêtre en x (nb de pixels)
                    hauteur taille de la fenêtre en y (nb de pixels)
                    r       contenu en rouge de la couleur de fond 0-255
                    g       contenu en vert de la couleur de fond 0-255
                    b       contenu en bleu de la couleur de fond 0-255
                    a       transparence à donner à la couleur 0-255 (255=pas transparent)
    Retour:         renvoi 0 en cas de problème ou 1 en cas de succès
    Ex. d'utilisation:
                    graph_init(); // initialise la fenêtre graphique
*/
int graph_init(char* titre,int largeur, int hauteur,int r, int g, int b, int a){

    int reussite=0; // variable indiquant la réussite de l'initialisation

    // initialisation du graphisme SDL
    if(SDL_Init( SDL_INIT_VIDEO )<0){
            printf( "SDL ne peut pas être initialisée! SDL_Error: %s\n", SDL_GetError() );
    }
    else{
        //Création de la fenetre graphique et ajout du titre
        fenetre = SDL_CreateWindow(titre,
                                   SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                   largeur, hauteur,
                                   SDL_WINDOW_SHOWN );
        if( fenetre == NULL ){
            printf( "La fenetre graphique ne peut pas etre creee! SDL_Error: %s\n", SDL_GetError() );
        }
        else
        {
            // creation du renderer
            renderer = SDL_CreateRenderer(fenetre, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if (renderer==NULL){
                printf("Erreur lors de la creation d'un renderer : %s",SDL_GetError());
            }


            // fixer la couleur du fond
            SDL_SetRenderDrawColor(renderer,r,g,b,a);
            SDL_RenderClear(renderer);

            // mettre à jour l'affichage
            SDL_RenderPresent(renderer);

            // l'initialisation du graphisme et la création de la fenêtre ont réussi
            reussite=1;

        }
    }

    return reussite;
}

// ---------------------------------------------
/* fonction: graph_fin
    Description:    Fermeture de la fenêtre graphique et fin des fonctions graphiques
    Paramètre(s):   Aucun
    Retour:         Aucun
    Ex. d'utilisation:
                    graph_fin();    // fin du graphisme
*/
void graph_fin(){
    //Fermeture de la fenêtre graphique et du renderer
    SDL_DestroyWindow(fenetre);
    SDL_DestroyRenderer(renderer);

    //Fin des fonctions graphiques de la librairie SDL
    SDL_Quit();
}
