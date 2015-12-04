#include "GPUParticle.h"
#include <gl_core_4_4.h>

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



}
