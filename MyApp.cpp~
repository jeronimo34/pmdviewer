#include "glsl.h"
#include "MyApp.h"
#include "MotionManager.h"
#include "VmdLoader.h"


CMyApp::CMyApp(){

  //glutかgl初期設定してからでないとバグリます。
  //appleは初期化不要だそうです。
  #ifndef __APPLE__
  cout << "shader init" << endl;
  if(CGLSL::glslInit()){
    cout << "shader error" << endl;
    throw "error"; 
  }
  #endif

  CPMDLoader *pmdloader = new CPMDLoader("../resource/cirno.pmd");
  CVmdLoader *vmdloader = new CVmdLoader("nya.vmd");

  CMotionManager& inst = CMotionManager::instance();
  inst.registVMDMotion(vmdloader);
  m_pmdMesh = new CPmdMesh(pmdloader);

  m_pmdMesh->SetAnimationSpeed(0.5);
  m_pmdMesh->AnimationStart();

  //  inst.getAttribute(0,40,*m_pmdMesh);

  SAFE_DELETE(pmdloader);
  SAFE_DELETE(vmdloader);  
}

CMyApp::~CMyApp(){
  SAFE_DELETE(m_pmdMesh);
}

void CMyApp::Draw(){
  m_pmdMesh->render();
}
