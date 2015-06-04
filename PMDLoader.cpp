#include "PMDLoader.h"
#include <fstream>
#include <string.h>
#include <cstdlib>
#include <iostream>

using namespace std;

CPMDLoader::CPMDLoader(const char* fname)
{
  m_pVertex = NULL;
  m_pMaterial = NULL;
  m_pFaceIndex = NULL;
  m_vertexNum = 0;

  ifstream ifs;
  ifs.open(fname, ifstream::binary);

  if(ifs.fail()){
    printf("can not open file\n");
    throw "error";
  }

  ifs.seekg(0, ifstream::beg);
  char magic[4];
  ifs.read(magic,sizeof(char)*3);

  magic[3] = '\0';
  printf("%s\n",magic);
  printf("seekpos = %d\n",(int)ifs.tellg());
  float version;

  ifs.read((char*)&version,sizeof(float));
  printf("version = %f\n",version);
  printf("seekpos = %d\n",(int)ifs.tellg());

  struct f{
    static void zenkakucounter(char* s){
      for(int i = 0; s[i] != '\0'; ++i){
	unsigned char c = s[i];
	 if ( (c>=0x81 && c<=0x9f) 
	      || (c>=0xe0 && c<=0xfc)){    // 全角の第１バイト
	   //zennkaku
	   printf("N");//zennkakumojidatta
	   i++;
	 }else{
	   printf("%c",c);
	 }
      }
      printf("\n");
    }
  };

  char mname[21];
  ifs.read(mname,sizeof(char)*20);
  mname[20] = '\0';
  f::zenkakucounter(mname);
  printf("seekpos = %d\n",(int)ifs.tellg());

  char comment[257];
  ifs.read(comment, sizeof(char)*256);
  comment[256] = '\0';
  f::zenkakucounter(comment);
  printf("seekpos = %d\n",(int)ifs.tellg());

  DWORD vsize;
  ifs.read((char*)&vsize, sizeof(DWORD));
  int tmp = vsize;
  printf("vsize = %d\n",tmp);
  printf("seekpos = %d\n",(int)ifs.tellg());

  m_vertexNum = vsize;

  m_pVertex = new MmdStruct::PmdVertex[vsize];

  for(DWORD i = 0; i < m_vertexNum; ++i){
    ifs.read((char*)&m_pVertex[i].pos,sizeof(float)*3);
    ifs.read((char*)&m_pVertex[i].normal_vec,sizeof(float)*3);
    ifs.read((char*)&m_pVertex[i].uv,sizeof(float)*2);
    ifs.read((char*)&m_pVertex[i].bone_num,sizeof(unsigned short)*2);
    ifs.read((char*)&m_pVertex[i].bone_weight,sizeof(unsigned char));
    ifs.read((char*)&m_pVertex[i].edge_flag,sizeof(unsigned char));
    //openglは右手座標系
    m_pVertex[i].pos[2] *= -1;
    m_pVertex[i].pos[0] *= -1;
    m_pVertex[i].normal_vec[0] *= -1;
    m_pVertex[i].normal_vec[2] *= -1;
    //    m_pVertex[i].normal_vec[2] *= -1;
  }

  printf("vertex\n");
  printf("seekpos = %d\n",(int)ifs.tellg());
  //face syutoku
  ifs.read((char*)&m_faceNum, sizeof(DWORD));

  printf("facenum %d\n",m_faceNum);
  printf("seekpos = %d\n",(int)ifs.tellg());

  m_pFaceIndex = new WORD[m_faceNum];
  ifs.read((char*)m_pFaceIndex, sizeof(WORD)*m_faceNum);

  printf("idx\n");
  printf("seekpos = %d\n",(int)ifs.tellg());

  ifs.read((char*)&m_materialNum, sizeof(DWORD));
  tmp = m_materialNum;
  printf("material  = %d\n",tmp);
  printf("seekpos = %d\n",(int)ifs.tellg());

  m_pMaterial = new MmdStruct::PmdMaterial[m_materialNum];
  //  ifs.read((char*)m_pMaterial, sizeof(MmdStruct::PmdMaterial));
  
  for(DWORD i = 0; i < m_materialNum; ++i){
    MmdStruct::PmdMaterial *m = &m_pMaterial[i];
    ifs.read((char*)&m->diffuse_color[0], sizeof(float)*3);
    ifs.read((char*)&m->alpha, sizeof(float));
    ifs.read((char*)&m->specularity, sizeof(float));
    ifs.read((char*)&m->specular_color[0], sizeof(float)*3);
    ifs.read((char*)&m->mirror_color[0],sizeof(float)*3);
    ifs.read((char*)&m->toon_index,sizeof(BYTE));
    ifs.read((char*)&m->edge_flag,sizeof(BYTE));
    ifs.read((char*)&m->face_vert_count,sizeof(DWORD));
    ifs.read((char*)&m->texture_file_name, sizeof(char)*20);
}

  ifs.read((char*)&m_boneNum, sizeof(WORD));

  cout << "bone koko" << m_boneNum << " " << endl ;
  m_pBone = new MmdStruct::PmdBone[m_boneNum];
  
  for(WORD i = 0; i < m_boneNum; ++i){
    MmdStruct::PmdBone* m = &m_pBone[i];
    ifs.read((char*)&m->bone_name, 20);
    ifs.read((char*)&m->parent_bone_index, sizeof(WORD));
    ifs.read((char*)&m->tail_pos_bone_index, sizeof(WORD));
    ifs.read((char*)&m->bone_type, 1);
    ifs.read((char*)&m->ik_parent_bone_index, sizeof(WORD));
    ifs.read((char*)&m->bone_head_pos[0], sizeof(float)*3);
    //openglは右手座標系
    m->bone_head_pos[0] *= -1.0;
    m->bone_head_pos[2] *= -1.0;
    
  }

  ifs.read((char*)&m_ikNum, sizeof(WORD));
  cout << "ikNum" << m_ikNum << endl;
  m_pIK = new MmdStruct::PmdIK[m_ikNum];

  for(WORD i = 0; i < m_ikNum; ++i){
    MmdStruct::PmdIK* m = &m_pIK[i];
    ifs.read((char*)&m->ik_bone_index, sizeof(WORD));
    ifs.read((char*)&m->ik_target_bone_index, sizeof(WORD));
    ifs.read((char*)&m->ik_chain_length, sizeof(BYTE));
    ifs.read((char*)&m->iterations, sizeof(WORD));
    ifs.read((char*)&m->control_weight, sizeof(float));
    //new siteruyo

    m->ik_child_bone_index = new WORD[m->ik_chain_length];
    for(int j = 0; j < m->ik_chain_length; ++j){
      ifs.read((char*)&m->ik_child_bone_index[j], sizeof(WORD));
    }
  }

  //表情リスト
  ifs.read((char*)&m_skinNum, sizeof(WORD));
  m_pMorph = new MmdStruct::PmdMorph[m_skinNum];
  
  for(int i = 0; i < m_skinNum; ++i){
    MmdStruct::PmdMorph *m = &m_pMorph[i];
    ifs.read(m->skin_name, 20);
    cout << m->skin_name << endl;

    ifs.read((char*)&m->skin_vert_count, sizeof(DWORD));
    ifs.read((char*)&m->skin_type, sizeof(BYTE));

    m->skin_data = new MmdStruct::PmdSkinVertData[m->skin_vert_count];
    for(WORD j = 0; j < m->skin_vert_count; ++j){
      MmdStruct::PmdSkinVertData *mm = &m->skin_data[j];
      ifs.read((char*)&mm->skin_vert_index,sizeof(DWORD));
      ifs.read((char*)&mm->skin_vert_pos[0],sizeof(float)*3);
      //openglは右
      mm->skin_vert_pos[0] *= -1.0;
      mm->skin_vert_pos[2] *= -1.0;
    }
  }
  
  ifs.close();
}

CPMDLoader::~CPMDLoader(){
  SAFE_DELETE_ARRAY(m_pVertex);
  SAFE_DELETE_ARRAY(m_pFaceIndex);
  SAFE_DELETE_ARRAY(m_pMaterial);
  SAFE_DELETE_ARRAY(m_pBone);
  for(WORD i = 0; i < m_ikNum; ++i){
    SAFE_DELETE_ARRAY(m_pIK[i].ik_child_bone_index);
  }
  SAFE_DELETE_ARRAY(m_pIK);
  for(WORD i = 0; i < m_skinNum; ++i){
    SAFE_DELETE_ARRAY(m_pMorph[i].skin_data);
  }
  SAFE_DELETE_ARRAY(m_pMorph);
}

