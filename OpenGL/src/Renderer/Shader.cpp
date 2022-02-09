#include "Shader.h"

#include <fstream>
#include <glm/gtc/type_ptr.hpp>

static GLenum ShaderTypeFromString(const std::string& type)
{
  if (type == "vertex")
    return GL_VERTEX_SHADER;
  if (type == "fragment" || type == "pixel")
    return GL_FRAGMENT_SHADER;

  HZ_CORE_ASSERT(false, "Unknown shader type!");
  return 0;
}

Shader::Shader(const std::string& filepath)
{
  std::string source = ReadFile(filepath);
  auto shaderSources = PreProcess(source);
  Compile(shaderSources);
}

Shader::~Shader()
{
  glDeleteProgram(m_RendererID);
}

std::string Shader::ReadFile(const std::string& filepath)
{
  std::string result;
  std::ifstream in(filepath, std::ios::in | std::ios::binary);
  if (in)
  {
    in.seekg(0, std::ios::end);
    result.resize(in.tellg());
    in.seekg(0, std::ios::beg);
    in.read(&result[0], result.size());
    in.close();
  }
  else
  {
    HZ_HAZEL_ERROR("Could not open file '{0}'", filepath);
  }

  return result;
}

std::unordered_map<GLenum, std::string> Shader::PreProcess(const std::string& source)
{
  std::unordered_map<GLenum, std::string> shaderSources;

  const char* typeToken = "#type";
  size_t typeTokenLength = strlen(typeToken);
  size_t pos = source.find(typeToken, 0);

  while (pos != std::string::npos)
  {
    size_t eol = source.find_first_of("\r\n", pos);
    HZ_CORE_ASSERT(eol != std::string::npos, "Syntax error");
    size_t begin = pos + typeTokenLength + 1;
    std::string type = source.substr(begin, eol - begin);
    HZ_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified");

    size_t nextLinePos = source.find_first_not_of("\r\n", eol);
    pos = source.find(typeToken, nextLinePos);
    shaderSources[ShaderTypeFromString(type)] = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
  }

  return shaderSources;
}

void Shader::Compile(std::unordered_map<GLenum, std::string>& shaderSources)
{
  GLuint program = glCreateProgram();
  HZ_CORE_ASSERT(shaderSources.size() <= 2, "We only support 2 shaders for now");
  std::array<GLenum, 2> glShaderIDs;
  int glShaderIDIndex = 0;

  for (auto& kv : shaderSources)
  {
    GLenum type = kv.first;
    const std::string& source = kv.second;

    // Create an empty vertex shader handle
    GLuint shader = glCreateShader(type);

    // Send the vertex shader source code to GL
    // Note that std::string's .c_str is NULL character terminated.
    const GLchar* sourceCStr = source.c_str();
    glShaderSource(shader, 1, &sourceCStr, 0);

    // Compile the vertex shader
    glCompileShader(shader);

    GLint isCompiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
      GLint maxLength = 0;
      glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

      // The maxLength includes the NULL character
      std::vector<GLchar> infoLog(maxLength);
      glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

      // We don't need the shader anymore.
      glDeleteShader(shader);

      HZ_HAZEL_ERROR("{0}", infoLog.data());
      HZ_CORE_ASSERT(false, "Vertex shader compilation failure!");
      break;
    }

    glAttachShader(program, shader);
    glShaderIDs[glShaderIDIndex++] = shader;
  }

  // Vertex and fragment shaders are successfully compiled.
  // Now time to link them together into a program.
  // Get a program object.
  m_RendererID = program;

  // Link our program
  glLinkProgram(m_RendererID);

  // Note the different functions here: glGetProgram* instead of glGetShader*.
  GLint isLinked = 0;
  glGetProgramiv(m_RendererID, GL_LINK_STATUS, (int*)&isLinked);
  if (isLinked == GL_FALSE)
  {
    GLint maxLength = 0;
    glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);

    // The maxLength includes the NULL character
    std::vector<GLchar> infoLog(maxLength);
    glGetProgramInfoLog(m_RendererID, maxLength, &maxLength, &infoLog[0]);

    // We don't need the program anymore.
    glDeleteProgram(m_RendererID);

    // Don't leak shaders either.
    for (auto id : glShaderIDs)
      glDeleteShader(id);

    HZ_HAZEL_ERROR("{0}", infoLog.data());
    HZ_CORE_ASSERT(false, "Shader link failure!");
    return;
  }

  // Always detach shaders after a successful link.
  for (auto id : glShaderIDs)
    glDetachShader(program, id);
}

void Shader::Bind() const
{
  glUseProgram(m_RendererID);
}

void Shader::UnBind() const
{
  glUseProgram(0);
}

void Shader::UploadUniformInt(const std::string& name, int value)
{
  GLint location = glGetUniformLocation(m_RendererID, name.c_str());
  glUniform1i(location, value);
}

void Shader::UploadUniformFloat(const std::string& name, float value)
{
  GLint location = glGetUniformLocation(m_RendererID, name.c_str());
  glUniform1f(location, value);
}

void Shader::UploadUniformFloat2(const std::string& name, const glm::vec2& value)
{
  GLint location = glGetUniformLocation(m_RendererID, name.c_str());
  glUniform2f(location, value.x, value.y);
}

void Shader::UploadUniformFloat3(const std::string& name, const glm::vec3& value)
{
  GLint location = glGetUniformLocation(m_RendererID, name.c_str());
  glUniform3f(location, value.x, value.y, value.z);
}

void Shader::UploadUniformFloat4(const std::string& name, const glm::vec4& value)
{
  GLint location = glGetUniformLocation(m_RendererID, name.c_str());
  glUniform4f(location, value.x, value.y, value.z, value.w);
}

void Shader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
{
  GLint location = glGetUniformLocation(m_RendererID, name.c_str());
  glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
{
  GLint location = glGetUniformLocation(m_RendererID, name.c_str());
  glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

