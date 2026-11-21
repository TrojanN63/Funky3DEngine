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

using namespace std;

GLuint VBO;
GLuint IBO;

GLint gWVPLocation;

const char* pVSFileName = "../shader.vs";
const char* pFSFileName = "../shader.fs";

int width = 1280;
int height = 800;

Transform Cube;
Camera Player;
Projection Proj(width, height);

static void RenderSceneCB()
{
  glClear(GL_COLOR_BUFFER_BIT);

  Cube.Scale(1.0f, 1.0f, 1.0f);
  Cube.Position(0.0f, 0.0f, 0.0f);
  Cube.Rotate(0.0f, 0.01f, 0.0f);
  
  Matrix4f Transformation = Cube.Matrix();

  Matrix4f CameraTransform = Player.Matrix();

  Matrix4f Screen = Proj.Matrix();

  Matrix4f WVP = Screen * CameraTransform * Transformation;

  glUniformMatrix4fv(gWVPLocation, 1, GL_TRUE, &WVP.m[0][0]);
  
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float),0);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
  
  glDrawElements(GL_TRIANGLES,36,GL_UNSIGNED_INT,0);

  glDisableVertexAttribArray(0);

  glutPostRedisplay();

  glutSwapBuffers();
}

struct Vertex {
  Vector3f pos;
  Vector3f color;

  Vertex() {}

  Vertex(float x, float y, float z)
  {
    pos = Vector3f(x, y, z);
    float red   = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    float green = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    float blue  = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    color = Vector3f(red, green, blue);
  }
};

static void CreateVertexBuffer()
{
  Vertex Vertices[8];

  Vertices[0] = Vertex(0.5f, 0.5f, 0.5f);
  Vertices[1] = Vertex(-0.5f, 0.5f, -0.5f);
  Vertices[2] = Vertex(-0.5f, 0.5f, 0.5f);
  Vertices[3] = Vertex(0.5f, -0.5f, -0.5f);
  Vertices[4] = Vertex(-0.5f, -0.5f, -0.5f);
  Vertices[5] = Vertex(0.5f, 0.5f, -0.5f);
  Vertices[6] = Vertex(0.5f, -0.5f, 0.5f);
  Vertices[7] = Vertex(-0.5f, -0.5f, 0.5f);

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

static void KeyboardCB(unsigned char key, int mouse_x, int mouse_y)
{
  Player.KeyBoard(key);
}
static void SpecialKeyboardCB(int key, int mouse_x, int mouse_y)
{
  Player.KeyBoard(key);
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

  CreateVertexBuffer();
  CreateIndexBuffer();

  CompileShaders();

  glutDisplayFunc(RenderSceneCB);
  glutKeyboardFunc(KeyboardCB);
  glutSpecialFunc(SpecialKeyboardCB);

  glutMainLoop();

  return 0;
}
