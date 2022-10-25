#version 460 core
layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 texCoordIn;

out vec2 texCoord;

uniform mat4 projection;

void main() {
    gl_Position = projection * vec4(pos, 0.0, 1.0);
    texCoord = texCoordIn;
}  