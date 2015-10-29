#version 410

in vec2 vTexCoord;

out vec3 LightOutput;

struct DirectionalLight
{
    vec3 color;
    vec3 direction;
};

//direction in view-space
uniform DirectionalLight directional;

uniform vec3 cameraPosition;
uniform float specPower;

uniform vec3 ambient;

uniform sampler2D positionTexture;
uniform sampler2D normalTexture;

void main()
{

    vec3 normal = normalize(texture(normalTexture, vTexCoord).xyz);
    vec3 position = texture(positionTexture, vTexCoord).xyz;

    //compute diffuse lighting
    vec3 L = directional.direction;
    float diffuseLight = max(dot(normal, L), 0);
    vec3 diffuseResult = directional.color * diffuseLight;

    //compute specular lighting
    vec3 V = normalize(cameraPosition - position);
    vec3 H = normalize(L + V);
    float specularLight = pow(max(dot(normal, H), 0), specPower);
    if (diffuseLight <= 0)
    {
        specularLight = 0;
    }
    vec3 specularResult = directional.color * specularLight;


    LightOutput = ambient + (diffuseLight + specularLight);

}