#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include<GL/glew.h>


class Texture
{
  public:
    Texture(GLenum TextureTarget, const std::string& FileName);
    bool Load();
    void Bind(GLenum texture_unit);

  private:
    std::string file_name;
    GLenum texture_target;
    GLuint texture_obj;

};

#endif // TEXTURE_H
