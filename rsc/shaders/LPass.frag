#version 410

in vec2 vTexCoord;

out vec3 LightOutput;

//struct DirectionalLight
//{
uniform    vec3 color;
uniform    vec3 direction;
uniform    mat4 projection;
uniform    mat4 view;
//};

//direction in view-space
//uniform DirectionalLight directional;

uniform mat4 cameraView;
uniform vec3 cameraPosition;
uniform float specPower;

uniform vec3 ambient;

uniform sampler2D positionTexture;
uniform sampler2D normalTexture;

uniform mat4 textureSpaceOffset;
uniform sampler2D shadowMap;
uniform float shadowBias = .01;

void main()
{

    vec3 normal = normalize(texture(normalTexture, vTexCoord).xyz);
    vec3 position = texture(positionTexture, vTexCoord).xyz;

    //compute diffuse
    vec3 lightDirection = (cameraView * vec4(direction, 0)).xyz;
    float d = max(dot(normal, lightDirection), 0); //lamb
    
    //Shadows
    mat4 lightViewProjection = textureSpaceOffset * projection * view;
    vec4 shadowCoord = lightViewProjection * inverse(cameraView) * vec4(position, 1);

    //shade
    //d = texture(shadowMap, shadowCoord.xy).r;
    
    //Texel Setup from ShadowMap
    //vec2 texel = 1.0f / textureSize(shadowMap, 0).xy;

    //Box Blur Attempt on Shado
    vec3 camViewPosition = (cameraView * vec4(cameraPosition, 1)).xyz;
    vec3 E = normalize(camViewPosition - position);
    vec3 R = reflect(-lightDirection, normal);
    float s = pow(max(dot(E,R), 0), specPower);



    if(texture(shadowMap, shadowCoord.xy).z < shadowCoord.z - shadowBias){
        d = 0;
        s = 0;
    }
    


    //compute specular lighting


    LightOutput = /*ambient + */(color * d) + (color * s);
}