#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

float gangle = 0;

void pyramid(float angle) {
  glRotatef(angle, 0, 0, 1);
  
  glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0, 0, 0);
    glVertex3f(10, 20, 10);
    glVertex3f(-10, 20, 10);
    glVertex3f(-10, 20, -10);
    glVertex3f(10, 20, -10);
    glVertex3f(10, 20, 10);
  glEnd();
  glBegin(GL_QUADS);
    glVertex3f(10, 20, 10);
    glVertex3f(-10, 20, 10);
    glVertex3f(-10, 20, -10);
    glVertex3f(10, 20, -10);
  glEnd();
}

void display(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  pyramid(gangle);
  glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y){
}

void keyboard2(int key, int x, int y){
  if( key == GLUT_KEY_UP ){
    gangle += 10;
    display();  
  }
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
  glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutCreateWindow("Star");
  glutDisplayFunc(display);  
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(keyboard2);
  init();
  glutMainLoop();
  return 0;             
}
