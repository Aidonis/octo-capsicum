#version 410

in vec2 vTexCoord;

out vec4 FragColor;

uniform sampler2D Albedo;
uniform sampler2D Position;
uniform sampler2D Normal;
uniform sampler2D Depth;
uniform sampler2D Light;

void main()
{
    vec3 albedoColor = texture(Albedo, vTexCoord).rgb;
    vec3 positionColor = texture(Position, vTexCoord).rgb;
    vec3 normalColor = texture(Normal, vTexCoord).rgb;
    vec3 lightColor = texture(Light, vTexCoord).rgb;


    FragColor = vec4(albedoColor, 1);
}