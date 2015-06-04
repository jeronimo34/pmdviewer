#include "VmdLoader.h"
#include <ostream>
#include <istream>
#include <fstream>

#include <iostream>

CVmdLoader::CVmdLoader(const char* filename){

// PMDファイルからPMDデータを抽出
  ifstream ifs(filename, ios::binary);
  if (ifs.fail()) throw "cannot find file.";
  char hoge[50];
  ifs.read(hoge, 50);
  int numVmdMotion = 0;
  ifs.read((char*)&numVmdMotion, 4);
  vmdMotions.resize(numVmdMotion);

  for(int i = 0; i < (int)vmdMotions.size(); ++i){
    VmdMotion v;
    ifs.read(v.boneName, 15);
    ifs.read((char*)&v.flameNo, sizeof(float));
    ifs.read((char*)&v.location[0], sizeof(float) * 3);
    ifs.read((char*)&v.rotation[0], sizeof(float) * 4);
    ifs.read((char*)&v.interpolation[0], 64);
    //openglは右手
    v.location[0] *= -1.0;
    v.location[2] *= -1.0;

    v.rotation[0] *= -1.0;
    v.rotation[2] *= -1.0;

    vmdMotions[i] = v;
  }

  //表情データ
  DWORD morphsize = 0;
  ifs.read((char*)&morphsize, sizeof(DWORD));
  m_vmdMorphs.resize(morphsize);
  
  for(size_t i = 0; i < m_vmdMorphs.size(); ++i){
    VmdMorph v;
    ifs.read((char*)&v.skinName, 15);
    ifs.read((char*)&v.flameNo, sizeof(DWORD));
    ifs.read((char*)&v.weight, sizeof(float));
    m_vmdMorphs[i] = v;
  }
  
  ifs.close();
}


CVmdLoader::~CVmdLoader() {
  while(!vmdMotions.empty())
    vmdMotions.pop_back();

  while(!m_vmdMorphs.empty())
    m_vmdMorphs.pop_back();
}

  VmdHeader CVmdLoader::GetHeader() {
    return vmdHeader;
  }
  vector<VmdMotion> CVmdLoader::GetMotions() const {
    return vmdMotions;
  }

