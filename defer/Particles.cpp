#include "Particles.h"


//Particle
void Particle::update(float deltaTime){
	m_position += m_velocity * deltaTime;
}


//Particle Batch
ParticleBatch::ParticleBatch(){
}

ParticleBatch::~ParticleBatch() {
	delete[] m_particles;
}

void ParticleBatch::init(int maxParticles, float decayRate, Geometry model){

		m_maxParticles = maxParticles;
		m_particles = new Particle[maxParticles];
		m_decayRate = decayRate;
		m_model = new Geometry(model);
}

void ParticleBatch::Update(float deltaTime){
	for (int i = 0; i < m_maxParticles; i++) {
		if (m_particles[i].m_lifetime > 0.0f) {
			m_particles[i].update(deltaTime);
			m_particles[i].m_lifetime -= m_decayRate * deltaTime;
		}
	}
}

void ParticleBatch::Draw(){
	for (int i = 0; i < m_maxParticles; i++) {
		if (m_particles[i].m_lifetime > 0.0f) {
		}
	}
}

void ParticleBatch::addParticle(const glm::vec2& position, const glm::vec2& velocity){
	int particleIndex = findFreeParticle();

	auto& p = m_particles[particleIndex];

	p.m_lifetime = 1.0f;
	p.m_position = position;
	p.m_velocity = velocity;
}

void ParticleBatch::addParticle(const glm::vec2& position, const glm::vec2& velocity, const glm::vec4& color){
	int particleIndex = findFreeParticle();

	auto& p = m_particles[particleIndex];

	p.m_lifetime = 1.0f;
	p.m_position = position;
	p.m_velocity = velocity;
	p.m_color = color;
}

int ParticleBatch::findFreeParticle(){
	for (int i = m_lastFreeParticle; i < m_maxParticles; i++){
		if(m_particles[i].m_lifetime <= 0.0f){
			m_lastFreeParticle = i;
			return i;
		}
	}

	for (int i = 0; i < m_lastFreeParticle; i++){
		if(m_particles[i].m_lifetime < 0.0f){
			m_lastFreeParticle = i;
			return i;
		}
	}

	//Nothing Free overwrite original
	return 0;
}