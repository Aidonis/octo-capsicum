#version 410

in vec2 vTexCoord;

out vec4 FragColor;

uniform sampler2D Albedo;
uniform sampler2D Light;

uniform sampler2D Position;
uniform sampler2D Normal;


void main()
{
    vec3 albedoColor = texture(Albedo, vTexCoord ).rgb;
    vec3 lightColor = texture(Light, vTexCoord).rgb;
    

    FragColor = vec4(albedoColor * lightColor, 1);
}