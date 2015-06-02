#pragma once

#ifdef __APPLE__
//#include "../glew/glew.h"
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include <cstdio>
#include <ostream>
#include <fstream>

using namespace std;

class CGLSL{
 private:
  GLuint m_gl2Program;
  void printShaderInfoLog(GLuint shader);
  void printProgramInfoLog(GLuint program);
  bool loadShader(GLuint shader, const char *file);
 public:
  CGLSL(){};
  ~CGLSL(){};

  GLuint makeProgram(const char *vname, const char *fname);
#ifndef __APPLE__
  static bool glslInit(){return glewInit();}
#endif
};
