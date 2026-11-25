#ifndef CAMERA_H
#define CAMERA_H

#include "math.h"

class Camera
{
  public:
    Camera(int WindowWidth, int WindowHeight);
    Camera(int WindowWidth, int WindowHeight, const Vector3f& Pos, const Vector3f& Target, const Vector3f& Up);
    void Position(float x, float y, float z);
    void KeyDown(unsigned char key);
    void KeyUp(unsigned char key);
    void Move();
    void Mouse(int x, int y);

    Matrix4f Matrix();

  private:
    void Init();
    void Update();

    Vector3f position;
    Vector3f target;
    Vector3f up;
    float speed = 0.1f;

    int windowWidth;
    int windowHeight;

    float AngleH = 0.0f;
    float AngleV = 0.0f;

    Vector2i mouse_position;

    bool Keys[256] = {false};
};

#endif /* CAMERA_H */
