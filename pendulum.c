#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

const float ANGLE_X_STEP = 2;
const float ANGLE_Y_STEP = 2;
const float SCALE_STEP = 1.1; //>1.0

float angleX = 0; //any
float angleY = 45; //-90 +90
float scale = 0.3;



//Degrees to radians
float toRadians(float angle) {
  return angle * (M_PI / 180);
}

float sinD(float degree) {
  return sin(toRadians(degree));
}

float cosD(float degree) {
  return cos(toRadians(degree));
}

float fai = 0;
float radius = 0;
float x = 0;
float y = 10;
float z = 0;

float* points;
int SIZE = 10000;
int pointer = 0;

void display(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  glTranslatef(0, 0, -100);
  glScalef(scale, scale, scale);
  glRotatef(angleY, 1, 0, 0);
  glRotatef(angleX, 0, 1, 0);

  glColor3f(1.0, 1.0, 0.0);
  glBegin(GL_QUADS);
    glVertex3f(-50, 0, -50);
    glVertex3f(-50, 0, 50);
    glVertex3f(50, 0, 50);
    glVertex3f(50, 0, -50);
  glEnd();
  glColor3f(1.0, 0.0, 0.0);
  glBegin(GL_LINES);
    glVertex3f(0, 100, 0);
    glVertex3f(x, y, z);
  glEnd();
  glColor3f(0.0, 0.0, 0.0);
  glBegin(GL_LINES);
    int i = 0;
    while (1) {
      if ((points[i] < -10000) || (points[i+1] < -10000)
          || (points[i+2] < -10000) || (points[i+3] < -10000)) {
        break;
      }
      glVertex3f(points[i], 0.1, points[i+1]);
      
      glVertex3f(points[i+2], 0.1, points[i+3]);
      i += 2;
    }
  glEnd();
  glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y){
 if (key == 'q') {
   scale *= SCALE_STEP;
 } else if (key == 'e') {
   scale /= SCALE_STEP;
 } else if (key == 'w'){
    angleY += ANGLE_Y_STEP;
    if (angleY > 90)
      angleY = 90;
    display();  
  } else if (key == 's'){
    angleY -= ANGLE_Y_STEP;
    if (angleY < -90)
      angleY = -90;
    display();  
  } else if (key == 'd'){
    angleX += ANGLE_X_STEP;
    display();  
  } else if (key == 'a'){
    angleX -= ANGLE_X_STEP;
    display();  
  }
}

void init(void) {
  points = (float*)malloc(SIZE*sizeof(float));
  points[0] = -20000;
  /*glEnable(GL_LIGHT0);
  glEnable(GL_LIGHTING);
  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);*/
  glEnable(GL_DEPTH_TEST);

  glMatrixMode(GL_PROJECTION);
  //glOrtho(-50, 50, -50, 50, -50, 50);
  gluPerspective(50.0, 1.0, 1.0, 1000.0);
  glMatrixMode(GL_MODELVIEW);
}

float func(float ang) {
  return 50*cosD(3.1*ang+20)*sinD(3.1*ang+20);
}

//y - up
void update(int value) {
  fai += 1;
  radius = func(fai);
  x = radius*cosD(fai);
  z = radius*sinD(fai);
  y = 10;
  points[pointer] = x;
  pointer++;
  points[pointer] = z;
  pointer++;
  if (pointer > SIZE-10) {
    points[0] = -20000;
  } else {
    points[pointer] = -20000;
  }
  glutTimerFunc(30, update, 0);
  display();
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
