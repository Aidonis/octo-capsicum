#version 410

in vec4 vPosition;
in vec4 vNormal;
in vec2 vTexCoord;

// these should correlate to the color attachments!
layout(location = 0) out vec3 gpassAlbedo;
layout(location = 1) out vec3 gpassPosition;
layout(location = 2) out vec3 gpassNormal;

uniform sampler2D DiffuseTexture;
//uniform sampler2D NormalTexture;

void main()
{
	// output the data
	
	// Note: you could use a material color
	// or sample a texture for albedo
	
    //gpassAlbedo = texture(DiffuseTexture, vTexCoord).xyz;
    gpassAlbedo = texture(DiffuseTexture, vTexCoord).xyz;
	
	gpassPosition = vPosition.xyz;
	gpassNormal = vNormal.xyz;
}