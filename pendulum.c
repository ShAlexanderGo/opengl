//////////////////////////////////////////
// K08-361 Shtanko
// Variant #3
// gcc -o exeas pendulum.c -lGL -lGLU -lglut -lm
//////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

//////////////////
// camera rotation
const float ANGLE_X_STEP = 2;
const float ANGLE_Y_STEP = 2;
const float SCALE_STEP = 1.1; //>1.0

float angleX = 0; //any
float angleY = 45; //-90 +90
float scale = 0.3;
//////////////////

/////////////////
// pendulum position
float fai = 0;
float pen_radius = 0;
float x = 0;
float y = 10;
float z = 0;
////////////////

/////////////
// pendulum parameters
const float OSC_RADIUS = 90;
const float DAMPING_RATE = 400;
const float THREAD_LENGTH = 90;
const float SHIFT_UP = 10;
const float MOV_SPEED = 1;
/////////////

////////////////
// lighting
const float LIGHT0_DIFF[] = {1.0, 0.5, 0.5, 1.0};
const float LIGHT_RADIUS = 100;
float light_angle = 0;
const float LIGHT1_AMB[] = {0.1, 0.1, 0.1, 1.0};
///////////////

////////////////////
// geometry
float toRadians(float angle) {
  return angle * (M_PI / 180);
}

float toDegrees(float angle) {
  return angle * (180 / M_PI);
}

float sinD(float degree) {
  return sin(toRadians(degree));
}

float cosD(float degree) {
  return cos(toRadians(degree));
}
//////////////////

// sphere from lab 5
void drawHalfSphere(float radius, int vert, int hor) {
  float vertAngleStep = 90.0 / vert;
  float horAngleStep = 360.0 / hor;
  glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0, 0, radius);
    glVertex3f(0, 0, radius);
    int i,j;
    for (i = hor; i >= 0; i--) {
      glNormal3f(radius*cosD(90.0 - vertAngleStep)*sinD(i*horAngleStep), 
                 radius*cosD(90.0 - vertAngleStep)*cosD(i*horAngleStep), 
                 radius*sinD(90.0 - vertAngleStep)); 
      glVertex3f(radius*cosD(90.0 - vertAngleStep)*sinD(i*horAngleStep), 
                 radius*cosD(90.0 - vertAngleStep)*cosD(i*horAngleStep), 
                 radius*sinD(90.0 - vertAngleStep)); 
    }
  glEnd();
  glBegin(GL_QUADS);
    for (j = 1; j < vert; j++) {
      for (i = 0; i < hor; i++) {
        int i1 = i;
        int j1 = j;
        int i2 = i + 1;
        int j2 = j + 1;
        float radius1 = radius*cosD(90.0-j1*vertAngleStep);
        float radius2 = radius*cosD(90.0-j2*vertAngleStep);
        glNormal3f(radius1*sinD(i1*horAngleStep), 
                 radius1*cosD(i1*horAngleStep), 
                 radius*sinD(90.0-j1*vertAngleStep));
        glVertex3f(radius1*sinD(i1*horAngleStep), 
                 radius1*cosD(i1*horAngleStep), 
                 radius*sinD(90.0-j1*vertAngleStep));
        glNormal3f(radius1*sinD(i2*horAngleStep), 
                 radius1*cosD(i2*horAngleStep), 
                 radius*sinD(90.0-j1*vertAngleStep)); 
        glVertex3f(radius1*sinD(i2*horAngleStep), 
                 radius1*cosD(i2*horAngleStep), 
                 radius*sinD(90.0-j1*vertAngleStep));
        glNormal3f(radius2*sinD(i2*horAngleStep), 
                 radius2*cosD(i2*horAngleStep), 
                 radius*sinD(90.0-j2*vertAngleStep)); 
        glVertex3f(radius2*sinD(i2*horAngleStep), 
                 radius2*cosD(i2*horAngleStep), 
                 radius*sinD(90.0-j2*vertAngleStep));
        glNormal3f(radius2*sinD(i1*horAngleStep), 
                 radius2*cosD(i1*horAngleStep), 
                 radius*sinD(90.0-j2*vertAngleStep));  
        glVertex3f(radius2*sinD(i1*horAngleStep), 
                 radius2*cosD(i1*horAngleStep), 
                 radius*sinD(90.0-j2*vertAngleStep)); 
      }
    }
  glEnd();
}

void drawPendulum() {
   glPushMatrix();
   glTranslatef(0,5,0);
   glRotatef(90,1,0,0);
   glutSolidCone(2, 5, 30, 30);
   glRotatef(180,1,0,0);
   drawHalfSphere(2, 30, 30);
   glPopMatrix();
}

float* points;
int SIZE = 10000;
int pointer = 0;

void display(void) {
  int i, j;
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  glTranslatef(0, 0, -100);
  glScalef(scale, scale, scale);
  glRotatef(angleY, 1, 0, 0);
  glRotatef(angleX, 0, 1, 0);

  glLightfv(GL_LIGHT0, GL_DIFFUSE, LIGHT0_DIFF);
  const float light0_pos[] = {LIGHT_RADIUS*sinD(light_angle), 20.0, 
                              LIGHT_RADIUS*cosD(light_angle), 1.0};
  glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
  const float light0_dir[] = {0-light0_pos[0], 0-light0_pos[1], 0-light0_pos[2], 1.0};
  glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light0_dir);
  //////marker0
  glPushMatrix();
  glTranslatef(light0_pos[0], light0_pos[1], light0_pos[2]);
  glDisable(GL_LIGHTING);
  glColor3fv(LIGHT0_DIFF);
  glutSolidSphere(1, 10, 10);
  glEnable(GL_LIGHTING);
  glPopMatrix(); 
  //////    
  const GLfloat mat_sand[] = {1.0, 1.0, 0.0, 1.0};  
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_sand);
  glBegin(GL_QUADS);
    //many quads for lighting to work properly
    glNormal3f(0, 1, 0);
    for (i = -5; i < 5; i++) {
      for (j = -5; j < 5; j++) {
        glVertex3f(i*10, 0, j*10);
        glVertex3f(i*10, 0, (j+1)*10);
        glVertex3f((i+1)*10, 0, (j+1)*10);
        glVertex3f((i+1)*10, 0, j*10);
      }
    }
  glEnd();
  const GLfloat mat_rope[] = {1.0, 0.0, 0.0, 1.0};  
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_rope);
  glBegin(GL_LINES);
    glVertex3f(0, 100, 0);
    glVertex3f(x, y, z);
  glEnd();
  const GLfloat mat_traj[] = {0.0, 0.0, 0.0, 1.0};  
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_traj);
  glBegin(GL_LINES);
    i = 0;
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
  glTranslatef(x, y, z);
  glRotatef(toDegrees(asin(sqrt(x*x+z*z)/THREAD_LENGTH)), -z, 0, x);
  const GLfloat mat_pend[] = {1.0, 1.0, 0.0, 0.3};  
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_pend);
  drawPendulum();
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
  glEnable (GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_LIGHTING);
  
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  glLightfv(GL_LIGHT1, GL_AMBIENT, LIGHT1_AMB);
  
  glEnable(GL_DEPTH_TEST);

  glMatrixMode(GL_PROJECTION);
  gluPerspective(50.0, 1.0, 1.0, 1000.0);
  glMatrixMode(GL_MODELVIEW);
}

float func(float ang) {
  return OSC_RADIUS*cosD(3.1*ang+20)*sinD(3.1*ang+20)/(1 + ang/DAMPING_RATE);
}

//y - up
void update(int value) {
  light_angle += 1;
  float deltafai = MOV_SPEED*(1 - fabs(pen_radius) / OSC_RADIUS);
  fai += deltafai;
  pen_radius = func(fai);
  x = pen_radius*cosD(fai);
  z = pen_radius*sinD(fai);
  y = SHIFT_UP + THREAD_LENGTH - sqrt(THREAD_LENGTH*THREAD_LENGTH - pen_radius*pen_radius);
  points[pointer] = x;
  pointer++;
  points[pointer] = z;
  pointer++;
  if (pointer > 500) {
    int i = 0;
    while (i < pointer) {
      points[i] = points[i+2];
      points[i+1] = points[i+3];
      i += 2;
    }
    pointer -= 2;
  }
  points[pointer] = -20000;
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
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
  glutCreateWindow("Pendulum");
  glutDisplayFunc(display);  
  glutKeyboardFunc(keyboard);
  glutReshapeFunc(reshape);
  init();
  glutTimerFunc(30, update, 0);
  glutMainLoop();
  return 0;             
}
