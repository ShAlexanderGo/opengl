#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

const float ANGLE_X_STEP = 2;
const float ANGLE_Y_STEP = 2;
const float SCALE_STEP = 1.1; //>1.0
const float SPEED = 1.5;

const float ANGLE_WIDTH = 10.0; //how flat pyramids
const float LENGTH = 30.0; //how long pyramids
const float SPHERE_RADIUS = 5.0;

const float SPHERE_SHIFT_MIN = 35.0;
const float SPHERE_SHIFT_MAX = 100.0;

const float PYRAMID_COLOR[] = {1.0, 0.0, 1.0};
const float SPHERE_COLOR[] = {0.0,0.0,1.0};

const float LIGHT_DIF[] = {1.0, 1.0, 1.0, 1.0};
const float LIGHT_POS[] = {-1.0, -1.0, 1.0, 0.0};

float angleX = 0; //any
float angleY = 0; //-90 +90
float shift = 35;
float anDir = 1.0;
float scale = 1.0;
int animated = 0;

////////////////////////////////////
float radius = 100;
float speed = 0.02;
float eyeX = 0;
float eyeY = 0;
float eyeZ = 1;
float upX = 1;
float upY = 0;
float upZ = 0;
float rightX = 0;
float rightY = 1;
float rightZ = 0;
/*
eye*up=right
right*eye=up
up*right=eye
*/
void normalize(float* x, float* y, float* z, float length) {
  float cLength = sqrt((*x)*(*x)+(*y)*(*y)+(*z)*(*z));
  (*x) = (*x) * length / cLength;
  (*y) = (*y) * length / cLength;
  (*z) = (*z) * length / cLength;
}
void crossProduct(float x1, float y1, float z1,
                  float x2, float y2, float z2,
                  float* x3, float* y3, float* z3,
                  float length) {
  (*x3) = (y1*z2 - z1*y2);
  (*y3) = (z1*x2 - x1*z2);
  (*z3) = (x1*y2 - y1*x2);
  normalize(x3, y3, z3, length);
}
/////////////////////////////////////

//Degrees to radians
float toRadians(float angle) {
  return angle * (M_PI / 180);
}


void display(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  //glTranslatef(0, 0, -100);
  //glScalef(scale, scale, scale);
  //glRotatef(angleY, 1, 0, 0);
  //glRotatef(angleX, 0, 1, 0);
  gluLookAt(radius*eyeX, radius*eyeY, radius*eyeZ, 0, 0, 0, upX, upY, upZ);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, LIGHT_DIF);
  glLightfv(GL_LIGHT0, GL_POSITION, LIGHT_POS);
  glutSolidCube(20);
  glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y){
 if (key == 'w') {
   eyeX += upX * speed;
   eyeY += upY * speed;
   eyeZ += upZ * speed;
   normalize(&eyeX, &eyeY, &eyeZ, 1);
   crossProduct(rightX, rightY, rightZ, eyeX, eyeY, eyeZ, &upX, &upY, &upZ, 1);
 }
}

void init(void) {
  
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHTING);
  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  glEnable(GL_DEPTH_TEST);

  glMatrixMode(GL_PROJECTION);
  //glOrtho(-50, 50, -50, 50, -50, 50);
  gluPerspective(50.0, 1.0, 1.0, 1000.0);
  glMatrixMode(GL_MODELVIEW);
}

void update(int value) {
  if (animated) {
    shift += anDir * SPEED;
  }
  if (shift > SPHERE_SHIFT_MAX) {
    anDir = -1.0;
  }
  if (shift < SPHERE_SHIFT_MIN) {
    anDir = 1.0;
  }
  glutTimerFunc(30, update, 0);
  display();
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
  glutCreateWindow("Star");
  glutDisplayFunc(display);  
  glutKeyboardFunc(keyboard);
  init();
  glutTimerFunc(30, update, 0);
  glutMainLoop();
  return 0;             
}
