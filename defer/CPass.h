#pragma once

#include "../nsfwgl/nsfw.h"
#include "../nsfwgl/Render.h"
#include "Camera.h"

class CPass : public nsfw::RenderPass
{
	nsfw::Asset<nsfw::ASSET::TEXTURE> albedo, light;

public:
											
	CPass(const char *shaderName, const char *fboName) 
	: RenderPass(shaderName, fboName), albedo("GPassAlbedo"), light("LPassColor"){}


	void prep(){
		//TODO_D("glUseProgram, glClear, glBindFrameBuffer, glViewPort, glEnable etc...");
		//
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.35f, 0.35f, 0.35f, 0);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(*shader);

#ifdef _DEBUG
		GLenum status = glGetError();
		if (status != GL_NO_ERROR) {
			std::cerr	<<	"An error occurred while prepping a render pass." << std::endl;

			std::cerr << (status == GL_INVALID_VALUE) ? "GL_INVALID_VALUE :: An invalid shader name was provided." :
				(status == GL_INVALID_OPERATION) ? "GL_INVALID_OPERATION :: The shader could not be bound, if valid" :
				"An unknown error occurred.";
			assert(false && "Check stderr for more information.");
		}
#endif

	}
	void post(){
		//TODO_D("Unset any gl settings");
		//
		glUseProgram(0);
		glBindVertexArray(0);
	}


	void draw()
	{
		// Set uniforms for textures we're going to composite-> NAMES ARE FROM SHADER!
		setUniform("Albedo", nsfw::UNIFORM::TEX2, albedo, 0);
		setUniform("Light", nsfw::UNIFORM::TEX2, light, 1);
		
		unsigned quadVAOHandle = nsfw::Assets::instance().get<nsfw::ASSET::VAO>("Quad");
		unsigned quadNumtris   = nsfw::Assets::instance().get<nsfw::ASSET::SIZE>("Quad");

		glBindVertexArray(quadVAOHandle);
		glDrawElements(GL_TRIANGLES, quadNumtris, GL_UNSIGNED_INT, 0);

		//TODO_D("GL BindVAO/DrawElements with quad size and vao");
	}
};