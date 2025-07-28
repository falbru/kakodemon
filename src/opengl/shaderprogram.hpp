#ifndef SHADERPROGRAM_HPP_INCLUDED
#define SHADERPROGRAM_HPP_INCLUDED

#include <glm/mat4x4.hpp>
#include <string>

namespace opengl
{

class ShaderProgram
{
  public:
    ShaderProgram();

    bool compile();
    void use();

    void setInt(const std::string &name, int value, bool useShader = false);
    void setVector4f(const std::string &name, float x, float y, float z, float w, bool useShader = false);
    void setMatrix4(const std::string &name, const glm::mat4 &mat, bool useShader = false);

  private:
    unsigned int m_shader_program;
};

} // namespace opengl

#endif
