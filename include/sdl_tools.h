#ifndef SDL_TOOLS_H_
#define SDL_TOOLS_H_

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>

void PutPixel(SDL_Surface* surface, int x, int y, Uint32 pixel);
void createTexture(GLuint* t, char* lien);

#endif
