#include "camera.h"
#include "freeglut_std.h"
#include <cmath>
#include <cstdio>

Camera::Camera(int WindowWidth, int WindowHeight, const Vector3f& Pos, const Vector3f& Target, const Vector3f& Up)
{
  windowWidth = WindowWidth;
  windowHeight = WindowHeight;

  position = Vector3f(0.0f, 0.0f, 0.0f);

  target = Vector3f(0.0f, 0.0f, 1.0f);
  target.Normalize();

  up = Vector3f(0.0f, 1.0f, 0.0f);
  up.Normalize();

  Init();
}

void Camera::Init()
{
  Vector3f HTarget(target.x, 0.0f, target.z);
  HTarget.Normalize();

  float Angle = asin(abs(HTarget.z));

  if (HTarget.z >= 0.0f){
    if (HTarget.x >= 0.0f){
      AngleH = 8 * atan(1) - Angle;
    }else{
      AngleH = 4 * atan(1) + Angle;
    }
  }else{
    if (HTarget.x >= 0.0f){
      AngleH = Angle;
    }else{
      AngleH = 4 * atan(1) - Angle;
    }
  }

  AngleV = -asin(target.y);

  mouse_position.x = windowWidth / 2;
  mouse_position.y = windowHeight / 2;
}

void Camera::Mouse(int x, int y)
{
  int DeltaX = x - mouse_position.x;
  int DeltaY = y - mouse_position.y;

  mouse_position.x = x;
  mouse_position.y = y;

  AngleH += (float)DeltaX / 2000.0f;
  AngleV += (float)DeltaY / 5000.0f;

  Update();
}

void Camera::Update()
{
  Vector3f Yaxis(0.0f, 1.0f, 0.0f);

  Vector3f View(1.0f, 0.0f, 0.0f);
  View.Rotate(AngleH, Yaxis);
  View.Normalize();

  Vector3f U = Yaxis.Cross(View);
  U.Normalize();
  View.Rotate(AngleV, U);

  target = View;
  target.Normalize();

  //up = target.Cross(U);
  //up.Normalize();
}

void Camera::Position(float x, float y, float z)
{
  position = Vector3f(x, y, z);
}

void Camera::KeyBoard(int key)
{
  switch (key) {
    case GLUT_KEY_UP:
    {
      Vector3f Front(0.0f, 0.0f, speed);
      position += Front;
      break;
    }
    case GLUT_KEY_DOWN:
    {
      Vector3f Back(0.0f, 0.0f, -speed);
      position += Back;
      break;
    }
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

  Matrix4f Translation(
    1.0f, 0.0f, 0.0f, -position.x,
    0.0f, 1.0f, 0.0f, -position.y,
    0.0f, 0.0f, 1.0f, -position.z,
    0.0f, 0.0f, 0.0f, 1.0f
  );

  Matrix4f Rotation(
    U.x, U.y, U.z, 0.0f,
    V.x, V.y, V.z, 0.0f,
    N.x, N.y, N.z, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
  );

  return Translation * Rotation; 
}
