#pragma once
#include "../nsfwgl/nsfw.h"
#include "Light.h"
#include "Camera.h"

class LPassD : public nsfw::RenderPass
{
	nsfw::Asset<nsfw::ASSET::TEXTURE> normal, position, specular, shadowmap;
	glm::vec3 ambientLight = glm::vec3(0, 0, .25f);
	float specPower = 128;

public:


	LPassD(const char *shaderName, const char *fboName) : RenderPass(shaderName, fboName), position("GPassPosition"), normal("GPassNormal"), specular("GPassSpecular"), shadowmap("ShadowMap") {}

	void prep(){
		//TODO_D("glUseProgram, glClear, glBindFrameBuffer, glViewPort, glEnable etc...");
		glBindFramebuffer(GL_FRAMEBUFFER, *fbo);

		glClear(GL_COLOR_BUFFER_BIT);

		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);

		glUseProgram(*shader);

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



	}
	void post(){
		//TODO_D("Unset any gl settings");
		glDisable(GL_BLEND);
		glUseProgram(0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}


	void draw(const Camera &c, const LightD &l)
	{
		glm::mat4 textureSpaceOffset(
			0.5f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.5f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.5f, 0.0f,
			0.5f, 0.5f, 0.5f, 1.0f
			);

		//Light
		setUniform("directional.direction", nsfw::UNIFORM::TYPE::FLO3, glm::value_ptr(l.direction));
		setUniform("directional.color", nsfw::UNIFORM::TYPE::FLO3, glm::value_ptr(l.color));
		setUniform("directional.projection", nsfw::UNIFORM::TYPE::MAT4, glm::value_ptr(l.lightProjection));
		setUniform("directional.view", nsfw::UNIFORM::TYPE::MAT4, glm::value_ptr(l.view));

		//setUniform("Normal", nsfw::UNIFORM::TEX2, &normal, 0);
		//setUniform("Position", nsfw::UNIFORM::TEX2, &position, 1);

		//Camera
		setUniform("cameraPosition", nsfw::UNIFORM::FLO3, glm::value_ptr(c.transform[3]));
		setUniform("cameraView", nsfw::UNIFORM::MAT4, glm::value_ptr(c.getView()));

		//
		setUniform("specPower", nsfw::UNIFORM::TYPE::FLO1, &specPower);
		setUniform("ambient", nsfw::UNIFORM::TYPE::FLO3, glm::value_ptr(ambientLight));
		setUniform("positionTexture", nsfw::UNIFORM::TEX2, position, 0);
		setUniform("normalTexture", nsfw::UNIFORM::TEX2, normal, 1);
		
		//Shade
		setUniform("shadowMap", nsfw::UNIFORM::TEX2, &shadowmap, 2);
		setUniform("textureSpaceOffset", nsfw::UNIFORM::MAT4, glm::value_ptr(textureSpaceOffset));

		//setUniform("TexelScalar",    nsfw::UNIFORM::MAT4, glm::value_ptr(nsfw::Window::instance().getTexelAdjustmentMatrix()));

		unsigned quadVAOHandle  = nsfw::Assets::instance().get<nsfw::ASSET::VAO>("Quad");
		unsigned quadNumtris    = nsfw::Assets::instance().get<nsfw::ASSET::SIZE>("Quad");

		//TODO_D("GL BindVAO/DrawElements with quad size and vao");

		glBindVertexArray(quadVAOHandle);
		glDrawElements(GL_TRIANGLES, quadNumtris, GL_UNSIGNED_INT, 0);
	}
};