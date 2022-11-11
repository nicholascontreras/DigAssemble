#version 330 core
out vec4 color;

in vec2 texCoord;
in vec3 fragPos;

uniform sampler2D tex;

uniform vec3 cameraPos;
uniform int startFogDistance;
uniform int fullyFogDistance;
uniform vec3 fogColor;

float getFogFactor() {
    float dist = distance(cameraPos, fragPos);
    return clamp((dist - startFogDistance) / (fullyFogDistance - startFogDistance), 0, 1);
}

vec3 applyFogToColor(vec3 baseColor) {
    float fogFactor = getFogFactor();
    return ((fogColor - baseColor) * fogFactor) + baseColor;
}

void main() {
    vec4 textureColor = texture(tex, texCoord);
    color = vec4(applyFogToColor(vec3(textureColor)), textureColor.a);
} 