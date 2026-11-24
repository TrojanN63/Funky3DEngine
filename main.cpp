#include <stdio.h>
#include <string.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <vector>
#include <fstream>
#include <cmath>
#include "transform.h"
#include "camera.h"
#include "projection.h"
#include "texture.h"

using namespace std;

GLuint VBO;
GLuint IBO;

GLuint gWVPLocation;
GLuint gSamplerLocation;
Texture* texture = NULL;

const char* pVSFileName = "../shader.vs";
const char* pFSFileName = "../shader.fs";

int width = 1920;
int height = 1200;

Transform Cube;
Vector3f CameraPos(0.0f, 0.0f, 0.0f);
Vector3f CameraTarget(0.0f, 0.0f, 1.0f);
Vector3f CameraUp(0.0f, 1.0f, 0.0f);
Camera Player(width, height, CameraPos, CameraTarget, CameraUp);
Projection Proj(width, height);

struct Vertex {
  Vector3f pos;
  Vector2f tex;

  Vertex() {}

  Vertex(const Vector3f& pos_, const Vector2f& tex_)
  {
    pos = pos_;
    tex = tex_;
  }
};

static void Scene()
{
  glClear(GL_COLOR_BUFFER_BIT);

  Cube.Scale(0.5f, 0.5f, 0.5f);
  Cube.Position(0.0f, 0.0f, 5.0f);
  Cube.Rotate(0.0f, 0.0f, 0.0f);
  
  Matrix4f Transformation = Cube.Matrix();

  Matrix4f CameraTransform = Player.Matrix();
  Player.Move();

  Matrix4f Screen = Proj.Matrix();

  Matrix4f WVP = Screen * CameraTransform * Transformation;

  glUniformMatrix4fv(gWVPLocation, 1, GL_TRUE, &WVP.m[0][0]);
  
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

  texture->Bind(GL_TEXTURE0);
  glUniform1i(gSamplerLocation, 0);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),0);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
  
  glDrawElements(GL_TRIANGLES,36,GL_UNSIGNED_INT,0);

  glDisableVertexAttribArray(0);

  glutPostRedisplay();

  glutSwapBuffers();
}

static void CreateVertexBuffer()
{
  Vertex Vertices[8];

  Vector2f t00 = Vector2f(0.0f, 0.0f);
  Vector2f t01 = Vector2f(0.0f, 1.0f);
  Vector2f t10 = Vector2f(1.0f, 0.0f);
  Vector2f t11 = Vector2f(1.0f, 1.0f);

  Vertices[0] = Vertex(Vector3f(0.5f, 0.5f, 0.5f), t00);
  Vertices[1] = Vertex(Vector3f(-0.5f, 0.5f, -0.5f), t01);
  Vertices[2] = Vertex(Vector3f(-0.5f, 0.5f, 0.5f), t10);
  Vertices[3] = Vertex(Vector3f(0.5f, -0.5f, -0.5f), t11);
  Vertices[4] = Vertex(Vector3f(-0.5f, -0.5f, -0.5f), t00);
  Vertices[5] = Vertex(Vector3f(0.5f, 0.5f, -0.5f), t10);
  Vertices[6] = Vertex(Vector3f(0.5f, -0.5f, 0.5f), t01);
  Vertices[7] = Vertex(Vector3f(-0.5f, -0.5f, 0.5f), t11);

  glGenBuffers(1,&VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
}

static void CreateIndexBuffer()
{
  unsigned int Indices[] = {
    0, 1, 2,
    1, 3, 4,
    5, 6, 3,
    7, 3, 6,
    2, 4, 7,
    0, 7, 6,
    0, 5, 1,
    1, 5, 3,
    5, 0, 6,
    7, 4, 3,
    2, 1, 4,
    0, 2, 7
  };

  glGenBuffers(1, &IBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
}

bool ReadFile(const char* pFIleName, string& outFile)
{
  ifstream f(pFIleName);

  bool ret = false;

  if (f.is_open()) {
    string line;
    while (getline(f, line)) {
      outFile.append(line);
      outFile.append("\n");
    }
    f.close();
    ret = true;
  }else{
    fprintf(stderr, "Erro na leitura de arquivo de shader\n");
    exit(1);
  }

  return ret;
}

static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
  GLuint ShaderObj = glCreateShader(ShaderType);

  if (ShaderObj==0){
    fprintf(stderr, "Erro criando tipo do shader %d\n", ShaderType);
    exit(0);
  }

  const GLchar* p[1];
  p[0] = pShaderText;

  GLint Lengths[1];
  Lengths[0] = (GLint)strlen(pShaderText);

  glShaderSource(ShaderObj, 1, p, Lengths);
  glCompileShader(ShaderObj);

  GLint success;
  glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);

  if (!success){
    GLchar InfoLog[1024];
    glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
    fprintf(stderr, "Erro ao compilar tipo de shader %d: '%s'\n", ShaderType, InfoLog);
    exit(1);
  }

  glAttachShader(ShaderProgram, ShaderObj);
}

static void CompileShaders()
{
  GLuint ShaderProgram = glCreateProgram();

  if (ShaderProgram == 0){
    fprintf(stderr, "Erro ao criar programa do shader\n");
    exit(1);
  }

  string vertS, fragS;

  if (!ReadFile(pVSFileName, vertS)) {
    exit(1);
  }

  AddShader(ShaderProgram, vertS.c_str(), GL_VERTEX_SHADER);
  
  if (!ReadFile(pFSFileName, fragS)) {
    exit(1);
  }

  AddShader(ShaderProgram, fragS.c_str(), GL_FRAGMENT_SHADER);

  GLint Success = 0;
  GLchar ErrorLog[1024] = { 0 };

  glLinkProgram(ShaderProgram);

  glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);

  if (Success == 0){
    glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
    fprintf(stderr, "Erro linkando programa do shader: '%s'\n", ErrorLog);
  }

  glValidateProgram(ShaderProgram);
  glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
  if (!Success){
    glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
    fprintf(stderr, "Programa de shader inválido: '%s'\n", ErrorLog);
    exit(1);
  }

  glUseProgram(ShaderProgram);
}

static void KeyboardDown(unsigned char key, int mouse_x, int mouse_y)
{
  Player.KeyDown(key);
}
static void SpecialKeyboardDown(int key, int mouse_x, int mouse_y)
{
  Player.KeyDown(key);
}

static void KeyboardUp(unsigned char key, int mouse_x, int mouse_y)
{
  Player.KeyUp(key);
}
static void SpecialKeyboardUp(int key, int mouse_x, int mouse_y)
{
  Player.KeyUp(key);
}

static void PassiveMouse(int x, int y)
{
  Player.Mouse(x, y);
}

static void InitializeGlutCallbacks()
{
  glutDisplayFunc(Scene);

  glutKeyboardFunc(KeyboardDown);
  glutKeyboardUpFunc(KeyboardUp);

  glutSpecialFunc(SpecialKeyboardDown);
  glutSpecialUpFunc(SpecialKeyboardUp);

  glutPassiveMotionFunc(PassiveMouse);
}

int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);

  glutInitWindowSize(width, height);

  int x = 1000;
  int y = 600;
  glutInitWindowPosition(x, y);
  int win = glutCreateWindow("Test");

  InitializeGlutCallbacks();
  glutIgnoreKeyRepeat(1);
  glutSetCursor(GLUT_CURSOR_NONE);

  GLenum res = glewInit();
  if (res != GLEW_OK){
    fprintf(stderr, "ERROR: '%s'\n", glewGetErrorString(res));
    return -1;
  }

  GLclampf Red = 0.1f, Green = 0.1f, Blue = 0.1f, Alpha = 1.0f;
  glClearColor(Red, Green, Blue, Alpha);

  glEnable(GL_CULL_FACE);
  glFrontFace(GL_CW);
  glCullFace(GL_BACK);

  int texture_units = 0;
  glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &texture_units);
  printf("Texture units: %d\n", texture_units);

  CreateVertexBuffer();
  CreateIndexBuffer();

  CompileShaders();

  texture = new Texture(GL_TEXTURE_2D, "../craft.png");
  if (!texture->Load()){
    return 1;
  }

  glutMainLoop();

  return 0;
}
