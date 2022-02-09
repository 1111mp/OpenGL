// Basic Texture Shader

#type vertex
#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f);
  FragPos = vec3(model * vec4(position, 1.0f));
  Normal = mat3(transpose(inverse(model))) * normal;
  TexCoords = texCoords;
}

#type fragment
#version 330 core

struct Material
{
  sampler2D diffuse;
  sampler2D specular;
  float shininess;
};

struct Light
{
  vec3 position;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 color;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
  // 环境光
  vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

  // 漫反射
  vec3 norm = normalize(Normal);
  vec3 lightDir = normalize(light.position - FragPos);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

  // 镜面反射
  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

	color = vec4(ambient + diffuse + specular, 1.0f);
}
