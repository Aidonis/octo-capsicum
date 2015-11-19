#pragma once

#include "../nsfwgl/nsfw.h"

#include "Camera.h"
#include "Geometry.h"

class GPass : public nsfw::RenderPass
{


public:	
	void prep(){
		//TODO_D("glUseProgram, glClear, glBindFrameBuffer, glViewPort, glEnable etc...");
		//
		glBindFramebuffer(GL_FRAMEBUFFER, *fbo);
		//glDepthMask(GL_TRUE);
		glEnable(GL_DEPTH_TEST);
		//glEnable(GL_CULL_FACE);

		//
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(*shader);

	}
	void post(){
		//TODO_D("Unset any gl settings");
		//
		glDisable(GL_DEPTH_TEST);
		//glDepthMask(GL_FALSE);
		glUseProgram(0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_CULL_FACE);
		//glUseProgram(0);
	}

	GPass(const char *shaderName, const char *fboName) : RenderPass(shaderName, fboName) {}

	void draw(const Camera &c, const Geometry &g)	
	{

		setUniform("Projection",	nsfw::UNIFORM::TYPE::MAT4, glm::value_ptr(c.getProjection()));
		setUniform("View",			nsfw::UNIFORM::TYPE::MAT4, glm::value_ptr(c.getView()));
		setUniform("Model",			nsfw::UNIFORM::TYPE::MAT4, glm::value_ptr(g.transform));

		setUniform("DiffuseTexture",	nsfw::UNIFORM::TEX2, g.diffuse,  0);
		setUniform("NormalMap",		nsfw::UNIFORM::TEX2, g.normal, 1);
		setUniform("Specular",		nsfw::UNIFORM::TEX2, g.specular, 2);

		//HACKHACK
		bool usingTexture = false;
		if(g.diffuse.name != ""){
			setUniform("DiffuseTexture", nsfw::UNIFORM::TEX2, g.diffuse, 0);
			usingTexture = true;
		}

		setUniform("isTexture", nsfw::UNIFORM::BOOL, &usingTexture);
		
		glBindVertexArray(*g.mesh);
		//unsigned ind_count = nsfw::Assets::instance().get(g.tris);
		glDrawElements(GL_TRIANGLES, *g.tris, GL_UNSIGNED_INT, nullptr);
	}

	//void draw(const Camera &c, const ParticleBatch &pb) {
	//	TODO_D("Build Draw for particle batch");

	//	for (int i = 0; i < pb.m_maxParticles; i++) {

	//		//pb.m_model->transform = glm::scale(pb.m_model->transform, 0.5f, .5f, .5f);
	//		//pb.m_model->transform *= glm::translate(0.f, i * 1.0f, 0.f) * glm::scale(0.5f, 0.5f, 0.5f);

	//		setUniform("Projection", nsfw::UNIFORM::TYPE::MAT4, glm::value_ptr(c.getProjection()));
	//		setUniform("View", nsfw::UNIFORM::TYPE::MAT4, glm::value_ptr(c.getView()));
	//		setUniform("Model", nsfw::UNIFORM::TYPE::MAT4, glm::value_ptr(pb.m_model->transform));

	//		setUniform("DiffuseTexture", nsfw::UNIFORM::TEX2, pb.m_model->diffuse, 0);
	//		setUniform("NormalMap", nsfw::UNIFORM::TEX2, pb.m_model->normal, 1);
	//		setUniform("Specular", nsfw::UNIFORM::TEX2, pb.m_model->specular, 2);

	//		//Hack
	//		bool usingTexture = false;
	//		//if (pb.diffuse.name != "") {
	//		//	setUniform("DiffuseTexture", nsfw::UNIFORM::TEX2, pb.diffuse, 0);
	//		//	usingTexture = true;
	//		//}

	//		setUniform("isTexture", nsfw::UNIFORM::BOOL, &usingTexture);

	//		glBindVertexArray(*pb.m_model->mesh);
	//		glDrawElements(GL_TRIANGLES, *pb.m_model->tris, GL_UNSIGNED_INT, nullptr);
	//	}
	//}
};