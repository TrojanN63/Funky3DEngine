#include "projection.h"
#include <cmath>

Projection::Projection(int _w, int _h)
{
  width = _w;
  height = _h;
  FOV = 0.5f;
  nZ = 1.0f;
  fZ = 100.0f;
}

void Projection::Window(int _w, int _h)
{
  width = _w;
  height = _h;
}

void Projection::SetFOV(float _f)
{
  FOV = _f;
}

void Projection::Z(float _nz, float _fz)
{
  nZ = _nz;
  fZ = _fz;
}

Matrix4f Projection::Matrix(){
  float tanHalfFOV = tanf(FOV/2);
  float f = 1/tanHalfFOV;
  float ratio = (float)width/(float)height;

  float Zr = nZ - fZ;

  float A = (-fZ - nZ) / Zr;
  float B = 2.0f * fZ * nZ / Zr;

  Matrix4f FinalProj(
    f/ratio, 0.0f, 0.0f, 0.0f,
    0.0f, f, 0.0f, 0.0f,
    0.0f, 0.0f, A, B,
    0.0f, 0.0f, 1.0f, 0.0f
  );

  return FinalProj;
}
