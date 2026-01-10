#ifndef SHADERPROGRAM_HPP_INCLUDED
#define SHADERPROGRAM_HPP_INCLUDED

#include <glm/mat4x4.hpp>
#include <string>

namespace opengl
{

enum class RenderType
{
    Text,
    ColoredText,
    Rectangle,
    Shadow,
    RoundedRectangle
};

class ShaderProgram
{
  public:
    ShaderProgram();

    void compile();
    void use();

    void setRenderType(RenderType type);
    void setInt(const std::string &name, int value);
    void setFloat(const std::string &name, float value);
    void setVector4f(const std::string &name, float x, float y, float z, float w);
    void setMatrix4(const std::string &name, const glm::mat4 &mat);

  private:
    unsigned int m_shader_program;
};

} // namespace opengl

#endif
