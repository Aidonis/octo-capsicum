#pragma once

#include <glm\glm.hpp>
#include "../nsfwgl/Window.h"
#include "Geometry.h"

auto &w = nsfw::Window::instance();

class Particle {
public:
	friend class ParticleBatch;

	void update() {
		m_position += m_velocity * w.getDeltaTime();
	}

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
	
	~ParticleBatch() {
		delete[] m_particles;
	}

	void init(	int maxParticles,
				float decayRate,
				Geometry model){
		m_maxParticles = maxParticles;
		m_particles = new Particle[maxParticles];
		m_decayRate = decayRate;

		m_model = new Geometry(model);
	}

	void Update() {
		for (int i = 0; i < m_maxParticles; i++) {
			if (m_particles[i].m_lifetime > 0.0f) {
				m_particles[i].update();
				m_particles[i].m_lifetime -= m_decayRate * w.getDeltaTime();
			}
		}
	}

	void Draw() {
		for (int i = 0; i < m_maxParticles; i++) {
			if (m_particles[i].m_lifetime > 0.0f) {
			}
		}
	}

	void addParticle(	const glm::vec2& position, 
						const glm::vec2& velocity);
	
	void addParticle(	const glm::vec2& position, 
						const glm::vec2& velocity,
						const glm::vec4& color);

	float		m_decayRate;
	Particle*	m_particles = nullptr;
	int			m_maxParticles = 0;
	Geometry*	m_model = nullptr;

private:

};