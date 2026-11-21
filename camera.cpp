#include "camera.h"
#include "freeglut_std.h"

Camera::Camera()
{
  position = Vector3f(0.0f, 0.0f, 0.0f);
  target = Vector3f(0.0f, 0.0f, 1.0f);
  up = Vector3f(0.0f, 1.0f, 0.0f);
}

void Camera::Position(float x, float y, float z)
{
  position = Vector3f(x, y, z);
}

void Camera::KeyBoard(int key)
{
  switch (key) {
    case GLUT_KEY_UP:
      target.x, target.y, target.z *= speed;
      position += target;
      break;
    case GLUT_KEY_DOWN:
      target.x, target.y, target.z *= speed;
      position -= target;
      break;
    case GLUT_KEY_LEFT:
    {
      Vector3f Left = target.Cross(up);
      Left*=speed;
      position += Left;
      break;
    }
    case GLUT_KEY_RIGHT:
    {
      Vector3f Right = up.Cross(target);
      Right*=speed;
      position += Right;
      break;
    }
  }
}

Matrix4f Camera::Matrix()
{
  Vector3f N = target;
  Vector3f U = up.Cross(N);
  Vector3f V = N.Cross(U);

  Matrix4f Transformation(
    U.x, U.y, U.z, -position.x,
    V.x, V.y, V.z, -position.y,
    N.x, N.y, N.z, -position.z,
    0.0f, 0.0f, 0.0f, 1.0f
  );

  return Transformation;
}
