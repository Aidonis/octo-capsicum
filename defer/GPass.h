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
		glEnable(GL_DEPTH_TEST);
		glBindFramebuffer(GL_FRAMEBUFFER, *fbo);
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.55f, 0.25f, 0.25f, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glViewport(0, 0, 800, 600);
		glUseProgram(*shader);
	}
	void post(){
		//TODO_D("Unset any gl settings");
		//
		glDisable(GL_DEPTH_TEST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindVertexArray(0);
		//glUseProgram(0);
	}

	GPass(const char *shaderName, const char *fboName) : RenderPass(shaderName, fboName) {}

	void draw(const Camera &c, const Geometry &g)	
	{

		setUniform("Projection",	nsfw::UNIFORM::TYPE::MAT4, glm::value_ptr(c.getProjection()));
		setUniform("View",			nsfw::UNIFORM::TYPE::MAT4, glm::value_ptr(c.getView()));
		setUniform("Model",			nsfw::UNIFORM::TYPE::MAT4, glm::value_ptr(g.transform));

		setUniform("DiffuseMap",	nsfw::UNIFORM::TEX2, g.diffuse,  0);
		setUniform("NormalMap",		nsfw::UNIFORM::TEX2, g.normal, 1);
		//setUniform("SpecularMap",	nsfw::UNIFORM::TEX2,  g.specular, 2);

		//setUniform("SpecularPower", nsfw::UNIFORM::FLO1, (void*)&g.specPower);

        /*nsfw::Assets::instance().get(g.mesh);
        nsfw::Assets::instance().get(g.tris);
        *g.mesh;
        *g.tris;*/

		glBindVertexArray(*g.mesh);
		unsigned ind_count = nsfw::Assets::instance().get(g.tris);
		glDrawElements(GL_TRIANGLES, *g.tris, GL_UNSIGNED_INT, 0);
		//TODO_D("bindVAO and Draw Elements!");
	}
};