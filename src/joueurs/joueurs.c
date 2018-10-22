#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "joueurs/joueurs.h"


/* FONCTIONS RELATIVES AUX JOUEURS */
Joueur* init_joueur(float ptx, float pty, float vx, float vy){
	Joueur* joueur;
	joueur = malloc(sizeof(Joueur));
	if(joueur == NULL){
		printf("Erreur de memoire\n");
		return NULL;
	}
	else{
		/*printf("Initjoueur\n");*/
		joueur->points = 10;
		joueur->timer = 0.0;
		joueur->largeur = 0.2;
		joueur->position = createPt(0.0, 0.0);
		relanceBalle(ptx, pty, vx, vy, &(joueur->balle));

		return joueur;
	}
}

/* Dessine le joueur */
void draw_raquette(float largeur){
	glBegin(GL_QUADS);
	glVertex2f(-largeur, -0.030f);
	glVertex2f(-largeur, 0.030f);
	glVertex2f(largeur, 0.030f);
	glVertex2f(largeur, -0.030f);
	glEnd(); 
}

void deplace_joueur(Joueur * joueur, float x, float y){
	glPushMatrix();
	glTranslatef(x, y , 0.0);
	draw_raquette(joueur->largeur);
	glPopMatrix();
}