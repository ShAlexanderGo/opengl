#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

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

float shift = 35;
float anDir = 1.0;
float scale = 1.0;
int animated = 0;
////////////////////////////////////
float radius = 100;
const float ANGLE_SPEED = 0.07;
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
  gluLookAt(radius*eyeX, radius*eyeY, radius*eyeZ, 0, 0, 0, upX, upY, upZ);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, LIGHT_DIF);
  glLightfv(GL_LIGHT0, GL_POSITION, LIGHT_POS);
  petal(0);
  petal(90);
  petal(180);
  petal(270);
  glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y){
  if (key == 'w') {
    eyeX += upX * ANGLE_SPEED;
    eyeY += upY * ANGLE_SPEED;
    eyeZ += upZ * ANGLE_SPEED;
    normalize(&eyeX, &eyeY, &eyeZ, 1);
    crossProduct(rightX, rightY, rightZ, 
                 eyeX, eyeY, eyeZ, 
                 &upX, &upY, &upZ, 1);
    display();
  } else if (key == 's') {
    eyeX -= upX * ANGLE_SPEED;
    eyeY -= upY * ANGLE_SPEED;
    eyeZ -= upZ * ANGLE_SPEED;
    normalize(&eyeX, &eyeY, &eyeZ, 1);
    crossProduct(rightX, rightY, rightZ, 
                 eyeX, eyeY, eyeZ, 
                 &upX, &upY, &upZ, 1);
    display();
  } else if (key == 'a') {
    eyeX -= rightX * ANGLE_SPEED;
    eyeY -= rightY * ANGLE_SPEED;
    eyeZ -= rightZ * ANGLE_SPEED;
    normalize(&eyeX, &eyeY, &eyeZ, 1);
    crossProduct(eyeX, eyeY, eyeZ,
                 upX, upY, upZ,
                 &rightX, &rightY, &rightZ, 1);
    display();
  } else if (key == 'd') {
    eyeX += rightX * ANGLE_SPEED;
    eyeY += rightY * ANGLE_SPEED;
    eyeZ += rightZ * ANGLE_SPEED;
    normalize(&eyeX, &eyeY, &eyeZ, 1);
    crossProduct(eyeX, eyeY, eyeZ,
                 upX, upY, upZ,
                 &rightX, &rightY, &rightZ, 1);
    display();
  } else if (key == 'q') {
    upX -= rightX * ANGLE_SPEED;
    upY -= rightY * ANGLE_SPEED;
    upZ -= rightZ * ANGLE_SPEED;
    normalize(&upX, &upY, &upZ, 1);
    crossProduct(eyeX, eyeY, eyeZ,
                 upX, upY, upZ,
                 &rightX, &rightY, &rightZ, 1);
    display();
  } else if (key == 'e') {
    upX += rightX * ANGLE_SPEED;
    upY += rightY * ANGLE_SPEED;
    upZ += rightZ * ANGLE_SPEED;
    normalize(&upX, &upY, &upZ, 1);
    crossProduct(eyeX, eyeY, eyeZ,
                 upX, upY, upZ,
                 &rightX, &rightY, &rightZ, 1);
    display();
  } else if (key == 'z') {
    radius *= SCALE_STEP;
    display();
  } else if (key == 'x') {
    radius /= SCALE_STEP;
    display();
  } else if (key == 'p') {
    animated = 1;
    display();
  } else if (key == 'o') {
    animated = 0;
    display();
  }
}

void reshape(GLint w, GLint h)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    glViewport(0, 0, w, h);
    float ratio = 1.0 * w / h;
    gluPerspective(50.0, ratio, 1.0, 1000.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
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
  glutReshapeFunc(reshape);
  init();
  glutTimerFunc(30, update, 0);
  glutMainLoop();
  return 0;             
}
