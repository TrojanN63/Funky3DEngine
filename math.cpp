#include "math.h"

Quaternion operator* (const Quaternion& q, const Vector3f& v)
{
  float w = -(q.x * v.x) - (q.y * v.y) - (q.z * v.z);
  float x = (q.w * v.x) + (q.y * v.z) - (q.z * v.y);
  float y = (q.w * v.y) + (q.z * v.x) - (q.x * v.z);
  float z = (q.w * v.z) + (q.x * v.y) - (q.y * v.x);

  Quaternion ret(x, y, z, w);

  return ret;
}

Quaternion operator* (const Quaternion& l, const Quaternion& r)
{
  float w = (l.w * r.w) - (l.x * r.x) - (l.y * r.y) - (l.z * r.z);
  float x = (l.x * r.w) + (l.w * r.x) + (l.y * r.z) - (l.z * r.y);
  float y = (l.y * r.w) + (l.w * r.y) + (l.z * r.x) - (l.x * r.z);
  float z = (l.z * r.w) + (l.w * r.z) + (l.x * r.y) - (l.y * r.x);

  Quaternion ret(x, y, z, w);

  return ret;
}

void Vector3f::Rotate(float Angle, const Vector3f& V)
{
  Quaternion RotationQ(Angle, V);

  Quaternion ConjugateQ = RotationQ.Conjugate();

  Quaternion W = RotationQ * (*this) * ConjugateQ;

  x = W.x;
  y = W.y;
  z = W.z;
}
Vector3f& Vector3f::Normalize()
{
  float len = (x*x + y*y + z*z);

  assert(len != 0);

  x /= len;
  y /= len;
  z /= len;

  return *this;
}


