#include "vector3.h"


  //a * v
CVector3 operator * (const float a, const CVector3& v){
    return CVector3(a * v.x, a * v.y, a * v.z);
}

//mat4
CMatrix4 Mat4Translated(double x, double y, double z){
  CMatrix4 t;
  t.m[12] = x;
  t.m[13] = y;
  t.m[14] = z;
  return t;
}
//mat4
//0, 4, 8, 12
//1, 5, 9, 13
//2, 6, 10, 14
//3, 7, 11, 15

CMatrix4 Mat4RotatedY(double angle){
  double pp = PI/180.0;
  CMatrix4 t;
  t.m[0] = cos(pp * angle);
  t.m[2] = -sin(pp * angle);
  t.m[5] = 1.0;
  t.m[8] = sin(pp * angle);
  t.m[10] = cos(pp * angle);
  t.m[15] = 1.0;
  return t;
}
CMatrix4 Mat4RotatedZ(double angle){
  double pp = PI/180.0;
  CMatrix4 t;
  t.m[0] = cos(angle * pp);
  t.m[1] = sin(angle * pp);
  t.m[4] = -sin(angle * pp);
  t.m[5] = cos(angle * pp);
  t.m[10] = 1.0;
  t.m[15] = 1.0;
  return t;
}
CMatrix4 Mat4RotatedX(double angle){
  double pp = PI/180.0;
  CMatrix4 t;
  t.m[0] = 1.0;
  t.m[5] = cos(angle*pp);
  t.m[6] = sin(angle*pp);
  t.m[9] = -sin(angle*pp);
  t.m[10] = cos(angle*pp);
  t.m[15] = 1.0;
  return t;
}

//axis haseikikasareteiru hituyougaarumasu
CQuaternion makeFromAxis(const double rad, const CVector3& axis)
{
  CQuaternion Q(cos(rad/2), axis.x * sin(rad/2),
		axis.y * sin(rad/2),axis.z * sin(rad/2));
  return Q;
}

CQuaternion slerp(const CQuaternion &a, const CQuaternion &b, double t){
  CQuaternion Q;
  double qr = a.w*b.w + a.x*b.x + a.y*b.y + a.z*b.z;
  double ss  = 1.0 - qr*qr, sp;
  
  if(ss <= 0.0 || (sp = sqrt(ss)) == 0.0){
    Q.w = a.w;
    Q.x = a.x;
    Q.y = a.y;
    Q.z = a.z;
  }
  else {
    double ph = acos(qr);
    double pt = ph * t;
    double t1 = sin(pt)/sp;
    double t0 = sin(ph - pt)/ sp;
    
    Q.w = a.w * t0 + b.w * t1;
    Q.x = a.x * t0 + b.x * t1;
    Q.y = a.y * t0 + b.y * t1;
    Q.z = a.z * t0 + b.z * t1;
  }
  return Q;
}

CMatrix4 QuaternionToMatrix(const CQuaternion& q){
  CMatrix4 mat;
  mat.m[0] = 1 - 2*(q.y * q.y + q.z * q.z);
  mat.m[1] = 2 * (q.x * q.y + q.z * q.w);
  mat.m[2] = 2 * (q.z * q.x - q.y * q.w);
  mat.m[3] = 0.0;
  mat.m[4] = 2 * (q.x * q.y - q.z * q.w);
  mat.m[5] = 1 - 2*(q.z * q.z + q.x * q.x);
  mat.m[6] = 2 * (q.y * q.z + q.x * q.w);
  mat.m[7] = 0.0;
  mat.m[8] = 2 * (q.z * q.x + q.y * q.w);
  mat.m[9] = 2 * (q.y * q.z - q.x * q.w);
  mat.m[10] = 1 - 2*(q.x * q.x + q.y * q.y);
  mat.m[11] = 0.0;
  mat.m[12] = 0.0;
  mat.m[13] = 0.0;
  mat.m[14] = 0.0;
  mat.m[15] = 1.0;
  return mat;
}
