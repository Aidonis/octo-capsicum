#pragma once

#include <glm\glm.hpp>
#include "GPass.h"
#include "Geometry.h"


class Particle {
public:
	friend class ParticleBatch;

	void update(float deltaTime);

private:
	glm::vec2	m_position = glm::vec2(0.0f);
	glm::vec2	m_velocity = glm::vec2(0.0f);
	glm::vec4	m_color = glm::vec4(1, 1, 1, 1);
	
	float	m_width = 0.0f;
	float	m_lifetime = 0.0f; // if > 0 isActive

};

class ParticleBatch{
public:
	ParticleBatch();
	
	~ParticleBatch();

	void init(int maxParticles, float decayRate, Geometry model);

	void Update(float deltaTime);

	void Draw();

	void addParticle(	const glm::vec2& position, 
						const glm::vec2& velocity);
	
	void addParticle(	const glm::vec2& position, 
						const glm::vec2& velocity,
						const glm::vec4& color);

	int findFreeParticle();

	float		m_decayRate;
	int			m_maxParticles = 0;
	int			m_lastFreeParticle = 0;

	Particle*	m_particles = nullptr;
	Geometry*	m_model = nullptr;

private:

};