#version 460 core
in vec2 vUv;
out vec4 FragColor;

uniform mat4 transform;
uniform mat4 projection;
uniform sampler2D canvasTexture;

void main()
{
    vec4 color = texture(canvasTexture, vUv);
    if (color.a < 0.01)
        discard;
    FragColor = color;
}
