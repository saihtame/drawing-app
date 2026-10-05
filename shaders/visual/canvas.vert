#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUv;

out vec2 vUv;

uniform mat4 transform;
uniform mat4 projection;
// NB: no sampler declared here. An unused sampler2D defaults to texture unit 0,
// which is where canvas.frag's canvasTexture lives. Keeping a second sampler in
// this program risks the linker reporting two samplers of different types
// sharing one texture unit.

void main()
{
    gl_Position = projection * transform * vec4(aPos, 1.0);
    vUv = aUv;
}
