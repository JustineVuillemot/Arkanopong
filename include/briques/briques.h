#ifndef BRIQUES_H_
#define BRIQUES_H_

#include "balle/balle.h"

typedef struct Brique{
	Point position;
	float largeur, hauteur;
	int bonus;
	int nb_coup;
}Brique;

void drawBrique(Brique b, GLuint* tab_brique);
void drawBriqueTab(Brique* tab_br, int taille, GLuint* tab_brique);
Brique createBrique(float posX, float posY, int bonus, int coup, float l, float h);
Brique* init_briques(int* tab_type, int largeur, int hauteur);
void init_texture_briques(GLuint* tab_brique);
Brique init_bonus(float posX, float posY, float largeur, float hauteur, int type);
void delete_briques_texture(GLuint* tab_brique, int taille);

#endif