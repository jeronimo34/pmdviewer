#pragma once

#include "VmdLoader.h"
#include <map>
#include <vector>
#include "constant.h"
#include "vector3.h"
#include "PmdMesh.h"
#include "PMDLoader.h"


using namespace std;

struct MotionInfo{
  int motionpos;
  int flame;
  int lastflame;
};

class CMotionManager {
 private:
  CMotionManager(){
    m_motionNum = 0;
  }
  ~CMotionManager();
  map<int, vector<VmdMotion> >  m_Motions;
  vector<float> m_prevFrames, m_nextFrames;

  static int m_motionNum;

  //ボーンそれぞれのキーフレームを取得 pair  フレーム番号 motionの位置
  map<string, vector< pair<int, int> > > m_Bonemap;
  //IK
  int m_lastframe;

 public:
  static CMotionManager& instance(){
    static CMotionManager inst;
    return inst;
  }  
  void getAttributeIK(const CPmdMesh &mesh);  
  CMatrix4 *getAttribute(int motionid, float frame, const CPmdMesh &mesh); 

  void registVMDMotion(CVmdLoader* loader);
};


