#pragma once
#include "vector3.h"
#include "constant.h"

class CInput{
 private:
  CInput(){
    middle = false;
    left = false;
    updirection.y = 1.0;
    eyeposition = CVector3(0,10,100);
    downeyeposition = eyeposition;
    downupdirection = updirection;
  }
  ~CInput(){}

 public:
  static CInput& instance(){
    static CInput inst;
    return inst;
  }

  void update(){
    CVector3 localx = (eyeposition - centerposition).det(updirection).normal();
    CVector3 localy = updirection;

    if(middle){
      //平行移動

      eyeposition = downeyeposition 
	+ (nowmousepos - downmousepos).x * 0.1 * localx
	+ (nowmousepos - downmousepos).y * 0.1 * localy;

      centerposition = downcenterposition 
	+ (nowmousepos - downmousepos).x * 0.1 * localx
	+ (nowmousepos - downmousepos).y * 0.1 * localy;
    }

    if(left){
      //現在の注視点を中心として回転
      //軸
      //アングル
      CVector3 axis = (nowmousepos - downmousepos).det(downcenterposition - downeyeposition).normal();
      double angle = (nowmousepos - downmousepos).length() * PI/180.0;
      CQuaternion q = makeFromAxis(angle, axis);

      eyeposition = QuaternionToMatrix(q) * downeyeposition;
      updirection = QuaternionToMatrix(q) * downupdirection;
    }
    
    glLoadIdentity();
    gluLookAt(
	      eyeposition.x, eyeposition.y, eyeposition.z,
	      centerposition.x,centerposition.y,centerposition.z,
	      updirection.x, updirection.y, updirection.z
	      );
  }

  void mousedown(int x, int y){
    downmousepos.x = x;
    downmousepos.y = y;
    nowmousepos = downmousepos;
    if(middle){
      //平行移動処理
      downeyeposition = eyeposition;
      downcenterposition = centerposition;
    }
    else if(left){
      downupdirection = updirection;
    }
  }

  void mouseup(){
    if(middle){
      //平行移動
      downmousepos = eyeposition;
      downcenterposition = centerposition;

      nowmousepos.x = downmousepos.x;
      nowmousepos.y = downmousepos.y;
      prevmousepos = nowmousepos;
    }
    else if(left){
      downupdirection = updirection;
      downeyeposition = eyeposition;
    }
  }

  static CVector3 nowmousepos;
  static CVector3 prevmousepos;
  static CVector3 downmousepos;

  static bool middle;
  static bool left;

  //camera
  static CVector3 eyeposition;//camera no pos
  static CVector3 downeyeposition;

  static CVector3 centerposition;//camera no tyuusi ten
  static CVector3 downcenterposition;//camera no tyuusi ten


  static CVector3 updirection;//camera no ue
  static CVector3 downupdirection;//camera no ue
};
