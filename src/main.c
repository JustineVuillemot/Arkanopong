#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "jeu.h"
#include "partie.h"
#include "sdl_tools.h"

static const unsigned int BIT_PER_PIXEL = 32;
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;


void reshape(unsigned int width, unsigned int height) {
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(-2., 2., -2., 2.);
}

SDL_Surface * setVideoMode(unsigned int width, unsigned int height) {
  SDL_Surface *ecran = NULL;
  ecran = SDL_SetVideoMode(width, height, BIT_PER_PIXEL, SDL_OPENGL | SDL_RESIZABLE);
  if(NULL == ecran) {
    fprintf(stderr, "Impossible d'ouvrir la fenetre. Fin du programme.\n");
    exit(EXIT_FAILURE);
  }
  reshape(width, height);
  return ecran;
}

int main(int argc, char** argv) {
  unsigned int WINDOW_WIDTH = 800;
  unsigned int WINDOW_HEIGHT = 800;
  SDL_Surface *ecran = NULL;
  GLuint titre;
  
  if(-1 == SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER)) {
    fprintf(stderr, "Impossible d'initialiser la SDL. Fin du programme.\n");
    return EXIT_FAILURE;
  }
   
  ecran = setVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT);
  if(ecran == NULL){
    printf("Ecran non alloue\n");
  }

  SDL_WM_SetCaption("Arkanopong", NULL);

  /*******/
  createTexture(&titre, "img/arkanopong.jpg");
  /*******/

  /* Initialisation de GLUT */
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);


  int loop = 1, rejouer=1;
  while(loop) {
    Uint32 startTime = SDL_GetTicks();

    /* Dessin */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, titre);

    glBegin(GL_QUADS);
        /*FOND*/
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

    glBegin(GL_QUADS);

        /*Rect1*/
    glColor3f(1, 1, 1);
    glVertex2f(-0.7f, 1.3f);
    glVertex2f(0.7f, 1.3f);
    glVertex2f(0.7f, 0.75f);
    glVertex2f(-0.7f, 0.75f);
    /*Rect2*/
    glVertex2f(-0.7f, -1.25f);
    glVertex2f(0.7f, -1.25f);
    glVertex2f(0.7f, -0.7f);
    glVertex2f(-0.7f, -0.7f);
    glEnd();

    glColor3f(0, 0, 0);
    afficheTexte(-0.2, 1, "2 Joueurs", GLUT_BITMAP_HELVETICA_18);
    afficheTexte(-0.45, -1, "1 Joueur contre l'ordi", GLUT_BITMAP_HELVETICA_18);
    glColor3f(1, 1, 1);

    SDL_GL_SwapBuffers();
    /* ****** */

    SDL_Event e;
    while(SDL_PollEvent(&e)) {
      if(e.type == SDL_QUIT) {
        loop = 0;
        break;
      }
      
      switch(e.type) {          
        case SDL_VIDEORESIZE:
          WINDOW_WIDTH = e.resize.w;
          WINDOW_HEIGHT = e.resize.h;
          setVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT);
          break;

        case SDL_KEYDOWN:
          if (e.key.keysym.sym == 'q' || e.key.keysym.sym == SDLK_ESCAPE) {
            loop = 0;
          }
          break;

        case SDL_MOUSEBUTTONUP:
            if(e.button.y / (float)WINDOW_HEIGHT < 0.5){
                rejouer = partie(2);
            }else{
                rejouer = partie(1);
            }
            break;
          
        default:
          break;
      }
    }

    if(rejouer == 0){
        loop = 0;
    }
    
    Uint32 elapsedTime = SDL_GetTicks() - startTime;
    if(elapsedTime < FRAMERATE_MILLISECONDS) {
      SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
    }
  }
  /* Mettre les fonctions de free */

  SDL_Quit();


  return EXIT_SUCCESS;
}
