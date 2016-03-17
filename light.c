#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

const float SCALE_STEP = 1.1;

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

void display(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  gluLookAt(radius*eyeX, radius*eyeY, radius*eyeZ, 0, 0, 0, upX, upY, upZ);
  
  const float light_diff[] = {1.0, 1.0, 1.0, 1.0};
  const float light_pos[] = {-100.0, -100.0, 0.0, 1.0};
  const float light_dir[] = {100.0, 100.0, 0.0, 1.0};
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diff);
  glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
  glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_dir);
  glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 3);
  
  const GLfloat mat_diff[] = {0.0,0.5,0.0,1};
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diff);
  const GLfloat mat_spec[] = {1,0.0,0.0,1};
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_spec); 
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 90);

  
   
  glutSolidSphere(10, 100, 100);
  glTranslatef(20, 0, 0);
  glutSolidCube(10);
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
  
  glEnable(GL_DEPTH_TEST);

  glMatrixMode(GL_PROJECTION);
  //glOrtho(-50, 50, -50, 50, -50, 50);
  gluPerspective(50.0, 1.0, 1.0, 1000.0);
  glMatrixMode(GL_MODELVIEW);
}


int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
  glutCreateWindow("Star");
  glutDisplayFunc(display);  
  glutKeyboardFunc(keyboard);
  glutReshapeFunc(reshape);
  init();
  glutMainLoop();
  return 0;             
}
