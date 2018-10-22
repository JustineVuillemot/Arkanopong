#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "sdl_tools.h"
#include "jeu.h"

/* Chargement du fichier pour démarrer le jeu : 
Fichier à la racine du template : lancement.txt */
int* lancement(int* largeur, int* hauteur){
  FILE* fichier=NULL;
  int* tabJeu;
  int i;

  fichier = fopen("lancement.txt", "r");

  if(fichier == NULL){
    fclose(fichier);
    return NULL;
  }

  fscanf(fichier, "%d %d", largeur, hauteur);

  tabJeu = malloc((*largeur)*(*hauteur)*sizeof(int));

  if(tabJeu == NULL){
    fclose(fichier);
    return NULL;
  }

  for(i=0; i<(*largeur)*(*hauteur); i++){
    fscanf(fichier, "%d ", &tabJeu[i]);
  }

  return tabJeu; 
}

/* Collision entre un cercle et un rectangle */
int collision(Balle* b, float posX, float posY, float h, float w){

  if(b->position.x > posX - w
    && b->position.x < posX + w){

    if(b->position.y - b->rayon - abs(b->vitesse.y) < posY + h
      && b->position.y - b->rayon > posY){
      /*HAUT*/
      b->vitesse = changementVitesse(b->vitesse, createVect(0.0, 1.0));
      return 1;

    }
    if(b->position.y + b->rayon + abs(b->vitesse.y) > posY - h
      && b->position.y + b->rayon < posY){
      /*BAS*/
      b->vitesse = changementVitesse(b->vitesse, createVect(0.0, 1.0));
      return 1;

    }
  }

  if(b->position.y > posY - h
    && b->position.y < posY  + h){

    if(b->position.x + b->rayon > posX - w
      && b->position.x < posX - w){
      /*GAUCHE*/
      b->vitesse = changementVitesse(b->vitesse, createVect(1.0, 0.0));
      return 1;

    }
    if(b->position.x - b->rayon < posX + w
      && b->position.x > posX + w){
      /*DROITE*/
      b->vitesse = changementVitesse(b->vitesse, createVect(1.0, 0.0));
      return 1;

    }
  }

  return 0;
}

void collisionJoueur(Balle* b, Joueur j){
  collision(b, j.position.x, j.position.y, 0.030, j.largeur);
}

/* Retourne le numéro de la brique, si bonus2, -1 sinon */
int collisionBriques(Brique* tab_br, Joueur* j, int taille){
  int bonus;
  int i, testCollision;
  if(tab_br == NULL){
    exit(1);
  }

  for(i=0; i<taille; i++){

    if(tab_br[i].nb_coup != 0){
      testCollision = collision(&(j->balle), tab_br[i].position.x, tab_br[i].position.y, tab_br[i].hauteur/2, tab_br[i].largeur/2);

      if(testCollision){
        tab_br[i].nb_coup -= 1;

        if(tab_br[i].nb_coup == 0){
          bonus = selectBonus(tab_br[i], j);

          if(bonus == 2){
            return i;
          }
        }
      }
    }
  }
  return -1;
}

/* Collision pour le malus lancé de laser */ 
void collisionLasers(Balle *tab_laser, int* affiche, int nb_lasers, Joueur *j){
  int i;

  for(i=0; i<nb_lasers; i++){
    if(affiche[i] != 0){
      tab_laser[i].position = PointPlusVector(tab_laser[i].position, tab_laser[i].vitesse);
      if(collision(&(tab_laser[i]), j->position.x, j->position.y, 0.030, j->largeur)){
        affiche[i] = 0;
        j->points--;
      }else if(tab_laser[i].position.y <= -2 || tab_laser[i].position.y >= 2 ){
        affiche[i] = 0;
      }
    }
  }
}

/* Gère le bonus, renvoie le numéro du bonus. */
int selectBonus(Brique brique, Joueur *j){
SDL_TimerID timer;
  switch(brique.bonus){
    case 0 :
    /* Rien faire */
    return 0;
    break;

    case 1 :
    /* Augmenter la largeur du joueur */
    j->largeur += 0.2;
    timer = SDL_AddTimer(12000, callBackTailleJoueur, &(j->largeur));
    return 1;
    break;

    case 2 :
    /* laser */
    return 2;
    break;

    case 3 :
    /* Immortalite */
    j->largeur = 2;
    j->position.x = 0.0;
    timer = SDL_AddTimer(8000, callBackTailleJoueur, &(j->largeur));
    return 3;
    break;

    case 4 :
    /* Balle plus rapide */
    j->balle.vitesse.x *= 1.4;
    j->balle.vitesse.y *= 1.4;
    return 4;
    break;
  }
  return -1;
}

Uint32 callBackTailleJoueur(Uint32 intervalle, void * param){
  float *largeur;
  largeur = param;
  if(*largeur > 0.2){
    *largeur = 0.2;
  }
  return 0;
}
  
/* renvoie 1 si sortie balle joueur 1, 2 pour sortie balle joueur 2 sinon 0 */
int comptePoint(Joueur *joueur1, Joueur *joueur2){

  if(joueur1->balle.position.y > 2 + joueur1->balle.rayon || joueur2->balle.position.y > 2.0 + joueur2->balle.rayon){
    joueur1->points -= 1;
  }
  else if(joueur2->balle.position.y < - 2 - joueur2->balle.rayon || joueur1->balle.position.y < -2.0 - joueur1->balle.rayon){
    joueur2->points -= 1;
  }
  if(joueur2->balle.position.y > 2.0 + joueur2->balle.rayon || joueur2->balle.position.y < - 2 - joueur2->balle.rayon){
    return 2;
  }
  else if(joueur1->balle.position.y > 2 + joueur1->balle.rayon || joueur1->balle.position.y < -2.0 - joueur1->balle.rayon){
    return 1;
  }

  return 0;
}

void afficheTexte(float x, float y, char *texte, void *font){
  int taille, i;

  glRasterPos2f(x, y);
  
  taille = strlen(texte);
  for(i = 0; i < taille; i++){
    glutBitmapCharacter(font, texte[i]);
  }
}

/* Affichage des coeurs */
void affichePoints(GLuint *coeurs_image, float x, float y, int points){
  int i = 0;

  if(y < 0){
    createTexture(coeurs_image, "img/coeurorange.png");
  }
  else{
    createTexture(coeurs_image, "img/coeurbleu.png");
  }

  for(i = 0; i < points; i++){
  glPushMatrix();
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, *coeurs_image);
      glBegin(GL_QUADS);
      glTexCoord2f(0, 0);
      glVertex2f(x, y);
      glTexCoord2f(1, 0);
      glVertex2f(x+0.08, y);
      glTexCoord2f(1, 1);
      glVertex2f(x+0.08, y-0.08);
      glTexCoord2f(0, 1);
      glVertex2f(x, y-0.08);
      glEnd();
      x += 0.1;
  glBindTexture(GL_TEXTURE_2D, 0);
  glDisable(GL_TEXTURE_2D);
  glPopMatrix();
  }
}

/* AI suit la balle en fonction de sa vitesse / nombre aléatoire
1 chance sur 10 qu'elle ne bouge pas.*/ 
void bougeAI(Joueur *j1, Joueur *j2){
  int nbr_rand = 0;
  Balle balle;
  srand(time(NULL));
  nbr_rand = rand() % 10;

  if(j2->balle.position.y < j1->balle.position.y){
    balle = j2->balle;
  }
  else{
    balle = j1->balle;
  }
  if(nbr_rand != 2){
    if(balle.vitesse.x > 0 && balle.vitesse.y > 0){
      if(j2->position.x < 2.0 -j2->largeur && balle.position.x > j2->position.x && balle.position.y - j2->position.y > 0.2){
        j2->position.x += 0.05;
      }
    }
    else if(balle.vitesse.x > 0 && balle.vitesse.y < 0){
      if(j2->position.x < 2.0 -j2->largeur){
        if(balle.position.x < j2->position.x && balle.position.y - j2->position.y > 0.05){
          j2->position.x -= 0.05;
        }
        else if(balle.position.x > j2->position.x && balle.position.y - j2->position.y > 0.05){
          j2->position.x += 0.05;
        }
      }
    }
    else if(balle.vitesse.x < 0 && balle.vitesse.y > 0){
      if(j2->position.x > -2.0 + j2->largeur && balle.position.x < j2->position.x && balle.position.y - j2->position.y > 0.05){
        j2->position.x -= 0.05;
      }
    }
    else if(balle.vitesse.x < 0 && balle.vitesse.y < 0){
      if(j2->position.x > -2.0 + j2->largeur){
        if(balle.position.x < j2->position.x + j2->largeur && balle.position.y - j2->position.y > 0.05){
          j2->position.x -= 0.05;
        }
        else if(balle.position.x > j2->position.x +j2->largeur){
          if(j2->position.x < 2.0 - j2->largeur){
            if(balle.position.y - j2->position.y > 0.05){
              j2->position.x += 0.05;
            }
          }
        }
      }
    } 
  }
}