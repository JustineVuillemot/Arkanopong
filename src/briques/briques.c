#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "sdl_tools.h"
#include "briques/briques.h"

Brique createBrique(float posX, float posY, int bonus, int coup, float l, float h){
	Brique br;

	br.position = createPt(posX, posY);
	br.nb_coup = coup;
	br.largeur = l;
	br.hauteur = h;
	br.bonus = bonus;

	return br;
}

Brique init_bonus(float posX, float posY, float largeur, float hauteur, int type){
	switch(type){
		case 1 :
			return createBrique(posX, posY, 1, 2, largeur, hauteur);
			break;
		case 2 :
			return createBrique(posX, posY, 2, 3, largeur, hauteur);
			break;
		case 3 :
			return createBrique(posX, posY, 3, 5, largeur, hauteur);
			break;
		case 4 :
			return createBrique(posX, posY, 4, 4, largeur, hauteur);
			break;
		default: /*basique*/
			return createBrique(posX, posY, 0, 1, largeur, hauteur);
			break;
	}
}

Brique* init_briques(int* tab_type, int largeur, int hauteur){
	int i, j;
	float h, l, coeffh, coeffl;
	Brique* tab_br;

	tab_br = malloc(hauteur*largeur*sizeof(Brique));

	if(tab_br == NULL){
		exit(1);
	}

	h = 1.5/hauteur;
	l = 4.0/largeur;
	coeffh = (hauteur/2)*h-((h/2)*((hauteur+1)%2));
	coeffl = (largeur/2)*l-((l/2)*((largeur+1)%2));

	for(i=0; i<hauteur; i++){
		for(j=0; j<largeur; j++){
			tab_br[i*largeur+j] = init_bonus(l*j-coeffl, h*(hauteur-1-i)-coeffh, l, h, tab_type[i*largeur+j]);
		}
	}
	return tab_br;
}

void init_texture_briques(GLuint* tab_brique){

	createTexture(&tab_brique[0], "img/bonus_neutre.png");
	createTexture(&tab_brique[1], "img/bonus_taille_joueur.png");
	createTexture(&tab_brique[2], "img/bonus_laser.png");
	createTexture(&tab_brique[3], "img/bonus_immortel.png");
	createTexture(&tab_brique[4], "img/bonus_balle.png");
}

void drawBrique(Brique b, GLuint* tab_brique){
	glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tab_brique[b.bonus]);

	glBegin(GL_QUADS);
		glTexCoord2f(0, 0);
		glVertex2f(-b.largeur/2, b.hauteur/2);
		glTexCoord2f(1, 0);
		glVertex2f(b.largeur/2, b.hauteur/2);
		glTexCoord2f(1, 1);
		glVertex2f(b.largeur/2, -b.hauteur/2);
		glTexCoord2f(0, 1);
		glVertex2f(-b.largeur/2, -b.hauteur/2);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}

void drawBriqueTab(Brique* tab_br, int taille, GLuint* tab_brique){
	int i;

	if(tab_br == NULL){
		exit(1);
	}

	for(i=0; i<taille; i++){
		if(tab_br[i].nb_coup != 0){
			glPushMatrix();
		    glTranslatef(tab_br[i].position.x, tab_br[i].position.y, 0.0);
		    drawBrique(tab_br[i], tab_brique);
		    glPopMatrix();
		}
	}
}

void delete_briques_texture(GLuint* tab_brique, int taille){
	int i;

	for(i=0; i<taille; i++){
		glDeleteTextures(1, &tab_brique[i]);
	}
}