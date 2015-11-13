#version 410

layout(location = 0) in vec4 Position;

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model;

void main(){
	gl_Position = Projection * View * Model * Position;
}