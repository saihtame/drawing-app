#version 460 core
in vec2 vUv;
out vec4 FragColor;

uniform mat4 transform;
uniform mat4 projection;
uniform sampler2D texture;

void main()
{
    FragColor = vec4(1.0);
}
