#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

const float angle = 0;

float angleFunc(float angle, float z) {
  

x*x/(a*a) + y*y/(b*b) - z*z/(c*c) = 1
x = l*sinu
y = l*cosu


void display(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();



  glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y){
 if (key == 'r') {
   angle += 10;
 }
}


int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
  glutCreateWindow("Curve");
  glutDisplayFunc(display);  
  glutKeyboardFunc(keyboard);
  glEnable(GL_DEPTH_TEST);
  glMatrixMode(GL_PROJECTION);
  glOrtho(0, 0, -256, 512, 512, 256);
  glutMainLoop();
  return 0;             
}
