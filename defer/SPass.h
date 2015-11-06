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
		glBindFramebuffer(GL_FRAMEBUFFER, *fbo);
		glEnable(GL_DEPTH_TEST);


		glClear(GL_DEPTH_BUFFER_BIT);
		glUseProgram(*shader);
	}

	void draw(const LightD &l, const Geometry &g){
		setUniform("LightMatrix",	nsfw::UNIFORM::TYPE::MAT4, glm::value_ptr(l.projection * l.view));
		setUniform("Model",			nsfw::UNIFORM::TYPE::MAT4, glm::value_ptr(g.transform));

		glBindVertexArray(*g.mesh);
		glDrawElements(GL_TRIANGLES, *g.tris, GL_UNSIGNED_INT, nullptr);
	}

	void post(){
		glDisable(GL_DEPTH_TEST);
		glUseProgram(0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}


};