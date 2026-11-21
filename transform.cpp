#include "transform.h"
#include <cmath>

void Transform::Scale(float x, float y, float z)
{
  scale = Vector3f(x, y, z);
}
void Transform::Rotation(float x, float y, float z)
{
  rotation = Vector3f(x, y, z);
}
void Transform::Position(float x, float y, float z)
{
  position = Vector3f(x, y, z);
}

void Transform::Rotate(float x, float y, float z)
{
  rotation.x += x;
  rotation.y += y;
  rotation.z += z;
}

Matrix4f Transform::Matrix()
{
  Matrix4f Scaled(
    scale.x, 0.0f, 0.0f, 0.0f,
    0.0f, scale.y, 0.0f, 0.0f,
    0.0f, 0.0f, scale.z, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
  );

  Matrix4f RotatedY(
    cosf(rotation.y), 0.0f, -sinf(rotation.y), 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    sinf(rotation.y), 0.0f, cosf(rotation.y), 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
  );

  Matrix4f RotatedZ(
    cosf(rotation.z), -sinf(rotation.z), 0.0f, 0.0f,
    sinf(rotation.z), cosf(rotation.z), 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
  );
  
  Matrix4f RotatedX(
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, cosf(rotation.x), -sinf(rotation.z), 0.0f,
    0.0f, sinf(rotation.z), cosf(rotation.z), 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
  );

  Matrix4f Rotated = RotatedZ * RotatedX * RotatedY;

  Matrix4f Positioned(
    1.0f, 0.0f, 0.0f, position.x,
    0.0f, 1.0f, 0.0f, position.y,
    0.0f, 0.0f, 1.0f, position.z,
    0.0f, 0.0f, 0.0f, 1.0f
  );

  return Positioned * Rotated * Scaled;
}
