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
		glDepthMask(GL_TRUE);
		glEnable(GL_DEPTH_TEST);

#ifdef _DEBUG
		GLenum status = glGetError();
		if (status != GL_NO_ERROR)
		{
			std::cerr << "A error occurred while prepping a render pass." << std::endl;

			std::cerr << (status == GL_INVALID_OPERATION) ? "GL_INVALID_OPERATION :: An invalid FBO name was provided." :
				"An unknown error occurred.";

			assert(false && "Check stderr for more information.");
		}
#endif

		//
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(*shader);


#ifdef _DEBUG
		status = glGetError();
		if (status != GL_NO_ERROR)
		{
			std::cerr << "A error occurred while prepping a render pass." << std::endl;

			std::cerr << (status == GL_INVALID_VALUE) ? "GL_INVALID_VALUE :: An invalid shader name was provided." :
				(status == GL_INVALID_OPERATION) ? "GL_INVALID_OPERATION :: The shader could not be bound, if valid." :
				"An unknown error occurred.";

			assert(false && "Check stderr for more information.");
		}
#endif
	}
	void post(){
		//TODO_D("Unset any gl settings");
		//
		glDisable(GL_DEPTH_TEST);
		glDepthMask(GL_FALSE);
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
		//setUniform("NormalMap",		nsfw::UNIFORM::TEX2, g.normal, 1);
		
		glBindVertexArray(*g.mesh);
		//unsigned ind_count = nsfw::Assets::instance().get(g.tris);
		unsigned int cnt = *g.tris;
		glDrawElements(GL_TRIANGLES, *g.tris, GL_UNSIGNED_INT, nullptr);
		//TODO_D("bindVAO and Draw Elements!");
	}
};