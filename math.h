#ifndef MATH_H
#define MATH_H
#include <cmath>
#include <cassert>

struct Vector2i
{
  int x;
  int y;

  Vector2i() {}
  Vector2i(int _x, int _y)
  {
    x = _x;
    y = _y;
  }
};

struct Vector3f
{
  float x;
  float y;
  float z;

  Vector3f() {}
  Vector3f(float _x, float _y, float _z)
  {
    x = _x;
    y = _y;
    z = _z;
  }
  Vector3f(const float* pFloat)
  {
    x,y,z = pFloat[0];
  }
  Vector3f(float f)
  {
    x,y,z = f;
  }
  Vector3f Cross(const Vector3f& v) const
  {
    const float _x = y * v.z - z * v.y;
    const float _y = z * v.x - x * v.z;
    const float _z = x * v.y - y * v.x;

    return Vector3f(_x, _y, _z);
  }
  Vector3f& operator*=(float f)
  {
    x *= f;
    y *= f;
    z *= f;

    return *this;
  }
  Vector3f& operator+=(const Vector3f& r)
  {
    x += r.x;
    y += r.y;
    z += r.z;

    return *this;
  }
  Vector3f& operator-=(const Vector3f& r)
  {
    x -= r.x;
    y -= r.y;
    z -= r.z;

    return *this;
  }
  Vector3f& Normalize();
  void Rotate(float Angle, const Vector3f& V);
};

struct Quaternion
{
  float x, y, z, w;

  Quaternion(float Angle, const Vector3f& V)
  {
    float SinHalfAngle = sinf(Angle/2);
    float CosHalfAngle = cosf(Angle/2);

    x = V.x * SinHalfAngle;
    y = V.y * SinHalfAngle;
    z = V.z * SinHalfAngle;
    w = CosHalfAngle;
  }

  Quaternion(float _x, float _y, float _z, float _w)
  {
    x = _x;
    y = _y;
    z = _z;
    w = _w;
  }

  void Normalize()
  {
    float Length = sqrtf(x * x + y * y + z * z + w * w);

    x/=Length;
    y/=Length;
    z/=Length;
    w/=Length;
  }

  Quaternion Conjugate() const
  {
    Quaternion ret(-x, -y, -z, w);
    return ret;
  }
};

class Matrix4f
{
  public:
    float m[4][4];

    Matrix4f() {}

    Matrix4f(
      float a00, float a01, float a02, float a03,
      float a10, float a11, float a12, float a13,
      float a20, float a21, float a22, float a23,
      float a30, float a31, float a32, float a33
    ){
      m[0][0] = a00; m[0][1] = a01; m[0][2] = a02; m[0][3] = a03;
      m[1][0] = a10; m[1][1] = a11; m[1][2] = a12; m[1][3] = a13;
      m[2][0] = a20; m[2][1] = a21; m[2][2] = a22; m[2][3] = a23;
      m[3][0] = a30; m[3][1] = a31; m[3][2] = a32; m[3][3] = a33;
    }
    
    inline Matrix4f operator*(const Matrix4f& Right) const
    {
      Matrix4f Ret;

      for (unsigned int i = 0 ; i < 4 ; i++) {
        for (unsigned int j = 0 ; j < 4 ; j++) {
          Ret.m[i][j] = m[i][0] * Right.m[0][j] +
            m[i][1] * Right.m[1][j] +
            m[i][2] * Right.m[2][j] +
            m[i][3] * Right.m[3][j];
          }
        }
      return Ret;
    }

};

struct Vector2f
{
  float x;
  float y;

  Vector2f() {}
  Vector2f(float _x, float _y)
  {
    x = _x;
    y = _y;
  }
  Vector2f(const float* pFloat)
  {
    x,y = pFloat[0];
  }
  Vector2f(float f)
  {
    x,y = f;
  }
  Vector2f& operator*=(float f)
  {
    x *= f;
    y *= f;

    return *this;
  }
  Vector2f& operator+=(const Vector2f& r)
  {
    x += r.x;
    y += r.y;

    return *this;
  }
  Vector2f& operator-=(const Vector2f& r)
  {
    x -= r.x;
    y -= r.y;

    return *this;
  }
};

#endif /* MATH_H */
