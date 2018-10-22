#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "partie.h"

static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

int partie(int nbjoueur){

  Brique* br;
  Balle lasers[10];
  int bool_lasers[10];
  int nb_lasers=0, hauteur, largeur;
  GLuint img_balle1, img_balle2, fond, coeurs_image;
  GLuint briques_textures[5];
  Joueur* joueur1; 
  Joueur* joueur2;
  int* tabBriqType;

  char string_points1[NB];
  char string_points2[NB];

  int compte_pt = 0;
  int bonus_1;
  int bonus_2;

  joueur1 = init_joueur(0, 1.6, -0.015, -0.010);
  joueur2 = init_joueur(0, -1.6, 0.015, 0.010);

  joueur1->position.y = 1.8;
  joueur2->position.y = -1.8;

  /*******/
  /*******/

  tabBriqType = lancement(&largeur, &hauteur);


  createTexture(&img_balle1, "img/balle.png");
  createTexture(&img_balle2, "img/balle_orange.png");
  createTexture(&fond, "img/fond.jpg");

  br = init_briques(tabBriqType, largeur, hauteur);
  init_texture_briques(briques_textures);


  int loop = 1, boolj1=0, boolj2=0, pause=0, rejouer=1;
  while(loop) {
    Uint32 startTime = SDL_GetTicks();

    /* Dessin */
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      

      /*fond*/
      glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, fond);

          glBegin(GL_QUADS);
          glTexCoord2f(0, 0);
          glVertex2f(-2.0f, 2.0f);
          glTexCoord2f(1, 0);
          glVertex2f(2.0f, 2.0f);
          glTexCoord2f(1, 1);
          glVertex2f(2.0f, -2.0f);
          glTexCoord2f(0, 1);
          glVertex2f(-2.0f, -2.0f);
          glEnd();

        glBindTexture(GL_TEXTURE_2D, 0);
      glDisable(GL_TEXTURE_2D);

      glColor3f(0.21, 0.76, 0.81);
      deplace_joueur(joueur1, joueur1->position.x, joueur1->position.y);
      glColor3f(1, 0.78, 0.15);
      deplace_joueur(joueur2, joueur2->position.x, joueur2->position.y);
      glColor3f(1, 1, 1);

      affichePoints(&coeurs_image, -1.9, 1.75, joueur1->points);
      affichePoints(&coeurs_image, -1.9, -1.55, joueur2->points);

    /*balleJ1*/
    glPushMatrix();
    glTranslatef(joueur1->balle.position.x, joueur1->balle.position.y, 0.0);

    glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, img_balle1);
      draw_circle(joueur1->balle.rayon);
      glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);

    glPopMatrix();

    /*balleJ2*/
    glPushMatrix();
    glTranslatef(joueur2->balle.position.x, joueur2->balle.position.y, 0.0);


    glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, img_balle2);
      draw_circle(joueur2->balle.rayon);
      glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);

    glPopMatrix();

    /*Lasers*/
    drawLasers(lasers, bool_lasers, nb_lasers);

    /*Briques*/
    drawBriqueTab(br, largeur*hauteur, briques_textures);

    sprintf(string_points1, "%d", joueur1->points);
    sprintf(string_points2, "%d", joueur2->points);

    /* Vérification partie perdue */

    if(joueur1->points == 0){
      pause = 1;
      glColor3f(0, 0, 0);
      afficheTexte(-0.45, 0.0, "Le joueur ORANGE gagne !", GLUT_BITMAP_HELVETICA_18);
      glColor3f(1, 1, 1);
    }
    else if(joueur2->points == 0){
      pause = 1;
      glColor3f(0, 0, 0);
      afficheTexte(-0.45, 0.0, " Le joueur BLEU gagne !", GLUT_BITMAP_HELVETICA_18);
      glColor3f(1, 1, 1);
    }

    /* Gestion des collisions avec les briques : */
    bonus_1 = collisionBriques(br, joueur1, largeur*hauteur);
    if(bonus_1 != -1){
      if(br[bonus_1].bonus == 2){
        lasers[nb_lasers] = createBalle(br[bonus_1].position, createVect(0.0, 0.015), 0.05);
        bool_lasers[nb_lasers] = 1;
        nb_lasers++;
      }

    }
    bonus_2 = collisionBriques(br, joueur2, largeur*hauteur);
    if(bonus_2 != -1){
      if(br[bonus_2].bonus == 2){
        lasers[nb_lasers] = createBalle(br[bonus_2].position, createVect(0.0, -0.015), 0.05);
        bool_lasers[nb_lasers] = 1;
        nb_lasers++;
      }
    }

    if(pause){
        glColor3f(0, 0, 0);
        afficheTexte(-0.2, 0.45, " PAUSE ", GLUT_BITMAP_HELVETICA_18);
        afficheTexte(-0.7, -0.45, " Cliquez sur ESPACE pour relancer ! ", GLUT_BITMAP_HELVETICA_18);
        glColor3f(1, 1, 1);
    }

    SDL_GL_SwapBuffers();
    /* ****** */

    /*Actualisation position balle*/
    if(!pause){
        joueur2->balle.position = PointPlusVector(joueur2->balle.position, joueur2->balle.vitesse);
        joueur1->balle.position = PointPlusVector(joueur1->balle.position, joueur1->balle.vitesse);
    }

    if(joueur2->balle.position.x >= 2 || joueur2->balle.position.x <= -2){
      joueur2->balle.vitesse = changementVitesse(joueur2->balle.vitesse, createVect(1.0, 0.0));
    }

    if(joueur1->balle.position.x >= 2 || joueur1->balle.position.x <= -2){
      joueur1->balle.vitesse = changementVitesse(joueur1->balle.vitesse, createVect(1.0, 0.0));
    }

    /*collision joueurs / balle*/
    collisionJoueur(&(joueur2->balle), *joueur1);
    collisionJoueur(&(joueur2->balle), *joueur2);

    collisionJoueur(&(joueur1->balle), *joueur1);
    collisionJoueur(&(joueur1->balle), *joueur2);

    collisionLasers(lasers, bool_lasers, nb_lasers, joueur1);
    collisionLasers(lasers, bool_lasers, nb_lasers, joueur2);


    /* Décompte des points */
    compte_pt = comptePoint(joueur1, joueur2);
    if(compte_pt == 2){
        relanceBalle(0.0, -1.5, 0.0, 0.0, &(joueur2->balle));
    }
    else if(compte_pt == 1){
        relanceBalle(0.0, 1.5, 0.0, 0.0, &(joueur1->balle));
    }

    if(joueur2->balle.vitesse.x == 0.0 && nbjoueur == 1){
      relanceBalle(joueur2->balle.position.x, joueur2->balle.position.y, 0.015, 0.010, &(joueur2->balle));
    }


    /*position joueurs*/
    if(boolj1 != 0 && !pause){
        switch(boolj1){
            case 97 : /*LEFT*/
                if(joueur1->position.x >= (-2 + joueur1->largeur)){
                    joueur1->position.x -= 0.05;
                }
                break;
            case 122 : /*RIGHT*/
                if(joueur1->position.x <= 2 - joueur1->largeur){
                    joueur1->position.x += 0.05;
                }
                break;
        }
    }

    if(boolj2 != 0 && !pause && nbjoueur == 2){
        switch(boolj2){
            case 276 : /*LEFT*/
                if(joueur2->position.x >= (-2 + joueur2->largeur)){
                    joueur2->position.x -= 0.05;
                }
                break;
            case 275 : /*RIGHT*/
                if(joueur2->position.x <= 2 - joueur2->largeur){
                    joueur2->position.x += 0.05;
                }
                break;
        }
    }
    else if(!pause && nbjoueur == 1){
      bougeAI(joueur1, joueur2);
    }

    /***/

    SDL_Event e;
    while(SDL_PollEvent(&e)) {
      if(e.type == SDL_QUIT) {
        loop = 0;
        rejouer = 0;
        break;
      }
      
      switch(e.type) {

        case SDL_KEYDOWN:
          if (e.key.keysym.sym == 'q' || e.key.keysym.sym == SDLK_ESCAPE) {
            loop = 0;
            rejouer = 0;
          }else{
            if(e.key.keysym.sym == 97 || e.key.keysym.sym == 122){ /*JOUEUR 1*/
                boolj1 = e.key.keysym.sym;

            }
            if(e.key.keysym.sym == 276 || e.key.keysym.sym == 275){ /*JOUEUR 2*/
                boolj2 = e.key.keysym.sym;
            }
            if(e.key.keysym.sym == 32){ /*APRES 1 POINT MARQUE*/
                if(joueur2->balle.vitesse.x != 0.0 && joueur1->balle.vitesse.x != 0.0){
                  if(pause)
                      pause = 0;
                  else
                      pause = 1;
                }
                if(joueur2->balle.vitesse.x == 0.0){
                    relanceBalle(joueur2->balle.position.x, joueur2->balle.position.y, 0.015, 0.010, &(joueur2->balle));
                }
                if(joueur1->balle.vitesse.x == 0.0){
                    relanceBalle(joueur1->balle.position.x, joueur1->balle.position.y, -0.015, -0.010, &(joueur1->balle));
                }
                if(joueur1->points == 0 || joueur2->points == 0){
                    loop = 0;
                    rejouer = 1;
                }
              
            }
          }
          break;

        case SDL_KEYUP:
            if(e.key.keysym.sym == 97 || e.key.keysym.sym == 122){
                boolj1 = 0;

            }else if(e.key.keysym.sym == 276 || e.key.keysym.sym == 275){ /* LEFT */
                boolj2 = 0;
            }
            break;
          
        default:
          break;
      }
    }
    
    Uint32 elapsedTime = SDL_GetTicks() - startTime;
    if(elapsedTime < FRAMERATE_MILLISECONDS) {
      SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
    }
  }
  
  glBindTexture(GL_TEXTURE_2D, 0);
  glDeleteTextures(1, &fond);
  glDeleteTextures(1, &img_balle1);
  glDeleteTextures(1, &img_balle2);
  delete_briques_texture(briques_textures, 5);
  free(joueur1);
  free(joueur2);
  free(tabBriqType);
  free(br);

  return rejouer;
}
