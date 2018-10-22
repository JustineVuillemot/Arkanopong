#ifndef JOUEURS_H
#define JOUEURS_H
#define COORD 4

#include "balle/balle.h"

/* Structures */

typedef struct _joueur{
	int points;
	float timer;
	float largeur;
	Point position;
	Balle balle;
}Joueur;

/* PROTOTYPES */
void draw_raquette(float largeur);
Joueur* init_joueur(float ptx, float pty, float vx, float vy);
void deplace_joueur(Joueur * joueur, float x, float y);


#endif