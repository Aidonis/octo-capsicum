#pragma once
#include "../nsfwgl/nsfw.h"
#include "Light.h"
#include "Geometry.h"

//Shadow Pass
class SPass : public nsfw::RenderPass{
	
	//nsfw::Asset<nsfw::ASSET::TEXTURE> shadows, shadowmap;

public:
	SPass(const char* shaderName, const char* fboName) : RenderPass(shaderName, fboName){}

	void prep(){
		
		
		glViewport(0, 0, 2048, 2048);

		glBindFramebuffer(GL_FRAMEBUFFER, *fbo);
		glUseProgram(*shader);
		glEnable(GL_DEPTH_TEST);

		glFrontFace(GL_CW);

		glClear(GL_DEPTH_BUFFER_BIT);



	}

	void draw(const LightD &l, const Geometry &g){
		setUniform("Projection", nsfw::UNIFORM::TYPE::MAT4, glm::value_ptr(l.getProjection()));
		setUniform("View",	nsfw::UNIFORM::TYPE::MAT4, glm::value_ptr( l.getView()));
		setUniform("Model",			nsfw::UNIFORM::TYPE::MAT4, glm::value_ptr(g.transform));

		glBindVertexArray(*g.mesh);
		glDrawElements(GL_TRIANGLES, *g.tris, GL_UNSIGNED_INT, nullptr);
	}

	void post(){
		glFrontFace(GL_CCW);
		glDisable(GL_DEPTH_TEST);
		glUseProgram(0);
		glViewport(0, 0, 800, 600);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}


};