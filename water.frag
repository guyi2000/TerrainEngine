#version 460 core
in vec2 TexCoords;

out vec4 color;

uniform sampler2D sprite;
uniform vec2 offset;

void main()
{
    color = texture(sprite, TexCoords + offset) * vec4(1.0f, 1.0f, 1.0f, 0.7f);
}

