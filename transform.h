#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "math.h"

class Transform {
  public:
    Transform() {}

    void Scale(float x, float y, float z);
    void Rotation(float x, float y, float z);
    void Position(float x, float y, float z);

    void Rotate(float x, float y, float z);

    Matrix4f Matrix();

  private:
    Vector3f scale = Vector3f(1.0f, 1.0f, 1.0f);
    Vector3f rotation = Vector3f(0.0f, 0.0f, 0.0f);
    Vector3f position = Vector3f(0.0f, 0.0f, 0.0f);
};

#endif /* TRANSFORM_H */
