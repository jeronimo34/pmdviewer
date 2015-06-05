#include "MotionManager.h"

#include <math.h>
#include <algorithm>
#include <string>
#include <set>


using namespace std;

int CMotionManager::m_motionNum;

CMatrix4* CMotionManager::getAttribute(int motionid, float frame
				       , const CPmdMesh &mesh){
  vector<VmdMotion>& m = m_Motions[motionid];
  int boneNum = mesh.boneNum();

  Bone *b = mesh.bone();
  float p = 0, n = 0;
  int prevpos = 0, nextpos = 0;
  CMatrix4 *pDef = mesh.boneDefMat();
  CMatrix4 prevtrans, nexttrans;
  CQuaternion prevr, nextr;

  for(int i = 0; i < boneNum; ++i){
    string s = b[i].boneName;
    bool flag = true;

    if(m_Bonemap.find(s) != m_Bonemap.end()){
      //      cout << "name : " << s << endl;
      //boneNameがモーションの中にある
      p = 0; n = 0; 
      vector< pair<int, int> >& v=m_Bonemap[s];
      for(int j = 0; j < (int)v.size()-1; ++j){
	n = (float)v[j+1].first;
	if(p <= frame && frame <= n){
	  prevpos = v[j].second;
	  nextpos = v[j+1].second;
	  flag = false;
	  break;
	}
	p = n;
      }


      if(v.size() == 1 || flag){
	prevpos = v[v.size()-1].second;
	nextpos = v[v.size()-1].second;
      }

      prevtrans = Mat4Translated(m[prevpos].location[0],
				 m[prevpos].location[1],
				 m[prevpos].location[2]);//boneの前の姿勢
      nexttrans = Mat4Translated(m[nextpos].location[0],
				 m[nextpos].location[1],
				 m[nextpos].location[2]);//boneの次の姿勢
      prevr = CQuaternion(
			  m[prevpos].rotation[3],//w
			  m[prevpos].rotation[0],//x
			  m[prevpos].rotation[1],//y
			  m[prevpos].rotation[2]//z
			  );
      nextr = CQuaternion(
			  m[nextpos].rotation[3],//w
			  m[nextpos].rotation[0],//x
			  m[nextpos].rotation[1],//y
			  m[nextpos].rotation[2]//z
			  );
      float s = 0.0;
      if(fabs(n - p) < 0.000000001){
      }else{
	s = (n - frame)/(n - p);    
      }

      pDef[i] = (prevtrans * s + nexttrans * (1.0 - s)) * 
	(QuaternionToMatrix(slerp(prevr, nextr, 1.0-s)));
      //      b[i].boneMat = b[i].initMat * pDef[i];
    }
    //モーションの中に一致するボーンがありません
  }
  return NULL;
}

//IK
void CMotionManager::getAttributeIK(const CPmdMesh &mesh)
{
//segru
  CVector3 localEffectorPos, localTargetPos;
  WORD ikNum = mesh.getIkNum();
  MmdStruct::PmdIK *ikdata = mesh.getIkData();

  Bone *bones = mesh.bone();
  //  CMatrix4 *pDef = mesh.boneDefMat();

  for(WORD k = 0; k < ikNum; ++k){
    MmdStruct::PmdIK *data = &ikdata[k];

    for(WORD i = 0; i < data->iterations; ++i){
      for(WORD j = 0; j < data->ik_chain_length; ++j){

	WORD index = data->ik_child_bone_index[j];
	CVector3 iktarget 
	  = bones[data->ik_target_bone_index].GetModelLocalPosition();

	CVector3 ik = bones[data->ik_bone_index].GetModelLocalPosition();
	/*
	if(ik.y < 0){
	  ik.y = 0;
	}
	*/

	CMatrix4 tmp = bones[index].GetModelLocalBoneMat();
	CMatrix4 invCoord = tmp.inverse();

	localEffectorPos = invCoord * iktarget;
	localTargetPos = invCoord * ik;

	CVector3 localEffectorDir = localEffectorPos.normal();
	CVector3 localTargetDir = localTargetPos.normal();

	double p = localEffectorDir.dot(localTargetDir);
	//1 wo koenai youni
	//	cout << p << endl;
	//角度制限
	
	if(p > 1.0 - 1.0e-5f)continue;

	double angle = acos(p);
	if(fabs(angle) > data->control_weight*PI/180.0 ){
	  angle = (angle > 0) ? data->control_weight*PI/180.0 : -data->control_weight * PI/180.0;
	}

	CVector3 axis = localEffectorDir.det(localTargetDir);
	CMatrix4 rotation = QuaternionToMatrix(
					       makeFromAxis(angle, axis)
					       );

	bones[index].boneMat = bones[index].boneMat * rotation;
      }
    }
  }
}

void CMotionManager::getAttributeMorph(int motionid, float flame,const CPmdMesh &mesh){
  MmdStruct::PmdMorph *pmdmorph = mesh.getPmdMorph();
  MmdStruct::PmdVertex *pmdVertex = mesh.getPmdVertex();
  WORD morphNum = mesh.getMorphNum();
  //init morph
  //hyoujou wo base ni suru
  for(WORD i = 0; i < pmdmorph[0].skin_vert_count; ++i){
    int index = pmdmorph[0].skin_data[i].skin_vert_index;
    pmdVertex[index].pos[0] = pmdmorph[0].skin_data[i].skin_vert_pos[0];
    pmdVertex[index].pos[1] = pmdmorph[0].skin_data[i].skin_vert_pos[1];
    pmdVertex[index].pos[2] = pmdmorph[0].skin_data[i].skin_vert_pos[2];
  }


  float p = 0;//prevflame
  float n = 0;//nextflame
  int prev = 0, next = 0;
  //  vector<VmdMorph>& morph = m_Morphs[motionid];

  //i > 0 base ha tobasu
  //wakame
  for(int i = 1; i < morphNum; ++i){
    string s = pmdmorph[i].skin_name;
    bool flag = true;

    if(m_MorphMap.find(s) != m_MorphMap.end()){
      //motion name ga atta
      vector<MorphInfo>& v = m_MorphMap[s];
      for(size_t j = 0; j < v.size()-1; ++j){
	n = v[j+1].flameNo;
	if(p <= flame && flame <= n){
	  prev = j;
	  next = j+1;
	  flag = false;
	  break;
	}
	p = n;
      }

      if(flag){
	prev = next = 0;
      }
      float s = 0.0;
      if(fabs(n - p) < 0.000000001){
      }else{
	s = (n - flame)/(n - p);    
      }

      for(WORD j = 0; j < pmdmorph[i].skin_vert_count; ++j){
	int index = pmdmorph[i].skin_data[j].skin_vert_index;
	index = pmdmorph[0].skin_data[index].skin_vert_index;
	pmdVertex[index].pos[0] += pmdmorph[i].skin_data[j].skin_vert_pos[0]* (v[prev].weight * s + v[next].weight * (1.0-s));
	pmdVertex[index].pos[1] += pmdmorph[i].skin_data[j].skin_vert_pos[1]* (v[prev].weight * s + v[next].weight * (1.0-s));
	pmdVertex[index].pos[2] += pmdmorph[i].skin_data[j].skin_vert_pos[2]* (v[prev].weight * s + v[next].weight * (1.0-s));
      }
    }
  }
}

static bool convFlame(const VmdMotion& a, const VmdMotion &b){
  return a.flameNo < b.flameNo;
}

static bool convFlameMorph(const VmdMorph &a, const VmdMorph &b){
  return a.flameNo < b.flameNo;
}

//vmd motion no touroku
void CMotionManager::registVMDMotion(CVmdLoader *loader){

  m_Motions[m_motionNum] = loader->GetMotions();
  vector<VmdMotion>& m = m_Motions[m_motionNum];

  sort(m.begin(), m.end(), convFlame);

  m_lastframe = 0;
  for(int i = 0; i < (int)m.size(); ++i){
    m_Bonemap[m[i].boneName].push_back( pair<int,int>(m[i].flameNo, i));
    m_lastframe = max((double)m_lastframe, (double)m[i].flameNo);    
  }

  //表情データ

  m_Morphs[m_motionNum] = loader->GetMorphs();
  vector<VmdMorph>& mm = m_Morphs[m_motionNum];
  sort(mm.begin(), mm.end(), convFlameMorph);  

  for(size_t i = 0; i < mm.size(); ++i){
    MorphInfo mi;
    mi.flameNo = mm[i].flameNo;
    mi.morphpos = i;
    mi.weight = mm[i].weight;
    string s = mm[i].skinName;
    m_MorphMap[s].push_back(mi);

    
    cout << i << " Name : " << s <<
      "flameNo : " << mm[i].flameNo <<
      "weight : " << mm[i].weight << endl;
  }
  cout << "vmd morph : "<< mm.size() << endl;
  
  m_motionNum++;
}


CMotionManager::~CMotionManager(){
  for(int i = 0; i < (int)m_Motions.size(); ++i){
    while(!m_Motions[i].empty())
      m_Motions[i].pop_back();
  }
  m_Bonemap.clear();
}

