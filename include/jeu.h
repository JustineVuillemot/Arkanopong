#ifndef JEU_H_
#define JEU_H_
#define NB 2
#define MAXP 3

#include "balle/balle.h"
#include "joueurs/joueurs.h"
#include "briques/briques.h"

int* lancement(int* largeur, int* hauteur);
int collision(Balle* b, float posX, float posY, float h, float w);
void collisionJoueur(Balle* b, Joueur j);
int collisionBriques(Brique* tab_br, Joueur* j, int taille);
void collisionLasers(Balle *tab_laser, int* affiche, int nb_lasers, Joueur *j);
int selectBonus(Brique brique, Joueur *j);
Uint32 callBackTailleJoueur(Uint32 intervalle, void * param);
int comptePoint(Joueur *joueur1, Joueur *joueur2);
void afficheTexte(float x, float y, char *texte, void *font);
void affichePoints(GLuint *coeurs_image, float x, float y, int points);
void bougeAI(Joueur *j1, Joueur *j2);

#endif