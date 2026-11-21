#ifndef CAMERA_H
#define CAMERA_H

#include "math.h"

class Camera
{
  public:
    Camera();

    void Position(float x, float y, float z);
    void KeyBoard(int key);

    Matrix4f Matrix();

  private:
    Vector3f position;
    Vector3f target;
    Vector3f up;
    float speed = 1.0f;
};

#endif /* CAMERA_H */
