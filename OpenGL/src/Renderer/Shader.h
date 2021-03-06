#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

class Shader
{
public:
  Shader(const std::string& filepath);
  ~Shader();

  void Bind() const;
  void UnBind() const;

  void UploadUniformInt(const std::string& name, int value);

  void UploadUniformFloat(const std::string& name, float value);
  void UploadUniformFloat2(const std::string& name, const glm::vec2& value);
  void UploadUniformFloat3(const std::string& name, const glm::vec3& value);
  void UploadUniformFloat4(const std::string& name, const glm::vec4& value);

  void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
  void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);
private:
  std::string ReadFile(const std::string& filepath);
  std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
  void Compile(std::unordered_map<GLenum, std::string>& shaderSources);
private:
  uint32_t m_RendererID;
};
