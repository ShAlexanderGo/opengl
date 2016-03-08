#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

void pyramid() {
  glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0, 0, 0);
    glVertex3f(10, 10, 20);
    glVertex3f(-10, 10, 20);
    glVertex3f(-10, -10, 20);
    glVertex3f(10, -10, 20);
    glVertex3f(10, 10, 20);
  glEnd();
}

void display(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  pyramid();
  glutSwapBuffers();
}

void init(void) {
  GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};
  GLfloat light_position[] = {-1.0, -1.0, 1.0, 0.0};

  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHTING);

  glEnable(GL_DEPTH_TEST);
  
  glMatrixMode(GL_PROJECTION);
  glOrtho(-50, 50, -50, 50, -50, 50);

  //gluPerspective(50.0, 1.0, 1.0, 15.0);
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutCreateWindow("Star");
  glutDisplayFunc(display);  
  init();
  glutMainLoop();
  return 0;             
}
