#include <cstdio>
#include <iostream>
#include <ostream>
#include <fstream>
#include <unistd.h>
#include <iconv.h>
#include "MyApp.h"
#include "PMDLoader.h"
#include "Input.h"

using namespace std;
CMyApp* g_app = NULL;
int g_W, g_H;

static void display();
static void key(unsigned char key, int x, int y);
static void reshape(int w, int h);
static void idle();
static void mouse(int button, int state, int x, int y);
static void motion(int x, int y);



void glInit(int argc, char** argv){
  //mac の場合初期化の必要ないそうです。  


  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

  glutInitWindowSize(400,400);
  glutInitWindowPosition(100, 100);
  glutCreateWindow ("Sample");

  glClearColor (0.0, 0.0, 0.0, 0.0);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);//back wo kesu

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  float green[] = {0.8,0.8,0.8,1.0};
  glLightfv(GL_LIGHT0, GL_DIFFUSE, green);
  glLightfv(GL_LIGHT0, GL_AMBIENT, green);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, green);
  glLightfv(GL_LIGHT1, GL_SPECULAR, green);
   /* initialize viewing values  */
   /* orthographic projection */

  glutDisplayFunc(display);
  glutKeyboardFunc(key); 
  glutReshapeFunc(reshape);
  glutIdleFunc(idle);
  glutMotionFunc(motion);
  glutMouseFunc(mouse);
}


void display(){
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  CInput& input = CInput::instance();
  input.update();

  static float lightpos0[] = {0,10,10,1.0};
  static float lightpos[] = {0, 10, 100, 1.0};
  glLightfv(GL_LIGHT0, GL_POSITION, lightpos0);
  glLightfv(GL_LIGHT1, GL_POSITION, lightpos);

  glPushMatrix();
  g_app->Draw();
  glPopMatrix();

  glPushMatrix();
  //Fllor
  static int size = 5;
  for(int i = -50; i < 50; i++){
    for(int j = -50; j < 50; ++j){
      glBegin(GL_LINE_LOOP);
      glVertex3f(i*size,0,j*size);
      glVertex3f((i+1)*size,0,j*size);
      glVertex3f((i+1)*size,0,(j+1)*size);
      glVertex3f(i*size,0,(j+1)*size);
      glEnd();
    }
  }
  glPopMatrix();
  glutSwapBuffers();
}

void key(unsigned char key, int x, int y){
}

void reshape(int w, int h){
  g_W = w;
  g_H = h;
  glViewport(0,0,w,h);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(30.0, (double)g_W/(double)g_H, 1.0, 1000.0);
  glMatrixMode(GL_MODELVIEW);
}

void idle(){
  usleep(16 * 1000);
  glutPostRedisplay();
}

void mouse(int button, int state, int x, int y){
  CInput& input = CInput::instance();

  if(state == GLUT_DOWN){

    switch(button){
    case GLUT_MIDDLE_BUTTON:
      CInput::middle = true;
      break;
    case GLUT_LEFT_BUTTON:
      CInput::left = true;
      break;
    }
    input.mousedown(x,y);
  }

  if(state == GLUT_UP){
    input.mouseup();  
    switch(button){
    case GLUT_MIDDLE_BUTTON:
      CInput::middle = false;
      break;
    case GLUT_LEFT_BUTTON:
      CInput::left = false;
      break;
    }
  }  
}

static void motion(int x, int y){
  CInput::nowmousepos.x = x;
  CInput::nowmousepos.y = y;
}

int main(int argv, char* argc[]){
  glInit(argv, argc);
  g_app = new CMyApp;

  glutMainLoop();

  SAFE_DELETE(g_app);
  return 0;
}
