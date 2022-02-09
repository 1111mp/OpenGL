// Basic Texture Shader

#type vertex
#version 330 core

layout (location = 0) in vec3 position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f);
}

#type fragment
#version 330 core

out vec4 color;

void main()
{
	color = vec4(1.0f);
}
