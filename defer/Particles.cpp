#include "Particles.h"


//Particle
void Particle::update(float deltaTime){
	m_position += m_velocity * deltaTime;
	m_transform += glm::translate(m_transform, m_velocity);
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
			m_particles[i].m_lifetime -= m_decayRate * deltaTime * 5;
		}
	}
}

void ParticleBatch::Draw(){
	for (int i = 0; i < m_maxParticles; i++) {
		if (m_particles[i].m_lifetime > 0.0f) {

		}
	}
}

void ParticleBatch::addParticle(const glm::vec3& position, const glm::vec3& velocity){
	int particleIndex = findFreeParticle();

	auto& p = m_particles[particleIndex];

	p.m_transform = glm::translate(glm::mat4(1) ,0.f, 4.f, 3.f);
	p.m_lifetime = 1.0f;
	p.m_position = position;
	p.m_velocity = velocity;

	float s_velocity = (rand() / (float)RAND_MAX) * (.1f - .01f) + .01f;
	p.m_velocity.x = (rand() / (float)RAND_MAX) * 2 - 1;
	p.m_velocity.y = (rand() / (float)RAND_MAX) * 2 - 1;
	p.m_velocity.z = (rand() / (float)RAND_MAX) * 2 - 1;
	p.m_velocity = glm::normalize(p.m_velocity) * s_velocity;
}

void ParticleBatch::addParticle(const glm::vec3& position, const glm::vec3& velocity, const glm::vec4& color){
	int particleIndex = findFreeParticle();

	auto& p = m_particles[particleIndex];

	p.m_transform = glm::mat4(1);
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