#version 410

in vec2 vTexCoord;

out vec3 LightOutput;

struct DirectionalLight
{
    vec3 color;
    vec3 direction;
    mat4 projection;
    mat4 view;
};

//direction in view-space
uniform DirectionalLight directional;

uniform mat4 cameraView;
uniform vec3 cameraPosition;
uniform float specPower;

uniform vec3 ambient;

uniform sampler2D positionTexture;
uniform sampler2D normalTexture;

uniform mat4 textureSpaceOffset;
uniform sampler2D shadowMap;

void main()
{

    vec3 normal = normalize(texture(normalTexture, vTexCoord).xyz);
    vec3 position = texture(positionTexture, vTexCoord).xyz;

    //compute diffuse lighting
    //vec3 L = directional.direction;
    //vec3 L = (cameraView * vec4(directional.direction, 0)).xyz;
   // float diffuseLight = max(dot(normal, L), 0);
    //vec3 diffuseResult = directional.color * diffuseLight;

    //Shadows
    mat4 lightViewProjection = textureSpaceOffset * directional.projection * directional.view;
    vec4 shadowCoord = lightViewProjection * inverse(cameraView) * vec4(position, 1);

    //compute diffuse
    vec3 lightDirection = directional.direction;
    float d = max(dot(normal, -lightDirection), 0); //lamb
    
    //shade
    if(texture(shadowMap, shadowCoord.xy).r < shadowCoord.z){
        d = 0;
    }
    d = texture(shadowMap, shadowCoord.xy).r;

    


    //compute specular lighting
    vec3 camViewPosition = (cameraView * vec4(cameraPosition, 1)).xyz;
    vec3 E = normalize(camViewPosition - position);
    vec3 R = reflect(-lightDirection, normal);
    float s = pow(max(dot(E,R), 0), specPower);

    LightOutput = ambient + (directional.color * d) + (directional.color * s);



}