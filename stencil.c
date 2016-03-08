#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

void drawBox(void) {

  /* Clear stencil buffer */
  
  glClearStencil(0x0);

  /* Use stencil buffering. */
  
  glEnable(GL_STENCIL_TEST);

  /* Fill cube zone with 1 in stencil buffer */
  glStencilFunc(GL_ALWAYS, 1, 0xffffffff);
  glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);  
  glutSolidCube(2.0);  

  /* Fill sphere zone with 2 in stencil buffer if depth test is done */
  glStencilFunc(GL_ALWAYS, 2, 0xffffffff);
  glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);  
  glutSolidSphere(1.2, 100, 100);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  /* Output cube zone where stencil buffer value equal to 1 */
  glStencilFunc(GL_EQUAL, 1, 0xffffffff);
  glutSolidCube(2.0);  
}

void display(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  drawBox();
  glutSwapBuffers();
}

void init(void) {

  /* Enable OpenGL light. */

  GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};   /* Cyan diffuse light. */
  GLfloat light_position[] = {-1.0, -1.0, 1.0, 1.0};  /* Infinite light1 location. */

  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHTING);

  /* Use depth buffering for hidden surface elimination. */
  glEnable(GL_DEPTH_TEST);
  
  /* Setup the view of the cube. */
  glMatrixMode(GL_MODELVIEW);
  gluLookAt(7.0, 0.0, 5.0,  /* eye is at (7,0,5) */
            0.0, 0.0, 0.0,  /* center is at (0,0,0) */
            0.0, 1.0, 0.0); /* up is in positive Y direction */

  /* Adjust cube position to be asthetic angle. */
  glTranslatef(0.0, 0.0, -1.0);  /* shift to z=-1 */
  glRotatef(60, 1.0, 0.0, 0.0);  /* rotate 60 degree around x-axe */
  glRotatef(-20, 0.0, 0.0, 1.0); /* rotate -20 degree around z-axe */

  /* Setup the projection model */
  glMatrixMode(GL_PROJECTION);
  gluPerspective( /* field of view in degree */ 50.0,
    /* aspect ratio */ 1.0,
    /* Z near */ 1.0, /* Z far */ 15.0);
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);
  glutCreateWindow("Cube");
  glutDisplayFunc(display);  
  init();
  glutMainLoop();
  return 0;             
}
