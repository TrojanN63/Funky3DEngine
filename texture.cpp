#include "texture.h"
#include "stb_image.h"

Texture::Texture(GLenum TextureTarget, const std::string& FileName)
{
  texture_target = TextureTarget;
  file_name = FileName;
}

bool Texture::Load()
{
  stbi_set_flip_vertically_on_load(1);
  int width = 0, height = 0, bpp = 0;
  unsigned char* image_data = stbi_load(file_name.c_str(), &width, &height, &bpp, 0);

  if (!image_data){
    printf("Não foi possível carregar textura de '%s' - %s\n", file_name.c_str(), stbi_failure_reason());
    exit(0);
  }
  printf("Largura %d, altura %d, bpp %d\n", width, height, bpp);
       
  glGenTextures(1, &texture_obj);
  glBindTexture(texture_target, texture_obj);
  if (texture_target == GL_TEXTURE_2D){
    glTexImage2D(texture_target, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
  }else{
    printf("Suporte para alvo de textura %x não implementado\n", texture_target);
  }

  glTexParameterf(texture_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(texture_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(texture_target, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameterf(texture_target, GL_TEXTURE_WRAP_T, GL_CLAMP);

  glBindTexture(texture_target, 0);

  return true;
}

void Texture::Bind(GLenum TextureUnit)
{
  glActiveTexture(TextureUnit);
  glBindTexture(texture_target, texture_obj);
}
