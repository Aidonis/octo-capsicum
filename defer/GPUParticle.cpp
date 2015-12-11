#include "GPUParticle.h"
#include <gl_core_4_4.h>
#include <fstream>
#include <iostream>



GPUParticleEmitter::GPUParticleEmitter() : m_particles(nullptr),
m_maxParticles(0),
m_position(0,0,0),
m_drawShader(0),
m_updateShader(0),
m_lastDrawTime(0){
	m_vao[0] = 0;
	m_vao[1] = 0;
	m_vbo[0] = 0;
	m_vbo[1] = 0;
}

GPUParticleEmitter::~GPUParticleEmitter(){
	delete[] m_particles;

	glDeleteVertexArrays(2, m_vao);
	glDeleteBuffers(2, m_vbo);
	// delete the shaders
	glDeleteProgram(m_drawShader);
	glDeleteProgram(m_updateShader);
}

void GPUParticleEmitter::init(unsigned a_maxParticles,
							float a_lifespanMin, float a_lifespanMax,
							float a_velocityMin, float a_velocityMax,
							float a_startSize,	float a_endSize,
							const glm::vec4& a_startColor, const glm::vec4& a_endColor){

	m_startColor = a_startColor;
	m_endColor = a_endColor;
	m_startSize = a_startSize;
	m_endSize = a_endSize;
	m_velocityMax = a_velocityMax;
	m_velocityMin = a_velocityMin;
	m_lifespanMin = a_lifespanMin;
	m_lifespanMax = a_lifespanMax;
	m_maxParticles = a_maxParticles;

	m_particles = new GPUParticle[a_maxParticles];

	createBuffers();

	createUpdateShader();

	createDrawShader("GeometryShader", "../rsc/shaders/Particle.vert", "../rsc/shaders/Particle.frag", "../rsc/shaders/Particle.geom");


}

void GPUParticleEmitter::draw(float time, const glm::mat4& a_cameraTransform, const glm::mat4& a_projection, const glm::mat4& a_view){
	//update particles using transform
	glUseProgram(m_updateShader);

	//bind info
	int location = glGetUniformLocation(m_updateShader, "time");
	glUniform1f(location, time);

	float deltaTime = time - m_lastDrawTime;
	m_lastDrawTime = time;

	location = glGetUniformLocation(m_updateShader, "deltaTime");
	glUniform1f(location, deltaTime);

	//bind emitter position
	location = glGetUniformLocation(m_updateShader, "emitterPosition");
	glUniform3fv(location, 1, &m_position[0]);

	glEnable(GL_RASTERIZER_DISCARD);

	//bind buffer we will update
	glBindVertexArray(m_vao[m_activeBuffer]);

	//work otherBuffer
	unsigned int otherBuffer = (m_activeBuffer + 1) % 2;

	//bind buffer that will be updated
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_vbo[otherBuffer]);
	glBeginTransformFeedback(GL_POINTS);

	glDrawArrays(GL_POINTS, 0, m_maxParticles);

	//Disable transform feedback
	glEndTransformFeedback();
	glDisable(GL_RASTERIZER_DISCARD);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, 0);

	//draw using geo shader to billboard
	glUseProgram(m_drawShader);

	location = glGetUniformLocation(m_drawShader, "view");
	glUniformMatrix4fv(location, 1, false, &a_view[0][0]);

	location = glGetUniformLocation(m_drawShader, "projection");
	glUniformMatrix4fv(location, 1, false, &a_projection[0][0]);

	location = glGetUniformLocation(m_drawShader, "cameraTransform");
	glUniformMatrix4fv(location, 1, false, &a_cameraTransform[0][0]);

	//draw particles in the other-buffer
	glBindVertexArray(m_vao[otherBuffer]);
	glDrawArrays(GL_POINTS, 0, m_maxParticles);

	//swap frame

	m_activeBuffer = otherBuffer;
}

void GPUParticleEmitter::createBuffers(){
	//create opengl buffers

	glGenVertexArrays(2, m_vao);
	glGenBuffers(2, m_vbo);

	//setup the first buffers
	glBindVertexArray(m_vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, m_maxParticles * sizeof(GPUParticle), m_particles, GL_STREAM_DRAW);

	glEnableVertexAttribArray(0);	//position
	glEnableVertexAttribArray(1);	//velocity
	glEnableVertexAttribArray(2);	//lifetime
	glEnableVertexAttribArray(3);	//lifespan

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GPUParticle), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GPUParticle), ((char*)0) + 12);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(GPUParticle), ((char*)0) + 24);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(GPUParticle), ((char*)0) + 28);

	//setup the first buffers
	glBindVertexArray(m_vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, m_maxParticles * sizeof(GPUParticle), 0, GL_STREAM_DRAW);

	glEnableVertexAttribArray(0);	//position
	glEnableVertexAttribArray(1);	//velocity
	glEnableVertexAttribArray(2);	//lifetime
	glEnableVertexAttribArray(3);	//lifespan

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GPUParticle), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GPUParticle), ((char*)0) + 12);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(GPUParticle), ((char*)0) + 24);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(GPUParticle), ((char*)0) + 28);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GPUParticleEmitter::createUpdateShader(){
	//create shader
	unsigned int uvs = loadShaderFile(GL_VERTEX_SHADER, "../rsc/shaders/Particle.upvert");

	m_updateShader = glCreateProgram();
	glAttachShader(m_updateShader, uvs);

	//specify data that will stream back
	const char* varyings[] = { "position", "velocity", "lifetime", "lifespan" };
	glTransformFeedbackVaryings(m_updateShader, 4, varyings, GL_INTERLEAVED_ATTRIBS);
	glLinkProgram(m_updateShader);

	//remove handles
	glDeleteShader(uvs);

	//bind shader
	glUseProgram(m_updateShader);

	//bind lifetime
	int location = glGetUniformLocation(m_updateShader, "lifeMin");
	glUniform1f(location, m_lifespanMin);
	location = glGetUniformLocation(m_updateShader, "lifeMax");
	glUniform1f(location, m_lifespanMax);
}

void GPUParticleEmitter::createDrawShader(const char* name, const char* vpath, const char* fpath, const char* gpath){
	unsigned int vs = loadShaderFile(GL_VERTEX_SHADER, vpath);
	unsigned int fs = loadShaderFile(GL_FRAGMENT_SHADER, fpath);
	unsigned int gs = loadShaderFile(GL_GEOMETRY_SHADER, gpath);

	m_drawShader = glCreateProgram();
	glAttachShader(m_drawShader, vs);
	glAttachShader(m_drawShader, fs);
	glAttachShader(m_drawShader, gs);
	glLinkProgram(m_drawShader);

	//
	glDeleteShader(vs);
	glDeleteShader(fs);
	glDeleteShader(gs);

	glUseProgram(m_drawShader);

	//bind size
	int location = glGetUniformLocation(m_drawShader, "sizeStart");
	glUniform1f(location, m_startSize);
	location = glGetUniformLocation(m_drawShader, "sizeEnd");
	glUniform1f(location, m_endSize);

	//bind color info
	location = glGetUniformLocation(m_drawShader, "colorStart");
	glUniform4fv(location, 1, &m_startColor[0]);
	location = glGetUniformLocation(m_drawShader, "colorEnd");
	glUniform4fv(location, 1, &m_endColor[0]);

}

unsigned GPUParticleEmitter::loadShaderFile(unsigned type, const char* path){

	FILE* file = fopen(path, "rb");
	if (file == nullptr)
	{
		assert(false);
		return 0;
	}

	fseek(file, 0, SEEK_END);
	unsigned int length = ftell(file);
	fseek(file, 0, SEEK_SET);
	char* source = new char[length + 1];
	memset(source, 0, length + 1);
	fread(source, sizeof(char), length, file);
	fclose(file);

	unsigned int shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, 0);
	glCompileShader(shader);
	GLint success = GL_FALSE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		int length = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		char* log = new char[length];
		glGetShaderInfoLog(shader, length, 0, log);
		std::cout << "Error compiling shader.\n" << log << std::endl;
		delete[] log;
		assert(false);
	}
	delete[] source;

	return shader;
}