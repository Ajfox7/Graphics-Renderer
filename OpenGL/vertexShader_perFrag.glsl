#version 330 core

layout(location=0) in vec3 in_Position;
layout(location=1) in vec3 in_Normal;
layout(location=2) in vec2 in_TexCoord;

uniform mat4 projMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat4 normalMatrix;

uniform vec3 camPosWorld;

out vec3 v_Normal;
out vec3 v_Half;
out vec3 v_LightDir;
out vec2 tCoord;

struct LightData {
    vec3 position;
    vec3 intensity;
};
uniform LightData light;

void main(void)
{
    tCoord = in_TexCoord;
    
    vec4 pos = viewMatrix * modelMatrix * vec4 ( in_Position , 1.0 );
    vec4 lightPos = viewMatrix * vec4 ( light.position, 1.0 );

    v_Normal = normalize(mat3(normalMatrix) * in_Normal);

    vec3 v = normalize( -pos.xyz );
    v_LightDir = normalize( lightPos.xyz - pos.xyz );
    v_Half = normalize( v + v_LightDir );

    gl_Position = projMatrix * pos;
}