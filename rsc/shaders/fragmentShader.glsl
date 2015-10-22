#version 410

in vec4 vPosition;
in vec4 vNormal;

in vec2 vTexCoord;

// these should correlate to the color attachments!
layout(location = 0) out vec3 gpassAlbedo;
layout(location = 1) out vec3 gpassPosition;
layout(location = 2) out vec3 gpassNormal;

uniform sampler2D Diffuse;
uniform sampler2D Normal;

void main()
{
	// output the data
	
	// Note: you could use a material color
	// or sample a texture for albedo
	
	gpassAlbedo = vec3(1);
    //gpassAlbedo = texture(Diffuse, vTexCoord).xyz;
	
	gpassPosition = vPosition.xyz;
	gpassNormal = vNormal.xyz;
    //gpassNormal = texture(Normal, vTexCoord).xyz;
}