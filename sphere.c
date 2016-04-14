#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#include "bmp_texture.h"

//eye position
float angleH = 0;
float angleV = 45;
float scale = 1;
const float SCALE_STEP = 1.1;
const float ANGLE_SPEED = 2;
//light position
float lightH = 0;
float lightV = 0;
//params
int textType = 0;
int normType = 0;
int strips = 10;

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
GLuint texId;
GLuint loadBMPTexture (const char *filename){
  gl_texture_t *bmp_tex = NULL;
  GLuint tex_id = 0;

  bmp_tex = ReadBMPFile (filename);

  if (bmp_tex && bmp_tex->texels)
    {
      /* generate texture */
      glGenTextures (1, &bmp_tex->id);
      glBindTexture (GL_TEXTURE_2D, bmp_tex->id);

      /* setup some parameters for texture filters and mipmapping */
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      /*
      glTexImage2D (GL_TEXTURE_2D, 0, bmp_tex->internalFormat,
		    bmp_tex->width, bmp_tex->height, 0, bmp_tex->format,
		    GL_UNSIGNED_BYTE, bmp_tex->texels);
      */

      gluBuild2DMipmaps (GL_TEXTURE_2D, bmp_tex->internalFormat,
			 bmp_tex->width, bmp_tex->height,
			 bmp_tex->format, GL_UNSIGNED_BYTE, bmp_tex->texels);

      tex_id = bmp_tex->id;

      /* OpenGL has its own copy of texture data */
      free (bmp_tex->texels);
      free (bmp_tex);
    }

  return tex_id;
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
  glEnable (GL_TEXTURE_2D);
  glBindTexture (GL_TEXTURE_2D, texId);
  int i,j;
  glBegin(GL_QUADS);
    for (j = 0; j < vert; j++) {
      for (i = 0; i < hor; i++) {
        int i1 = i;
        int j1 = j;
        int i2 = i + 1;
        int j2 = j + 1;
        int iM = 0.5 * (i1 + i2);
        int jM = 0.5 * (j1 + j2);
        float radius1 = radius * cosD(90.0 - j1 * vertAngleStep);
        float radius2 = radius * cosD(90.0 - j2 * vertAngleStep);
        float radiusM = radius * cosD(90.0 - jM * vertAngleStep);        

        if (!normType)
          glNormal3f(radiusM * sinD(iM * horAngleStep), 
                     radiusM * cosD(iM * horAngleStep), 
                     radiusM * sinD(90.0 - jM * vertAngleStep));

        glTexCoord2f ((float)i1 / hor, (float)j1 / vert);
        if (normType)
          glNormal3f(radius1 * sinD(i1 * horAngleStep), 
                     radius1 * cosD(i1 * horAngleStep), 
                     radius * sinD(90.0 - j1 * vertAngleStep));
        glVertex3f(radius1 * sinD(i1 * horAngleStep), 
                   radius1 * cosD(i1 * horAngleStep), 
                   radius * sinD(90.0 - j1 * vertAngleStep));
        
        glTexCoord2f ((float)i2 / hor, (float)j1 / vert);
        if (normType)
          glNormal3f(radius1 * sinD(i2 * horAngleStep), 
                     radius1 * cosD(i2 * horAngleStep), 
                     radius * sinD(90.0 - j1 * vertAngleStep)); 
        glVertex3f(radius1 * sinD(i2 * horAngleStep), 
                   radius1 * cosD(i2 * horAngleStep), 
                   radius * sinD(90.0 - j1 * vertAngleStep));
        
        glTexCoord2f ((float)i2 / hor, (float)j2 / vert);
        if (normType)
          glNormal3f(radius2 * sinD(i2 * horAngleStep), 
                     radius2 * cosD(i2 * horAngleStep), 
                     radius * sinD(90.0 - j2 * vertAngleStep)); 
        glVertex3f(radius2 * sinD(i2 * horAngleStep), 
                   radius2 * cosD(i2 * horAngleStep), 
                   radius * sinD(90.0 - j2 * vertAngleStep));
        
        glTexCoord2f ((float)i1 / hor, (float)j2 / vert);
        if (normType)
          glNormal3f(radius2 * sinD(i1 * horAngleStep), 
                     radius2 * cosD(i1 * horAngleStep), 
                     radius * sinD(90.0 - j2 * vertAngleStep));  
        glVertex3f(radius2 * sinD(i1 * horAngleStep), 
                   radius2 * cosD(i1 * horAngleStep), 
                   radius * sinD(90.0 - j2 * vertAngleStep)); 
      }
    }
  glEnd();

}

void display(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  gluLookAt(scale * 100 * cosD(-angleH) * cosD(-angleV), 
            scale * 100 * sinD(-angleH) * cosD(-angleV), 
            scale * 100 * sinD(-angleV), 0, 0, 0, 0, 0, -1);
  const float LIGHT_DIF[] = {1.0, 1.0, 1.0, 1.0};
  glLightfv(GL_LIGHT0, GL_DIFFUSE, LIGHT_DIF);
  const float LIGHT_POS[] = {100 * cosD(-lightH) * cosD(-lightV), 
                             100 * sinD(-lightH) * cosD(-lightV), 
                             100 * sinD(-lightV), 1.0};
  glLightfv(GL_LIGHT0, GL_POSITION, LIGHT_POS);
  drawSphere(20, strips, strips);
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
  } else if (key == '+') {
    strips += 1;
    if (strips > 100)
      strips = 100;
  } else if (key == '-') {
    strips -= 1;
    if (strips < 3)
      strips = 3;
  } else if (key == 'o') {
    textType = !textType;
    char* name;
    if (textType) {
      name = "earth.bmp";
    } else {
      name = "mars.bmp";
    }
    if (!(texId = loadBMPTexture(name))){
      exit (-1);
    }
  } else if (key == 'p') {
    normType = !normType;
  }
  display();
}

void keyboard2(int key, int x, int y){
  if (key == GLUT_KEY_UP ){
    lightV += ANGLE_SPEED;
    if (lightV > 89.9)
      lightV = 89.9;
  } else if (key == GLUT_KEY_DOWN ){
    lightV -= ANGLE_SPEED;
    if (lightV < -89.9)
      lightV = -89.9;
  } else if (key == GLUT_KEY_RIGHT ){
    lightH += ANGLE_SPEED;
  } else if (key == GLUT_KEY_LEFT ){
    lightH -= ANGLE_SPEED;    
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
  glEnable(GL_LIGHTING);
  
  const GLfloat MAT_DIFF[] = {0.3,0.3,0.3,1};
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MAT_DIFF);
  const GLfloat MAT_AMB[] = {1.0,1.0,1.0,1};
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MAT_AMB);
  const float LIGHT_AMB[] = {0.2, 0.2, 0.2, 1.0};
  glLightfv(GL_LIGHT0, GL_AMBIENT, LIGHT_AMB);

  glEnable(GL_DEPTH_TEST);
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glMatrixMode(GL_PROJECTION);
  //glOrtho(-50, 50, -50, 50, -50, 50);
  gluPerspective(50.0, 1.0, 1.0, 1000.0);

  /* load png texture */
  if ( !(texId = loadBMPTexture("mars.bmp"))){
    exit (-1);
  }

  glMatrixMode(GL_MODELVIEW);
}


int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
  glutCreateWindow("Star");
  glutDisplayFunc(display);  
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(keyboard2);
  glutReshapeFunc(reshape);
  init();
  glutMainLoop();
  return 0;             
}
