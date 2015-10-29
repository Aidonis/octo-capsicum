#pragma once
#include "../nsfwgl/nsfw.h"
#include "Light.h"
#include "Camera.h"

class LPassD : public nsfw::RenderPass
{
	nsfw::Asset<nsfw::ASSET::TEXTURE> normal, position;

public:
	LPassD(const char *shaderName, const char *fboName) : RenderPass(shaderName, fboName) {}

	void prep(){
		//TODO_D("glUseProgram, glClear, glBindFrameBuffer, glViewPort, glEnable etc...");
		glBindFramebuffer(GL_FRAMEBUFFER, *fbo);

#ifdef _DEBUG
		GLenum status = glGetError();
		if (status != GL_NO_ERROR)
		{
			std::cerr << "A error occurred while prepping a render pass." << std::endl;

			std::cerr << (status == GL_INVALID_OPERATION) ? "GL_INVALID_OPERATION :: An invalid FBO name was provided." :
				"An unknown error occurred.";

			assert(false && "Check std::cerr for more information.");
		}
#endif

		glClear(GL_COLOR_BUFFER_BIT);

		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);

		glUseProgram(*shader);

	}
	void post(){
		//TODO_D("Unset any gl settings");
		glDisable(GL_BLEND);
		glUseProgram(0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}


	void draw(const Camera &c, const LightD &l)
	{
		setUniform("LightDirection", nsfw::UNIFORM::TYPE::FLO3, glm::value_ptr(l.direction));
		setUniform("LightColor",     nsfw::UNIFORM::TYPE::FLO3, glm::value_ptr(l.color));

		setUniform("Normal", nsfw::UNIFORM::TEX2, &normal, 0);
		setUniform("Position", nsfw::UNIFORM::TEX2, &position, 1);

		setUniform("ViewPosition", nsfw::UNIFORM::FLO3, glm::value_ptr(c.transform[3]));
		float specPow = 16;
		setUniform("SpecularPower", nsfw::UNIFORM::FLO1, &specPow);

		//setUniform("TexelScalar",    nsfw::UNIFORM::MAT4, glm::value_ptr(nsfw::Window::instance().getTexelAdjustmentMatrix()));

		unsigned quadVAOHandle  = nsfw::Assets::instance().get<nsfw::ASSET::VAO>("Quad");
		unsigned quadNumtris    = nsfw::Assets::instance().get<nsfw::ASSET::SIZE>("Quad");

		//TODO_D("GL BindVAO/DrawElements with quad size and vao");

		glBindVertexArray(quadVAOHandle);
		glDrawElements(GL_TRIANGLES, quadNumtris, GL_UNSIGNED_INT, 0);
	}
};