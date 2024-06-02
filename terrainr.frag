#version 460 core
in vec2 TexCoords;
in vec3 Position;

out vec4 color;

uniform sampler2D tex0;
uniform sampler2D tex1;

void main()
{
    if (Position.y > -30.0)
        discard;
    else
        color = texture(tex0, TexCoords) + texture(tex1, TexCoords * 10.0) - vec4(0.5, 0.5, 0.5, 0.5);
}