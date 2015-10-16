#version 410

in vec2 vTexCoord;

out vec4 FragColor;

uniform sampler2D albedoTexture;
uniform sampler2D lightTexture;

void main(){

	vec3 light = texture(lightTexture, vTexCoord).rgb;
	vec3 albedo = texture(albedoTexture, vTexCoord).rgb;

	FragColor = vec4(albedo * light, 1);
}