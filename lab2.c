#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

const float ANGLE_X_STEP = 2;
const float ANGLE_Y_STEP = 2;
const float SCALE_STEP = 1.1; //>1.0

float angleX = 0; //any
float angleY = 0; //-90 +90
float scale = 1.0;

float angle = 0;
float A = 1;
float B = 1;
float C = 1;

float func(float x, float y) {
  float right = ((x*x)/(A*A)) + ((y*y)/(B*B)) - 1;
  return (C*sqrt(right));
}

void display(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(256, 256, 0);
  glScalef(scale, scale, scale);
  glRotatef(angleY, 1, 0, 0);
  glRotatef(angleX, 0, 1, 0);
  glRotatef(90, 1, 0, 0);

  glTranslatef(15, 10, 0);
  glRotatef(angle, 0, 0, 1);
  glTranslatef(-15, -10, 0);
  int x, y;
  for (x = -100; x < 100; x += 5) {
    for (y = -100; y < 100; y += 5) {
      glBegin(GL_QUADS);
        glVertex3f(x, y, func(x, y));
        glVertex3f(x+5, y, func(x+5, y));
        glVertex3f(x+5, y+5, func(x+5, y+5));
        glVertex3f(x, y+5, func(x, y+5));
        glVertex3f(x, y, -func(x, y));
        glVertex3f(x+5, y, -func(x+5, y));
        glVertex3f(x+5, y+5, -func(x+5, y+5));
        glVertex3f(x, y+5, -func(x, y+5));
      glEnd();
    }
  }
  glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y){
 if (key == 'r') {
   angle += 10;
   display();
 } else if (key == '+') {
   scale *= SCALE_STEP;
   display();
 } else if (key == '-') {
   scale /= SCALE_STEP;
   display();
 } else if (key == 'q') {
   A *= SCALE_STEP;
   display();
 } else if (key == 'a') {
   A /= SCALE_STEP;
   display();
 } else if (key == 'w') {
   B *= SCALE_STEP;
   display();
 } else if (key == 's') {
   B /= SCALE_STEP;
   display();
 } else if (key == 'e') {
   C *= SCALE_STEP;
   display();
 } else if (key == 'd') {
   C /= SCALE_STEP;
   display();
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

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutCreateWindow("Curve");
  glutDisplayFunc(display);  
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(keyboard2);
  glEnable(GL_DEPTH_TEST);
  glMatrixMode(GL_PROJECTION);
  glOrtho(0, 512, 0, 512, -256, 256);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glutMainLoop();
  return 0;             
}
