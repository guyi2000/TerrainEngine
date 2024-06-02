#version 460 core
layout (location = 0) in vec3 vertex;
layout (location = 1) in vec2 texCoords;

out vec3 Position;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
    Position = vec3(model * vec4(vertex, 1.0));
    gl_Position = proj * view * model * vec4(vertex, 1.0);
    TexCoords = texCoords;
}