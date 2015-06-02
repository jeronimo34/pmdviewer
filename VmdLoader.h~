#pragma once
#pragma pack(push,1) //アラインメント制御をオフ

#include <vector>
#include "constant.h"

using namespace std;

struct VmdHeader {
  char vmdHeader[30];
  char vmdModelName[20];
};

struct VmdMotion {
  char boneName[15];
  DWORD flameNo;
  float location[3];
  float rotation[4];
  BYTE interpolation[64];
};

#pragma pack(pop)

class CVmdLoader{
 private:
  VmdHeader vmdHeader;
  vector<VmdMotion> vmdMotions;
 public:
  CVmdLoader(const char* filename);
  virtual ~CVmdLoader();
  VmdHeader GetHeader();
  vector<VmdMotion> GetMotions() const;
};


