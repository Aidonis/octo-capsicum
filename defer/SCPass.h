#pragma once
#include "../nsfwgl/nsfw.h"
#include "Light.h"
#include "Camera.h"


//Shadow Composite Pass
class SCPass : public nsfw::RenderPass {


public:
	SCPass();

	void prep() {
		glBindFramebuffer(GL_FRAMEBUFFER, *fbo);
	}

	void draw() {

	}

	void post() {

	}


};