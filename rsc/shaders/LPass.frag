#version 410

in vec2 vTexCoord;

out vec3 LightOutput;

uniform vec3 LightDirection;
uniform vec3 LightColor;

uniform sampler2D Albedo;
uniform sampler2D Normal;
uniform sampler2D Position;

uniform vec3 ViewPosition;
uniform float SpecularPower = 16.0f;

void main()
{
    vec3 normalColor    = texture(Normal, vTexCoord).xyz;
    vec3 positionColor  = texture(Position, vTexCoord).xyz;
    vec3 diffuseColor   = texture(Albedo, vTexCoord).xyz;
    float specularValue = texture(Albedo, vTexCoord).a;

    vec3 viewDir = normalize(ViewPosition - positionColor);

    // Diffuse
    vec3 diffuse = max(0.0, dot(normalColor, LightDirection)) * LightColor;

    // Specular
    vec3 halfwayDir = normalize(LightDirection + viewDir);
    float spec      = pow(max(dot(normalColor, halfwayDir), 0), SpecularPower);
    vec3 specular   = LightColor * spec * specularValue;

    LightOutput = diffuse + specular;
}