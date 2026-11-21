#ifndef PROJECTION_H
#define PROJECTION_H

#include "math.h"

class Projection
{
  public:
    Projection(int _w, int _h);

    void Window(int _w, int _h);
    void SetFOV(float _f);
    void Z(float _nz, float _fz);

    Matrix4f Matrix();
  private:
    int width;
    int height;
    float nZ;
    float fZ;
    float FOV;
};

#endif /* PROJECTION_H */
