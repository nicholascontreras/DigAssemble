#version 460 core
in vec2 texCoord;

out vec4 color;

uniform sampler2D tex;
uniform vec3 textColor;

void main() {    
    float alpha = texture(tex, texCoord).r;
    color = vec4(textColor, alpha);
}  