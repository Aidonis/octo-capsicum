
#include "DefApp.h"

#include "Geometry.h"
#include "Light.h"
#include "Camera.h"
#include "GPUParticle.h"
#include "Particles.h"

#include "GPass.h"
#include "CPass.h"
#include "LPassD.h"
#include "SPass.h"

using namespace nsfw;

int main()
{
	DeferredApplication d;

	d.init(800, 600);
	d.play();
	d.term();

	system("pause");
}


void DeferredApplication::onInit()
{
	auto &w = nsfw::Window::instance();
	auto &a = nsfw::Assets::instance();

	// Load Shaders
	a.loadShader("GeometryPassPhong", "../rsc/shaders/GPass.vert", "../rsc/shaders/GPass.frag");
	a.loadShader("LightPassDirectional", "../rsc/shaders/LPass.vert", "../rsc/shaders/LPass.frag");
	a.loadShader("CompPass", "../rsc/shaders/CPass.vert", "../rsc/shaders/CPass.frag");
	a.loadShader("ShadowMapPass", "../rsc/shaders/SPass.vert", "../rsc/shaders/SPass.frag");
	
	//Working here
	//a.createGeoShader("GeometryShader", "../rsc/shaders/Particles.vert", "../rsc/shaders/Particles.frag", "../rsc/shaders/Particles.geom");

	// Setup FBOs
	//GeoPass
	const char *gpassTextureNames[] = { "GPassAlbedo","GPassPosition","GPassNormal","GPassDepth" };
	const unsigned gpassDepths[] = { GL_RGB8,GL_RGB32F,GL_RGB32F,GL_DEPTH_COMPONENT }; // GL_RGB8, GL_RGB32, GL_RGB32, GL_DEPTH_COMPONENT
	a.makeFBO("GeometryPass", w.getWidth(), w.getHeight(), 4, gpassTextureNames, gpassDepths);

	//LightPass
	const char *lpassTextureNames[] = { "LPassColor" };
	const unsigned lpassDepths[] = { GL_RGB8 }; // GL_RGB8
	a.makeFBO("LightPass", w.getWidth(), w.getHeight(), 1, lpassTextureNames, lpassDepths);

	//ShadowPass
	const char* spassTextureNames[] = { "ShadowMap" };
	const unsigned spassDepths[] = { GL_DEPTH_COMPONENT };
	a.makeFBO("ShadowPass", 2048, 2048, 1, spassTextureNames, spassDepths);

	// Load any other textures and geometry we want to use
	a.loadFBX("Soulspear",	"../rsc/models/soulspear/soulspear.fbx");
	a.loadTexture("Quad",	"../rsc/textures/uv.png");


}

void DeferredApplication::onPlay()
{
	//TODO_D("Initialize our scene objects!");
	m_camera		= new FlyCamera;
	m_light			= new LightD;
	m_soulspear		= new Geometry;
	m_soulspear2	= new Geometry;
	m_floor			= new Geometry;
	m_cube			= new Geometry;
	m_cubes			= new Geometry;
	m_particleEmitter = new GPUParticleEmitter;
	//m_particleBatch = new ParticleBatch;


	m_camera->lookAt(glm::vec3(0,2,10), glm::vec3(0,2,0), glm::vec3(0, 1, 0));
	//m_camera->lookAt()

	//Spear 1
	m_soulspear->mesh	   = "SoulSpear_Low:SoulSpear_Low1";
	m_soulspear->tris	   = "SoulSpear_Low:SoulSpear_Low1";
	m_soulspear->diffuse   = "soulspear_diffuse.tga";	// loadFBX will need to name every handle it creates,
	m_soulspear->normal    = "soulspear_normal.tga";		// These handle names may not be what your loadFBX sets 
	m_soulspear->specular  = "soulspear_specular.tga";	// them as! (Assets will report what the key names are though)
	m_soulspear->specPower = 40.0f;
	m_soulspear->transform = mat4(1);

	//Spear 2
	m_soulspear2->mesh = "SoulSpear_Low:SoulSpear_Low1";
	m_soulspear2->tris = "SoulSpear_Low:SoulSpear_Low1";
	m_soulspear2->diffuse = "soulspear_diffuse.tga";	// loadFBX will need to name every handle it creates,
	m_soulspear2->normal = "soulspear_normal.tga";		// These handle names may not be what your loadFBX sets 
	m_soulspear2->specular = "soulspear_specular.tga";	// them as! (Assets will report what the key names are though)
	m_soulspear2->specPower = 40.0f;
	m_soulspear2->transform = translate(-5, 0, 0);

	//Cube
	m_cube->mesh = "Cube";
	m_cube->tris = "Cube";
	m_cube->diffuse = "";
	m_cube->specPower = 0.0f;
	m_cube->transform = translate(5, 3, 0);

	//Cubes
	m_cubes->mesh = "Cube";
	m_cubes->tris = "Cube";
	m_cubes->diffuse = "";
	m_cubes->specPower = 0.0f;
	m_cubes->transform = translate(0, 2, 3);

	//Cube particles
	//m_particleBatch->init(3, .1f, *m_cubes);
	//for (int i = 0; i < m_particleBatch->m_maxParticles; i ++){
	//	m_particleBatch->addParticle(glm::vec3(0, 2, 0), glm::vec3(1, 0, 0));
	//}

	//GPU Particle
	m_particleEmitter->init(100, 0.1f, 5.0f, 1, 10, 1, 0.1f,
							glm::vec4(1, 0, 0, 1), glm::vec4(1, 1, 0, 1));

	//TODO_D("Initialize our render passes!");
	
	//Directional Light
	m_light->color = glm::vec3(1, 1, 1);
	m_light->direction = glm::normalize(glm::vec3(-.5f, .5f, 1));
	m_light->ambientIntensity = 1;
	m_light->diffuseIntensity = 1;

	//Floor-Quad
	m_floor->mesh = "Quad";
	m_floor->tris = "Quad";
	m_floor->transform = glm::rotate(90.0f, glm::vec3(1, 0, 0)) * glm::scale(glm::vec3(10, 10, 1));
	m_floor->diffuse = "";
	m_floor->specPower = 0.0f;

	//Passes
	m_geometryPass			= new GPass ("GeometryPassPhong", "GeometryPass");
	m_directionalLightPass  = new LPassD("LightPassDirectional", "LightPass");
	m_compositePass			= new CPass ("CompPass", "Screen"); // Screen is defined in nsfw::Assets::init()
	m_shadowPass			= new SPass ("ShadowMapPass", "ShadowPass");
}

void DeferredApplication::onStep()
{
	auto &w = nsfw::Window::instance();

	float time = w.getTime();
	float deltaTime = w.getDeltaTime();

	if (w.getKey(GLFW_KEY_F1)) {
		shadowPass = !shadowPass;
	}

	m_soulspear->transform = glm::rotate(time * 100, glm::vec3(0.f, 1.f, 0.f));
	//TODO_D("Update our game objects-- IF THEY EVEN DO ANYTHING");
	m_light->update();
	m_camera->update();
	m_soulspear->update();
	//m_particleBatch->Update(deltaTime);
	
	//TODO_D("Draw all of our renderpasses!");
	m_geometryPass->prep();
	m_geometryPass->draw(*m_camera, *m_soulspear);
	m_geometryPass->draw(*m_camera, *m_soulspear2);
	m_geometryPass->draw(*m_camera, *m_cube);
	

	//m_geometryPass->draw(*m_camera, *m_particleBatch);
	//m_geometryPass->draw(*m_camera, *m_cubes);

	m_geometryPass->draw(*m_camera, *m_floor);
	m_geometryPass->draw(*m_camera, *m_particleEmitter);
	m_geometryPass->post();

	
	//Shadow Pass
	m_shadowPass->prep();
	if (shadowPass) {
	m_shadowPass->draw(*m_light, *m_soulspear);
	m_shadowPass->draw(*m_light, *m_soulspear2);
	//m_shadowPass->draw(*m_light, *m_cube);
	m_shadowPass->draw(*m_light, *m_floor);
	}
	m_shadowPass->post();
	

	//Light Pass
	m_directionalLightPass->prep();
	m_directionalLightPass->draw(*m_camera, *m_light);
	m_directionalLightPass->post();

	m_compositePass->prep();
	m_compositePass->draw();
	m_compositePass->post();
}

void DeferredApplication::onTerm()
{
	delete m_camera;
	delete m_light;
	delete m_soulspear;
	delete m_soulspear2;

	delete m_compositePass;
	delete m_geometryPass;
	delete m_directionalLightPass;
	delete m_shadowPass;
}
