#pragma once

#include "../nsfwgl/nsfw.h"

#include "Camera.h"
#include "Geometry.h"

class GPass : public nsfw::RenderPass
{


public:
	GPass(const char *shaderName, const char *fboName) : RenderPass(shaderName, fboName) {}
	void prep() 
	{
		glBindFramebuffer(GL_FRAMEBUFFER, *fbo);
		glEnable(GL_DEPTH_TEST);
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(*shader);
	}
	void post()
	{
		glDisable(GL_DEPTH_TEST);
		glUseProgram(0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}



	void draw(Camera &c, const Geometry &g)
	{

		setUniform("Projection", nsfw::UNIFORM::TYPE::MAT4, glm::value_ptr(c.GetProjection()));
		setUniform("View", nsfw::UNIFORM::TYPE::MAT4, glm::value_ptr(c.GetView()));
		setUniform("Model", nsfw::UNIFORM::TYPE::MAT4, glm::value_ptr(g.transform));

		bool usingTexture = false;
		if (g.diffuse.name != "")
		{
			setUniform("DiffuseTexture", nsfw::UNIFORM::TEX2, g.diffuse, 0);
			usingTexture = true;
		}
		//HACKHACK
		setUniform("isTexture", nsfw::UNIFORM::BOOL, &usingTexture);
		

		glBindVertexArray(*g.mesh);
		glDrawElements(GL_TRIANGLES, *g.tris, GL_UNSIGNED_INT, nullptr);
	}
};