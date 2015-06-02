#pragma once
#include "glsl.h"

#include "constant.h"
#include "PMDLoader.h"
#include "PmdMesh.h"


class CMyApp{
 private:
  CPMDLoader* m_pmdloader;
  CPmdMesh* m_pmdMesh;
  GLuint m_shaderProg;
 public:
  CMyApp();
  CMyApp(const CMyApp&);
  ~CMyApp();
  void Draw();
};
