#include "shaderprogram.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "opengl.hpp"
#include <spdlog/spdlog.h>

const char *vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texCoords;

out vec2 TexCoords;
uniform mat4 projection;

void main()
{
    gl_Position = projection * vec4(position, 0.0, 1.0);
    TexCoords = texCoords;
}
)";

const char *fragmentShaderSource = R"(
#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D text;
uniform vec4 textColor;
uniform vec4 rectColor;
uniform int renderType;

uniform vec4 rectBounds;
uniform float shadowRadius; // Shadow blur radius in pixels
uniform vec4 cornerRadii; // top-left, top-right, bottom-right, bottom-left

float sdBox(vec2 p, vec2 b) {
    vec2 d = abs(p) - b;
    return length(max(d, 0.0)) + min(max(d.x, d.y), 0.0);
}

float sdRoundedBox(vec2 p, vec2 b, float r) {
    vec2 q = abs(p) - b + r;
    return min(max(q.x, q.y), 0.0) + length(max(q, 0.0)) - r;
}

float sdRoundedBoxPerCorner(vec2 p, vec2 b, vec4 r) {
    float radius;
    if (p.x < 0.0 && p.y < 0.0) {
        radius = r.x; // top-left
    } else if (p.x >= 0.0 && p.y < 0.0) {
        radius = r.y; // top-right
    } else if (p.x >= 0.0 && p.y >= 0.0) {
        radius = r.z; // bottom-right
    } else {
        radius = r.w; // bottom-left
    }

    vec2 q = abs(p) - b + radius;
    return min(max(q.x, q.y), 0.0) + length(max(q, 0.0)) - radius;
}

void main()
{
   if (renderType == 0) { // Text
       vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
       color = textColor * sampled;
   } else if (renderType == 1) { // ColoredText
       color = texture(text, TexCoords);
   } else if (renderType == 2) { // Rectangle
       color = rectColor;
   } else if (renderType == 3) { // Shadow
      vec2 rectPos = rectBounds.xy + rectBounds.zw * 0.5;
      vec2 rectSize = rectBounds.zw;
      vec2 pixelPos = gl_FragCoord.xy;
      vec2 localPos = pixelPos - rectPos;

      float dist = sdBox(localPos, rectSize * 0.5);
      float shadowAlpha = 1.0 - smoothstep(0.0, shadowRadius, dist);

      color = vec4(vec3(0.0), 0.5 * shadowAlpha);
   } else if (renderType == 4) { // RoundedRectangle
      vec2 rectPos = rectBounds.xy + rectBounds.zw * 0.5;
      vec2 rectSize = rectBounds.zw;
      vec2 pixelPos = gl_FragCoord.xy;
      vec2 localPos = pixelPos - rectPos;

      float dist = sdRoundedBoxPerCorner(localPos, rectSize * 0.5, cornerRadii);
      float alpha = 1.0 - smoothstep(-1.0, 1.0, dist);

      color = vec4(rectColor.rgb, rectColor.a * alpha);
   } else if (renderType == 5) { // RoundedShadow
      vec2 rectPos = rectBounds.xy + rectBounds.zw * 0.5;
      vec2 rectSize = rectBounds.zw;
      vec2 pixelPos = gl_FragCoord.xy;
      vec2 localPos = pixelPos - rectPos;

      float dist = sdRoundedBoxPerCorner(localPos, rectSize * 0.5, cornerRadii);
      float shadowAlpha = 1.0 - smoothstep(0.0, shadowRadius, dist);

      color = vec4(vec3(0.0), 0.5 * shadowAlpha);
   }
}
)";

opengl::ShaderProgram::ShaderProgram() {
}

void opengl::ShaderProgram::compile() {
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        throw std::runtime_error("Shader vertex compilation failed: " + std::string(infoLog));
    }

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        throw std::runtime_error("Shader fragment compilation failed: " + std::string(infoLog));
    }

    m_shader_program = glCreateProgram();
    glAttachShader(m_shader_program, vertexShader);
    glAttachShader(m_shader_program, fragmentShader);
    glLinkProgram(m_shader_program);

    glGetProgramiv(m_shader_program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(m_shader_program, 512, NULL, infoLog);
        throw std::runtime_error("Shader program linking failed: " + std::string(infoLog));
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void opengl::ShaderProgram::use() {
    glUseProgram(m_shader_program);
}

void opengl::ShaderProgram::setRenderType(opengl::RenderType type) {
    setInt("renderType", (int)type);

    if (type == RenderType::ColoredText) {
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    }else {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
}

void opengl::ShaderProgram::setFloat(const std::string &name, float value) {
    glUniform1f(glGetUniformLocation(m_shader_program, name.c_str()), value);
}

void opengl::ShaderProgram::setInt(const std::string& name, int value) {
    glUniform1i(glGetUniformLocation(m_shader_program, name.c_str()), value);
}

void opengl::ShaderProgram::setVector4f(const std::string& name, float x, float y, float z, float w) {
    glUniform4f(glGetUniformLocation(m_shader_program, name.c_str()), x, y, z, w);
}

void opengl::ShaderProgram::setMatrix4(const std::string& name, const glm::mat4& mat)
{
    glUniformMatrix4fv(glGetUniformLocation(m_shader_program, "projection"), 1, GL_FALSE, glm::value_ptr(mat));
}
