#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

const float SPHERE_SHIFT_MIN = 35.0;
const float SPHERE_SHIFT_MAX = 100.0;

const float PYRAMID_COLOR[] = {1.0, 0.0, 1.0};
const float SPHERE_COLOR[] = {0.0,0.0,1.0};

const float LIGHT_DIF[] = {0.05, 0.05, 0.05, 1.0};
const float LIGHT_POS[] = {-100.0, -100.0, 100.0, 1.0};

float angleX = 0; //any
float angleY = 0; //-90 +90
float shift = 35;
float anDir = 1.0;
float scale = 1.0;
int animated = 0;

////////////////////////////////////
float eyeRadius = 100;
float speed = 0.07;
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

float sinD(float degree) {
  return sin(toRadians(degree));
}

float cosD(float degree) {
  return cos(toRadians(degree));
}

void drawSphere(float radius, int vert, int hor) {
  float vertAngleStep = 180.0 / vert;
  float horAngleStep = 360.0 / hor;
  glBegin(GL_TRIANGLE_FAN);
    //glColor3f(1,0,0);
    glNormal3f(0, 0, radius);
    glVertex3f(0, 0, radius);
    int i,j;
    for (i = hor; i >= 0; i--) {
      //glColor3f(1.0*i/hor,0,0);
      glNormal3f(radius*cosD(90.0 - vertAngleStep)*sinD(i*horAngleStep), 
                 radius*cosD(90.0 - vertAngleStep)*cosD(i*horAngleStep), 
                 radius*sinD(90.0 - vertAngleStep)); 
      glVertex3f(radius*cosD(90.0 - vertAngleStep)*sinD(i*horAngleStep), 
                 radius*cosD(90.0 - vertAngleStep)*cosD(i*horAngleStep), 
                 radius*sinD(90.0 - vertAngleStep)); 
    }
  glEnd();
  glBegin(GL_QUADS);
    for (j = 1; j < vert-1; j++) {
      for (i = 0; i < hor; i++) {
        int i1 = i;
        int j1 = j;
        int i2 = i + 1;
        int j2 = j + 1;
        float radius1 = radius*cosD(90.0-j1*vertAngleStep);
        float radius2 = radius*cosD(90.0-j2*vertAngleStep);
        //glColor3f(1,0,0);
        glNormal3f(radius1*sinD(i1*horAngleStep), 
                 radius1*cosD(i1*horAngleStep), 
                 radius*sinD(90.0-j1*vertAngleStep));
        glVertex3f(radius1*sinD(i1*horAngleStep), 
                 radius1*cosD(i1*horAngleStep), 
                 radius*sinD(90.0-j1*vertAngleStep));
        //glColor3f(0,1,0);
        glNormal3f(radius1*sinD(i2*horAngleStep), 
                 radius1*cosD(i2*horAngleStep), 
                 radius*sinD(90.0-j1*vertAngleStep)); 
        glVertex3f(radius1*sinD(i2*horAngleStep), 
                 radius1*cosD(i2*horAngleStep), 
                 radius*sinD(90.0-j1*vertAngleStep));
        //glColor3f(0,0,1);
        glNormal3f(radius2*sinD(i2*horAngleStep), 
                 radius2*cosD(i2*horAngleStep), 
                 radius*sinD(90.0-j2*vertAngleStep)); 
        glVertex3f(radius2*sinD(i2*horAngleStep), 
                 radius2*cosD(i2*horAngleStep), 
                 radius*sinD(90.0-j2*vertAngleStep));
        //glColor3f(1,1,1);
        glNormal3f(radius2*sinD(i1*horAngleStep), 
                 radius2*cosD(i1*horAngleStep), 
                 radius*sinD(90.0-j2*vertAngleStep));  
        glVertex3f(radius2*sinD(i1*horAngleStep), 
                 radius2*cosD(i1*horAngleStep), 
                 radius*sinD(90.0-j2*vertAngleStep)); 
      }
    }
  glEnd();
  glBegin(GL_TRIANGLE_FAN);
    //glColor3f(1,0,0);
    glNormal3f(0, 0, -radius);
    glVertex3f(0, 0, -radius);
    for (i = 0; i <= hor; i++) {
      //glColor3f(1.0-1.0*i/hor,0,0);
      glNormal3f(radius*cosD(90.0 - vertAngleStep)*sinD(i*horAngleStep), 
                 radius*cosD(90.0 - vertAngleStep)*cosD(i*horAngleStep), 
                 radius*sinD(-90.0 + vertAngleStep)); 
      glVertex3f(radius*cosD(90.0 - vertAngleStep)*sinD(i*horAngleStep), 
                 radius*cosD(90.0 - vertAngleStep)*cosD(i*horAngleStep), 
                 radius*sinD(-90.0 + vertAngleStep)); 
    }
  glEnd();
}

/*Second variant*/
void drawSphere2(float radius, int vert, int hor) {
  float vertAngleStep = 180.0 / vert;
  float horAngleStep = 360.0 / hor;
  glBegin(GL_TRIANGLES);
    //glColor3f(1,0,0);
    int i,j;
    for (i = hor; i >= 0; i--) {
      //glColor3f(1.0*i/hor,0,0);
      float x1 = radius*cosD(90.0 - vertAngleStep)*sinD(i*horAngleStep);
      float y1 = radius*cosD(90.0 - vertAngleStep)*cosD(i*horAngleStep);
      float z1 = radius*sinD(90.0 - vertAngleStep) - radius;
      float x2 = radius*cosD(90.0 - vertAngleStep)*sinD((i-1)*horAngleStep);
      float y2 = radius*cosD(90.0 - vertAngleStep)*cosD((i-1)*horAngleStep);
      float z2 = radius*sinD(90.0 - vertAngleStep) - radius;
      float x3;
      float y3;
      float z3;
      crossProduct(x1, y1, z1, x2, y2, z2, &x3, &y3, &z3, 1);
      glNormal3f(x3, 
                 y3, 
                 z3); 
      glVertex3f(0, 0, radius);
      glVertex3f(radius*cosD(90.0 - vertAngleStep)*sinD(i*horAngleStep), 
                 radius*cosD(90.0 - vertAngleStep)*cosD(i*horAngleStep), 
                 radius*sinD(90.0 - vertAngleStep)); 
      glVertex3f(radius*cosD(90.0 - vertAngleStep)*sinD((i-1)*horAngleStep), 
                 radius*cosD(90.0 - vertAngleStep)*cosD((i-1)*horAngleStep), 
                 radius*sinD(90.0 - vertAngleStep)); 
    }
  glEnd();
  glBegin(GL_QUADS);
    for (j = 1; j < vert-1; j++) {
      for (i = 0; i < hor; i++) {
        int i1 = i;
        int j1 = j;
        int i2 = i + 1;
        int j2 = j + 1;
        int iM = 0.5*(i1+i2);
        int jM = 0.5*(j1+j2);
        float radius1 = radius*cosD(90.0-j1*vertAngleStep);
        float radius2 = radius*cosD(90.0-j2*vertAngleStep);
        float radiusM = radius*cosD(90.0-jM*vertAngleStep);
        //glColor3f(1,0,0);
        glNormal3f(radiusM*sinD(iM*horAngleStep), 
                 radiusM*cosD(iM*horAngleStep), 
                 radiusM*sinD(90.0-jM*vertAngleStep));
        glVertex3f(radius1*sinD(i1*horAngleStep), 
                 radius1*cosD(i1*horAngleStep), 
                 radius*sinD(90.0-j1*vertAngleStep));
        //glColor3f(0,1,0);
        /*glNormal3f(radius1*sinD(i2*horAngleStep), 
                 radius1*cosD(i2*horAngleStep), 
                 radius*sinD(90.0-j1*vertAngleStep)); */
        glVertex3f(radius1*sinD(i2*horAngleStep), 
                 radius1*cosD(i2*horAngleStep), 
                 radius*sinD(90.0-j1*vertAngleStep));
        //glColor3f(0,0,1);
        /*glNormal3f(radius2*sinD(i2*horAngleStep), 
                 radius2*cosD(i2*horAngleStep), 
                 radius*sinD(90.0-j2*vertAngleStep)); */
        glVertex3f(radius2*sinD(i2*horAngleStep), 
                 radius2*cosD(i2*horAngleStep), 
                 radius*sinD(90.0-j2*vertAngleStep));
        //glColor3f(1,1,1);
        /*glNormal3f(radius2*sinD(i1*horAngleStep), 
                 radius2*cosD(i1*horAngleStep), 
                 radius*sinD(90.0-j2*vertAngleStep));  */
        glVertex3f(radius2*sinD(i1*horAngleStep), 
                 radius2*cosD(i1*horAngleStep), 
                 radius*sinD(90.0-j2*vertAngleStep)); 
      }
    }
  glEnd();
  /*glBegin(GL_TRIANGLE_FAN);
    //glColor3f(1,0,0);
    glNormal3f(0, 0, -radius);
    glVertex3f(0, 0, -radius);
    for (i = 0; i <= hor; i++) {
      //glColor3f(1.0-1.0*i/hor,0,0);
      glNormal3f(radius*cosD(90.0 - vertAngleStep)*sinD(i*horAngleStep), 
                 radius*cosD(90.0 - vertAngleStep)*cosD(i*horAngleStep), 
                 radius*sinD(-90.0 + vertAngleStep)); 
      glVertex3f(radius*cosD(90.0 - vertAngleStep)*sinD(i*horAngleStep), 
                 radius*cosD(90.0 - vertAngleStep)*cosD(i*horAngleStep), 
                 radius*sinD(-90.0 + vertAngleStep)); 
    }
  glEnd();*/
}

float lLen = 1.2;

void drawNormals(float radius, int vert, int hor) {
  float vertAngleStep = 180.0 / vert;
  float horAngleStep = 360.0 / hor;
  glBegin(GL_LINES);
    glVertex3f(0, 0, radius);
    glVertex3f(0, 0, radius*lLen);
    int i,j;
    for (j = 1; j < vert; j++) {
      for (i = 0; i < hor; i++) {
        float radius1 = radius*cosD(90.0-j*vertAngleStep);
        glVertex3f(radius1*sinD(i*horAngleStep), 
                 radius1*cosD(i*horAngleStep), 
                 radius*sinD(90.0-j*vertAngleStep));
        glVertex3f(radius1*sinD(i*horAngleStep)*lLen, 
                 radius1*cosD(i*horAngleStep)*lLen, 
                 radius*sinD(90.0-j*vertAngleStep)*lLen);
      }
    }
    glVertex3f(0, 0, -radius);
    glVertex3f(0, 0, -radius*lLen);
  glEnd();
}

int normals = 0;

void display(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  //glTranslatef(0, 0, -100);
  //glScalef(scale, scale, scale);
  //glRotatef(angleY, 1, 0, 0);
  //glRotatef(angleX, 0, 1, 0);
  gluLookAt(eyeRadius*eyeX, eyeRadius*eyeY, eyeRadius*eyeZ, 0, 0, 0, upX, upY, upZ);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, LIGHT_DIF);
  glLightfv(GL_LIGHT0, GL_POSITION, LIGHT_POS);
  glColor3f(1,1,1);
  drawSphere2(20, 10, 10);
  if (normals) {
    glColor3f(1,0,0);
    drawNormals(20, 10, 10);
  }
  glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y){
  if (key == 'w') {
    eyeX += upX * speed;
    eyeY += upY * speed;
    eyeZ += upZ * speed;
    normalize(&eyeX, &eyeY, &eyeZ, 1);
    crossProduct(rightX, rightY, rightZ, 
                 eyeX, eyeY, eyeZ, 
                 &upX, &upY, &upZ, 1);
    display();
  } else if (key == 's') {
    eyeX -= upX * speed;
    eyeY -= upY * speed;
    eyeZ -= upZ * speed;
    normalize(&eyeX, &eyeY, &eyeZ, 1);
    crossProduct(rightX, rightY, rightZ, 
                 eyeX, eyeY, eyeZ, 
                 &upX, &upY, &upZ, 1);
    display();
  } else if (key == 'a') {
    eyeX -= rightX * speed;
    eyeY -= rightY * speed;
    eyeZ -= rightZ * speed;
    normalize(&eyeX, &eyeY, &eyeZ, 1);
    crossProduct(eyeX, eyeY, eyeZ,
                 upX, upY, upZ,
                 &rightX, &rightY, &rightZ, 1);
    display();
  } else if (key == 'd') {
    eyeX += rightX * speed;
    eyeY += rightY * speed;
    eyeZ += rightZ * speed;
    normalize(&eyeX, &eyeY, &eyeZ, 1);
    crossProduct(eyeX, eyeY, eyeZ,
                 upX, upY, upZ,
                 &rightX, &rightY, &rightZ, 1);
    display();
  } else if (key == 'q') {
    upX -= rightX * speed;
    upY -= rightY * speed;
    upZ -= rightZ * speed;
    normalize(&upX, &upY, &upZ, 1);
    crossProduct(eyeX, eyeY, eyeZ,
                 upX, upY, upZ,
                 &rightX, &rightY, &rightZ, 1);
    display();
  } else if (key == 'e') {
    upX += rightX * speed;
    upY += rightY * speed;
    upZ += rightZ * speed;
    normalize(&upX, &upY, &upZ, 1);
    crossProduct(eyeX, eyeY, eyeZ,
                 upX, upY, upZ,
                 &rightX, &rightY, &rightZ, 1);
    display();
  } else if (key == 'z') {
    eyeRadius *= 1.1;
    display();
  } else if (key == 'x') {
    eyeRadius /= 1.1;
    display();
  } else if (key == 'n') {
    normals = !normals;
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
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glMatrixMode(GL_PROJECTION);
  //glOrtho(-50, 50, -50, 50, -50, 50);
  gluPerspective(50.0, 1.0, 1.0, 1000.0);
  glMatrixMode(GL_MODELVIEW);
  
}


int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
  glutCreateWindow("Star");
  glutDisplayFunc(display);  
  glutKeyboardFunc(keyboard);
  glutReshapeFunc(reshape);
  init();
  glutMainLoop();
  return 0;             
}
