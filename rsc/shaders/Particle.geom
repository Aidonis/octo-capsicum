#version 410

//input
layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

//from shader
in vec3 position[];
in float lifetime[];
in float lifespan[];

//output to frag
out vec4 Color;
out vec4 Normal;
out vec4 Position;


uniform mat4 view;
uniform mat4 projection;

uniform mat4 cameraTransform;

uniform float sizeStart;
uniform float sizeEnd;

uniform vec4 colorStart;
uniform vec4 colorEnd;

void main(){
	mat4 projectionView = projection * view;
	//interpolate color
	Color = mix(colorStart, colorEnd, lifetime[0] / lifespan[0]);

	//calc size and create corners of quad
	float halfSize = mix(sizeStart, sizeEnd, lifetime[0] / lifespan[0]) * 0.5f;

	vec3 corners[4];
	corners[0] = vec3(halfSize,	-halfSize, 0);
	corners[1] = vec3(halfSize,	halfSize, 0);
	corners[2] = vec3(-halfSize,-halfSize, 0);
	corners[3] = vec3(-halfSize, halfSize, 0);

	//billboard
	vec3 zAxis = normalize(cameraTransform[3].xyz - position[0]);
	vec3 xAxis = cross(cameraTransform[1].xyz, zAxis);
	vec3 yAxis = cross(zAxis, xAxis);
	mat3 billboard = mat3(xAxis, yAxis, zAxis);

	Normal = view * vec4(zAxis,0);

	//Emit
	gl_Position = projectionView * vec4(billboard * corners[0] + position[0], 1);
	Position = view * inverse(projectionView) * gl_Position;
	EmitVertex();

	gl_Position = projectionView * vec4(billboard * corners[1] + position[0], 1);
	Position = view * inverse(projectionView) * gl_Position;
	EmitVertex();

	gl_Position = projectionView * vec4(billboard * corners[2] + position[0], 1);
	Position = view * inverse(projectionView) * gl_Position;
	EmitVertex();

	gl_Position = projectionView * vec4(billboard * corners[3] + position[0], 1);
	Position = view * inverse(projectionView) * gl_Position;
	EmitVertex();
}