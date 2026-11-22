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
  if (x == windowWidth/2 && y == windowHeight/2){
    return;
  }
  int DeltaX = x - windowWidth/2;
  int DeltaY = y - windowHeight/2;

  AngleH += (float)DeltaX / 2000.0f;
  AngleV += (float)DeltaY / 5000.0f;

  Update();

  glutWarpPointer(windowWidth / 2, windowHeight / 2);
}

void Camera::Update()
{
  Vector3f Yaxis(0.0f, 1.0f, 0.0f);

  Vector3f View(0.0f, 0.0f, 1.0f);
  View.Rotate(AngleH, Yaxis);
  View.Normalize();

  Vector3f U = Yaxis.Cross(View);
  U.Normalize();
  View.Rotate(AngleV, U);

  target = View;
  target.Normalize();

}

void Camera::Position(float x, float y, float z)
{
  position = Vector3f(x, y, z);
}

void Camera::KeyDown(unsigned char key)
{
  Keys[key] = true;
}
void Camera::KeyUp(unsigned char key)
{
  Keys[key] = false;
}

void Camera::Move()
{
  if (Keys['w']){
    Vector3f Front(target.x, 0.0f, target.z);
    Front *= speed;
    position += Front;
  }
  if (Keys['s']){
    Vector3f Front(target.x, 0.0f, target.z);
    Front *= speed;
    position -= Front;
  }
  if (Keys['a']){
    Vector3f Left = target.Cross(up);
    Left *= speed;
    position += Left;
  }
  if (Keys['d']){
    Vector3f Right = up.Cross(target);
    Right*=speed;
    position += Right;
  }
}

Matrix4f Camera::Matrix()
{
  Vector3f N = target;
  N.Normalize();
  Vector3f U = up.Cross(N);
  U.Normalize();
  Vector3f V = N.Cross(U);
  N.Normalize();

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

  return Rotation * Translation; 
}
