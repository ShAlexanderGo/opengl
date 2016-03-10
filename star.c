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
const float SPHERE_SHIFT_MAX = 45.0;

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

//Degrees to radians
float toRadians(float angle) {
  return angle * (M_PI / 180);
}

//triangle along y axis, with angleRot rotation, angleUp - angle from axis y
void triangle(float angleUp, float angleRot, float length) {
  glPushMatrix();
  glRotatef(angleRot, 0, 1, 0);
  glRotatef(angleUp, 1, 0, 0);

  glColor3fv(PYRAMID_COLOR);
  glBegin(GL_TRIANGLES);
    glNormal3d(0, 0, 1);
    glVertex3f(0, 0, 0);
    glVertex3f(length * sin(toRadians(angleUp)), 
               length, 0);
    glVertex3f(-length * sin(toRadians(angleUp)), 
               length, 0);
  glEnd();
  glPopMatrix();
}

//angleWidth - how flat pyramid
void pyramid(float angleWidth, float length) {
  glPushMatrix();
  triangle(angleWidth, 0, length);
  triangle(angleWidth, 90, length);
  triangle(angleWidth, 180, length);
  triangle(angleWidth, 270, length);
  float cosWidth = length * cos(toRadians(angleWidth));
  float sinWidth = length * sin(toRadians(angleWidth));
  glBegin(GL_QUADS);
    glNormal3d(0, 1, 0);
    glVertex3f(sinWidth, 
               cosWidth, 
               -sinWidth);
    glVertex3f(-sinWidth, 
               cosWidth, 
               -sinWidth);
    glVertex3f(-sinWidth, 
               cosWidth, 
               sinWidth);
    glVertex3f(sinWidth, 
               cosWidth, 
               sinWidth);
  glEnd();
  glPopMatrix();
}

//angle - direction of petal
void petal(float angle) {
  glPushMatrix();
  glRotatef(angle, 0, 0, 1);
  pyramid(ANGLE_WIDTH, LENGTH);
  glTranslatef(0, shift, 0);
  glColor3fv(SPHERE_COLOR);
  glutSolidSphere(SPHERE_RADIUS, 100, 100);
  glPopMatrix();
}

void display(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  glTranslatef(0, 0, -100);
  glScalef(scale, scale, scale);
  glRotatef(angleY, 1, 0, 0);
  glRotatef(angleX, 0, 1, 0);

  glLightfv(GL_LIGHT0, GL_DIFFUSE, LIGHT_DIF);
  glLightfv(GL_LIGHT0, GL_POSITION, LIGHT_POS);
  petal(0);
  petal(90);
  petal(180);
  petal(270);
  glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y){
 if (key == 'p') {
   animated = 0;
 } else if (key == 's') {
   animated = 1;
 } else if (key == '+') {
   scale *= SCALE_STEP;
 } else if (key == '-') {
   scale /= SCALE_STEP;
 }
}

void keyboard2(int key, int x, int y){
  if( key == GLUT_KEY_UP ){
    angleY += ANGLE_Y_STEP;
    if (angleY > 90)
      angleY = 90;
    display();  
  } else if( key == GLUT_KEY_DOWN ){
    angleY -= ANGLE_Y_STEP;
    if (angleY < -90)
      angleY = -90;
    display();  
  } else if( key == GLUT_KEY_RIGHT ){
    angleX += ANGLE_X_STEP;
    display();  
  } else if( key == GLUT_KEY_LEFT ){
    angleX -= ANGLE_X_STEP;
    display();  
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
  glutSpecialFunc(keyboard2);
  init();
  glutTimerFunc(30, update, 0);
  glutMainLoop();
  return 0;             
}
