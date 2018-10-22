#ifndef BALLE_H_
#define BALLE_H_

typedef struct Vecteur{
	float x, y;
}Vecteur;

typedef struct Point{
	float x, y;
}Point;

typedef struct Balle{
	Point position;
	Vecteur vitesse;
	float rayon;
}Balle;

void draw_circle(float r);
Point createPt(float x, float y);
Vecteur createVect(float x, float y);
Point PointPlusVector(Point p, Vecteur v);
Balle createBalle(Point p, Vecteur v, float rayon);
Vecteur changementVitesse(Vecteur vitesse, Vecteur normale);
void relanceBalle(float ptx, float pty, float vx, float vy, Balle *balle);
void drawLasers(Balle* tab_laser, int* affiche, int nb_lasers);

#endif

#ifndef M_PI
#define M_PI 3.14159265
#endif