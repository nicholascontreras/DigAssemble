#version 330 core
out vec4 color;

in vec2 texCoord;
in vec3 fragPos;

uniform sampler2D tex;

uniform vec3 cameraPos;
uniform int startFogDistance;
uniform int fullyFogDistance;

float getFogFactor() {
    float dist = distance(cameraPos, fragPos);
    if (dist < startFogDistance) {
        return 1;
    } else if (dist > fullyFogDistance) {
        return 0;
    } else {
        return (fullyFogDistance - dist) / (fullyFogDistance - startFogDistance);
    }
}

void main() {
    vec4 textureColor = texture(tex, texCoord);
    color = vec4(vec3(textureColor), textureColor.a * getFogFactor());
} 