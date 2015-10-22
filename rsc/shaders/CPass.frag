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
    vec3 light = texture(Light, vTexCoord).rgb;
    vec3 albedo = texture(Albedo, vTexCoord).rgb;

    FragColor = vec4(vTexCoord, albedo.z, 1);

}