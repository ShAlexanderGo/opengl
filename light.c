#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

int cube = 1;

//camera rotation
float angleH = 0;
float angleV = 45;
float scale = 1;
const float SCALE_STEP = 1.1;
const float ANGLE_SPEED = 2;

//sphere
float sphere_angle = 0;
const float ROTATION_STEP = 2;
float radius = 15;
GLfloat mat_emiss[] = {0.2, 0.2, 0.0, 1};

//lights
float cutoff = 30;
const float CUTOFF_STEP = 2;
const float LIGHT0_RADIUS = 50;
const float LIGHT1_RADIUS = 50;
float light1_angle = 90;
const float LIGHT1_STEP = 2;
float light0_diff[] = {1.0, 0.0, 0.0, 1.0};
float light1_diff[] = {0.0, 0.0, 1.0, 1.0};
float light_null_pos[] = {0.0, 0.0, 0.0, 1.0};

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

void coordinates(int length) {
  glDisable(GL_LIGHTING);
  glBegin(GL_LINES);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(0, 0, 0);
    glVertex3f(length, 0, 0);
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, length, 0);
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, length);
  glEnd();
  glEnable(GL_LIGHTING);
}

void drawSide(float size, int strips) {
  float step = size / strips;
  glBegin(GL_QUADS);
    glNormal3f(0, 0, 1);
    int i,j;
    for (i = 0; i < strips; i++) {
      for (j = 0; j < strips; j++) {
        glVertex3f((i+1)*step-size/2, (j+1)*step-size/2, size/2);
        glVertex3f((i)*step-size/2, (j+1)*step-size/2, size/2);
        glVertex3f((i)*step-size/2, (j)*step-size/2, size/2);
        glVertex3f((i+1)*step-size/2, (j)*step-size/2, size/2);
      }
    }
  glEnd();
}

void drawCube(float size, int strips) {
  glPushMatrix();
    drawSide(size, strips);
    glRotatef(90, 1, 0, 0);
    drawSide(size, strips);
    glRotatef(90, 1, 0, 0);
    drawSide(size, strips);
    glRotatef(90, 1, 0, 0);
    drawSide(size, strips);
    glRotatef(90, 1, 0, 0);
    glRotatef(-90, 0, 1, 0);
    drawSide(size, strips);
    glRotatef(180, 0, 1, 0);
    drawSide(size, strips);
  glPopMatrix();
}

void display(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  gluLookAt(100*cosD(45), 100*sinD(45), 0, 0, 0, 0, 0, 0, 1);
  glRotatef(angleV, -1, 1, 0);
  glRotatef(angleH, 0, 0, -1);
  glScalef(scale, scale, scale);
  coordinates(50);
  /**
  * light 0, z = 0, y = 0, x =-radius
  * dir = {1, 0, 0}
  */
  const float light0_pos[] = {-LIGHT1_RADIUS, 0.0, 20.0, 1.0};
  const float light0_dir[] = {0.0 - light0_pos[0], 
                              0.0 - light0_pos[1], 
                              0.0 - light0_pos[2], 1.0};
  glPushMatrix();
  glTranslatef(light0_pos[0], light0_pos[1], light0_pos[2]);
  glRotatef(cutoff, 0, 0, 1);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diff);
  glLightfv(GL_LIGHT0, GL_POSITION, light_null_pos);
  glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light0_dir);
  glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, cutoff);
  glDisable(GL_LIGHTING);
  glColor3fv(light0_diff);
  glutSolidSphere(1, 10, 10);
  glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(light0_dir[0], light0_dir[1], light0_dir[2]);
  glEnd();
  glEnable(GL_LIGHTING);
  glPopMatrix(); 
  //////  
  /**
  * light 1, z = 0, y = 0, x =+radius
  * dir = {-1, 0, 0}
  */
  const float light1_pos[] = {LIGHT1_RADIUS * cosD(light1_angle), 
                              LIGHT1_RADIUS * sinD(light1_angle), 20.0, 1.0};
  const float light1_dir[] = {0.0 - light1_pos[0], 
                              0.0 - light1_pos[1], 
                              0.0 - light1_pos[2], 1.0};
  glPushMatrix();
  glTranslatef(light1_pos[0], light1_pos[1], light1_pos[2]);
  glRotatef(cutoff, 0, 0, 1);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diff);
  glLightfv(GL_LIGHT1, GL_POSITION, light_null_pos);
  glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light1_dir);
  glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, cutoff);
  glDisable(GL_LIGHTING);
  glColor3fv(light1_diff);
  glutSolidSphere(1, 10, 10);
  glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(light1_dir[0], light1_dir[1], light1_dir[2]);
  glEnd();
  glEnable(GL_LIGHTING);
  glPopMatrix();  
  //////    

  glPushMatrix();
  glTranslatef(0, 0, -20); 
  if (cube) {
    drawCube(30, 50);
  } else {
    glutSolidCube(30);
  }
  glPopMatrix();

  glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat_emiss);
  glPushMatrix();
  glRotatef(sphere_angle, 0, 0, 1);
  glTranslatef(radius, 0, 0);
  glutSolidSphere(4, 100, 100);
  glPopMatrix(); 
  const GLfloat mat_emiss_null[] = {0.0, 0.0, 0.0, 0.0};  
  glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat_emiss_null);

  glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y){
  if (key == 'i') {
    angleV += ANGLE_SPEED;
    if (angleV > 90) angleV = 90;
  } else if (key == 'k') {
    angleV -= ANGLE_SPEED;
    if (angleV < -90) angleV = -90;
  } else if (key == 'j') {
    angleH -= ANGLE_SPEED;
  } else if (key == 'l') {
    angleH += ANGLE_SPEED;
  } else if (key == 'u') {
    scale *= SCALE_STEP;
  } else if (key == 'o') {
    scale /= SCALE_STEP;
  } else if (key == 'n') {
    radius -= 1;
    if (radius < 0) radius = 0;
  } else if (key == 'm') {
    radius += 1;
  } else if (key == 'p') {
    if (cube) {
      cube = 0;
    } else {
      cube = 1;
    }
  } else if (key == '+') {
    cutoff += CUTOFF_STEP;
  } else if (key == '-') {
    cutoff -= CUTOFF_STEP;
    if (cutoff < 0) cutoff = 0;
  } else if (key == 'w') {
    light1_angle += LIGHT1_STEP;
  } else if (key == 's') {
    light1_angle -= LIGHT1_STEP;
  } else if (key == '1') {
    if (light0_diff[0] > 0.5) {
      light0_diff[0] = 0.0;
    } else {
      light0_diff[0] = 1.0;
    }
  } else if (key == '2') {
    if (light0_diff[1] > 0.5) {
      light0_diff[1] = 0.0;
    } else {
      light0_diff[1] = 1.0;
    }
  } else if (key == '3') {
    if (light0_diff[2] > 0.5) {
      light0_diff[2] = 0.0;
    } else {
      light0_diff[2] = 1.0;
    }
  } else if (key == '4') {
    if (light1_diff[0] > 0.5) {
      light1_diff[0] = 0.0;
    } else {
      light1_diff[0] = 1.0;
    }
  } else if (key == '5') {
    if (light1_diff[1] > 0.5) {
      light1_diff[1] = 0.0;
    } else {
      light1_diff[1] = 1.0;
    }
  } else if (key == '6') {
    if (light1_diff[2] > 0.5) {
      light1_diff[2] = 0.0;
    } else {
      light1_diff[2] = 1.0;
    }
  } else if (key == '7') {
    if (mat_emiss[0] > 0.1) {
      mat_emiss[0] = 0.0;
    } else {
      mat_emiss[0] = 0.2;
    }
  } else if (key == '8') {
    if (mat_emiss[1] > 0.1) {
      mat_emiss[1] = 0.0;
    } else {
      mat_emiss[1] = 0.2;
    }
  } else if (key == '9') {
    if (mat_emiss[2] > 0.1) {
      mat_emiss[2] = 0.0;
    } else {
      mat_emiss[2] = 0.2;
    }
  }
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

void init(void) {
  
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  glEnable(GL_LIGHTING);
  
  glEnable(GL_DEPTH_TEST);

  glMatrixMode(GL_PROJECTION);
  //glOrtho(-50, 50, -50, 50, -50, 50);
  gluPerspective(50.0, 1.0, 1.0, 1000.0);
  glMatrixMode(GL_MODELVIEW);

  const GLfloat mat_diff[] = {0.5,0.5,0.5,1};
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diff);
  const GLfloat mat_spec[] = {1,0.0,0.0,1};
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_spec); 
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 90);
}

void update(int value) {
  sphere_angle += ROTATION_STEP;
  if (sphere_angle > 360) sphere_angle -= 360;
  glutTimerFunc(30, update, 0);
  display();
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
  glutCreateWindow("Star");
  glutDisplayFunc(display);  
  glutKeyboardFunc(keyboard);
  glutReshapeFunc(reshape);
  init();
  glutTimerFunc(30, update, 0);
  glutMainLoop();
  return 0;             
}
