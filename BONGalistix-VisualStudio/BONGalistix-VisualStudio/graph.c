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
// et des �v�nemensts de souris avec la librairie
// SDL
// ---------------------------------------------
SDL_Window* fenetre = NULL;     // Fenetre graphique
SDL_Renderer* renderer;         // renderer utilis� pour le graphisme
Uint8 r,g,b,a;                  // variables pour la gestion des couleurs
SDL_Event event;                // evenement souris/clavier/croix/...


// ---------------------------------------------
// Fonctions graphique et de gestion des �v�nements
// de souris
// ---------------------------------------------


// ---------------------------------------------
/* fonction: graph_select_couleur
    Description:    fonction qui permet de choisir la couleur (RGB) � utiliser pour les
                    prochains trac�s.
    Param�tre(s):   r       Quantit� de rouge (0-255)
                    g       Quantit� de vert (0-255)
                    b       Quantit� de bleu (0-255)
                    a       transparence (0-255) 0=invisible, 255=aucune transparence
    Retour:         Aucun mais la couleur active est modifi�e
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
                    � l'�cran qu'apr�s un appel de la fonction graph_mise_a_jour.
    Param�tre(s):   mousex  adresse de l'entier o� la position en x doit �tre �crite
                    mousey  adresse de l'entier o� la position en y doit �tre �crite
    Retour:         Aucun, les r�sultats de position se retrouvent dans les par
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
    Description:    trace un point � la position x,y de la fen�tre graphique.
    Note:           Comme pour toute les fonctions de dessin, l'effet ne sera visible
                    � l'�cran qu'apr�s un appel de la fonction graph_mise_a_jour.
                    Comme pour toute les fonctions de dessin, l'origine des coordonn�es
                    est dans le coin inf�rieur gauche de la fen�tre graphique
    Param�tre(s):   x       abscisse du point � tracer
                    y       ordonn�e du point � tracer
    Retour:         Aucun
    Ex. d'utilisation:
                    graph_trace_point(10,15); // tracer un point � x=10 et y=15
*/
void graph_trace_point(int x,int y){
    SDL_RenderDrawPoint(renderer,x,LARGEUR_FEN-y-1);
}

// ---------------------------------------------
/* fonction: graph_trace_segment
    Description:    trace un segment entres les points (x1,y1) et (x2,y2).
    Note:           Comme pour toute les fonctions de dessin, l'effet ne sera visible
                    � l'�cran qu'apr�s un appel de la fonction graph_mise_a_jour.
                    Comme pour toute les fonctions de dessin, l'origine des coordonn�es
                    est dans le coin inf�rieur gauche de la fen�tre graphique
    Param�tre(s):   x1      abscisse de la premi�re extr�mit� du segment
                    y1      ordonn�e de la premi�re extr�mit� du segment
                    x2      abscisse de la deuxi�me extr�mit� du segment
                    y2      ordonn�e de la deuxi�me extr�mit� du segment
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
    Description:    fonction de dessin de rectangle donn� par son coin inf�rieur
                    gauche et sup�rieur droit.
    Note:           Comme pour toute les fonctions de dessin, l'effet ne sera visible
                    � l'�cran qu'apr�s un appel de la fonction graph_mise_a_jour.
                    Comme pour toute les fonctions de dessin, l'origine des coordonn�es
                    est dans le coin inf�rieur gauche de la fen�tre graphique
    Param�tre(s):   x1      abscisse du coin inf�rieur droit
                    y1      ordonn�e du coin inf�rieur gauche
                    x2      abscisse du coin sup�rieur droit
                    y2      ordonn�e du coin sup�rieur gauche
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
    Description:    mise a jour de la fenetre graphique pour faire apparaitre tous les trac�s
                    effectu�s depuis le dernier appel de cette m�me fonction
    Note:           Pour toute les fonctions de dessin, l'effet n'est visible � l'�cran
                    qu'apr�s l'appel de cette fonction. En attendant, les trac�s se
                    font en m�moire et peuvent �tre transf�rer tous ensembles au moment
                    voulu par un appel de graph_mise_a_jour.
    Param�tre(s):   Aucun
    Retour:         Aucun
    Ex. d'utilisation:
                    graph_mise_a_jour();    // transf�rer les trac�s en m�moire dans la
                                            // fen�tre graphique
*/
void graph_mise_a_jour(){
    SDL_RenderPresent(renderer);
}


// ---------------------------------------------
/* fonction: graph_get_mousexy
    Description:    fonction qui permet d'obtenir la position de la souris. Les
                    variables de positions (mousex et mousey) sont modifi�es par
                    la fonctions. Apr�s l'ex�cution de la fonction, elles contiennent
                    la position de la souris (passage des param�tres par adresse/r�f�rence).
    Param�tre(s):   mousex  adresse de l'entier o� la position en x doit �tre �crite
                    mousey  adresse de l'entier o� la position en y doit �tre �crite
    Retour:         Aucun, les r�sultats de position se retrouvent dans les par
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
    Param�tre(s):   milisec valeur du d�lai en milisecondes
    Retour:         Aucun
    Ex. d'utilisation:
                    graph_delai_ms(10); // attendre 10ms avant de continuer le programme
*/
void graph_delai_ms(int milisec){
    SDL_Delay(milisec);
}

// ---------------------------------------------
/* fonction: graph_titre
    Description:    ajustement du titre de la fen�tre graphique
    Param�tre(s):   titre  chaine de caract�re contenant le titre � utiliser
    Retour:         Aucun
    Ex. d'utilisation:
                    graph_titre("Ma fenetre graphique");
*/
void graph_titre(char* titre){
    SDL_SetWindowTitle(fenetre, titre);
}

// ---------------------------------------------
/* fonction: graph_init
    Description:    fonction d'initialisation du graphisme et de cr�ation
                    de la fen�tre graphique. Utilise les variables globales
                    fenetre et renderer. Encapsulation des fonctions d'initialisation
                    graphique de la librairie SDL.
    Param�tre(s):   titre   cha�ne de caract�res pour d�finir le titre � afficher en
                            haut de la fenetre
                    largeur taille de la fen�tre en x (nb de pixels)
                    hauteur taille de la fen�tre en y (nb de pixels)
                    r       contenu en rouge de la couleur de fond 0-255
                    g       contenu en vert de la couleur de fond 0-255
                    b       contenu en bleu de la couleur de fond 0-255
                    a       transparence � donner � la couleur 0-255 (255=pas transparent)
    Retour:         renvoi 0 en cas de probl�me ou 1 en cas de succ�s
    Ex. d'utilisation:
                    graph_init(); // initialise la fen�tre graphique
*/
int graph_init(char* titre,int largeur, int hauteur,int r, int g, int b, int a){

    int reussite=0; // variable indiquant la r�ussite de l'initialisation

    // initialisation du graphisme SDL
    if(SDL_Init( SDL_INIT_VIDEO )<0){
            printf( "SDL ne peut pas �tre initialis�e! SDL_Error: %s\n", SDL_GetError() );
    }
    else{
        //Cr�ation de la fenetre graphique et ajout du titre
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

            // mettre � jour l'affichage
            SDL_RenderPresent(renderer);

            // l'initialisation du graphisme et la cr�ation de la fen�tre ont r�ussi
            reussite=1;

        }
    }

    return reussite;
}

// ---------------------------------------------
/* fonction: graph_fin
    Description:    Fermeture de la fen�tre graphique et fin des fonctions graphiques
    Param�tre(s):   Aucun
    Retour:         Aucun
    Ex. d'utilisation:
                    graph_fin();    // fin du graphisme
*/
void graph_fin(){
    //Fermeture de la fen�tre graphique et du renderer
    SDL_DestroyWindow(fenetre);
    SDL_DestroyRenderer(renderer);

    //Fin des fonctions graphiques de la librairie SDL
    SDL_Quit();
}
