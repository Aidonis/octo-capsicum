#pragma once

#include <glm\glm.hpp>
#include <glm/ext.hpp>
#include "Geometry.h"


class Particle {
public:
	friend class ParticleBatch;

	void update(float deltaTime);

	float	m_lifetime = 0.0f; // if > 0 isActive


	glm::mat4	m_transform = glm::mat4(1);

	glm::vec3	m_position = glm::vec3(0.0f);
	glm::vec3	m_velocity = glm::vec3(0.0f);
	glm::vec4	m_color = glm::vec4(1, 1, 1, 1);
private:

};

class ParticleBatch{
public:
	ParticleBatch();
	
	~ParticleBatch();

	void init(int maxParticles, float decayRate, Geometry model);

	void Update(float deltaTime);

	void Draw();

	void addParticle(	const glm::vec3& position, 
						const glm::vec3& velocity);
	
	void addParticle(	const glm::vec3& position, 
						const glm::vec3& velocity,
						const glm::vec4& color);

	int findFreeParticle();

	float		m_decayRate;
	int			m_maxParticles = 0;
	int			m_lastFreeParticle = 0;

	Particle*	m_particles = nullptr;
	Geometry*	m_model = nullptr;

private:

};