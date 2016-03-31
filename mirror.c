#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

//camera rotation
float angleH = 0;
float angleV = 45;
float scale = 1;
const float SCALE_STEP = 1.1;
const float ANGLE_SPEED = 2;

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


void drawScene() {
  glPushMatrix();
  glBegin(GL_TRIANGLES);
    glNormal3f(1, 0, 0);
    glVertex3f(-20, 0, 30);
    glVertex3f(-20, -30, -30);
    glVertex3f(-20, 30, -30);
  glEnd();
  glTranslatef(0, 0, -20);
  glutSolidSphere(5, 50, 50);
  glPopMatrix();
}

void display(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  //glClearStencil(0x0);
  glLoadIdentity();
  //gluLookAt(scale*100*cosD(45+angleH)*cosD(angleV), scale*100*sinD(45+angleH)*cosD(angleV), scale*100*sinD(angleV), 0, 0, 0, 0, 0, 1);
  gluLookAt(scale*100*sinD(angleV), scale*100*cosD(45+angleH)*cosD(angleV), scale*100*sinD(45+angleH)*cosD(angleV), 0, 0, 0, 1, 0, 0);
  ////////////
  const GLfloat mat_diff[] = {0.5,0.5,0.5,1};
  const float mirror_diff[] = {1.0,1.0,0.0,0.05};
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diff);
  ////////////
  const GLfloat mat_spec[] = {1,0.0,0.0,1};
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_spec); 
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 90.0);
  ///////////
  glClear(GL_STENCIL_BUFFER_BIT);
  glEnable(GL_STENCIL_TEST);
  glStencilMask(0xFF);

  glStencilFunc(GL_ALWAYS, 1, 0xFF);
  glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
  glBegin(GL_QUADS);
    glNormal3f(0,0,1);
    glVertex3f(20, 20, 0);
    glVertex3f(-20, 20, 0);
    glVertex3f(-20, -20, 0);
    glVertex3f(20, -20, 0);
  glEnd();
  glStencilOp(GL_KEEP, GL_KEEP, GL_ZERO);
  drawScene();
  
  glClear(GL_DEPTH_BUFFER_BIT);

  glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
  glBegin(GL_QUADS);
    glNormal3f(0,0,1);
    glVertex3f(20, 20, 0);
    glVertex3f(-20, 20, 0);
    glVertex3f(-20, -20, 0);
    glVertex3f(20, -20, 0);
  glEnd();
  glStencilFunc(GL_ALWAYS, 2, 0xFF);
  glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
  glPushMatrix();
  glScalef(1.0,1.0,-1.0);
  drawScene();
  glPopMatrix();

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  coordinates(50);
  //////back
  glEnable(GL_LIGHT1);
  glDisable(GL_LIGHT2);
  ////////////
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diff);
  glStencilFunc(GL_EQUAL, 1, 0xFF); // Pass test if stencil value is 1
  glStencilMask(0x00); // Don't write anything to stencil buffer
  /////////////////
  const float light1_pos[] = {50.0, 0.0, -80.0, 1.0};
  const float light1_dir[] = {0.0 - light1_pos[0], 
                              0.0 - light1_pos[1], 
                              0.0 - light1_pos[2], 1.0};
  float light1_diff[] = {1.0, 1.0, 1.0, 1.0};
  float light_null_pos[] = {0.0, 0.0, 0.0, 1.0};
  float light1_spec[] = {1.0,1.0,1.0,1.0};
  ///////////
  glPushMatrix();
  glTranslatef(light1_pos[0], light1_pos[1], light1_pos[2]);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diff);
  glLightfv(GL_LIGHT1, GL_POSITION, light_null_pos);
  glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light1_dir);
  glLightfv(GL_LIGHT1, GL_SPECULAR, light1_spec);
  glPopMatrix(); 
  drawScene();
  

  //////front
  glEnable(GL_LIGHT2);
  glDisable(GL_LIGHT1);
  ////////////
  glStencilFunc(GL_EQUAL, 2, 0xFF); // Pass test if stencil value is 1
  glStencilMask(0x00); // Don't write anything to stencil buffer
  /////////////
  const float light2_pos[] = {50.0, 0.0, 80.0, 1.0};
  const float light2_dir[] = {0.0 - light2_pos[0], 
                              0.0 - light2_pos[1], 
                              0.0 - light2_pos[2], 1.0};
  float light2_diff[] = {1.0, 1.0, 1.0, 1.0};
  float light2_spec[] = {1.0,1.0,1.0,1.0};
  ///////////
  glPushMatrix();
  glTranslatef(light2_pos[0], light2_pos[1], light2_pos[2]);
  glLightfv(GL_LIGHT2, GL_DIFFUSE, light2_diff);
  glLightfv(GL_LIGHT2, GL_POSITION, light_null_pos);
  glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, light2_dir);
  glLightfv(GL_LIGHT2, GL_SPECULAR, light2_spec);
  glDisable(GL_LIGHTING);
  glColor3fv(light2_diff);
  glutSolidSphere(1, 10, 10);
  glEnable(GL_LIGHTING);
  glPopMatrix(); 
  glPushMatrix();
  glScalef(1.0,1.0,-1.0);
  drawScene();
  glPopMatrix();

  glDisable(GL_STENCIL_TEST);

  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mirror_diff);
  glBegin(GL_QUADS);
    glNormal3f(0,0,1);
    glVertex3f(20, 20, 0);
    glVertex3f(-20, 20, 0);
    glVertex3f(-20, -20, 0);
    glVertex3f(20, -20, 0);
  glEnd();
  float cover_diff[] = {0.0,0.3,0.0,1.0};
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, cover_diff);
  glBegin(GL_QUADS);
    glNormal3f(0,0,1);
    glVertex3f(30, 30, -0.1);
    glVertex3f(-20, 30, -0.1);
    glVertex3f(-20, -30, -0.1);
    glVertex3f(30, -30, -0.1);
  glEnd();

  glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y){
  if (key == 'w') {
    angleV += ANGLE_SPEED;
    if (angleV > 89.9) angleV = 89.9;
  } else if (key == 's') {
    angleV -= ANGLE_SPEED;
    if (angleV < -89.9) angleV = -89.9;
  } else if (key == 'a') {
    angleH -= ANGLE_SPEED;
  } else if (key == 'd') {
    angleH += ANGLE_SPEED;
  } else if (key == 'q') {
    scale *= SCALE_STEP;
  } else if (key == 'e') {
    scale /= SCALE_STEP;
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
  
  glEnable(GL_LIGHTING);
  
  glEnable (GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  glEnable(GL_DEPTH_TEST);

  glMatrixMode(GL_PROJECTION);
  gluPerspective(50.0, 1.0, 1.0, 1000.0);
  glMatrixMode(GL_MODELVIEW);

}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE | GLUT_STENCIL);
  glutCreateWindow("Star");
  glutDisplayFunc(display);  
  glutKeyboardFunc(keyboard);
  glutReshapeFunc(reshape);
  init();
  glutMainLoop();
  return 0;             
}
