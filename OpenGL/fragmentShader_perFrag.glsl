#version 330 core

layout(location=0) out vec4 fragmentColor;

in vec3 v_Normal;
in vec3 v_Half;
in vec3 v_LightDir;
in vec2 tCoord;

struct LightData {
    vec3 position;
    vec3 intensity;
};
uniform LightData light;

uniform vec3 Ia;
uniform vec3 ka, kd, ks;
uniform float phongExp;

uniform sampler2D textureUnit;

void main(void)
{
    vec3 N = normalize(v_Normal.xyz);
    vec3 H = normalize(v_Half);
    vec3 L = normalize(v_LightDir.xyz);

    vec3 texel = texture(textureUnit, tCoord).rgb;

    // Use absolute value so both 1.0 (front) and -1.0 (back) become 1.0
    float diffuseVal = max(abs(dot(N, L)), 0.1); 

    float specVal = max(abs(dot(N, H)), 0.1);

    vec3 intensity = ka * Ia + kd * light.intensity * diffuseVal * texel + ks * light.intensity * pow(specVal, phongExp);

    fragmentColor = vec4(intensity, 1.0);
}