#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "balle/balle.h"

void draw_circle(float r){
  int i;
  float rad_angle;

  glBegin(GL_POLYGON);

  for(i=0; i<360; i++){
    rad_angle = i*M_PI/180;
    glTexCoord2f((cos(rad_angle)+1)/2, -(sin(rad_angle)+1)/2);
    glVertex2f(cos(rad_angle)*r, sin(rad_angle)*r);
  }

  glEnd();

}

Point createPt(float x, float y){
  Point p;

  p.x = x;
  p.y = y;

  return p;
}

Vecteur createVect(float x, float y){
  Vecteur v;

  v.x = x;
  v.y = y;

  return v;
}

Point PointPlusVector(Point p, Vecteur v){
  Point pt;

  pt.x = p.x + v.x;
  pt.y = p.y + v.y;

  return pt;
}

Balle createBalle(Point p, Vecteur v, float rayon){
  Balle b;

  b.position = p;
  b.vitesse = v;
  b.rayon = rayon;

  return b;
}
 /* Symétrique de la normale */
Vecteur changementVitesse(Vecteur vitesse, Vecteur normale){
  Vecteur sym;

  sym.x = vitesse.x -2 * normale.x * vitesse.x;
  sym.y = vitesse.y -2 * normale.y * vitesse.y;

  return sym;
}

void relanceBalle(float ptx, float pty, float vx, float vy, Balle *balle){
  Point p;
  Vecteur v;
  int random;
  srand(time(NULL));
  random = rand() % 2;
  if(random == 1){
    vx = -vx;
  }
  v = createVect(vx, vy);
  p = createPt(ptx, pty);
  *balle = createBalle(p, v, 0.125);
}

/* Affichage du malus laser */
void drawLasers(Balle* tab_laser, int* affiche, int nb_lasers){
  int i;

  for(i=0; i<nb_lasers; i++){
    if(affiche[i] != 0){
      glPushMatrix();
      glTranslatef(tab_laser[i].position.x, tab_laser[i].position.y, 0.0);

        draw_circle(tab_laser[i].rayon);

      glPopMatrix();
    }
  }
}