#pragma once

#include <glm\glm.hpp>

struct GPUParticle{
	
	GPUParticle() : lifetime(1), lifespan(0){}

	glm::vec3 position;
	glm::vec3 velocity;
	float lifetime;
	float lifespan;
};

class GPUParticleEmitter{
public:

	GPUParticleEmitter();
	virtual ~GPUParticleEmitter();

	void init(unsigned int a_maxParticles,
		float a_lifespanMin,	float a_lifespanMax,
		float a_velocityMin,	float a_velocityMax,
		float a_startSize,		float a_endSize,
		const glm::vec4& a_startColor,
		const glm::vec4& a_endColor);

	void draw(float time, const glm::mat4& a_cameraTransform, const glm::mat4& a_projectionView);

protected:
	void createBuffers();
	void createUpdateShader();
	void createDrawShader(const char* name, const char* vpath, const char* fpath, const char* gpath);

	unsigned int loadShaderFile(unsigned int shaderType, const char* path);

	GPUParticle* m_particles;
	unsigned int m_maxParticles;

	glm::vec3 m_position;

	float m_lifespanMin, m_lifespanMax;
	float m_velocityMin, m_velocityMax;
	float m_startSize, m_endSize;

	glm::vec4 m_startColor;
	glm::vec4 m_endColor;

	unsigned int m_activeBuffer;
	unsigned int m_vao[2];
	unsigned int m_vbo[2];
	unsigned int m_drawShader;
	unsigned int m_updateShader;

	float m_lastDrawTime;

};