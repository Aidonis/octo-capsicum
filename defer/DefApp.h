#pragma once

#include "../nsfwgl/nsfw.h"


struct Camera;
struct Geometry;
struct LightD;
class ParticleBatch;
class GPUParticleEmitter;

class GPass;
class LPassD;
class CPass;
class SPass;
class SCPass;

class DeferredApplication : public nsfw::Application
{
	Camera   *m_camera;
	Geometry *m_soulspear;
	Geometry *m_soulspear2;
	
	Geometry *m_cube;
	Geometry *m_cubes;
	ParticleBatch *m_particleBatch;
	GPUParticleEmitter *m_particleEmitter;

	Geometry *m_floor;
	LightD	 *m_light;

	CPass   *m_compositePass;
	GPass   *m_geometryPass;
	LPassD  *m_directionalLightPass;
	SPass	*m_shadowPass;
	SCPass	*m_shadowCompositePass;

	bool shadowPass = true;

public:
	void onInit();
	void onStep();
	void onTerm();
	void onPlay();
};