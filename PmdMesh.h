#pragma once
#ifndef __PMDMESH_H
#define __PMDMESH_H

#include "constant.h"
#include "Mesh.h"
#include "PMDLoader.h"
#include "../VECTOR3/vector3.h"

#include <map>

const char SKINMESH_VERT[] = "skinmesh.vert";
const char SKINMESH_FRAG[] = "skinmesh.frag";

struct Bone{
  int id;
  Bone *parent;

  Bone *firstChild;
  Bone *sibling;
  CMatrix4 initMat;
  CMatrix4 boneMat;
  CMatrix4 offsetMat;
  CMatrix4 *combMatAry;
  char boneName[20];
  BYTE type;
  
Bone() : id(-1),
    firstChild(NULL),
    sibling(NULL),
    combMatAry(NULL),
    parent(NULL)
  {
    type = 0;
  }

  CMatrix4 GetModelLocalBoneMat() const {
    if(parent)return parent->GetModelLocalBoneMat() * boneMat;
    else return boneMat;
  }

  CVector3 GetModelLocalPosition() const {
    CVector3 v;
    return GetModelLocalBoneMat() * v;
  }
};

struct PMDVERTEX{
  CVector3 position;
  CVector3 weight;
  BYTE matrixIndex[4];
  CVector3 normal;
  float uv[2];
};

class CPmdMesh : public CMesh{
 private:
  DWORD m_vertexNum;
  MmdStruct::PmdVertex *m_pVertex;

  DWORD m_idxNum;
  WORD *m_pIndex;

  DWORD m_materialNum;
  MmdStruct::PmdMaterial *m_pMaterial;

  //bone
  DWORD m_boneNum;
  MmdStruct::PmdBone *m_pBone;
  
  //ik
  WORD m_ikNum;
  MmdStruct::PmdIK *m_pIK;

  void copyPmdIK(MmdStruct::PmdIK **out, const MmdStruct::PmdIK *in);

  //skinmesh
  Bone *m_pB;//bone
  CMatrix4 *m_pCombMat;
  GLuint m_skinshader;
  GLint m_uniform_defMat;
  GLint m_attribute_boneNo;
  GLint m_attribute_boneWeight;

  void print(CMatrix4* m);
  //現在の姿勢を保持する。
  CMatrix4 *m_pDefMat;

  //animation
  float m_flame;
  float m_startflame;
  float m_lastflame;
  bool m_repeat;
  float m_dt;
  bool m_animation;
 public:

  CPmdMesh(CPMDLoader* loader);
  virtual ~CPmdMesh();
  virtual void render();

  void AnimationStart(){
    m_animation = true;
  }
  
  void AnimationStop(){
    m_animation = false;
  }

  void AnimationReset(){
    m_flame = 0;
  }

  void SetAnimationSpeed(float speed){
    m_dt = speed;
  }
  
  void SetAnimationRepeat(bool rep){
    m_repeat = rep;
  }
  void AnimationUpdate(float dt);

  Bone *bone() const {
    return m_pB;
  }

  int boneNum() const {
    return m_boneNum;
  }
  
  //boneと同じ数ある
  CMatrix4 *boneDefMat() const {
    return m_pDefMat;
  }

  MmdStruct::PmdIK *getIkData()const{
    return m_pIK;
  }

  WORD getIkNum() const {
    return m_ikNum;
  }

};

#endif
