#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

float cube_size = 5.0;
float distance = 15.0;
float angleY = 0.0;

void display(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	  
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();	
  gluLookAt(0.0, 0.0, distance, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(50.0, 1.0, 1.0, distance + cube_size);
  glMatrixMode(GL_MODELVIEW);
	
  glColor3f(0.7, 0.0, 0.7);
  glRotatef(angleY, 0.0, 1.0, 0.0);
  glRotatef(45, 1.0, 0.0, 0.0);
  glRotatef(45, 0.0, 0.0, 1.0);
  glutWireCube(cube_size);

  glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y){
    if( key == '\033' ){
        exit(0);
    }
    if( key == 'w'){
	distance += 1;
	display();
    }
    if( key == 's'){
        distance -= 1;
	display();
    }
    if( key == 'a'){
	angleY += 1;
	display();
    }
    if( key == 'd'){
        angleY -= 1;
	display();
    }
}


int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutCreateWindow("3D cube with control");
  glutDisplayFunc(display);  
  glutKeyboardFunc(keyboard);

  glEnable(GL_DEPTH_TEST);

  glutMainLoop();
  return 0;             
}
