#version 410

in vec4 Color;
in vec4 Normal;
in vec4 Position;

//out vec4 fragColor;
//out vec4 normalColor;

layout(location = 0) out vec3 gpassAlbedo;
layout(location = 1) out vec3 gpassPosition;
layout(location = 2) out vec3 gpassNormal;

void main(){
	gpassAlbedo = Color.xyz;
	gpassPosition = Position.xyz;
	gpassNormal = Normal.xyz;
}