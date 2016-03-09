#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

float angleX = 0; //any
float angleY = 0; //-90 +90
float shift = 0;
float speed = 2;
float scale = 1.0;

float gangle = 0;
float gx = 0;
float gy = 0;
float gangleUp = 0;
float gangleRot = 0;

float toRadians(float angle) {
  return angle * (M_PI / 180);
}

void triangle(float angleUp, float angleRot, float length) {
  glPushMatrix();
  glRotatef(angleRot, 0, 1, 0);
  glRotatef(angleUp, 1, 0, 0);

  glColor3f(1.0,0.0,1.0);
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

void pyramid(float angle, float angleWidth, float length) {
  //glTranslatef(gx, gy, 0);
  glPushMatrix();
  //glRotatef(angle, 0, 0, 1);
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

void petal(float angle) {
  glPushMatrix();
  glRotatef(angle, 0, 0, 1);
  pyramid(0, 10, 20);
  glTranslatef(0, 35+shift, 0);
  glColor3f(0.0,0.0,1.0);
  glutSolidSphere(5, 100, 100);
  glPopMatrix();
}

void display(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  glTranslatef(0, 0, -100);
  glScalef(scale, scale, scale);
  glRotatef(angleY, 1, 0, 0);
  glRotatef(angleX, 0, 1, 0);

  
  GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};
  GLfloat light_position[] = {-1.0, -1.0, 1.0, 0.0};

  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  //glRotatef(100, -10, 0, 0);
  petal(0);
  petal(90);
  petal(180);
  petal(270);
  glutSwapBuffers();
}

int animated = 0;

void keyboard(unsigned char key, int x, int y){
 if (key == 'p') {
   animated = 0;
 } else if (key == 's') {
   animated = 1;
 } else if (key == '+') {
   scale *= 1.1;
 } else if (key == '-') {
   scale /= 1.1;
 }
}

void keyboard2(int key, int x, int y){
  if( key == GLUT_KEY_UP ){
    angleY += 2;
    if (angleY > 90)
      angleY = 90;
    display();  
  } else if( key == GLUT_KEY_DOWN ){
    angleY -= 2;
    if (angleY < -90)
      angleY = -90;
    display();  
  } else if( key == GLUT_KEY_RIGHT ){
    angleX += 2;
    display();  
  } else if( key == GLUT_KEY_LEFT ){
    angleX -= 2;
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
    shift += speed;
  }
  if ((shift > 10) || (shift < 0)) {
    speed *= -1;
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
